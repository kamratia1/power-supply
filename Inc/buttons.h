/******************************************************************************
* @file           : buttons.h
* @brief          : header file for buttons.c
******************************************************************************/

#ifndef _BUTTONS_H_
#define _BUTTONS_H_

#include "system_state.h"
#include "stm32f0xx_hal.h"


/* Function Prototypes ------------------------------------------------------*/
void Buttons_Init(void);
void Buttons_IRQ_Callback(uint16_t GPIO_Pin);
void Buttons_DebounceTimerCallback(void);
Flag_TypeDef Get_DebounceTimerFlag(void);
uint8_t DebounceTime_Inc(void);

#endif