/*
 * rt-thread.cpp
 *
 *  Created on: Feb 16, 2011
 *      Author: ali
 */

#include <stdio.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <sched.h>
#include <time.h>
#include "nuttx_thread.h"


NxThread::NxThread()
{ 
    run = NULL;
    thread = -1;
	policy = SCHED_RR;
	priority  = (sched_get_priority_min(policy) + sched_get_priority_max(policy)) / 2;
	period = 100000;    // 100ms
    state = NXTHREAD_STOPPED;
}

NxThread::~NxThread()
{
    stop();
}


bool NxThread::start(void)
{
    mutex.lock();        
    if(state != NXTHREAD_STOPPED)
    {
        mutex.unlock();
        return true;
    }        
    mutex.unlock();

    if(!run)
    {
        printf("ERROR: not any run callback is associated to the thread\n");
        return false;
    }

	int ret;	
    struct sched_param sch_param;
    mutex.lock();    
    bshould_stop = false;
    bshould_suspend = false;
    mutex.unlock();

	pthread_attr_init(&attr);
	sch_param.sched_priority = priority;
	
    if( (ret=pthread_attr_setschedpolicy(&attr, policy)) != OK)
	{
		printf("ERROR: pthread_attr_setschedpolicy failed with error %d\n", ret);
        return false;
	}

	if( (ret=pthread_attr_setschedparam(&attr, &sch_param)) != OK )
	{
		printf("ERROR: pthread_attr_setschedparam failed with error %d\n", ret);
        return false;
	}

	if ( (ret = pthread_create( &thread,
								&attr,
								thread_func, (FAR void*)this)) != OK )
	{
		printf("ERROR: pthread_create failed with error %d", ret);
		return false;
	}

	return true;
}


void NxThread::stop(void)
{
    mutex.lock();        
    if(state == NXTHREAD_STOPPED)
    {
        mutex.unlock();
        return;
    }        
    bshould_stop = true;
    mutex.unlock();

    resume(); 

    pthread_addr_t result;
    pthread_join(thread, &result);
    pthread_attr_destroy(&attr);
    state = NXTHREAD_STOPPED;
}

bool NxThread::suspend(void)
{
    mutex.lock();        
    if(state != NXTHREAD_RUNNING)
    {
        mutex.unlock();
        return false;
    }
    bshould_suspend = true;
    mutex.unlock();
    return true;
}

bool NxThread::resume(void)
{
    mutex.lock();        
    if(state != NXTHREAD_SUSPENDED)
    {
        mutex.unlock();
        return false;
    }
    bshould_suspend = false;
    mutex.unlock();  
    event.set();
    return true;
}

nxthread_state_t NxThread::getState(void)
{
    nxthread_state_t st;
    mutex.lock();       
    st = state;
    mutex.unlock();
    return st;
}

void *thread_func(void *arg)
{
	NxThread* pthread = (NxThread*) arg;

    pthread->mutex.lock();
    bool bshould_stop = pthread->bshould_stop;
    bool bshould_suspend = pthread->bshould_suspend;
    pthread->state = NXTHREAD_RUNNING;
    pthread->mutex.unlock();

	while(!bshould_stop)
	{
        if(bshould_suspend)
        {
            pthread->mutex.lock();
            pthread->state = NXTHREAD_SUSPENDED;
            pthread->mutex.unlock();

            pthread->event.wait();
            pthread->event.reset();

            pthread->mutex.lock();
            bshould_stop = pthread->bshould_stop;
            pthread->mutex.unlock();
            if(bshould_stop)
                break;

            pthread->mutex.lock();
            pthread->state = NXTHREAD_RUNNING;
            pthread->mutex.unlock();
        }

        if(pthread->run)
            pthread->run(pthread->arg);
        if(pthread->period > 0)    
		    usleep(pthread->period);
		//sched_yield();
        pthread->mutex.lock();
        bshould_stop = pthread->bshould_stop;
        bshould_suspend = pthread->bshould_suspend;
        pthread->mutex.unlock();
	}
		
	return NULL;
}


