/*
FreeRTOS V9.0.0 - Copyright (C) 2016 Real Time Engineers Ltd.
All rights reserved

VISIT http://www.FreeRTOS.org TO ENSURE YOU ARE USING THE LATEST VERSION.

This file is part of the FreeRTOS distribution.

FreeRTOS is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License (version 2) as published by the
Free Software Foundation >>>> AND MODIFIED BY <<<< the FreeRTOS exception.

***************************************************************************
>>!   NOTE: The modification to the GPL is included to allow you to     !<<
>>!   distribute a combined work that includes FreeRTOS without being   !<<
>>!   obliged to provide the source code for proprietary components     !<<
>>!   outside of the FreeRTOS kernel.                                   !<<
***************************************************************************

FreeRTOS is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE.  Full license text is available on the following
link: http://www.freertos.org/a00114.html

***************************************************************************
*                                                                       *
*    FreeRTOS provides completely free yet professionally developed,    *
*    robust, strictly quality controlled, supported, and cross          *
*    platform software that is more than just the market leader, it     *
*    is the industry's de facto standard.                               *
*                                                                       *
*    Help yourself get started quickly while simultaneously helping     *
*    to support the FreeRTOS project by purchasing a FreeRTOS           *
*    tutorial book, reference manual, or both:                          *
*    http://www.FreeRTOS.org/Documentation                              *
*                                                                       *
***************************************************************************

http://www.FreeRTOS.org/FAQHelp.html - Having a problem?  Start by reading
the FAQ page "My application does not run, what could be wrong?".  Have you
defined configASSERT()?

http://www.FreeRTOS.org/support - In return for receiving this top quality
embedded software for free we request you assist our global community by
participating in the support forum.

http://www.FreeRTOS.org/training - Investing in training allows your team to
be as productive as possible as early as possible.  Now you can receive
FreeRTOS training directly from Richard Barry, CEO of Real Time Engineers
Ltd, and the world's leading authority on the world's leading RTOS.

http://www.FreeRTOS.org/plus - A selection of FreeRTOS ecosystem products,
including FreeRTOS+Trace - an indispensable productivity tool, a DOS
compatible FAT file system, and our tiny thread aware UDP/IP stack.

http://www.FreeRTOS.org/labs - Where new FreeRTOS products go to incubate.
Come and try FreeRTOS+TCP, our new open source TCP/IP stack for FreeRTOS.

http://www.OpenRTOS.com - Real Time Engineers ltd. license FreeRTOS to High
Integrity Systems ltd. to sell under the OpenRTOS brand.  Low cost OpenRTOS
licenses offer ticketed support, indemnification and commercial middleware.

http://www.SafeRTOS.com - High Integrity Systems also provide a safety
engineered and independently SIL3 certified version for use in safety and
mission critical applications that require provable dependability.

1 tab == 4 spaces!
*/

/*
* main() creates all the demo application tasks, then starts the scheduler.
* The web documentation provides more details of the standard demo application
* tasks, which provide no particular functionality but do provide a good
* example of how to use the FreeRTOS API.
*
* In addition to the standard demo tasks, the following tasks and tests are
* defined and/or created within this file:
*
* "Check" task - This only executes every five seconds but has a high priority
* to ensure it gets processor time.  Its main function is to check that all the
* standard demo tasks are still operational.  While no errors have been
* discovered the check task will print out "OK" and the current simulated tick
* time.  If an error is discovered in the execution of a task then the check
* task will print out an appropriate error message.
*
*/


/* Standard includes. */
#include <stdio.h>
#include <stdlib.h>

#include "FreeRTOS.h"
#include "task.h"

/* Standard demo includes. */
#include "AbortDelay.h"
#include "BlockQ.h"
#include "blocktim.h"
#include "countsem.h"
#include "death.h"
#include "dynamic.h"
#include "EventGroupsDemo.h"
#include "flop.h"
#include "GenQTest.h"
#include "integer.h"
#include "IntSemTest.h"
#include "PollQ.h"
#include "QPeek.h"
#include "QueueOverwrite.h"
#include "QueueSet.h"
#include "QueueSetPolling.h"
#include "recmutex.h"
#include "semtest.h"
#include "TaskNotify.h"
#include "TimerDemo.h"

/* Priorities at which the tasks are created. */
#define mainCHECK_TASK_PRIORITY      ( configMAX_PRIORITIES - 2 )
#define mainQUEUE_POLL_PRIORITY      ( tskIDLE_PRIORITY + 1 )
#define mainSEM_TEST_PRIORITY      ( tskIDLE_PRIORITY + 1 )
#define mainBLOCK_Q_PRIORITY      ( tskIDLE_PRIORITY + 2 )
#define mainCREATOR_TASK_PRIORITY    ( tskIDLE_PRIORITY + 3 )
#define mainINTEGER_TASK_PRIORITY    ( tskIDLE_PRIORITY )
#define mainGEN_QUEUE_TASK_PRIORITY    ( tskIDLE_PRIORITY )
#define mainFLOP_TASK_PRIORITY      ( tskIDLE_PRIORITY )
#define mainQUEUE_OVERWRITE_PRIORITY  ( tskIDLE_PRIORITY )

#define mainTIMER_TEST_PERIOD      ( 50 )


/*
* Prototypes for the standard FreeRTOS callback/hook functions implemented
* within this file.
*/
void vApplicationMallocFailedHook( void );
void vApplicationIdleHook( void );
void vApplicationTickHook( void );

/* Custom Tasks for assignments */
#define SIZE 10
#define ROW SIZE
#define COL SIZE
static void matrix_task(void *pvParameters);
static void communication_task(void *pvParameters);
static void priorityset_task(void *pvParameters);
uint32_t ulCount = 0;
uint32_t ulMatrixPeriod = 0;
TaskHandle_t matrix_handle = NULL;
TaskHandle_t communication_handle = NULL;
TaskHandle_t priority_handle = NULL;

/*-----------------------------------------------------------*/

int main ( void )
{
  /* Create Task 1 and 2. */
  xTaskCreate(matrix_task, "Matrix", 1000, NULL, 3, &matrix_handle);
  xTaskCreate(communication_task, "Communication", \
                      configMINIMAL_STACK_SIZE, NULL, 1, &communication_handle);
  xTaskCreate(priorityset_task, "Priority", \
                      configMINIMAL_STACK_SIZE, NULL, 2, &priority_handle);

  /* Create the standard demo tasks. */
  vStartIntegerMathTasks( mainINTEGER_TASK_PRIORITY );
  vStartMathTasks( mainFLOP_TASK_PRIORITY );
  vStartBlockingQueueTasks( mainBLOCK_Q_PRIORITY );
  vStartPolledQueueTasks( mainQUEUE_POLL_PRIORITY );
  vStartSemaphoreTasks( mainSEM_TEST_PRIORITY );
  vStartDynamicPriorityTasks();
  vCreateBlockTimeTasks();
  vStartGenericQueueTasks( mainGEN_QUEUE_TASK_PRIORITY );
  vStartQueuePeekTasks();
  vStartCountingSemaphoreTasks();
  vStartRecursiveMutexTasks();

  vCreateAbortDelayTasks();
  vStartEventGroupTasks();
  vStartInterruptSemaphoreTasks();
  vStartQueueSetTasks();
  vStartQueueSetPollingTask();
  vStartQueueOverwriteTask( mainQUEUE_OVERWRITE_PRIORITY );
  vStartTaskNotifyTask();

  #if( configUSE_PREEMPTION != 0  )
  {
    /* Don't expect these tasks to pass when preemption is not used. */
    vStartTimerDemoTask( mainTIMER_TEST_PERIOD );
  }
  #endif

  /* The suicide tasks must be created last as they need to know how many
  tasks were running prior to their creation.  This then allows them to
  ascertain whether or not the correct/expected number of tasks are running at
  any given time. */
  vCreateSuicidalTasks( mainCREATOR_TASK_PRIORITY );

  /* Start the scheduler itself. */
  vTaskStartScheduler();

  /* Should never get here unless there was not enough heap space to create
  the idle and other system tasks. */
  return 0;
}
/*-----------------------------------------------------------*/

static void matrix_task(void *pvParameters)
{
  /* Just to remove compiler warning. */
  ( void ) pvParameters;

  int i;
  double **a = (double **)pvPortMalloc(ROW * sizeof(double*));
  for (i = 0; i < ROW; i++) a[i] = (double *)pvPortMalloc(COL * sizeof(double));
  double **b = (double **)pvPortMalloc(ROW * sizeof(double*));
  for (i = 0; i < ROW; i++) b[i] = (double *)pvPortMalloc(COL * sizeof(double));
  double **c = (double **)pvPortMalloc(ROW * sizeof(double*));
  for (i = 0; i < ROW; i++) c[i] = (double *)pvPortMalloc(COL * sizeof(double));

  double sum = 0.0;
  int j, k, l;

  for (i = 0; i < SIZE; i++) {
    for (j = 0; j < SIZE; j++) {
      a[i][j] = 1.5;
      b[i][j] = 2.6;
    }
  }

  while (1) {
    /*
    * In an embedded systems, matrix multiplication would block the CPU for a \
    *                                                                long time
    * but since this is a PC simulator we must add one additional dummy delay.
    */
    long simulationdelay;
    for (simulationdelay = 0; simulationdelay<1000000000; simulationdelay++)
      ;
    for (i = 0; i < SIZE; i++) {
      for (j = 0; j < SIZE; j++) {
        c[i][j] = 0.0;
      }
    }

    for (i = 0; i < SIZE; i++) {
      for (j = 0; j < SIZE; j++) {
        sum = 0.0;
        for (k = 0; k < SIZE; k++) {
          for (l = 0; l<10; l++) {
            sum = sum + a[i][k] * b[k][j];
          }
        }
        c[i][j] = sum;
      }
    }
    vTaskDelay(100);
    printf("Matrix Period is : %d\n", ulMatrixPeriod);
    fflush(stdout);
    ulMatrixPeriod = 0;
  }
}

static void communication_task(void *pvParameters)
{
  /* Just to remove compiler warning. */
  ( void ) pvParameters;

  while (1) {
    int priority = uxTaskPriorityGet(communication_handle);
    printf("Priority now is %d \n", priority);
    fflush(stdout);
    printf("Sending data...\n");
    fflush(stdout);
    vTaskDelay(100);
    printf("Data sent!\n");
    printf("Communication Period is : %d\n", ulCount);
    fflush(stdout);
    vTaskDelay(100);
    ulCount = 0;
  }
}

static void priorityset_task(void *pvParameters)
{
  /* Just to remove compiler warning. */
  ( void ) pvParameters;

  while(1) {
    if(ulCount > 400)
    {
      /* Increasing the priority if period is more than 400 instead of 1000 as
      Communication was not taking more than 600ms */
      vTaskPrioritySet(communication_handle, 4);
    }
    else if(ulCount < 200)
    {
      vTaskPrioritySet(communication_handle, 2);
    }
    vTaskDelay(100);
  }
}

/*-----------------------------------------------------------*/

void vApplicationMallocFailedHook( void )
{
  /* vApplicationMallocFailedHook() will only be called if
  configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h.  It is a hook
  function that will get called if a call to pvPortMalloc() fails.
  pvPortMalloc() is called internally by the kernel whenever a task, queue,
  timer or semaphore is created.  It is also called by various parts of the
  demo application.  If heap_1.c or heap_2.c are used, then the size of the
  heap available to pvPortMalloc() is defined by configTOTAL_HEAP_SIZE in
  FreeRTOSConfig.h, and the xPortGetFreeHeapSize() API function can be used
  to query the size of free heap space that remains (although it does not
  provide information on how the remaining heap might be fragmented). */
  vAssertCalled( __LINE__, __FILE__ );
}
/*-----------------------------------------------------------*/

void vApplicationIdleHook( void )
{
  /* vApplicationIdleHook() will only be called if configUSE_IDLE_HOOK is set
  to 1 in FreeRTOSConfig.h.  It will be called on each iteration of the idle
  task.  It is essential that code added to this hook function never attempts
  to block in any way (for example, call xQueueReceive() with a block time
  specified, or call vTaskDelay()).  If the application makes use of the
  vTaskDelete() API function (as this demo application does) then it is also
  important that vApplicationIdleHook() is permitted to return to its calling
  function, because it is the responsibility of the idle task to clean up
  memory allocated by the kernel to any task that has since been deleted. */

  /* Call the idle task processing used by the full demo.  The simple
  blinky demo does not use the idle task hook. */
  //vFullDemoIdleFunction();
}
/*-----------------------------------------------------------*/

void vApplicationTickHook( void )
{
  /* This function will be called by each tick interrupt if
  configUSE_TICK_HOOK is set to 1 in FreeRTOSConfig.h.  User code can be
  added here, but the tick hook is called from an interrupt context, so
  code must not attempt to block, and only the interrupt safe FreeRTOS API
  functions can be used (those that end in FromISR()). */

  /* Set Priority */
  ulCount++;
  ulMatrixPeriod++;

  /* Call the periodic timer test, which tests the timer API functions that
  can be called from an ISR. */
  #if( configUSE_PREEMPTION != 0 )
  {
    /* Only created when preemption is used. */
    vTimerPeriodicISRTests();
  }
  #endif

  /* Call the periodic queue overwrite from ISR demo. */
  vQueueOverwritePeriodicISRDemo();

  /* Write to a queue that is in use as part of the queue set demo to
  demonstrate using queue sets from an ISR. */
  vQueueSetAccessQueueSetFromISR();
  vQueueSetPollingInterruptAccess();

  /* Exercise event groups from interrupts. */
  vPeriodicEventGroupsProcessing();

  /* Exercise giving mutexes from an interrupt. */
  vInterruptSemaphorePeriodicTest();

  /* Exercise using task notifications from an interrupt. */
  xNotifyTaskFromISR();
}
/*-----------------------------------------------------------*/

void vAssertCalled( unsigned long ulLine, const char * const pcFileName )
{
  taskENTER_CRITICAL();
  {
    printf("[ASSERT] %s:%lu\n", pcFileName, ulLine);
    fflush(stdout);
  }
  taskEXIT_CRITICAL();
  exit(-1);
}
/*-----------------------------------------------------------*/
