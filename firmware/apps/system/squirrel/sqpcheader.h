/*	see copyright notice in squirrel.h */
#ifndef _SQPCHEADER_H_
#define _SQPCHEADER_H_

#include <nuttx/config.h>

#if defined(_MSC_VER) && defined(_DEBUG)
#include <crtdbg.h>
#endif 


#ifdef CONFIG_SQUIRREL_NO_COMPILER
#define NO_COMPILER 
#endif 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <new>
//squirrel stuff
#include <squirrel.h>
#include "sqobject.h"
#include "sqstate.h"

#endif //_SQPCHEADER_H_
