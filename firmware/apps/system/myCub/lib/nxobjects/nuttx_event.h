//
// EventClass.h: header file
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
// Date:
// Name:
// Description:
//
//
#ifndef EVENT_CLASS
#define EVENT_CLASS

#include <pthread.h>

class NxEvent {
public:
	bool wait();
	void set();
	void reset();    
	NxEvent(void);
	~NxEvent(void);

private:
	pthread_cond_t m_ready;
	pthread_mutex_t m_lock;  
};

#endif

