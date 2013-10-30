//
// MutexClass.cpp: implementation file
//
// Copyright (C) Walter E. Capers.  All rights reserved
//
// This source is free to use as you like.  If you make
// any changes please keep me in the loop.  Email them to
// walt.capers@comcast.net.
//
// PURPOSE:
//
//  To implement mutexes as a C++ object
//
// REVISIONS
// =======================================================
// Date: 10.25.07        
// Name: Walter E. Capers
// Description: File creation
//
// Date:
// Name:
// Description:
//
//
#include "nuttx_mutex.h"

NxMutex::NxMutex(void)
{
   pthread_mutexattr_t mattr;
   pthread_mutexattr_init( &mattr );
   pthread_mutex_init(&m_mutex, &mattr);
}

NxMutex::~NxMutex(void)
{
	pthread_mutex_lock(&m_mutex);
	pthread_mutex_unlock(&m_mutex); 
	pthread_mutex_destroy(&m_mutex);
}

/**
 *
 * Lock
 * the same thread can not lock the same mutex
 * more than once
 *
 **/
void NxMutex::lock()
{
    pthread_mutex_lock(&m_mutex);
}

/**
 *
 * Unlock
 * releases a mutex.  only the thread that acquires
 * the mutex can release it.
 *
 **/
void NxMutex::unlock()
{
    pthread_mutex_unlock(&m_mutex);
}

