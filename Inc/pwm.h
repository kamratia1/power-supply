/******************************************************************************
* @file           : pwm.h
* @brief          : header file for pwm.c
******************************************************************************/

#ifndef _PWM_H_
#define _PWM_H_

/* Function Prototypes ------------------------------------------------------*/
void PWM_Init(void);
void PWM_setDuty(uint16_t GPIO_Pin, uint16_t dutyCycle);

#endif