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
#include <string.h>

#include<arch/board/board.h>
#include <mycub_interface.h>

/*
 * these function declarations are required by underlying drivers
 */
extern "C"
{

int servo_devinit(void);
int range_devinit(void);
 
void sysDelay(unsigned long ulCount) 
{                                      
    __asm("    subs    r0, #1\n"       
          "    bne     sysDelay\n"  
          "    bx      lr");           
}  

}

myCubInterface::myCubInterface(void)
{
    fd_servo = -1;
    fd_range = -1;
    for(int i=0; i<4; i++)
        joints_pos[i] = 8;
}

myCubInterface::~myCubInterface()
{
    fini();
}

bool myCubInterface::init(void)
{
    if(fd_servo < 0)
    {
        int ret = servo_devinit();
        if (ret != OK)
        {
            printf("myCubInterface::init(): servo_devinit failed: %d\n", ret);
            return false;
        }

        fd_servo = open("/dev/servo0", O_RDONLY);
        if (fd_servo < 0)
        {
            printf("myCubInterface::init(): servo driver open failed: %d\n", errno);
            return false;
        }

        // Configure servo pins

        // front
        servo_drv[0].id   = 0;
        servo_drv[0].port = GPIO_PORTD;
        servo_drv[0].pin = GPIO_PIN_0;

        //right
        servo_drv[1].id   = 1;
        servo_drv[1].port = GPIO_PORTD;
        servo_drv[1].pin = GPIO_PIN_2;
        
        //back
        servo_drv[2].id   = 2;
        servo_drv[2].port = GPIO_PORTD;
        servo_drv[2].pin = GPIO_PIN_1;

        //left        
        servo_drv[3].id   = 3;
        servo_drv[3].port = GPIO_PORTD;
        servo_drv[3].pin = GPIO_PIN_3;

        for(int i=0; i<4; i++)
            ioctl(fd_servo, SERVOIOC_SETPINCONFIG, (unsigned long)((uintptr_t)&servo_drv[i]));
        
        for(int i=0; i<4; i++)
        {
            servo_drv[i].pos = joints_pos[i];
            ioctl(fd_servo, SERVOIOC_SETPOS, (unsigned long)((uintptr_t)&servo_drv[i]));
        }            
        usleep(500000);
    }

    if(fd_range < 0 )
    {
        int ret = range_devinit();
        if (ret != OK)
        {
            printf("myCubInterface::init(): range_devinit failed: %d\n", ret);
            return false;
        }

        fd_range = open("/dev/range0", O_RDONLY);
        if (fd_range < 0)
        {
            printf("myCubInterface::init(): range finder open failed: %d\n", errno);
            return false;
        }

        // Configure range finder
        
        // front
        //...
       
        //right
        range_drv[1].id = 1;
        range_drv[1].trig_port = GPIO_PORTE;
        range_drv[1].trig_pin = GPIO_PIN_2; 
        range_drv[1].echo_port = GPIO_PORTE;
        range_drv[1].echo_pin = GPIO_PIN_1; 
        ioctl(fd_range, RANGEIOC_ATTACHE, (unsigned long)((uintptr_t)&range_drv[1]));
    }

    return true;
}

void myCubInterface::fini(void)
{
    if(fd_servo != -1) {
        close(fd_servo); fd_servo = -1;
    }

    if(fd_range != -1)
    {
        close(fd_range); fd_range = -1;
    }        
}

// joint position and speed controller 
bool myCubInterface::gotoPose(const unsigned int joint, const int pos, const double t)
{
    // TODO: asynchrone  thread-based implementaion     
    return true;
}


bool myCubInterface::gotoPoseSync(const unsigned int joint, const int pos, const double t)
{
    if((pos<8) || (pos>160) || (joint > 3))
    {
        printf("myCubInterface::gotoPoseSync(): Invalid joint or position value!\n");
        return false;
    }        

    unsigned long oneMicSec = SYSCLK_FREQUENCY / 1000000 / 3;
    int curPos = joints_pos[joint];
    unsigned long delayStep = oneMicSec * ((t * 1000000 / 3) / (abs(pos - curPos))) / 3;
    if(delayStep < 5000)
        delayStep = 5000;
    if(pos > curPos) {
        for(int i=curPos; i<pos; i++)
        {       
            servo_drv[joint].pos = i;
            ioctl(fd_servo, SERVOIOC_SETPOS, (unsigned long)((uintptr_t)&servo_drv[joint]));
            joints_pos[joint] = pos;
            sysDelay(delayStep);
        }
    }    
    else {
        for(int i=curPos; i>pos; i--)
        {       
            servo_drv[joint].pos = i;
            ioctl(fd_servo, SERVOIOC_SETPOS, (unsigned long)((uintptr_t)&servo_drv[joint]));
            joints_pos[joint] = pos;
            sysDelay(delayStep);
        }
    }
    
    return true;
}

int myCubInterface::getPose(const unsigned int joint)
{
    if(joint > 3)
    {
        printf("myCubInterface::getPos(): Invalid joint index!\n");
        return -1;
    } 
    return joints_pos[joint];
}


// ultrasonic range finder 
int myCubInterface::getDistance(const int id)
{
    char data[128];
    int ret = read(fd_range, data, sizeof(data));
    if(ret > 0)   
       return getRange(0, data);
    
    return -1;
}

int myCubInterface::getRange(int id, const char* str)
{
    char dummy[32];
    sprintf(dummy, "(%d ", id);
    char* ptr = strstr(str, dummy);
    if(!ptr)
        return 0;
    ptr += strlen(dummy)-1; 
    char* ptr2 = strchr(ptr, ')');
    if(!ptr2)
        return 0;
    int i = 0;
    while(ptr++ != ptr2)
        dummy[i++] = *ptr;
    dummy[i-1] = NULL;        
    return atoi(dummy);
}


