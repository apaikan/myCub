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

using namespace yarp::os;
using namespace std;

#define SERVO_MIN_PULSE         55      // us
#define SERVO_MAX_PULSE         240     // us

#define JOINT1_MIN              70      // deg
#define JOINT1_MAX              155     // deg
#define JOINT2_MIN              10      // deg
#define JOINT2_MAX              180     // deg
#define JOINT1_REST             100
#define JOINT2_REST             105

#define JOINTS_SPEED            0.200     // sec

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
    }

    bool configure(ResourceFinder &rf) {
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
        shouldStop = !ret;
        return ret;
    }

    double getPeriod() {
        //makeRealTime();
        return 0.001; // run every 1ms 
    }
    
    bool updateModule() {
        
        Bottle* cmd = cmdPort.read();
        if(!cmd) 
            return true;

        //printf("cmd: %s\n", cmd->toString().c_str());
        if(cmd->size() < 2)
            return true; 

        string mode = cmd->get(0).asString();

        // Controlling in the joint mode
        if(mode == "joint")
        {
            Bottle* pos = cmd->get(1).asList();
            if(!pos || (pos->size() < 2))
                return true;        
            if(pos->size() >= 3)
                joints_speed = pos->get(2).asDouble();
            else
                joints_speed = JOINTS_SPEED;
            setAllJointsPose(pos->get(1).asInt(), pos->get(0).asInt(), joints_speed);
        }
        else if(mode == "cam")
        { ; }
        else if(mode == "cart")
        {
             Bottle* pos = cmd->get(1).asList();
            if(!pos || (pos->size() < 3))
                return true;
            if(pos->size() >= 4)
                joints_speed = pos->get(3).asDouble();
            else
                joints_speed = JOINTS_SPEED;
            double x =  pos->get(0).asDouble();
            double y =  pos->get(1).asDouble();
            double z =  pos->get(2).asDouble();
            x = (x==0.0) ? 0.01 : x;
            double j2 = asin(-z/x);
            j2 = (fabs(j2) == M_PI/2.0) ? j2+0.00001 : j2; 
            double j1 = asin(y/(x*cos(j2)));
            setAllJointsPose(j2*180/M_PI+JOINT1_REST, j1*180/M_PI+JOINT2_REST, joints_speed);
        }

        return true; 
    }

    bool interruptModule() { 
        cmdPort.interrupt();             // interupt the blocking read 
        shouldStop = true;
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
    	    thread_param.sched_priority = 99;
   	        pthread_setschedparam(pthread_self(), SCHED_FIFO, &thread_param);
            bIsRT = true;
	    }
    }


private:
    bool setAllJointsPose(int pos1, int pos2, double speed) {
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

private:
    BufferedPort<yarp::os::Bottle> cmdPort;
    bool bIsRT;
    FILE* fdServo;
    int joints_pos[2];
    double joints_speed;
    bool shouldStop;
};



int main(int argc, char* argv[])
{
    yarp::os::Network yarp;
    ResourceFinder rf;
    rf.configure(argc, argv);
    
    //mlockall(MCL_CURRENT | MCL_FUTURE);

    GazeControl module;
    if(!module.configure(rf)) {
        fprintf(stderr, "Error opening GazeControl\n");
        return -1;
    }

    printf("Press Ctrl+C to exit...\n"); fflush(stdout);
    module.runModule(); 

    return 0;
}

