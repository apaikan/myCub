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
#include <pthread.h>
#include <unistd.h>

#include <arch/board/board.h>
#include <mycub_interface.h>

extern "C"
{

/*
 * these function declarations are required by underlying drivers
 */
int servo_devinit(void);
int range_devinit(void);
int adclm4f_devinit(void);

/* very lowleve high precision delay loop */
void sysDelay(unsigned long ulCount) 
{                                      
    __asm("    subs    r0, #1\n"       
          "    bne     sysDelay\n"  
          "    bx      lr");           
}  

#define CTRLSTATE_STOP      1
#define CTRLSTATE_GOPOS     2
#define CTRLSTATE_WAITING   3
#define CTRLSTATE_STOPPING  4


/* controller thread's param */
typedef struct __controlparam_t {
    unsigned int joint;
    int targetPos;
    double time;
    int* curPos;
    servo_info_s* servo_drv;
    int fd_servo;
    unsigned int status;
    pthread_mutex_t mutex;
    pthread_cond_t  cond;
    void (*callback_routine) (int);
} controlparam_t;

static controlparam_t ctrl_params[4];
static pthread_t ctrl_threads[4];
pthread_mutex_t syncpos_mutex[4];
pthread_cond_t  syncpos_cond[4];


/* controller thread */
static void * controller(void *pParams) 
{
    controlparam_t* params = (controlparam_t *) pParams;
    bool should_run = true;
    while(should_run)
    {
        //printf("controller %d is waiting...\n", params->joint);
        pthread_mutex_lock(&(params->mutex));
        params->status = CTRLSTATE_WAITING;
        pthread_cond_wait(&(params->cond), &(params->mutex));
        pthread_mutex_unlock(&(params->mutex));

        switch(params->status) {
            case CTRLSTATE_STOP : {
                //printf("controller %d is stoping.\n", params->joint);              
                should_run = false;
                break;
            }                
            case CTRLSTATE_GOPOS : {
                //printf("controller %d is moving to pos %d with time %f ...\n", 
                //        params->joint, params->targetPos, (float)params->time);
                
                /* exclusive access to parameters */
                pthread_mutex_lock(&(params->mutex));
                int joint = params->joint;
                int pos = params->targetPos;
                int curPos = *(params->curPos);
                double t = params->time;
                int fd_servo = params->fd_servo;
                servo_info_s servo_drv = *params->servo_drv;
                pthread_mutex_unlock(&(params->mutex));
               
                unsigned long oneMicSec = SYSCLK_FREQUENCY / 1000000 / 3;
                unsigned long delayStep = oneMicSec * ((t * 1000000 / 2) / (abs(pos - curPos)));
                if(delayStep < 5000)
                    delayStep = 5000;
                
                if(pos > curPos) {
                    for(int i=curPos; i<pos; i++)
                    {       
                        servo_drv.pos = i;
                        ioctl(fd_servo, SERVOIOC_SETPOS, 
                              (unsigned long)((uintptr_t)&servo_drv));
                        sysDelay(delayStep);
                        /* exclusive access to parameters */
                        pthread_mutex_lock(&(params->mutex));
                        *(params->curPos) = i;
                        pthread_mutex_unlock(&(params->mutex));        
                    }
                }    
                else {
                    for(int i=curPos; i>pos; i--)
                    {       
                        servo_drv.pos = i;
                        ioctl(fd_servo, SERVOIOC_SETPOS, 
                              (unsigned long)((uintptr_t)&servo_drv));
                        sysDelay(delayStep);
                        /* exclusive access to parameters */
                        pthread_mutex_lock(&(params->mutex));
                        *(params->curPos) = i;
                        pthread_mutex_unlock(&(params->mutex));        
                    }
                }
                if(params->callback_routine)
                    params->callback_routine(joint);
                break;
            }
            
            case CTRLSTATE_WAITING :
                break;                  // nothing to be done here

            default: 
                break;
        };
    }        

    pthread_mutex_lock(&(params->mutex));
    params->status = CTRLSTATE_STOPPING;
    pthread_mutex_unlock(&(params->mutex));
    return NULL;
}   

void controllerCallback(int id)
{
    pthread_mutex_lock(&syncpos_mutex[id]);
    pthread_cond_signal(&syncpos_cond[id]);
    pthread_mutex_unlock(&syncpos_mutex[id]);
}


} // end of extern "C"

MyCubInterface::MyCubInterface(void)
{
    fd_servo = -1;
    fd_range = -1;
    fd_adc = -1;
    for(int i=0; i<4; i++)
        joints_pos[i] = 8;
}

MyCubInterface::~MyCubInterface()
{
    fini();
}

bool MyCubInterface::init(void)
{
    if(fd_servo < 0)
    {
        int ret = servo_devinit();
        if (ret != OK)
        {
            printf("MyCubInterface::init(): servo_devinit failed: %d\n", ret);
            return false;
        }

        fd_servo = open("/dev/servo0", O_RDONLY);
        if (fd_servo < 0)
        {
            printf("MyCubInterface::init(): servo driver open failed: %d\n", errno);
            fini();
            return false;
        }

        // Configure servo pins


        servo_drv[0].id   = 0;
        servo_drv[1].id   = 1;
        servo_drv[2].id   = 2;
        servo_drv[3].id   = 3;

        /*
        // front
        servo_drv[0].port = GPIO_PORTD;
        servo_drv[0].pin = GPIO_PIN_0;

        //right
        servo_drv[1].port = GPIO_PORTD;
        servo_drv[1].pin = GPIO_PIN_2;
        
        //back
        servo_drv[2].port = GPIO_PORTD;
        servo_drv[2].pin = GPIO_PIN_1;

        //left        
        servo_drv[3].port = GPIO_PORTD;
        servo_drv[3].pin = GPIO_PIN_3;
        
        for(int i=0; i<4; i++)
            ioctl(fd_servo, SERVOIOC_SETPINCONFIG, (unsigned long)((uintptr_t)&servo_drv[i]));
        */

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
            printf("MyCubInterface::init(): range_devinit failed: %d\n", ret);
            fini();
            return false;
        }

        fd_range = open("/dev/range0", O_RDONLY);
        if (fd_range < 0)
        {
            printf("MyCubInterface::init(): range finder open failed: %d\n", errno);
            fini();
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

    if(fd_adc < 0)
    {
        int ret = adclm4f_devinit();
        if (ret != OK)
        {
            printf("MyCubInterface::init(): adclm4f_devinit failed: %d\n", ret);
            fini();
            return false;
        }

        fd_adc = open("/dev/adclm4f0", O_RDONLY);
        if (fd_adc < 0)
        {
            printf("MyCubInterface::init(): adc driver open failed: %d\n", errno);
            fini();
            return false;
        }
    }


    // initializing and starting controller threads
    pthread_attr_t attr;
    struct sched_param sparam; 
    int status = pthread_attr_init(&attr);
    if (status != 0) {
        printf("MyCubInterface::int(): pthread_attr_init failed, status=%d\n", status);
        fini();
        return false;
    }
       
    //int prio_min = sched_get_priority_min(SCHED_RR);
    int prio_max = sched_get_priority_max(SCHED_RR);
    //int prio_mid = (prio_min + prio_max) / 2;
    sparam.sched_priority = prio_max;
    status = pthread_attr_setschedparam(&attr, &sparam);
    if (status != OK)
    {       
        printf("MyCubInterface::init(): pthread_attr_setschedparam failed, status=%d\n", status);
        fini();
        return false;
    }
    pthread_attr_destroy(&attr);

    // initialize thread params and start the threads
    for(int i=0; i<4; i++)
    {
        ctrl_params[i].joint = i;
        ctrl_params[i].targetPos = 8;
        ctrl_params[i].time = 1.0;    
        ctrl_params[i].curPos = &joints_pos[i];
        ctrl_params[i].fd_servo = fd_servo;
        ctrl_params[i].servo_drv = &servo_drv[i];
        ctrl_params[i].callback_routine = NULL;        

        status = pthread_mutex_init(&(ctrl_params[i].mutex), NULL);
        status |= pthread_mutex_init(&(syncpos_mutex[i]), NULL);
        if(status != 0) {
          printf("MyCubInterface::init(): ERROR pthread_mutex_init failed, status=%d\n", status);
          fini();
          return false;
        }

        status = pthread_cond_init(&(ctrl_params[i].cond), NULL);
        status |= pthread_cond_init(&(syncpos_cond[i]), NULL);
        if(status != 0)
        {
            printf("MyCubInterface::init(): ERROR pthread_condinit failed, status=%d\n", status);
        }

        status = pthread_create(&ctrl_threads[i], NULL, controller, &ctrl_params[i]);
        if (status != 0)
        {
            printf("MyCubInterface::init(): pthread_create failed, status=%d\n", status);
            fini();
            return false;
        }
    }


    return true;
}

void MyCubInterface::fini(void)
{
    for(int i=0; i<4; i++)
    {
        // first wait untile controllers goes to waiting state; 
        int controller_state;
        pthread_mutex_lock(&(ctrl_params[i].mutex));
        controller_state = ctrl_params[i].status;
        pthread_mutex_unlock(&(ctrl_params[i].mutex));
        while(controller_state != CTRLSTATE_STOPPING)
        {
            if(controller_state == CTRLSTATE_WAITING)
            {
                pthread_mutex_lock(&(ctrl_params[i].mutex));
                ctrl_params[i].status = CTRLSTATE_STOP;
                pthread_cond_signal(&(ctrl_params[i].cond));
                pthread_mutex_unlock(&(ctrl_params[i].mutex));
            }                
            usleep(100000);
            pthread_mutex_lock(&(ctrl_params[i].mutex));
            controller_state = ctrl_params[i].status;
            pthread_mutex_unlock(&(ctrl_params[i].mutex));
        }
        pthread_join(ctrl_threads[i], NULL);
        pthread_detach(ctrl_threads[i]);
        pthread_cancel(ctrl_threads[i]);

        pthread_mutex_destroy(&(ctrl_params[i].mutex));
        pthread_mutex_destroy(&(syncpos_mutex[i]));
        pthread_cond_destroy(&(ctrl_params[i].cond));
        pthread_cond_destroy(&(syncpos_cond[i]));
    }   

    for(int i=0; i<4; i++)
    {
        servo_drv[i].pos = 8;
        ioctl(fd_servo, SERVOIOC_SETPOS, (unsigned long)((uintptr_t)&servo_drv[i]));
    }
    
    sleep(2);

    if(fd_servo != -1) {
        close(fd_servo); fd_servo = -1;
    }

    if(fd_range != -1) {
        close(fd_range); fd_range = -1;
    }        

    if(fd_adc != -1) {
        close(fd_adc); fd_adc = -1;
    } 
}

bool MyCubInterface::setPose(const unsigned int joint, const int pos)
{
    if((pos<8) || (pos>160) || (joint > 3))
    {
        printf("MyCubInterface::gotoPoseSync(): Invalid joint or position value!\n");
        return false;
    }

    servo_drv[joint].pos = pos;
    ioctl(fd_servo, SERVOIOC_SETPOS, (unsigned long)((uintptr_t)&servo_drv[joint]));
    joints_pos[joint] = pos;
    return true;
}

// joint position and speed controller 
bool MyCubInterface::gotoPose(const unsigned int joint, const int pos, const double t)
{
    if((pos<8) || (pos>160) || (joint > 3))
    {
        printf("MyCubInterface::gotoPoseSync(): Invalid joint or position value!\n");
        return false;
    }        

    pthread_mutex_lock(&(ctrl_params[joint].mutex));
    ctrl_params[joint].targetPos = pos; 
    ctrl_params[joint].time = t;
    ctrl_params[joint].status = CTRLSTATE_GOPOS;
    pthread_cond_signal(&(ctrl_params[joint].cond));
    pthread_mutex_unlock(&(ctrl_params[joint].mutex));

    return true;
}


bool MyCubInterface::gotoPoseSync(const unsigned int joint, const int pos, const double t)
{
    if((pos<8) || (pos>160) || (joint > 3))
    {
        printf("MyCubInterface::gotoPoseSync(): Invalid joint or position value!\n");
        return false;
    }        

    pthread_mutex_lock(&(ctrl_params[joint].mutex));
    ctrl_params[joint].targetPos = pos; 
    ctrl_params[joint].time = t;
    ctrl_params[joint].status = CTRLSTATE_GOPOS;
    ctrl_params[joint].callback_routine = &controllerCallback;

    // get syncpose mutex
    pthread_mutex_lock(&syncpos_mutex[joint]);
    // signal the thread
    pthread_cond_signal(&(ctrl_params[joint].cond));
    // release thread mutex
    pthread_mutex_unlock(&(ctrl_params[joint].mutex));
    // wait for callback signal
    pthread_cond_wait(&syncpos_cond[joint], &syncpos_mutex[joint]);
    // release syncpos mutex
    pthread_mutex_unlock(&syncpos_mutex[joint]);

    /*
    int controller_state;
    pthread_mutex_lock(&(ctrl_params[i].mutex));
    controller_state = ctrl_params[i].status;
    pthread_mutex_unlock(&(ctrl_params[i].mutex));
    while(controller_state != CTRLSTATE_STOPPING)
           usleep(100000);
    */

    /*
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
    */
    return true;
}

int MyCubInterface::getPose(const unsigned int joint)
{
    if(joint > 3)
    {
        printf("MyCubInterface::getPos(): Invalid joint index!\n");
        return -1;
    }

    int curPos = -1;
    pthread_mutex_lock(&(ctrl_params[joint].mutex));
    curPos = joints_pos[joint];
    pthread_mutex_unlock(&(ctrl_params[joint].mutex));
    return curPos;
}

bool MyCubInterface::checkMotionDone(const unsigned int joint)
{
    int controller_state;
    pthread_mutex_lock(&(ctrl_params[joint].mutex));
    controller_state = ctrl_params[joint].status;
    pthread_mutex_unlock(&(ctrl_params[joint].mutex));
    return (controller_state !=  CTRLSTATE_GOPOS);
}

bool MyCubInterface::checkMotionDone(void)
{
    bool ret = true;
    for(int i=0; i<4; i++)
        ret &= checkMotionDone(i);
    return ret;
}

JointContext MyCubInterface::getContext(void)
{
    JointContext ctx;
    ctx.joint_0 = getPose(0);
    ctx.joint_1 = getPose(1);
    ctx.joint_2 = getPose(2);
    ctx.joint_3 = getPose(3);
    return ctx;
}
void MyCubInterface::setContext(JointContext &ctx)
{
    setPose(0, ctx.joint_0);
    setPose(1, ctx.joint_1);
    setPose(2, ctx.joint_2);
    setPose(3, ctx.joint_3);
    sleep(2);
}

// ultrasonic range finder 
int MyCubInterface::getDistance(const int id)
{
    char data[128];
    int ret = read(fd_range, data, sizeof(data));
    if(ret > 0)   
       return getRange(id, data);
    
    return -1;
}

int MyCubInterface::getRange(int id, const char* str)
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

int MyCubInterface::getRawAnalogData(const int channel, unsigned long freq,
                                     uint16_t* data, size_t len)
{
    adclm4f_info_s info;
    info.channel = channel;
    info.nsamples = len;
    info.freq = freq;
    ioctl(fd_adc, ADCIOC_SETCONF, (unsigned long)((uintptr_t)&info));
    return read(fd_adc, (char*)data, len*sizeof(uint16_t));
}

