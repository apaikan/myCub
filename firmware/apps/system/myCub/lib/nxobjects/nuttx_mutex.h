//
// MutexClass.h: header file
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

#ifndef MUTEX_CLASS
#define MUTEX_CLASS

#include <pthread.h>

class NxMutex {
public:
	void lock();
	void unlock();
	NxMutex(void);
	~NxMutex(void);
 
 private:
	pthread_mutex_t m_mutex;   
};
#endif

