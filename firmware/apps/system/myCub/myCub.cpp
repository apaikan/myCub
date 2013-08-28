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

#include <mycub_interface.h>

    
/****************************************************************************
 * Public Functions
 ****************************************************************************/
#define USEC_PER_MSEC 1000
#define MSEC_PER_SEC  1000
#define USEC_PER_SEC  (USEC_PER_MSEC * MSEC_PER_SEC)
#define SHORT_DELAY   (USEC_PER_SEC / 3)


#define FRONT_ARM   RIGHT_JOINT
#define BACK_ARM    LEFT_JOINT
#define RIGHT_ARM   FRONT_JOINT
#define LEFT_ARM    BACK_JOINT

extern "C"
{

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

    printf("-- myCub Demo --\n");  

    MyCubInterface mycub;
    mycub.init();

    JointContext ctx = mycub.getContext();
    mycub.gotoPose(FRONT_ARM, 160, 0.5);
    mycub.gotoPose(BACK_ARM, 100, 0.5);
    mycub.gotoPose(LEFT_ARM, 120, 0.5);
    mycub.gotoPose(RIGHT_ARM, 120, 0.5);
    while(!mycub.checkMotionDone()) sleep(1);
    
    should_stop = false;
    while(!should_stop)
    {
        int dist = mycub.getDistance(RIGHT_RANGE);
        if(dist == 0)
            printf("No obstacle in the range\n");
        else
            printf("Obstacle at distance %d\n", dist);

        /*
        int retry = 0;
        while(((dist = mycub.getDistance(RIGHT_RANGE)) <= 0) && (retry <10)) {        
            usleep(500000);
            retry++;
        }           
        if(dist<=0)
        {
            printf("Error in reading range finder %d \n", RIGHT_RANGE);
            //should_stop = true;
            //break;
        }
        */
        
        if((dist == 0) || (dist > 150))
        {
            mycub.gotoPoseSync(BACK_ARM, 100, 0.2);
            mycub.gotoPoseSync(FRONT_ARM, 8, 1.0);
            
            mycub.gotoPose(LEFT_ARM, 80, 0.2);
            mycub.gotoPose(RIGHT_ARM, 80, 0.2);        
            while(!mycub.checkMotionDone()) sleep(1);

            mycub.gotoPoseSync(FRONT_ARM, 160, 1.0);
            
            mycub.gotoPoseSync(BACK_ARM, 70, 0.2);      
            mycub.gotoPose(LEFT_ARM, 120, 0.2);
            mycub.gotoPose(RIGHT_ARM, 120, 0.2);        
            while(!mycub.checkMotionDone()) sleep(1);
        }
        else
            sleep(1);   //wait a bit
    }

    mycub.fini();
    printf("done!\n");

    return 0;
}

}
