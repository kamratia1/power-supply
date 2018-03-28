/*******************************************************************************
* File Name          : freertos.c
* Description        : Code for freertos applications
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "rtos.h"

/* Variables -----------------------------------------------------------------*/
osThreadId serialTaskHandle;

/* Hook prototypes */

/* Init FreeRTOS */
void RTOS_Init(void) {
 
  /* add mutexes, ... */
  
  /* add semaphores, ... */

  /* start timers, add new ones, ... */

  /* Create the thread(s) */
  // Create debug task
  osThreadDef(serialTask, StartSerialTask, osPriorityNormal, 0, 96);
  serialTaskHandle = osThreadCreate(osThread(serialTask), NULL);

  /* add queues, ... */
}


