/*
 * rt-thread.h
 *
 *  Created on: Feb 16, 2011
 *      Author: Ali Paikan
 */

#ifndef RTTHREAD_H_
#define RTTHREAD_H_

#include <pthread.h>
#include "nuttx_mutex.h"
#include "nuttx_event.h"

typedef enum {
    NXTHREAD_STOPPED,
    NXTHREAD_RUNNING,
    NXTHREAD_SUSPENDED
} nxthread_state_t;

void *thread_func(void *arg);

class NxThread {
public:
	NxThread(void);
	~NxThread();

    void setCallback( void (*_run)(void*) , void* _arg) { 
        run = _run; 
        arg = _arg;
    }

    bool start(void);
    void stop(void);
    bool suspend(void);
    bool resume(void);
    nxthread_state_t getState(void); 

    void setPriority(int _priority) { priority = _priority; }
    void setSchedPolicy(int _policy) { policy = _policy; }
    void setPeriod(unsigned long _period) { period = _period; }

    unsigned long getPeriod(void) { return period; }
    int getPriority(void) { return priority ;}
    int getSchedPolicy(void) { return policy; }
    unsigned long int getId(void) { return thread;}

    friend void* thread_func(void* arg);

private:
    pthread_t thread;
    pthread_attr_t attr;
    int priority;
    int policy;
    int period;
    bool bshould_stop;
    bool bshould_suspend;
    NxMutex mutex;
    NxEvent event;
    void (*run)(void*);
    void *arg;
    nxthread_state_t state;
};


#endif /* RTTHREAD_H_ */
