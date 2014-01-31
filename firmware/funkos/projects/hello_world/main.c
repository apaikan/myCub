//---------------------------------------------------------------------------
// FunkOS - Copyright (c) 2009, Funkenstein Software, See license.txt for details
//---------------------------------------------------------------------------
/*
	file:	myfirstapp.c

	This demo is the basic starting point demonstrating how to set up the RTOS -
	create tasks, initialize the libraries, and start the scheduler.
*/

#include <stdio.h>

#include "task.h"
#include "taskport.h"
#include "types.h"
#include "mutex.h"

#include "Arduino.h"
#include "uart.h"

//---------------------------------------------------------------------------
//Define a struct to contain all of the first task's task data in one place
typedef struct
{
	TASK_STRUCT stTask;
	USHORT ausStack[64];
} MY_TASK_STRUCT;

//---------------------------------------------------------------------------
// Define a struct to contain the task data etc. for the idle task
typedef struct
{
	TASK_STRUCT stTask;
	USHORT ausStack[64];
} IDLE_TASK_STRUCT;

//---------------------------------------------------------------------------
// functions for the tasks used in this demo
void MyTask(MY_TASK_STRUCT *pstThis_);
void MyTask2(MY_TASK_STRUCT *pstThis_);
void IdleTask(IDLE_TASK_STRUCT *pstIdle_);

MY_TASK_STRUCT 		stMyTask;					// Application Task
MY_TASK_STRUCT 		stMyTask2;					// Application Task
IDLE_TASK_STRUCT 	stIdleTask;					// Idle Task
//---------------------------------------------------------------------------

static MUTEX_STRUCT			stMutex;					// Mutual-exclusion object

int main(void)
{
    // initialization
    pinMode(13, OUTPUT); 
    uart_init();

	Task_Init();									// Initialize the RTOS
	
	// Create the application task
	Task_CreateTask(&(stMyTask.stTask), 			// Pointer to the task
					"Task 1",   					// Task name
					(UCHAR*)(stMyTask.ausStack),	// Task stack pointer
					64,								// Task Size
					1,								// Task Priority
					(TASK_FUNC)MyTask);					// Task function pointer
	
	// Create the application task
	Task_CreateTask(&(stMyTask2.stTask), 			// Pointer to the task
					"Task 2",   					// Task name
					(UCHAR*)(stMyTask2.ausStack),	// Task stack pointer
					64,								// Task Size
					2,								// Task Priority
					(TASK_FUNC)MyTask2);			// Task function pointer

	// Create the idle task 
	Task_CreateTask(&(stIdleTask.stTask), 
					"Idle Task",
					(UCHAR*)(stIdleTask.ausStack),
					64,
					0,								// !! Task priority is 0 for idle task !!
					(TASK_FUNC)IdleTask);	

	Task_Add((TASK_STRUCT*)&stMyTask);				// Add the tasks to the scheduler
	Task_Add((TASK_STRUCT*)&stMyTask2);			
	Task_Add((TASK_STRUCT*)&stIdleTask);

	Task_Start((TASK_STRUCT*)&stMyTask);			// Start the tasks
	Task_Start((TASK_STRUCT*)&stMyTask2);			
	Task_Start((TASK_STRUCT*)&stIdleTask);
	
    Mutex_Init(&stMutex);                           // initializing mutex

	Task_StartTasks();								// Start the scheduler 
	
	//--------------------------------------
	// Scheduler takes over - never returns
	//--------------------------------------
	
	return 0;
}
//---------------------------------------------------------------------------
// Entry Function for the user task
void MyTask(MY_TASK_STRUCT *pstThis_)
{
	ULONG iCount = 0;
	while(1)
	{
		iCount++;
		Task_Sleep(500);
		// Wait until we have permission to use the resource	
		Mutex_Claim(&stMutex, TIME_FOREVER);
        if(iCount%2)
            digitalWrite(13, HIGH);
        else
           digitalWrite(13, LOW);
        printf("Hello for Task 1\n");
		// release the resource
		Mutex_Release(&stMutex);
	}
}

//---------------------------------------------------------------------------
// Entry Function for the user task
void MyTask2(MY_TASK_STRUCT *pstThis_)
{
	while(1)
	{
		Task_Sleep(5000);
		// Wait until we have permission to use the resource	
		Mutex_Claim(&stMutex, TIME_FOREVER);
        digitalWrite(13, HIGH);
        printf("Hello for Task 2\n");
        Task_Sleep(2000);
		// release the resource
		Mutex_Release(&stMutex);
	}
}


//---------------------------------------------------------------------------
// Idle task - doesn't do anything useful
void IdleTask(IDLE_TASK_STRUCT *pstIdle_)
{
	while(1)
	{
		/* Do nothing */
        asm("nop");
	}
}


