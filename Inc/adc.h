/******************************************************************************
* @file           : adc.h
* @brief          : header file for adc.c
******************************************************************************/

#ifndef _ADC_H_
#define _ADC_H_

/* Function Prototypes ------------------------------------------------------*/
void ADC_Init(void);
uint16_t ADC_getReading(uint16_t GPIO_Pin);
void ADC_PrintReadings(void);
#endif