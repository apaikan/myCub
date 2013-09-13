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


int myCub_main(int argc, char *argv[])
{
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

    printf("-- myCub Intreface --\n");  

    MyCubInterface mycub;
    mycub.init();
    MyCubWalker::init();

    char buf[256];
    should_stop = false;
    while(!should_stop) 
    {
        //ssize_t len = readline_serial(comfd, buf, 256, "[myCub] ");
        printf("[myCub] "); fflush(stdout);
        ssize_t len = readline(buf, 256, stdin, stdout);
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
                    printf("[ok]"); fflush(stdout);
                }
                else
                    printf("[error]"); fflush(stdout);
            }
            else if(strcmp(mycub_cmd[0], "getPose")==0 ) {
                if(n >= 2)
                {
                    int pos = mycub.getPose(atoi(mycub_cmd[1]));
                    printf("%d\n", pos); fflush(stdout);
                }
                else
                    printf("[error]"); fflush(stdout);
            }
            else if(strcmp(mycub_cmd[0], "getDistance")==0 ) {
                if(n >= 2)
                {
                    int dist = mycub.getDistance(atoi(mycub_cmd[1]));
                    printf("%d\n", dist); fflush(stdout);
                }
                else
                    printf("[error]"); fflush(stdout);
            }


            else if(strcmp(mycub_cmd[0], "gotoPose")==0 ) {
                if(n >= 3)
                {
                    double t = 0.0;
                    if(n>=4)
                        t = atoi(mycub_cmd[3]) / 1000.0;
                    mycub.gotoPose(atoi(mycub_cmd[1]), atoi(mycub_cmd[2]), t);
                    printf("[ok]"); fflush(stdout);
                }
                else
                    printf("[error]"); fflush(stdout);
            }

            else if(strcmp(mycub_cmd[0], "gotoPoseSync")==0 ) {
                if(n >= 3)
                {
                    double t = 0.0;
                    if(n>=4)
                        t = atoi(mycub_cmd[3]) / 1000.0;
                    mycub.gotoPoseSync(atoi(mycub_cmd[1]), atoi(mycub_cmd[2]), t);
                    printf("[ok]"); fflush(stdout);
                }
                else
                    printf("[error]"); fflush(stdout);
            }            
            else if(strcmp(mycub_cmd[0], "moveFront")==0 ) {
               double t = 0.0;
                if(n>=2)
                    t = atoi(mycub_cmd[1]) / 1000.0;
                MyCubWalker::moveFront(mycub, t);
                printf("[ok]"); fflush(stdout);
            }
            else if(strcmp(mycub_cmd[0], "moveBack")==0 ) {
               double t = 0.0;
                if(n>=2)
                    t = atoi(mycub_cmd[1]) / 1000.0;
                MyCubWalker::moveBack(mycub, t);
                printf("[ok]"); fflush(stdout);
            }
            else if(strcmp(mycub_cmd[0], "moveRight")==0 ) {
               double t = 0.0;
                if(n>=2)
                    t = atoi(mycub_cmd[1]) / 1000.0;
                MyCubWalker::moveRight(mycub, t);
                printf("[ok]"); fflush(stdout);
            }
            else if(strcmp(mycub_cmd[0], "moveLeft")==0 ) {
               double t = 0.0;
                if(n>=2)
                    t = atoi(mycub_cmd[1]) / 1000.0;
                MyCubWalker::moveLeft(mycub, t);
                printf("[ok]"); fflush(stdout);
            }
            else if(strcmp(mycub_cmd[0], "stop")==0 ) {
                MyCubWalker::stop();
                printf("[ok]"); fflush(stdout);
            }
            else if(strcmp(mycub_cmd[0], "getADC")==0 ) {
                if(n >= 4)
                {
                    int channel = atoi(mycub_cmd[1]);
                    unsigned long freq = atol(mycub_cmd[2]);
                    size_t len = atoi(mycub_cmd[3]);
                    uint16_t* data = (uint16_t*) malloc(len*sizeof(uint16_t));
                    int ret = mycub.getRawAnalogData(channel, freq, data, len);
                    for(int i=0; i<ret; i++) {
                        printf("%d, ", data[i]); fflush(stdout);
                    }
                    printf("\n[ok]"); fflush(stdout);
                    free(data);
                }
                else
                    printf("[error]"); fflush(stdout);
            }


        }
        //usleep(100000);
	}

    MyCubWalker::fini();
    mycub.fini();
    printf("done!\n");

    return 0;
}

}
