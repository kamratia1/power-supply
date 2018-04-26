/******************************************************************************
* File Name          : rtos.h
* Description        : header file for rtos.c
******************************************************************************/

#ifndef RTOS_H_
#define RTOS_H_

#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"

/* Function prototypes -------------------------------------------------------*/
void RTOS_Init(void);
void StartSerialTask(void const * argument);
void StartUiTask(void const * argument);
void StartControlTask(void const * argument);

#endif