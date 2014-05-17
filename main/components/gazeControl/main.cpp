/*
 *  Yarp Modules Manager
 *  Copyright: (C) 2010 RobotCub Consortium
 *				Italian Institute of Technology (IIT)
 *				Via Morego 30, 16163, 
 *				Genova, Italy
 * 
 *  Copy Policy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
 *  Authors: Ali Paikan <ali.paikan@iit.it>
 *
 */


#include <stdio.h>
#include <string>
#include <sys/mman.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <math.h>
#include <string>

#include <yarp/os/all.h>
#include <yarp/os/Log.h>
#include <yarp/sig/Matrix.h>
#include <yarp/math/Math.h>
#include <yarp/os/RpcServer.h>

using namespace yarp::os;
using namespace yarp::sig;
using namespace yarp::math;
using namespace std;

#define SERVO_MIN_PULSE         55      // us
#define SERVO_MAX_PULSE         240     // us

#define JOINT1_MIN              10      // deg
#define JOINT1_MAX              155     // deg
#define JOINT2_MIN              10      // deg
#define JOINT2_MAX              180     // deg
#define JOINT1_REST             100
#define JOINT2_REST             105

#define JOINTS_SPEED            0.200    // sec

#define d2                      0.03     // meter
#define a2                      0.01
#define d1                      0.045
#define X_OFFSET                -0.045   // translating to the center of the robot
#define Y_OFFSET                0.03     // translating to the center of the robot


/*
 Camera matrix: 
   0.00426  -0.00000  -0.36063
   0.00000   0.00427  -0.25214
   0.00000   0.00000   1.00000
*/

class GazeControl: public RFModule {
public:

    GazeControl(void) {
        bIsRT = false;
        fdServo = NULL;
        joints_speed = JOINTS_SPEED;

        // camera params
        camParam.resize(3,3);
        camParam(0,0) = 0.00426;
        camParam(0,1) = 0.0;
        camParam(0,2) = -0.36063;
        camParam(1,0) = 0.0;
        camParam(1,1) = 0.00427;
        camParam(1,2) = -0.25214;
        camParam(2,0) = 0.0;
        camParam(2,1) = 0.0;
        camParam(2,2) = 1.0;
    }

    bool configure(ResourceFinder &rf) {
        fdServo = fopen("/dev/servoblaster", "r");
        if(!fdServo)
        {
            fprintf(stdout, "Cannot open ServoBalster driver! (dose not exist)\n");            
            return false;
        }
        fclose(fdServo);

        fdServo = fopen("/dev/servoblaster", "w");
        if(!fdServo)
        {
            fprintf(stdout, "Cannot open ServoBalster driver! (dose not exist)\n");            
            return false;
        }
        
        // Callibrating by reseting the gaze 
        setJointPose(0, JOINT1_REST);
        setJointPose(1, JOINT2_REST);
        joints_pos[0] = JOINT1_REST;
        joints_pos[1] = JOINT2_REST;
        bool ret = cmdPort.open("/GazeControl/cmd:i");
        if(ret)
            attach(cmdPort);
        shouldStop = !ret;
        return ret;
    }

    double getPeriod() {
        makeRealTime();
        return 0.01; // run every 10ms 
    }

    bool respond(const Bottle &cmd, Bottle &rep) {
        rep.clear(); 
        if(cmd.size() < 2){
            rep.addString("[nack]");
            return true; 
        }        
        setCommand(cmd);
        getReply(rep);
        return true;
    }


    bool updateModule() {
        Bottle cmd;
        Bottle rep;

        getCommand(cmd);

        // command exit
        if(cmd.get(0).asString() == "cmd_exit")
        {
            setReply(rep);
            return false;
        }

        string mode = cmd.get(0).asString();
        // Controlling in the joint mode
        if(mode == "joint")
        {
            Bottle* pos = cmd.get(1).asList();
            if(!pos || (pos->size() < 2))
            {
                rep.addString("[nack]");
                setReply(rep);
                return true;       
            }

            rep.addString("[ack]");
            setReply(rep);

            if(pos->size() >= 3)
                joints_speed = pos->get(2).asDouble();
            else
                joints_speed = JOINTS_SPEED;
            bool ret = setAllJointsPose(pos->get(1).asInt(), pos->get(0).asInt(), joints_speed);
        }
        else if(mode == "cam")
        { 
            Bottle* pos = cmd.get(1).asList();
            if(!pos || (pos->size() < 3))
            {
                rep.addString("[nack]");
                setReply(rep);
                return true;       
            }

            rep.addString("[ack]");
            setReply(rep);

            if(pos->size() >= 3)
                joints_speed = pos->get(3).asDouble();
            else
                joints_speed = JOINTS_SPEED;

            Matrix uv(3,1);
            uv(0,0) =  pos->get(0).asDouble();
            uv(1,0) =  pos->get(1).asDouble();
            uv(2,0) =  1.0;
            double d3 = pos->get(2).asDouble(); 

            // maping world to camera frame
            Matrix cFrame = camParam * uv;
            //printf("cFrame: %.2f, %.2f, %.2f\n", cFrame(0,0), cFrame(1,0), cFrame(2,0));
            // transfering to the end effector frame
            double x3 = -cFrame(1,0);
            double y3 = -cFrame(0,0);
            double z3 = 0.0;
            //double d3 = 0.5;
            //printf("joints: %d, %d\n", joints_pos[1], joints_pos[0]);
            double theta1 = (joints_pos[1] - JOINT2_REST) * M_PI/180.0;
            double theta2 = (joints_pos[0] - JOINT1_REST) * M_PI/180.0;
            //printf("thetas: %.2f, %.2f\n", theta1, theta2);

            double x = d3*cos(theta1)*cos(theta2) - y3*sin(theta1) - d2*sin(theta1) + a2*cos(theta1)*sin(theta2) + z3*cos(theta1)*cos(theta2) + x3*cos(theta1)*sin(theta2);
            double y = d2*cos(theta1) + y3*cos(theta1) + d3*cos(theta2)*sin(theta1) + a2*sin(theta1)*sin(theta2) + z3*cos(theta2)*sin(theta1) + x3*sin(theta1)*sin(theta2);
            double z = d1 + a2*cos(theta2) - d3*sin(theta2) + x3*cos(theta2) - z3*sin(theta2);
            //printf("xyz: %.5f %.5f %.5f\n", x, y ,z);
            bool ret = setCartPose(x, y, z, joints_speed);
        }
        else if(mode == "cart")
        {
             Bottle* pos = cmd.get(1).asList();
            if(!pos || (pos->size() < 3))
            {
                rep.addString("[nack]");
                setReply(rep);
                return true;
            }    

            rep.addString("[ack]");
            setReply(rep);

            if(pos->size() >= 4)
                joints_speed = pos->get(3).asDouble();
            else
                joints_speed = JOINTS_SPEED;

            double x =  pos->get(0).asDouble();// + X_OFFSET;
            double y =  pos->get(1).asDouble();// + Y_OFFSET;
            double z =  pos->get(2).asDouble();
            bool ret = setCartPose(x, y, z, joints_speed);            
        }
        else
        {
            rep.addString("[nack]");
            setReply(rep);
        }           
        return true; 
    }

    bool interruptModule() { 
        cmdPort.interrupt();             // interupt the blocking read 
        shouldStop = true;
        // send exit command to the thread
        Bottle cmd;
        cmd.addString("cmd_exit");
        setCommand(cmd);            
        return true; 
    }

    bool close() {
        cmdPort.close(); 
        //reseting the gaze 
        shouldStop = false;
        setAllJointsPose(JOINT1_REST, JOINT2_REST, JOINTS_SPEED);
        shouldStop = true;
        if(fdServo)
            fclose(fdServo);
        return true; 
    } 

    void makeRealTime() {
	    if(!bIsRT) {
	        struct sched_param thread_param;
    	    thread_param.sched_priority = 90;
   	        pthread_setschedparam(pthread_self(), SCHED_FIFO, &thread_param);
            bIsRT = true;
	    }
    }


private:
    void setCommand(const Bottle &cmd) {    
        cmdSemaphore.wait();
        shouldStop = true;
        command = cmd;    
        rpyEvent.reset();
        cmdEvent.signal();
        cmdSemaphore.post();
    }

    void getCommand(Bottle &cmd) {
        cmdEvent.wait();
        cmdSemaphore.wait();
        shouldStop = false;
        cmd = command;
        cmdSemaphore.post();
    }

    void setReply(const Bottle &rpy) {
        rpySemaphore.wait();
        reply = rpy;
        cmdEvent.reset();
        rpyEvent.signal();
        rpySemaphore.post();
    }

    void getReply(Bottle &rpy) {
        rpyEvent.wait();
        rpySemaphore.wait();
        rpy = reply;
        rpySemaphore.post();
    }


    bool setAllJointsPose(int pos1, int pos2, double speed) {
        printf("joints: %d, %d\n", pos2, pos1);
        // checking joint limits     
        pos1 = (pos1 < JOINT1_MIN) ? JOINT1_MIN : pos1;
        pos1 = (pos1 > JOINT1_MAX) ? JOINT1_MAX : pos1;
        pos2 = (pos2 < JOINT2_MIN) ? JOINT2_MIN : pos2;
        pos2 = (pos2 > JOINT2_MAX) ? JOINT2_MAX : pos2;

        int step1 = (pos1 > joints_pos[0]) ? 1 : -1; 
        int step2 = (pos2 > joints_pos[1]) ? 1 : -1; 
        int diff =  abs(pos1 - joints_pos[0]);
        if(diff < abs(pos2 - joints_pos[1]))
            diff = abs(pos2 - joints_pos[1]);

        double step_delay = (speed / (double)diff) * 1e6; //us
        step_delay = (step_delay < 5000.0) ? 5000.0 : step_delay;
        bool set1 = (pos1 == joints_pos[0]);
        bool set2 = (pos2 == joints_pos[1]);

        while((!set1 || !set2) && !shouldStop)
        {
            bool ret = true;
            if(pos1 != joints_pos[0]) 
                ret &= setJointPose(0, joints_pos[0] + step1);
            else
                set1 = true;

            if(pos2 != joints_pos[1]) 
                ret &= setJointPose(1, joints_pos[1] + step2);
             else
                set2 = true;
            if(!ret) return false;
            usleep(step_delay);
            //printf("joint1: %d, req_pos1:%d\n", joints_pos[0], pos1);
            //printf("joint2: %d, req_pos2:%d\n", joints_pos[1], pos2);
        }        
        return true; 
    }

    bool setJointPose(int joint, int pos) {
        if (joint < 0 ) return false; 

        // checking joint limits     
        if(joint == 0) {
            pos = (pos < JOINT1_MIN) ? JOINT1_MIN : pos;
            pos = (pos > JOINT1_MAX) ? JOINT1_MAX : pos;
        }
        else {
            pos = (pos < JOINT2_MIN) ? JOINT2_MIN : pos;
            pos = (pos > JOINT2_MAX) ? JOINT2_MAX : pos;
        }    
        
        int pulse_width = SERVO_MIN_PULSE + ((SERVO_MAX_PULSE - SERVO_MIN_PULSE) / 180) * pos;
        bool ret = (fprintf(fdServo, "%d=%d\n", joint, pulse_width) > 0); 
        fflush(fdServo);
        if(ret)
            joints_pos[joint] = pos;
        else
            printf("Cannot write to the servo driver!\n");
        return ret;
    }

    bool setCartPose(double x, double y, double z, double joints_speed) {
        double j1_1 = atan2(y,x) - atan2(d2, sqrt(x*x + y*y - (d2*d2)));
        double j1_2 = atan2(y,x) - atan2(d2, -sqrt(x*x + y*y - (d2*d2)));
        double A1 = x*cos(j1_1) + y*sin(j1_1);
        double A2 = x*cos(j1_2) + y*sin(j1_2);            

        double j2_1 = atan2(z-d1, -A1) - atan2(a2, sqrt((z-d1)*(z-d1) + A1*A1 - a2*a2));
        double j2_2 = atan2(z-d1, -A1) - atan2(a2, -sqrt((z-d1)*(z-d1) + A1*A1 - a2*a2));
        double j2_3 = atan2(z-d1, -A2) - atan2(a2, sqrt((z-d1)*(z-d1) + A2*A2 - a2*a2));
        double j2_4 = atan2(z-d1, -A2) - atan2(a2, -sqrt((z-d1)*(z-d1) + A2*A2 - a2*a2));
        
        //printf("[%.2f, %.2f, %.2f, %.2f]\n", j2_1, j2_2, j2_3, j2_4);
        double d3_1 = (d1 - z) * sin(j2_1) + A1*cos(j2_1); 
        double d3_2 = (d1 - z) * sin(j2_2) + A1*cos(j2_2); 
        double d3_3 = (d1 - z) * sin(j2_3) + A2*cos(j2_3); 
        double d3_4 = (d1 - z) * sin(j2_4) + A2*cos(j2_4); 
        
        double sol1[2]; 
        double sol2[2]; 

        if(d3_1 > 0.0) {
            sol1[0] = fmod(j1_1, 2*M_PI); 
            sol1[1] = fmod(j1_2, 2*M_PI); 
        }            
        else{
            sol1[0] = fmod(j1_1, 2*M_PI); 
            sol1[1] = fmod(j2_2, 2*M_PI);
        }

        if(d3_3 > 0.0) {
            sol2[0] = fmod(j1_2, 2*M_PI); 
            sol2[1] = fmod(j2_3, 2*M_PI); 
        }            
        else{
            sol2[0] = fmod(j1_2, 2*M_PI); 
            sol2[1] = fmod(j2_4, 2*M_PI);
        }
         
        for(int i=0; i<2; i++) {
            sol1[i] = (sol1[i] > M_PI) ? sol1[i] - 2*M_PI : sol1[i];
            sol1[i] = (sol1[i] < -M_PI) ? sol1[i] + 2*M_PI : sol1[i];
        }

        for(int i=0; i<2; i++) {
            sol2[i] = (sol2[i] > M_PI) ? sol2[i] - 2*M_PI : sol2[i];
            sol2[i] = (sol2[i] < -M_PI) ? sol2[i] + 2*M_PI : sol2[i];
        }

        //printf("d: %.2f, %.2f, %.2f, %.2f\n", d3_1, d3_2, d3_3, d3_4);
        //printf("sol1: %.2f, %.2f\n", sol1[0], sol1[1]);
        //printf("sol2: %.2f, %.2f\n", sol2[0], sol2[1]);
       
        sol1[0] *= 180 / M_PI;
        sol1[1] *= 180 / M_PI;
        sol2[0] *= 180 / M_PI;
        sol2[1] *= 180 / M_PI;
        
        //printf("sol1: %.2f, %.2f\n", sol1[0], sol1[1]);
        //printf("sol2: %.2f, %.2f\n", sol2[0], sol2[1]);

        double eng1 = abs(sol1[0]) + abs(sol1[1]);
        double eng2 = abs(sol2[0]) + abs(sol2[1]);

        //if((sol1[0]+JOINT1_REST) > JOINT1_MIN  && (sol1[1]+JOINT2_REST) < JOINT1_MAX )
        //if((sol2[0]+JOINT1_REST) > JOINT1_MIN  && (sol2[1]+JOINT2_REST) < JOINT1_MAX )
        
        bool ret;
        if(eng1 < eng2) 
            ret = setAllJointsPose(sol1[1]+JOINT1_REST, sol1[0]+JOINT2_REST, joints_speed);
        else
            ret = setAllJointsPose(sol2[1]+JOINT1_REST, sol2[0]+JOINT2_REST, joints_speed);
        return ret;     
    }

private:
    RpcServer cmdPort;
    Bottle command;
    Bottle reply;
    Event cmdEvent;
    Event rpyEvent;
    Semaphore cmdSemaphore;
    Semaphore rpySemaphore;
        
    bool bIsRT;
    FILE* fdServo;

    int joints_pos[2];
    double joints_speed;
    bool shouldStop;
    Matrix camParam;
    
};



int main(int argc, char* argv[])
{
    yarp::os::Network yarp;
    ResourceFinder rf;
    rf.configure(argc, argv);
    
    mlockall(MCL_CURRENT | MCL_FUTURE);
	struct sched_param sch_param;
	sch_param.__sched_priority = 90;
	if( sched_setscheduler(0, SCHED_FIFO, &sch_param) != 0 )
	{
		printf("sched_setscheduler failed.\n");
		return -1;
	}

    GazeControl module;
    if(!module.configure(rf)) {
        fprintf(stderr, "Error opening GazeControl\n");
        return -1;
    }

    printf("Press Ctrl+C to exit...\n"); fflush(stdout);
    module.runModule(); 

    return 0;
}

