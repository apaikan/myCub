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

#include <mycub_walk.h>

extern "C"
{

typedef struct __wall_param_t {
    MyCubInterface * mycub;
    double t;
    int front;
    int back;
    int right;
    int left;
    bool should_stop;
    bool running;
} walk_param_t;


static walk_param_t walk_param; 
static pthread_t walk_thread;
pthread_mutex_t params_mutex;


/* controller thread */
void * walker(void *pParams) 
{
    walk_param_t* params = (walk_param_t *) pParams;

    pthread_mutex_lock(&(params_mutex));
    //double t = params->t;
    int front = params->front;
    int back = params->back;
    int right = params->right;
    int left = params->left;
    bool should_stop = params->should_stop;    
    params->running = true;
    pthread_mutex_unlock(&(params_mutex));

    if(should_stop)
    {
        pthread_mutex_lock(&(params_mutex));
        params->running = false;
        pthread_mutex_unlock(&(params_mutex));
        return NULL;
    }

    params->mycub->gotoPose(front, 160, 0.5);
    params->mycub->gotoPose(back, 100, 0.5);
    params->mycub->gotoPose(left, 120, 0.5);
    params->mycub->gotoPose(right, 120, 0.5);
    while(!params->mycub->checkMotionDone()) usleep(300000);

    while(!should_stop)
    {
        pthread_mutex_lock(&(params_mutex));
        //t = params->t;
        front = params->front;
        back = params->back;
        right = params->right;
        left = params->left;
        params->running = true;
        should_stop = params->should_stop;
        pthread_mutex_unlock(&(params_mutex));

        params->mycub->gotoPoseSync(back, 100, 0.1);
        if(should_stop) break;
        params->mycub->gotoPoseSync(front, 8, 0.5);        
        if(should_stop) break;

        params->mycub->gotoPose(left, 70, 0.1);
        params->mycub->gotoPose(right, 70, 0.1);        
        while(!params->mycub->checkMotionDone()) usleep(100000);
        if(should_stop) break;

        params->mycub->gotoPoseSync(front, 160, 0.5);
        if(should_stop) break;

        params->mycub->gotoPoseSync(back, 80, 0.1);
        if(should_stop) break;
        params->mycub->gotoPose(left, 120, 0.1);
        params->mycub->gotoPose(right, 120, 0.1);        
        while(!params->mycub->checkMotionDone()) usleep(100000);
        if(should_stop) break;
    }        

    pthread_mutex_lock(&(params_mutex));
    params->running = false;
    pthread_mutex_unlock(&(params_mutex));
    return NULL;
}   

} // end of extern "C"


bool MyCubWalker::init(void)
{
    walk_param.running = false;
    int status = pthread_mutex_init(&params_mutex, NULL);
    if(status != 0) {
        printf("MyCubWalker::init(): ERROR pthread_mutex_init failed, status=%d\n", status);
        return false;
    }        
    return true;
}

void MyCubWalker::fini(void)
{
    MyCubWalker::stop();
    pthread_mutex_destroy(&params_mutex);
}

void MyCubWalker::stop(void)
{
    pthread_mutex_lock(&(params_mutex));
    walk_param.should_stop = true;
    pthread_mutex_unlock(&(params_mutex));

    pthread_join(walk_thread, NULL);
    pthread_detach(walk_thread);
    pthread_cancel(walk_thread);
}

bool MyCubWalker::moveFront(MyCubInterface& mycub, double t)
{
    MyCubWalker::stop();
   
    walk_param.mycub = &mycub;
    walk_param.t = t;
    walk_param.front = FRONT_JOINT;
    walk_param.back = BACK_JOINT; 
    walk_param.right = RIGHT_JOINT; 
    walk_param.left = LEFT_JOINT; 
    walk_param.should_stop = false;

    int status = pthread_create(&walk_thread, NULL, walker, &walk_param);
    if(status != 0)
    {
        printf("MyCubWalker::moveFront(): pthread_create failed, status=%d\n", status);
        return false;
    }
    return true;
}

bool MyCubWalker::moveBack(MyCubInterface& mycub, double t)
{
    MyCubWalker::stop();
   
    walk_param.mycub = &mycub;
    walk_param.t = t;
    walk_param.front = BACK_JOINT;
    walk_param.back = FRONT_JOINT; 
    walk_param.right = RIGHT_JOINT; 
    walk_param.left = LEFT_JOINT; 
    walk_param.should_stop = false;

    int status = pthread_create(&walk_thread, NULL, walker, &walk_param);
    if(status != 0)
    {
        printf("MyCubWalker::moveBack(): pthread_create failed, status=%d\n", status);
        return false;
    }
    return true;
}

bool MyCubWalker::moveRight(MyCubInterface& mycub, double t)
{
    MyCubWalker::stop();
   
    walk_param.mycub = &mycub;
    walk_param.t = t;
    walk_param.front = RIGHT_JOINT;
    walk_param.back = LEFT_JOINT; 
    walk_param.right = FRONT_JOINT; 
    walk_param.left = BACK_JOINT; 
    walk_param.should_stop = false;

    int status = pthread_create(&walk_thread, NULL, walker, &walk_param);
    if(status != 0)
    {
        printf("MyCubWalker::moveRight(): pthread_create failed, status=%d\n", status);
        return false;
    }
    return true;
}

bool MyCubWalker::moveLeft(MyCubInterface& mycub, double t)
{
    MyCubWalker::stop();
   
    walk_param.mycub = &mycub;
    walk_param.t = t;
    walk_param.front = LEFT_JOINT;
    walk_param.back = RIGHT_JOINT; 
    walk_param.right = FRONT_JOINT; 
    walk_param.left = BACK_JOINT; 
    walk_param.should_stop = false;

    int status = pthread_create(&walk_thread, NULL, walker, &walk_param);
    if(status != 0)
    {
        printf("MyCubWalker::moveLeft(): pthread_create failed, status=%d\n", status);
        return false;
    }
    return true;
}


