//
// EventClass.cpp: implementation file
//
// Copyright (C) Walter E. Capers.  All rights reserved
//
// This source is free to use as you like.  If you make
// any changes please keep me in the loop.  Email them to
// walt.capers@comcast.net.
//
// PURPOSE:
//
//  To implement event signals as a C++ object
//
// REVISIONS
// =======================================================
// Date: 10.25.07        
// Name: Walter E. Capers
// Description: File creation
//
// Date: 11/02/07
// Name: Walter E. Capers
// Description: removed unnessary code identified by On Freund from Code Project
//


#include "nuttx_event.h"


NxEvent::NxEvent(void)
{
	pthread_mutexattr_t mattr;
	pthread_mutexattr_init(&mattr);
	pthread_mutex_init(&m_lock, &mattr);
	pthread_cond_init(&m_ready, NULL);
}

NxEvent::~NxEvent(void)
{
	pthread_cond_destroy(&m_ready);
	pthread_mutex_destroy(&m_lock);
}


/**
 *
 * Set
 * set an event to signaled
 *
 **/
void NxEvent::set()
{
	pthread_cond_signal(&m_ready);
}

/**
 *
 * Wait
 * wait for an event -- wait for an event object
 * to be set to signaled.  must be paired with a
 * call to reset within the same thread.
 *
 **/
bool NxEvent::wait()
{
    pthread_mutex_lock(&m_lock);
    pthread_cond_wait(&m_ready, &m_lock);
    return true;
}

/**
 *
 * Reset
 * reset an event flag to unsignaled
 * wait must be paired with reset within the same thread.
 *
 **/
void NxEvent::reset()
{
    pthread_mutex_unlock(&m_lock);
}

