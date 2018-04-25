/******************************************************************************
* @file           : enable.h
* @brief          : header file for enable.c
******************************************************************************/

#ifndef _ENABLE_H_
#define _ENABLE_H_

/* Function Prototypes ------------------------------------------------------*/
void Enable_Init(void);
void Enable_Output(GPIO_PinState state);
void Enable_VSW(GPIO_PinState state);

#endif