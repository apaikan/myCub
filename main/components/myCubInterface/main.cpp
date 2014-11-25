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

#include <SerialPort.h> 

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
#include <yarp/dev/SerialInterfaces.h>
//#include <yarp/dev/PolyDriver.h>

#include <myCubInterface_IDL.h>

using namespace yarp::os;
//using namespace yarp::dev;
using namespace std;

#define JOINTS_SPEED            0.200     // sec
#define JOINTS_SCALE            2.2687
#define JOINTS_MIN              8

class MyCubInterface: public RFModule, public myCubInterface_IDL 
{
public:

    MyCubInterface(void) {
        bIsRT = false;
        pSerial = NULL;
        battery_volt = 0.0;
        battery_volt_count = 0;
        battery_plugged = false;
        serBusy = false;
    }
    
    ~MyCubInterface(void) {
        if(pSerial)
            delete pSerial;
    }
    
    inline bool checkJointLimits(int32_t pos) {
        return (((int)(pos*JOINTS_SCALE+JOINTS_MIN) <= 180) && ((int)(pos*JOINTS_SCALE+JOINTS_MIN) >= 0));
    }

    inline bool checkJointIDs(int32_t id) {
        return ((id <= 3) && (id >= 0));
    }

    bool configure(ResourceFinder &rf) {

        /*
        Property prop;        
        prop.put("device", "serialport");
        prop.put("comport", "/dev/ttyAMA0");
        prop.put("baudrate", "115200");
        //bool ret = prop.fromConfigFile(serial_filename.c_str(),config,false);
        //if (!ret) {
        //    fprintf(stderr, "Unable to read from serial port configuration file\n");
        //    return false;
        //}
        pSerial = NULL;
        driver.open(prop);
        if(!driver.isValid()){
            fprintf(stderr, "Error opening PolyDriver check parameters\n");
            return false;
        }

        driver.view(pSerial);  
        if(!pSerial)
        {
            fprintf(stderr, "Error opening serial driver. Device not available\n");
            return false;
        }

        // ping the board 
        Bottle msg, reply;
        msg.addString("ping\n");
        pSerial->send(msg);
        Time::delay(0.1);
        pSerial->receive(reply);
        printf("Recevived '%s'\n", reply.toString().c_str());
        */
        pSerial = new SerialPort("/dev/ttyAMA0");
        pSerial->Open();
        if(!pSerial->IsOpen())
                return false;
      
        // ping the board 
        if(!ping())
        {
            printf("Cannot communicate with myCub control board!\n");
            pSerial->Close();
            return false;
        }

        fdDisplay = fopen("/dev/display", "w");
        if(!fdDisplay)
        {
            fprintf(stdout, "Cannot open display driver! (dose not exist)\n");            
            //return false;
        }

        battery_volt = (float)(getBatteryVolt()/1000.0);
        battery_volt = (battery_volt-6.0) / (7.4-6.0) * 100.0;
        if(fdDisplay) {
            fprintf(fdDisplay, "\%bat\%% %d\n",(int)battery_volt); 
            fflush(fdDisplay);
        }

        // start the controller
        printf("Staring motors controller...\n");
        if(!startController())
        {
            printf("Cannot start the motors controller!\n");
            pSerial->Close();
            return false;
        }

        sleep(1);
        printf("Setting the joints to the default positions...\n");
        homeAll();

        bool ret = cmdPort.open("/MyCubInterface/cmd:i");
        if(ret)
            attach(cmdPort);
        return ret;
    }

    bool attach(yarp::os::RpcServer& source) {
        return this->yarp().attachAsServer(source);
    }

    double getPeriod() {
        makeRealTime();
        return 5.0; // run every 5 s 
    }
    
    bool updateModule() {
        
        //update battery plug state 
        /*
        int32_t cur = getBatteryCurrent();
        if((cur >= 0) != battery_plugged )
        {
            battery_plugged = (cur > 0);
            if(fdDisplay) {            
                bool ret = (fprintf(fdDisplay, "\%%plug\%% %d\n", (battery_plugged==true)?1:0) > 0); 
                fflush(fdDisplay);
            }     
        }
        */

        // update battery voltage status
        //printf("updateModule(): battery : %s\n", str.c_str());
        battery_volt += (float)(getBatteryVolt()/1000.0);
        if(battery_volt_count++ >= 12)
        {
            battery_volt /= battery_volt_count;
            battery_volt = (battery_volt-6.0) / (7.4-6.0) * 100;
            if(fdDisplay) {
                bool ret = (fprintf(fdDisplay, "\%%bat\%% %d\n",(int)battery_volt) > 0); 
                fflush(fdDisplay);
            }    
            battery_volt_count = 0;
            battery_volt = 0.0;
        }
        return true; 
    }

    /**
     * ping the control board
     * @return true/false on success/failure
     */
    virtual bool ping() {
        serMutex.lock();
        while(serBusy) Time::delay(0.1);
        serBusy = true;        
        pSerial->Write("ping\n");
        bool ret = (string(pSerial->ReadLine(5000)) == "[ok]");
        serBusy = false;
        serMutex.unlock();
        return ret;
    }

    /**
     * start motor controller
     * @return true/false on success/failure
     */
    virtual bool startController() {
        serMutex.lock();
        while(serBusy) Time::delay(0.1);
        serBusy = true;        
        pSerial->Write("startControl\n");
        bool ret = (string(pSerial->ReadLine(5000)) == "[ok]");
        serBusy = false;
        serMutex.unlock();
        return ret;
    }

    /**
     * stop motor controller
     * @return true/false on success/failure
     */
    virtual bool stopController() {
        serMutex.lock();
        while(serBusy) Time::delay(0.1);
        serBusy = true;        
        pSerial->Write("stopControl\n");
        bool ret = (string(pSerial->ReadLine(5000)) == "[ok]");
        serBusy = false;
        serMutex.unlock();
        return ret;
    }

    /**
     * move all joints to the home position
     * @return true/false on success/failure
     */
    virtual bool homeAll() {
        vector<int32_t> pos(4);
        vector<int32_t> speed(4);
        pos[0] = pos[1] = pos[2] = pos[3] = 0;
        speed[0] = speed[1] = speed[2] = speed[3] = 3000;
        return gotoPoseAll(pos, speed); 
    }

    /**
     * Set joint pos
     * @joint joint id (0..3)
     * @pos   joint pos (0..90)
     * @return true/false on success/failure
     */
    virtual bool setPose(const int32_t joint, const int32_t pos) {
        if(!checkJointIDs(joint) || !checkJointLimits(pos)) return false;
        serMutex.lock();
        while(serBusy) Time::delay(0.1);
        serBusy = true;
        char cmd[64];
        sprintf(cmd, "setPose %d %d\n", joint, (int)(pos*JOINTS_SCALE+JOINTS_MIN));
        pSerial->Write(cmd);
        bool ret = (string(pSerial->ReadLine(5000)) == "[ok]");
        serBusy = false;
        serMutex.unlock();
        return ret;
    }

    /**
    * get joint pos
    * @joint joint id (0..3)
    * @return joint pos
    */
    virtual int32_t getPose(const int32_t joint) {
       if(!checkJointIDs(joint)) return -1;
        serMutex.lock();
        while(serBusy) Time::delay(0.1);
        serBusy = true;
        char cmd[64];
        sprintf(cmd, "getPose %d\n", joint);
        pSerial->Write(cmd);
        int32_t pos = atoi(pSerial->ReadLine(5000));
        serBusy = false;
        serMutex.unlock();
        return (int)((pos-JOINTS_MIN+1)/JOINTS_SCALE);
    }

    /**
     * get all joint poses
     * @return joint poses
     */
    virtual std::vector<int32_t>  getPoseAll() {
        std::vector<int32_t> poses; 
        for(int i=0; i<4; i++)
            poses.push_back(getPose(i));
        return poses;
    }

    /**
     * Move a joint to position with time duration
     * @joint joint id (0..3)
     * @pos   joint pos (0..90)
     * @t     time duration in ms
     * @return true/false on success/failure
     */
    virtual bool gotoPose(const int32_t joint, 
                            const int32_t pos, const int32_t t = 100) {
        if(!checkJointIDs(joint) || !checkJointLimits(pos)) return false;

        serMutex.lock();
        while(serBusy) Time::delay(0.1);
        serBusy = true;
        char cmd[64];
        sprintf(cmd, "gotoPose %d %d %d\n", 
                joint, (int)(pos*JOINTS_SCALE+JOINTS_MIN), t);
        pSerial->Write(cmd);
        bool ret = (string(pSerial->ReadLine(5000)) == "[ok]");
        serBusy = false;
        serMutex.unlock();
        return ret;
    }

    /**
     * Move a joint to position with time duration
     * and wait until it reaches the desired position
     * @joint joint id (0..3)
     * @pos   joint pos (0..90)
     * @t     time duration in ms
     * @return true/false on success/failure
     */
    virtual bool gotoPoseSync(const int32_t joint, 
                                const int32_t pos, const int32_t t = 100) {
        gotoPose(joint, pos, t);
    }

    /**
     * Set all joint pos
     * @poses a vector of joint poses (must have 4 elements)
     * @return true/false on success/failure
     */
    virtual bool setPoseAll(const std::vector<int32_t> & poses) {
        if(poses.size() < 4)
            return false;

        serMutex.lock();
        while(serBusy) Time::delay(0.1);
        serBusy = true;
        
        bool ret = true;
        for(int i=0; i<poses.size(); i++)
        {
            if(checkJointLimits(poses[i])) {
                char cmd[64];
                sprintf(cmd, "setPose %d %d\n", 
                       i, 
                       (int)(poses[i]*JOINTS_SCALE+JOINTS_MIN));
                pSerial->Write(cmd);
                ret &= (string(pSerial->ReadLine(5000)) == "[ok]");
            } 
            else 
                ret = false;
        }
        serBusy = false;
        serMutex.unlock();
        return ret;
    }

    /**
     * move all joint pos with desired duration
     * @poses a vector of joint poses (must have 4 elements)
     * @times a vector of time duration (must have 4 elements)
     * @return true/false on success/failure
     */
      virtual bool gotoPoseAll(const std::vector<int32_t> & poses, 
                               const std::vector<int32_t> & times){
        if((poses.size() < 4) || (times.size() != poses.size()))
            return false;

        serMutex.lock();
        while(serBusy) Time::delay(0.1);
        serBusy = true;
        
        bool ret = true;
        for(int i=0; i<poses.size(); i++)
        {
            if(checkJointLimits(poses[i])) {
                char cmd[64];
                sprintf(cmd, "gotoPose %d %d %d\n", 
                       i, (int)(poses[i]*JOINTS_SCALE+JOINTS_MIN), times[i]);
                pSerial->Write(cmd);
                ret &= (string(pSerial->ReadLine(5000)) == "[ok]");
            }
            else
                ret = false;
        }
        serBusy = false;
        serMutex.unlock();
        return ret;
      }

    /**
     * get range-finder value
     * @id the sensor id started from 0
     * @return true/false on success/failure
     */
    virtual int32_t getDistance(const int32_t id){
        serMutex.lock();
        while(serBusy) Time::delay(0.1);
        serBusy = true;
        char cmd[64];
        sprintf(cmd, "getDistance %d\n", id);
        pSerial->Write(cmd);
        int32_t dist = atoi(pSerial->ReadLine(5000));
        serBusy = false;
        serMutex.unlock();
        return dist;
    }

    /**
     * get digital compass value
     * @return true/false on success/failure
     */
    virtual std::vector<int32_t>  getOrientation() {
        serMutex.lock();
        while(serBusy) Time::delay(0.1);
        serBusy = true;
        pSerial->Write("getOrientation\n");
        string ret = pSerial->ReadLine(5000);
        serBusy = false;
        serMutex.unlock();
        std::vector<int32_t> orit(3);
        sscanf(ret.c_str(), "%d %d %d", &orit[0], &orit[1], &orit[2]);
        return orit;
    }

    /**
    * get absolute heading
    * @return heading angle
    */
    virtual int32_t getHeading() {
        std::vector<int32_t> orit(3);
        orit = getOrientation();
        // Hold the module so that Z is pointing 'up' and you can measure the heading with x&y
        // Calculate heading when the magnetometer is level, then correct for signs of axis.
        float heading = atan2(orit[1], orit[0]);

        // Once you have your heading, you must then add your 'Declination Angle', which is the 'Error' of the magnetic field in your location.
        // Find yours here: http://www.magnetic-declination.com/
        // Mine is: -13* 2' W, which is ~13 Degrees, or (which we need) 0.22 radians
        // If you cannot find your Declination, comment out these two lines, your compass will be slightly off.
        heading += 0.22;
        
        // Correct for when signs are reversed.
        if(heading < 0)
            heading += 2*M_PI;

        // Check for wrap due to addition of declination.
        if(heading > 2*M_PI)
            heading -= 2*M_PI;
        return (int32_t) (heading * 180.0/M_PI);
    }

    /**
     * get battery voltage status
     * @return true/false on success/failure
     */
    virtual int32_t getBatteryVolt(){
        serMutex.lock();
        while(serBusy) Time::delay(0.1);
        serBusy = true;
        pSerial->Write("getBatteryVolt\n");
        int32_t volt = atoi(pSerial->ReadLine(5000));
        serBusy = false;
        serMutex.unlock();
        return volt;
    }

    /**
     * get battery current status
     * @return true/false on success/failure
     */
      virtual int32_t getBatteryCurrent() { 
        serMutex.lock();
        while(serBusy) Time::delay(0.1);
        serBusy = true;
        pSerial->Write("getBatteryCurrent\n");
        int32_t amp = atoi(pSerial->ReadLine(5000));
        serBusy = false;
        serMutex.unlock();
        return amp;
      }

    /*
    bool respond(const Bottle &command, Bottle &reply) {
        reply.clear();        
        if (command.size() < 2 ){
            reply.addString("[error]");
            return true;
        }

        if(command.get(0).asString() == "ping" && 
            command.get(1).asString() == "board") 
        {
            serMutex.lock();
            while(serBusy) Time::delay(0.1);
            serBusy = true;
            pSerial->Write("ping\n");
            reply.clear();
            reply.addString(pSerial->ReadLine(5000));
            serBusy = false;
            serMutex.unlock();
            return true;
        }
        else if(command.get(0).asString() == "set" && 
            command.get(1).asString() == "pos") 
        {
            if(command.size() < 4) {
                reply.clear();
                reply.addString("[error]");
                return true;
            }
            serMutex.lock();
            while(serBusy) Time::delay(0.1);
            serBusy = true;
            char cmd[64];
            sprintf(cmd, "setPose %d %d\n", command.get(2).asInt(), command.get(3).asInt());
            pSerial->Write(cmd);
            reply.clear();
            reply.addString(pSerial->ReadLine(5000));
            serBusy = false;
            serMutex.unlock();
            return true;
        }
        else if(command.get(0).asString() == "goto" && 
            command.get(1).asString() == "pos") 
        {
            if(command.size() < 4) {
                reply.clear();
                reply.addString("[error]");
                return true;
            }
            serMutex.lock();
            while(serBusy) Time::delay(0.1);
            serBusy = true;
            char cmd[64];
            if(command.size() > 4 )
                sprintf(cmd, "gotoPose %d %d %d\n", 
                        command.get(2).asInt(), 
                        (int)(command.get(3).asInt()*JOINTS_SCALE+JOINTS_MIN), 
                        command.get(4).asInt());
            else
                sprintf(cmd, "gotoPose %d %d\n", 
                       command.get(2).asInt(), 
                       (int)(command.get(3).asInt()*JOINTS_SCALE+JOINTS_MIN));
            pSerial->Write(cmd);
            reply.clear();
            reply.addString(pSerial->ReadLine(5000));
            serBusy = false;
            serMutex.unlock();
            return true;
        }
        else if(command.get(0).asString() == "goto" && 
            command.get(1).asString() == "syncpos") 
        {
            if(command.size() < 4) {
                reply.clear();
                reply.addString("[error]");
                return true;
            }
            serMutex.lock();
            while(serBusy) Time::delay(0.1);
            serBusy = true;
            char cmd[64];
            if(command.size() > 4 )
                sprintf(cmd, "gotoPoseSync %d %d %d\n",
                        command.get(2).asInt(), 
                        (int)(command.get(3).asInt()*JOINTS_SCALE+JOINTS_MIN), 
                        command.get(4).asInt());
            else
                sprintf(cmd, "gotoPoseSync %d %d\n",
                        command.get(2).asInt(), 
                        (int)(command.get(3).asInt()*JOINTS_SCALE+JOINTS_MIN)); 
            pSerial->Write(cmd);
            reply.clear();
            reply.addString(pSerial->ReadLine(5000));
            serBusy = false;
            serMutex.unlock();
            return true;
        }
        else if(command.get(0).asString() == "set" && 
            command.get(1).asString() == "posall") 
        {
            Bottle* poses = NULL;

            if((command.size() < 3) || 
               !command.get(2).asList() || 
               (command.get(2).asList()->size() != 4) ) {
                reply.clear();
                reply.addString("[error]");
                return true;
            }
            
            poses = command.get(2).asList(); 

            serMutex.lock();
            while(serBusy) Time::delay(0.1);
            serBusy = true;
            
            string rep;
            for(int i=0; i<4; i++)
            {
                char cmd[64];
                sprintf(cmd, "setPose %d %d\n", 
                       i, 
                       (int)(poses->get(i).asInt()*JOINTS_SCALE+JOINTS_MIN));
                pSerial->Write(cmd);
                rep = pSerial->ReadLine(5000);
            }
            reply.clear();
            reply.addString(rep);

            serBusy = false;
            serMutex.unlock();

            return true;
        }


        else if(command.get(0).asString() == "goto" && 
            command.get(1).asString() == "posall") 
        {
            Bottle* poses = NULL;
            Bottle* speeds = NULL;

            if((command.size() < 3) || 
               !command.get(2).asList() || 
               (command.get(2).asList()->size() != 4) ) {
                reply.clear();
                reply.addString("[error]");
                return true;
            }

            if((command.size() > 3) && 
               (!command.get(3).asList() || (command.get(3).asList()->size() != 4)) )
            {
                reply.clear();
                reply.addString("[error]");
                return true;
            }
             
            poses = command.get(2).asList(); 
            if(command.size() > 3 )
                speeds = command.get(3).asList(); 

            serMutex.lock();
            while(serBusy) Time::delay(0.1);
            serBusy = true;
            
            string rep;
            for(int i=0; i<4; i++)
            {
                char cmd[64];
                if(speeds != NULL)
                    sprintf(cmd, "gotoPose %d %d %d\n", 
                            i,
                            (int)(poses->get(i).asInt()*JOINTS_SCALE+JOINTS_MIN), 
                            speeds->get(i).asInt());
                else
                    sprintf(cmd, "gotoPose %d %d\n", 
                           i, 
                           (int)(poses->get(i).asInt()*JOINTS_SCALE+JOINTS_MIN));
                pSerial->Write(cmd);
                rep = pSerial->ReadLine(5000);
            }
            reply.clear();
            reply.addString(rep);

            serBusy = false;
            serMutex.unlock();

            return true;
        }
        else if(command.get(0).asString() == "get" && 
            command.get(1).asString() == "dist") 
        {
            if(command.size() < 3) {
                reply.clear();
                reply.addString("[error]");
                return true;
            }
            serMutex.lock();
            while(serBusy) Time::delay(0.1);
            serBusy = true;
            char cmd[64];
            sprintf(cmd, "getDistance %d\n", command.get(2).asInt());
            pSerial->Write(cmd);
            reply.clear();
            reply.addString(pSerial->ReadLine(5000));
            serBusy = false;
            serMutex.unlock();
            return true;
        }
        else if(command.get(0).asString() == "get" && 
            command.get(1).asString() == "head") 
        {
            serMutex.lock();
            while(serBusy) Time::delay(0.1);
            serBusy = true;
            pSerial->Write("getHeading\n");
            reply.clear();
            reply.addString(pSerial->ReadLine(5000));
            serBusy = false;
            serMutex.unlock();
            return true;
        }

        else if(command.get(0).asString() == "get" && 
            command.get(1).asString() == "meminfo") 
        {
            serMutex.lock();
            while(serBusy) Time::delay(0.1);
            serBusy = true;
            pSerial->Write("getMemoryInfo\n");
            reply.clear();
            reply.addString(pSerial->ReadLine(5000));
            serBusy = false;
            serMutex.unlock();
            return true;
        }
        else if(command.get(0).asString() == "get" && 
            command.get(1).asString() == "battery") 
        {
            if(command.size() < 3 || 
              (command.get(2).asString() != "volt" &&
               command.get(2).asString() != "amp")) {
                reply.clear();
                reply.addString("[error]");
                return true;
            }
            
            serMutex.lock();
            while(serBusy) Time::delay(0.1);
            serBusy = true;
            char cmd[64];
            if(command.get(2).asString() == "volt")
                sprintf(cmd, "getBatteryVolt\n");
            else
                sprintf(cmd, "getBatteryCurrent\n");
            pSerial->Write(cmd);
            reply.clear();
            reply.addString(pSerial->ReadLine(5000));
            serBusy = false;
            serMutex.unlock();
            return true;
        }
        else if(command.get(0).asString() == "get" && 
            command.get(1).asString() == "adc") 
        {
            if(command.size() < 5) {
                reply.clear();
                reply.addString("[error]");
                return true;
            }
            serMutex.lock();
            while(serBusy) Time::delay(0.1);
            serBusy = true;
            char cmd[64];
            sprintf(cmd, "getADC %d %d %d\n", command.get(2).asInt(), 
                    command.get(3).asInt(), command.get(4).asInt());
            pSerial->Write(cmd);
            reply.clear();
            reply.addString(pSerial->ReadLine());
            serBusy = false;
            serMutex.unlock();
            return true;
        }
        else
        {
            reply.addString("[error]");
            return true;
        }

        return true;
    }
    */

    bool interruptModule() { 
        cmdPort.interrupt();             // interupt the blocking read 
        return true; 
    }

    bool close() {
        cmdPort.close(); 
        //driver.close();
        if(fdDisplay)
            fclose(fdDisplay);
        printf("Stopping motors controller...\n");
        pSerial->Write("stopControl\n");
        std::string rep = pSerial->ReadLine(5000);
        return true; 
    } 

    void makeRealTime() {
	    if(!bIsRT) {
	        struct sched_param thread_param;
    	    thread_param.sched_priority = 30;
   	        pthread_setschedparam(pthread_self(), SCHED_FIFO, &thread_param);
            bIsRT = true;
	    }
    }


private:
	//PolyDriver driver;
    //ISerialDevice *pSerial;
    yarp::os::Mutex serMutex;
    bool serBusy; 
    SerialPort* pSerial; 
    FILE* fdDisplay;
    RpcServer cmdPort;
    bool bIsRT;
    double battery_volt;
    int battery_volt_count;
    bool battery_plugged;
};



int main(int argc, char* argv[])
{
    yarp::os::Network yarp;
    ResourceFinder rf;
    rf.configure(argc, argv);
    
    mlockall(MCL_CURRENT | MCL_FUTURE);
	struct sched_param sch_param;
	sch_param.__sched_priority = 80;
	if( sched_setscheduler(0, SCHED_FIFO, &sch_param) != 0 )
	{
		printf("sched_setscheduler failed.\n");
		return -1;
	}


    MyCubInterface module;
    if(!module.configure(rf)) {
        fprintf(stderr, "Error opening MyCubInterface\n");
        return -1;
    }

    printf("Press Ctrl+C to exit...\n"); fflush(stdout);
    module.runModule(); 

    return 0;
}

