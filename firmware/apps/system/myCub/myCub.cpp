/****************************************************************************
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/


/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

#include <mycub_interface.h>
#include <mycub_walk.h>
//#include <mycub_fun.h>
#include <nuttx_thread.h>

/*
#include <termios.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <fcntl.h>
*/

#include <apps/readline.h>
  
/****************************************************************************
 * Public Functions
 ****************************************************************************/
#define USEC_PER_MSEC 1000
#define MSEC_PER_SEC  1000
#define USEC_PER_SEC  (USEC_PER_MSEC * MSEC_PER_SEC)
#define SHORT_DELAY   (USEC_PER_SEC / 3)


#define FRONT_ARM       RIGHT_JOINT
#define BACK_ARM        LEFT_JOINT
#define RIGHT_ARM       FRONT_JOINT
#define LEFT_ARM        BACK_JOINT

#define MAX_CMD_NUM     16
#define MAX_CMD_LEN     32

#define BATTERY_CRITICAL_LEVE   7000 // 7.0VV

#define MYCUB_LOGO  "\
                  ____      _      \n\
 _ __ ___  _   _ / ___|   _| |__   \n\
| '_ ` _ \\| | | | |  | | | | '_ \\  \n\
| | | | | | |_| | |__| |_| | |_) | \n\
|_| |_| |_|\\__, |\\____\\__,_|_.__/  \n\
           |___/                   \n"

#define HELP_MESSAGE        "\
Commands:\n\
  setPose <joint> <pos> \n\
  getPose <joint> \n\
  gotoPose <joint> <pos> [time] \n\
  gotoPoseSync <joint> <pos> [time] \n\
  moveFront [time] \n\
  moveBack [time] \n\
  moveRight [time] \n\
  moveLeft [time] \n\
  getDistance <sonar> \n\
  getBatteryVolt \n\
  getADC <channel> <freq> <samples>  \n\
  help \n"

extern "C"
{

static char mycub_cmd[MAX_CMD_NUM][MAX_CMD_LEN];

size_t parse_string(char* str)
{
    char* token = strtok(str, " ");
    int i = 0;
    while((token != NULL) && (i<MAX_CMD_NUM))
    {
        strcpy(mycub_cmd[i++], token);
        token = strtok(NULL, " ");
    }
    return i;
}

bool should_stop;
void siguser_action(int signo, siginfo_t *siginfo, void *arg)
{
    if(signo == SIGUSR1)
    {
        printf("SIGUSR1 received\n");
        should_stop = true;
    }
}

/* dancer threads */
void dancer_body(void *pParams) 
{
    static unsigned long int cnt = 0;
    MyCubInterface* mycub = (MyCubInterface*) pParams;

    if(cnt%4)
    {
        mycub->gotoPose(FRONT_JOINT, 50, 0.2);
        mycub->gotoPose(BACK_JOINT, 10, 0.2);
    }        
    else
    {
        mycub->gotoPose(FRONT_JOINT, 10, 0.2);
        mycub->gotoPose(BACK_JOINT, 50, 0.2);        
    }     
    
    /*
    unsigned long freq = 200; 
    size_t samples = 50;
    uint16_t* data = (uint16_t*) malloc(samples*sizeof(uint16_t));
    int ret = mycub->getRawAnalogData(0, freq, data, samples);
    long avg = 0.0;
    for(int i=0; i<ret; i++)
        avg += data[i];
    avg /= ret;

    uint16_t max = 0;;
    for(int i=0; (i<ret) ; i++)
        if((data[i]-avg) > max)
            max = data[i]-avg;
    int pos = 50 + ((150 - 50) / (100-10)) * max;
    printf("max:%d, pos %d\n", max, pos);
    mycub->setPose(RIGHT_JOINT, pos);
    mycub->setPose(LEFT_JOINT, pos);
    free(data);
    */
    int pos = (int) (((double)rand() / (double)MAX_RAND) * 100.0) + 60;
    mycub->gotoPose(RIGHT_JOINT, pos, 0.1);
    mycub->gotoPose(LEFT_JOINT, pos, 0.1);
    cnt++;
}   


int myCub_main(int argc, char *argv[])
{
    NxThread dancer1;
    srand(23);

    struct sigaction act;
    struct sigaction oact;
    int status;

    memset(&act, 0, sizeof(struct sigaction));
    act.sa_sigaction = siguser_action;
    act.sa_flags     = SA_SIGINFO;
    (void)sigemptyset(&act.sa_mask);
    status = sigaction(SIGUSR1, &act, &oact);
    if(status != 0)
    {
        fprintf(stderr, "Failed to install SIGUSR1 handler, errno=%d\n", errno);
        exit(2);
    }

    bool bInteractive = false;
    if((argc > 1) && strcmp(argv[1], "--interactive") == 0)
        bInteractive = true;

    if(bInteractive)
        printf("%s\n", MYCUB_LOGO);

    MyCubInterface mycub;
    mycub.init();
    MyCubWalker::init();

    char buf[256];
    should_stop = false;
    while(!should_stop) 
    {
        if(bInteractive)
        {
            int power = (int) (mycub.getBatteryVolt() * 1000);
            if( power < BATTERY_CRITICAL_LEVE )
                printf("[WARNING] Battery volatge is critically low (%d.%d V) \n", 
                        power/1000, power%1000);
            //ssize_t len = readline_serial(comfd, buf, 256, "[myCub] ");
            printf("[myCub] "); fflush(stdout);
        }    

        ssize_t len;
        if(bInteractive)
            len = readline(buf, 256, stdin, stdout);
        else
            len = readline(buf, 256, stdin, NULL);
        if(len > 0)
        {
            buf[len-1] = '\0';
            int n = parse_string(buf);        
            if(n == 0)
                continue;
            // executing commands
            if(strcmp(mycub_cmd[0], "exit")==0 )
            {
                should_stop = true;            
            }                
            else if(strcmp(mycub_cmd[0], "setPose")==0 ) {
                if(n >= 3)
                {
                    mycub.setPose(atoi(mycub_cmd[1]), atoi(mycub_cmd[2]));
                    printf("[ok]\n"); fflush(stdout);
                }
                else
                    printf("[error]\n"); fflush(stdout);
            }
            else if(strcmp(mycub_cmd[0], "getPose")==0 ) {
                if(n >= 2)
                {
                    int pos = mycub.getPose(atoi(mycub_cmd[1]));
                    printf("%d\n", pos); fflush(stdout);
                }
                else
                    printf("[error]\n"); fflush(stdout);
            }
            else if(strcmp(mycub_cmd[0], "getDistance")==0 ) {
                if(n >= 2)
                {
                    int dist = mycub.getDistance(atoi(mycub_cmd[1]));
                    printf("%d\n", dist); fflush(stdout);
                }
                else
                    printf("[error]\n"); fflush(stdout);
            }


            else if(strcmp(mycub_cmd[0], "gotoPose")==0 ) {
                if(n >= 3)
                {
                    double t = 0.0;
                    if(n>=4)
                        t = atoi(mycub_cmd[3]);
                    mycub.gotoPose(atoi(mycub_cmd[1]), atoi(mycub_cmd[2]), t);
                    printf("[ok]\n"); fflush(stdout);
                }
                else
                    printf("[error]\n"); fflush(stdout);
            }

            else if(strcmp(mycub_cmd[0], "gotoPoseSync")==0 ) {
                if(n >= 3)
                {
                    double t = 0.0;
                    if(n>=4)
                        t = atoi(mycub_cmd[3]);
                    mycub.gotoPoseSync(atoi(mycub_cmd[1]), atoi(mycub_cmd[2]), t);
                    printf("[ok]\n"); fflush(stdout);
                }
                else
                    printf("[error]\n"); fflush(stdout);
            }            
            else if(strcmp(mycub_cmd[0], "moveFront")==0 ) {
               double t = 0.0;
                if(n>=2)
                    t = atoi(mycub_cmd[1]);
                MyCubWalker::moveFront(mycub, t);
                printf("[ok]\n"); fflush(stdout);
            }
            else if(strcmp(mycub_cmd[0], "moveBack")==0 ) {
               double t = 0.0;
                if(n>=2)
                    t = atoi(mycub_cmd[1]);
                MyCubWalker::moveBack(mycub, t);
                printf("[ok]\n"); fflush(stdout);
            }
            else if(strcmp(mycub_cmd[0], "moveRight")==0 ) {
               double t = 0.0;
                if(n>=2)
                    t = atoi(mycub_cmd[1]);
                MyCubWalker::moveRight(mycub, t);
                printf("[ok]\n"); fflush(stdout);
            }
            else if(strcmp(mycub_cmd[0], "moveLeft")==0 ) {
               double t = 0.0;
                if(n>=2)
                    t = atoi(mycub_cmd[1]);
                MyCubWalker::moveLeft(mycub, t);
                printf("[ok]\n"); fflush(stdout);
            }
            else if(strcmp(mycub_cmd[0], "dance")==0 ) {
                //MyCubFun::dance(mycub);
                dancer1.stop();
                mycub.gotoPose(FRONT_JOINT, 60, 0.5);
                mycub.gotoPose(BACK_JOINT, 60, 0.5);
                mycub.gotoPose(RIGHT_JOINT, 150, 0.5);
                mycub.gotoPose(LEFT_JOINT, 150, 0.5);

                dancer1.setCallback(dancer_body, &mycub);
                dancer1.setPeriod(500000);
                dancer1.start();
                printf("[ok]\n"); fflush(stdout);
            }
            else if(strcmp(mycub_cmd[0], "stop")==0 ) {
                MyCubWalker::stop();
                dancer1.stop();
                printf("[ok]\n"); fflush(stdout);
            }
            else if(strcmp(mycub_cmd[0], "getADC")==0 ) {
                if(n >= 4)
                {
                    int channel = atoi(mycub_cmd[1]);
                    unsigned long freq = atol(mycub_cmd[2]);
                    size_t samples = atoi(mycub_cmd[3]);
                    uint16_t* data = (uint16_t*) malloc(samples*sizeof(uint16_t));
                    int ret = mycub.getRawAnalogData(channel, freq, data, samples);
                    for(int i=0; i<ret; i++) {
                        printf("%d, ", data[i]); fflush(stdout);
                    }
                    if(bInteractive)
                        printf("\n[ok]\n"); fflush(stdout);
                    free(data);
                }
                else
                    printf("[error]\n"); fflush(stdout);
            }
            else if(strcmp(mycub_cmd[0], "getBatteryVolt")==0 ) {
                    int volt = (int) (mycub.getBatteryVolt() * 1000);                    
                    printf("%d.%d\n", volt/1000, volt%1000); fflush(stdout);
            }
            else if(strcmp(mycub_cmd[0], "help")==0 ) {
                    if(bInteractive)
                        printf("\n%s\n", HELP_MESSAGE); fflush(stdout);
            }
            else
            {
                if(bInteractive) {
                    printf("[error] type 'help' for more commands.\n", HELP_MESSAGE); fflush(stdout);
                }    
                else {
                    printf("[error]\n"); fflush(stdout);
                }    
            }
        }
        //usleep(100000);
	}

    MyCubWalker::fini();
    mycub.fini();
    if(bInteractive)
        printf("bye!\n");
    else
        printf("[ok]\n"); fflush(stdout);

    return 0;
}

} // extern "C"

