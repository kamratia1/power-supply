/*******************************************************************************
* File Name          : freertos.c
* Description        : Code for freertos applications
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "rtos.h"

/* Variables -----------------------------------------------------------------*/
//osThreadId uiTaskHandle;
//osThreadId controlTaskHandle;
osThreadId serialTaskHandle;

/* Hook prototypes */

/* Init FreeRTOS */
void RTOS_Init(void) {
 
  /* add mutexes, ... */
  
  /* add semaphores, ... */

  /* start timers, add new ones, ... */

  /* Create the thread(s) */
  
  //osThreadDef(uiTask, StartUiTask, osPriorityNormal, 0, 48);
  //uiTaskHandle = osThreadCreate(osThread(uiTask), NULL);
  
  //osThreadDef(controlTask, StartControlTask, osPriorityNormal, 0, 48);
  //controlTaskHandle = osThreadCreate(osThread(controlTask), NULL);

  osThreadDef(serialTask, StartSerialTask, osPriorityNormal, 0, 96);
  serialTaskHandle = osThreadCreate(osThread(serialTask), NULL);

  /* add queues, ... */
}


