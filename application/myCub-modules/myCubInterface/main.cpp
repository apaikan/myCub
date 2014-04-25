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

using namespace yarp::os;
//using namespace yarp::dev;
using namespace std;

#define JOINTS_SPEED            0.200     // sec
#define JOINTS_SCALE            2.2687
#define JOINTS_MIN              8

class MyCubInterface: public RFModule {
public:

    MyCubInterface(void) {
        bIsRT = false;
        pSerial = NULL;
    }
    
    ~MyCubInterface(void) {
        if(pSerial)
            delete pSerial;
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
        pSerial->Write("ping\n");
        std::string rep = pSerial->ReadLine();
        if(rep != "[ok]" )
        {
            printf("Cannot communicate with myCub control board!\n");
            pSerial->Close();
            return false;          
        }

        bool ret = cmdPort.open("/MyCubInterface/cmd:i");
        if(ret)
            attach(cmdPort);
        return ret;
    }

    double getPeriod() {
        //makeRealTime();
        return 10; // run every 10 s 
    }
    
    bool updateModule() {
 
        return true; 
    }

/*
 setPose <joint> <pos> 
  getPose <joint> 
  gotoPose <joint> <pos> [time] 
  gotoPoseSync <joint> <pos> [time] 
  moveFront [time] 
  moveBack [time] 
  moveRight [time] 
  moveLeft [time] 
  getDistance <sonar> 
  getBatteryVolt 
  getADC <channel> <freq> <samples>  
*/
    bool respond(const Bottle &command, Bottle &reply) {
        if (command.size() < 2 ){
            reply.addString("[error]");
            return true;
        }

        if(command.get(0).asString() == "set" && 
            command.get(1).asString() == "pos") 
        {
            if(command.size() < 4) {
                reply.clear();
                reply.addString("[error]");
                return true;
            }
            char cmd[64];
            sprintf(cmd, "setPose %d %d\n", command.get(2).asInt(), command.get(3).asInt());
            pSerial->Write(cmd);
            reply.clear();
            reply.addString(pSerial->ReadLine());
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
            reply.addString(pSerial->ReadLine());
            return true;
        }
        else if(command.get(0).asString() == "goto" && 
            command.get(1).asString() == "poss") 
        {
            if(command.size() < 4) {
                reply.clear();
                reply.addString("[error]");
                return true;
            }
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
            reply.addString(pSerial->ReadLine());
            return true;
        }
        else if(command.get(0).asString() == "move" && 
            command.get(1).asString() == "front") 
        {
            char cmd[64];
            if(command.size() > 2 )
                sprintf(cmd, "moveFront %d\n", command.get(2).asInt()); 
            else
                sprintf(cmd, "moveFront\n");
            pSerial->Write(cmd);
            reply.clear();
            reply.addString(pSerial->ReadLine());
            return true;
        }
        else if(command.get(0).asString() == "move" && 
            command.get(1).asString() == "back") 
        {
            char cmd[64];
            if(command.size() > 2 )
                sprintf(cmd, "moveBack %d\n", command.get(2).asInt()); 
            else
                sprintf(cmd, "moveBack\n");
            pSerial->Write(cmd);
            reply.clear();
            reply.addString(pSerial->ReadLine());
            return true;
        }
        else if(command.get(0).asString() == "move" && 
            command.get(1).asString() == "right") 
        {
            char cmd[64];
            if(command.size() > 2 )
                sprintf(cmd, "moveRight %d\n", command.get(2).asInt()); 
            else
                sprintf(cmd, "moveRight\n");
            pSerial->Write(cmd);
            reply.clear();
            reply.addString(pSerial->ReadLine());
            return true;
        }
        else if(command.get(0).asString() == "move" && 
            command.get(1).asString() == "left") 
        {
            char cmd[64];
            if(command.size() > 2 )
                sprintf(cmd, "moveLeft %d\n", command.get(2).asInt()); 
            else
                sprintf(cmd, "moveLeft\n");
            pSerial->Write(cmd);
            reply.clear();
            reply.addString(pSerial->ReadLine());
            return true;
        }
        else if(command.get(0).asString() == "move" && 
            command.get(1).asString() == "stop") 
        {
            char cmd[64];
            sprintf(cmd, "stop\n");
            pSerial->Write(cmd);
            reply.clear();
            reply.addString(pSerial->ReadLine());
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
            char cmd[64];
            sprintf(cmd, "getDistance %d\n", command.get(2).asInt());
            pSerial->Write(cmd);
            reply.clear();
            reply.addString(pSerial->ReadLine());
            return true;
        }
        else if(command.get(0).asString() == "get" && 
            command.get(1).asString() == "battery") 
        {
            char cmd[64];
            sprintf(cmd, "getBatteryVolt\n");
            pSerial->Write(cmd);
            reply.clear();
            reply.addString(pSerial->ReadLine());
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
            char cmd[64];
            sprintf(cmd, "getADC %d %d %d\n", command.get(2).asInt(), 
                    command.get(3).asInt(), command.get(4).asInt());
            pSerial->Write(cmd);
            reply.clear();
            reply.addString(pSerial->ReadLine());
            return true;
        }
        else
        {
            reply.addString("[error]");
            return true;
        }

        return true;
    }

    bool interruptModule() { 
        cmdPort.interrupt();             // interupt the blocking read 
        return true; 
    }

    bool close() {
        cmdPort.close(); 
        //driver.close();
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
	//PolyDriver driver;
    //ISerialDevice *pSerial;
    SerialPort* pSerial; 
    RpcServer cmdPort;
    bool bIsRT;
};



int main(int argc, char* argv[])
{
    yarp::os::Network yarp;
    ResourceFinder rf;
    rf.configure(argc, argv);
    
    //mlockall(MCL_CURRENT | MCL_FUTURE);

    MyCubInterface module;
    if(!module.configure(rf)) {
        fprintf(stderr, "Error opening MyCubInterface\n");
        return -1;
    }

    printf("Press Ctrl+C to exit...\n"); fflush(stdout);
    module.runModule(); 

    return 0;
}

