/******************************************************************************
* @file           : buttons.h
* @brief          : header file for buttons.c
******************************************************************************/

#ifndef _BUTTONS_H_
#define _BUTTONS_H_

/* Function Prototypes ------------------------------------------------------*/
void Buttons_Init(void);
void Buttons_IRQ_Callback(uint16_t GPIO_Pin);
void Buttons_DebounceTimerCallback(void);
#endif