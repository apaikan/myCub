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
        battery_volt = 0.0;
        battery_volt_count = 0;
        battery_plugged = false;
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
        std::string rep = pSerial->ReadLine(5000);
        if(rep != "[ok]" )
        {
            printf("Cannot communicate with myCub control board! (got '%s')\n", rep.c_str());
            pSerial->Close();
            return false;
        }

        fdDisplay = fopen("/dev/display", "w");
        if(!fdDisplay)
        {
            fprintf(stdout, "Cannot open display driver! (dose not exist)\n");            
            //return false;
        }
        char cmd[64];
        sprintf(cmd, "getBatteryVolt\n");
        pSerial->Write(cmd);
        battery_volt = (float)(atoi(pSerial->ReadLine(5000))/1000.0);
        battery_volt = (battery_volt-6.0) / (7.4-6.0) * 100.0;
        if(fdDisplay) {
            fprintf(fdDisplay, "\%bat\%% %d\n",(int)battery_volt); 
            fflush(fdDisplay);
        }

        // start the controller
        printf("Staring motors controller...\n");
        pSerial->Write("startControl\n");
        rep = pSerial->ReadLine(10000);
        if(rep != "[ok]" )
        {
            printf("Cannot start the motors controller!\n");
            pSerial->Close();
            return false;
        }
        sleep(1);
        printf("Setting the joints to the default positions...\n");
        // set the joints to the default positions
        for(int i=0; i<4; i++)
        {
            char cmd[64];
            sprintf(cmd, "gotoPose %d %d %d\n", 
                    i, (int)JOINTS_MIN+2, 3000);
            pSerial->Write(cmd);
            rep = pSerial->ReadLine(10000);
            printf("Joint %d ... %s\n", i, rep.c_str());
            Time::delay(0.2);
        }

        battery_volt = 0.0;
        serBusy = false;

        bool ret = cmdPort.open("/MyCubInterface/cmd:i");
        if(ret)
            attach(cmdPort);
        return ret;
    }

    double getPeriod() {
        makeRealTime();
        return 5.0; // run every 5 s 
    }
    
    bool updateModule() {
        serMutex.wait();
        while(serBusy) Time::delay(0.1);
        serBusy = true;
        char cmd[64];
        bool ret;

        //update battery plug state 
        sprintf(cmd, "getBatteryCurrent\n");
        pSerial->Write(cmd);
        string str = pSerial->ReadLine(5000);
        if((atof(str.c_str()) >= 0.0) != battery_plugged )
        {
            battery_plugged = (atof(str.c_str()) > 0.0);
            if(fdDisplay) {            
                ret = (fprintf(fdDisplay, "\%%plug\%% %d\n", (battery_plugged==true)?1:0) > 0); 
                fflush(fdDisplay);
            }     
        }    
        Time::delay(0.05);

        // update battery voltage status
        sprintf(cmd, "getBatteryVolt\n");
        pSerial->Write(cmd);
        str = pSerial->ReadLine(5000);
        //printf("updateModule(): battery : %s\n", str.c_str());
        battery_volt += (float)(atoi(str.c_str())/1000.0);
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

        serBusy = false;
        serMutex.post();
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
        reply.clear();        
        if (command.size() < 2 ){
            reply.addString("[error]");
            return true;
        }

        if(command.get(0).asString() == "ping" && 
            command.get(1).asString() == "board") 
        {
            serMutex.wait();
            while(serBusy) Time::delay(0.1);
            serBusy = true;
            pSerial->Write("ping\n");
            reply.clear();
            reply.addString(pSerial->ReadLine(5000));
            serBusy = false;
            serMutex.post();
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
            serMutex.wait();
            while(serBusy) Time::delay(0.1);
            serBusy = true;
            char cmd[64];
            sprintf(cmd, "setPose %d %d\n", command.get(2).asInt(), command.get(3).asInt());
            pSerial->Write(cmd);
            reply.clear();
            reply.addString(pSerial->ReadLine(5000));
            serBusy = false;
            serMutex.post();
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
            serMutex.wait();
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
            serMutex.post();
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
            serMutex.wait();
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
            serMutex.post();
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

            serMutex.wait();
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
            serMutex.post();

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

            serMutex.wait();
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
            serMutex.post();

            return true;
        }

        else if(command.get(0).asString() == "move" && 
            command.get(1).asString() == "front") 
        {
            serMutex.wait();
            while(serBusy) Time::delay(0.1);
            serBusy = true;
            char cmd[64];
            if(command.size() > 2 )
                sprintf(cmd, "moveFront %d\n", command.get(2).asInt()); 
            else
                sprintf(cmd, "moveFront\n");
            pSerial->Write(cmd);
            reply.clear();
            reply.addString(pSerial->ReadLine(5000));
            serBusy = false;
            serMutex.post();
            return true;
        }
        else if(command.get(0).asString() == "move" && 
            command.get(1).asString() == "back") 
        {
            serMutex.wait();
            while(serBusy) Time::delay(0.1);
            serBusy = true;
            char cmd[64];
            if(command.size() > 2 )
                sprintf(cmd, "moveBack %d\n", command.get(2).asInt()); 
            else
                sprintf(cmd, "moveBack\n");
            pSerial->Write(cmd);
            reply.clear();
            reply.addString(pSerial->ReadLine(5000));
            serBusy = false;
            serMutex.post();
            return true;
        }
        else if(command.get(0).asString() == "move" && 
            command.get(1).asString() == "right") 
        {
            serMutex.wait();
            while(serBusy) Time::delay(0.1);
            serBusy = true;
            char cmd[64];
            if(command.size() > 2 )
                sprintf(cmd, "moveRight %d\n", command.get(2).asInt()); 
            else
                sprintf(cmd, "moveRight\n");
            pSerial->Write(cmd);
            reply.clear();
            reply.addString(pSerial->ReadLine(5000));
            serBusy = false;
            serMutex.post();
            return true;
        }
        else if(command.get(0).asString() == "move" && 
            command.get(1).asString() == "left") 
        {
            serMutex.wait();
            while(serBusy) Time::delay(0.1);
            serBusy = true;
            char cmd[64];
            if(command.size() > 2 )
                sprintf(cmd, "moveLeft %d\n", command.get(2).asInt()); 
            else
                sprintf(cmd, "moveLeft\n");
            pSerial->Write(cmd);
            reply.clear();
            reply.addString(pSerial->ReadLine(5000));
            serBusy = false;
            serMutex.post();
            return true;
        }
        else if(command.get(0).asString() == "move" && 
            command.get(1).asString() == "stop") 
        {
            serMutex.wait();
            while(serBusy) Time::delay(0.1);
            serBusy = true;
            char cmd[64];
            sprintf(cmd, "stop\n");
            pSerial->Write(cmd);
            reply.clear();
            reply.addString(pSerial->ReadLine(5000));
            serBusy = false;
            serMutex.post();
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
            serMutex.wait();
            while(serBusy) Time::delay(0.1);
            serBusy = true;
            char cmd[64];
            sprintf(cmd, "getDistance %d\n", command.get(2).asInt());
            pSerial->Write(cmd);
            reply.clear();
            reply.addString(pSerial->ReadLine(5000));
            serBusy = false;
            serMutex.post();
            return true;
        }
        else if(command.get(0).asString() == "get" && 
            command.get(1).asString() == "head") 
        {
            serMutex.wait();
            while(serBusy) Time::delay(0.1);
            serBusy = true;
            pSerial->Write("getHeading\n");
            reply.clear();
            reply.addString(pSerial->ReadLine(5000));
            serBusy = false;
            serMutex.post();
            return true;
        }

        else if(command.get(0).asString() == "get" && 
            command.get(1).asString() == "meminfo") 
        {
            serMutex.wait();
            while(serBusy) Time::delay(0.1);
            serBusy = true;
            pSerial->Write("getMemoryInfo\n");
            reply.clear();
            reply.addString(pSerial->ReadLine(5000));
            serBusy = false;
            serMutex.post();
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
            
            serMutex.wait();
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
            serMutex.post();
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
            serMutex.wait();
            while(serBusy) Time::delay(0.1);
            serBusy = true;
            char cmd[64];
            sprintf(cmd, "getADC %d %d %d\n", command.get(2).asInt(), 
                    command.get(3).asInt(), command.get(4).asInt());
            pSerial->Write(cmd);
            reply.clear();
            reply.addString(pSerial->ReadLine());
            serBusy = false;
            serMutex.post();
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
    	    thread_param.sched_priority = 81;
   	        pthread_setschedparam(pthread_self(), SCHED_FIFO, &thread_param);
            bIsRT = true;
	    }
    }


private:
	//PolyDriver driver;
    //ISerialDevice *pSerial;
    yarp::os::Semaphore serMutex;
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

