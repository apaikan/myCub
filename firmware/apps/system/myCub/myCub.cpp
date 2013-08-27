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

/* Collects and reports system information.
 *
 * TODO: Gather information also from low-level devices, kernel/sched, clock,
 *   and further reporting as: sysinfo rtc, or sysinfo sched, ... with
 *   sysinfo help to report all of the options.
 */

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <mycub_interface.h>

    
/****************************************************************************
 * Public Functions
 ****************************************************************************/

extern "C"
{

int myCub_main(int argc, char *argv[])
{
    printf("-- myCub Demo --\n");  

    MyCubInterface mycub;
    mycub.init();

    
   
    /*
    sleep(1);
    mycub.gotoPoseSync(FRONT_JOINT, 160, 2.0);
    mycub.gotoPoseSync(BACK_JOINT, 100, 3.0);
    mycub.gotoPoseSync(RIGHT_JOINT, 100, 3.0);
    mycub.gotoPoseSync(LEFT_JOINT, 100, 3.0);
    printf("go pose done\n");
    
    sleep(1);
    */
    /*
    mycub.setPose(FRONT_JOINT, 8);
    mycub.setPose(BACK_JOINT, 8);
    mycub.setPose(RIGHT_JOINT, 8);
    mycub.setPose(LEFT_JOINT, 8);
    sleep(3);
    */
  
    JointContext ctx = mycub.getContext();
    mycub.gotoPose(FRONT_JOINT, 160, 0.5);
    mycub.gotoPose(BACK_JOINT, 100, 0.5);
    mycub.gotoPose(LEFT_JOINT, 120, 0.5);
    mycub.gotoPose(RIGHT_JOINT, 120, 0.5);
    while(!mycub.checkMotionDone()) sleep(1);
    
    for(int i=0; i<10; i++)
    {
        mycub.gotoPoseSync(BACK_JOINT, 100, 0.2);
        mycub.gotoPoseSync(FRONT_JOINT, 8, 1.0);
        
        mycub.gotoPose(LEFT_JOINT, 80, 0.2);
        mycub.gotoPose(RIGHT_JOINT, 80, 0.2);        
        while(!mycub.checkMotionDone()) sleep(1);

        mycub.gotoPoseSync(FRONT_JOINT, 160, 1.0);
        
        mycub.gotoPoseSync(BACK_JOINT, 70, 0.2);      
        mycub.gotoPose(LEFT_JOINT, 120, 0.2);
        mycub.gotoPose(RIGHT_JOINT, 120, 0.2);        
        while(!mycub.checkMotionDone()) sleep(1);
    }

    /*
    mycub.gotoPose(FRONT_JOINT, 130, 1.0);
    while(!mycub.checkMotionDone()) sleep(1);

    mycub.setContext(ctx);
    mycub.gotoPose(FRONT_JOINT, 130, 0.5);
    mycub.gotoPose(BACK_JOINT, 130, 0.5);
    while(!mycub.checkMotionDone()) sleep(1);

    mycub.setContext(ctx);
    mycub.gotoPose(FRONT_JOINT, 130, 0.3);
    mycub.gotoPose(BACK_JOINT, 130, 0.3);
    mycub.gotoPose(RIGHT_JOINT, 130, 0.3);
    while(!mycub.checkMotionDone()) sleep(1);

    mycub.setContext(ctx);
    mycub.gotoPose(FRONT_JOINT, 130, 0.0);
    mycub.gotoPose(BACK_JOINT, 130, 0.0);
    mycub.gotoPose(RIGHT_JOINT, 130, 0.0);
    mycub.gotoPose(LEFT_JOINT, 130, 0.0);
    while(!mycub.checkMotionDone()) sleep(1);
    */

    /*
    for(int i=0; i<3; i++)
    {
        printf("\n up \n");
        mycub.gotoPose(FRONT_JOINT, 130, 1.5);
        mycub.gotoPose(BACK_JOINT, 130, 1.5);
        mycub.gotoPose(RIGHT_JOINT, 130, 1.5);
        mycub.gotoPose(LEFT_JOINT, 130, 1.5);
        while(!mycub.checkMotionDone())
            sleep(1);
        printf("\n down \n");
        mycub.gotoPose(FRONT_JOINT, 8, 1.5);
        mycub.gotoPose(BACK_JOINT, 8, 1.5);
        mycub.gotoPose(RIGHT_JOINT, 8, 1.5);
        mycub.gotoPose(LEFT_JOINT, 8, 1.5);
        while(!mycub.checkMotionDone())
            sleep(1);
    }    
    */

    mycub.fini();

    /*
    srv_right.pos = 8;
    srv_left.pos = 8;
    ioctl(fd_servo, SERVOIOC_SETPOS, (unsigned long)((uintptr_t)&srv_right));
    ioctl(fd_servo, SERVOIOC_SETPOS, (unsigned long)((uintptr_t)&srv_left));
 
    while(1)
    {
        char data[128];
        ret = read(fd_range, data, sizeof(data));
        if(ret > 0)
        {
            unsigned int dist = getRange(0, data);
            if(dist > 0 && dist < 300)
            {
                uint32_t value = (uint32_t) (8.0 + ((180.0 - 8.0) / (300.0 - 5.0)) * (300-dist - 5.0));
                //printf("dist:%d, value:%d\n", dist, value);
                srv_front.pos = value;
                srv_back.pos = value;
                ioctl(fd_servo, SERVOIOC_SETPOS, (unsigned long)((uintptr_t)&srv_front));
                ioctl(fd_servo, SERVOIOC_SETPOS, (unsigned long)((uintptr_t)&srv_back));     
            }
        }
        usleep(1000);
    }
    */

   /*    
    srv_front.pos = 8;
    srv_back.pos = 8;
    srv_right.pos = 8;
    srv_left.pos = 8;
    ioctl(fd_servo, SERVOIOC_SETPOS, (unsigned long)((uintptr_t)&srv_front));
    ioctl(fd_servo, SERVOIOC_SETPOS, (unsigned long)((uintptr_t)&srv_back));
    ioctl(fd_servo, SERVOIOC_SETPOS, (unsigned long)((uintptr_t)&srv_right));
    ioctl(fd_servo, SERVOIOC_SETPOS, (unsigned long)((uintptr_t)&srv_left));
           
    // all streching
    printf("Streching...\n");  
    for(i=8; i<160; i+=2)       
    {
        srv_front.pos = i;
        srv_back.pos = i;
        srv_right.pos = i;
        srv_left.pos = i;
        ioctl(fd_servo, SERVOIOC_SETPOS, (unsigned long)((uintptr_t)&srv_front));
        ioctl(fd_servo, SERVOIOC_SETPOS, (unsigned long)((uintptr_t)&srv_back));
        ioctl(fd_servo, SERVOIOC_SETPOS, (unsigned long)((uintptr_t)&srv_right));
        ioctl(fd_servo, SERVOIOC_SETPOS, (unsigned long)((uintptr_t)&srv_left)); 
        usleep(20000);
    }

    for(i=160; i>8; i-=2)
    {
        srv_front.pos = i;
        srv_back.pos = i;
        srv_right.pos = i;
        srv_left.pos = i;
        ioctl(fd_servo, SERVOIOC_SETPOS, (unsigned long)((uintptr_t)&srv_front));
        ioctl(fd_servo, SERVOIOC_SETPOS, (unsigned long)((uintptr_t)&srv_back));
        ioctl(fd_servo, SERVOIOC_SETPOS, (unsigned long)((uintptr_t)&srv_right));
        ioctl(fd_servo, SERVOIOC_SETPOS, (unsigned long)((uintptr_t)&srv_left)); 
        usleep(20000);
    }      

    srv_front.pos = 40;
    srv_back.pos = 40;
    srv_right.pos = 40;
    srv_left.pos = 40;
    ioctl(fd_servo, SERVOIOC_SETPOS, (unsigned long)((uintptr_t)&srv_front));
    ioctl(fd_servo, SERVOIOC_SETPOS, (unsigned long)((uintptr_t)&srv_back));
    ioctl(fd_servo, SERVOIOC_SETPOS, (unsigned long)((uintptr_t)&srv_right));
    ioctl(fd_servo, SERVOIOC_SETPOS, (unsigned long)((uintptr_t)&srv_left));

    sleep(2);
*/

    printf("done!\n");

    return 0;
}

}
