/*******************************************************************************
* File Name          : control.h
* Description        : Control header file
*******************************************************************************/

#ifndef _CONTROL_H_
#define _CONTROL_H_

#include "hw_config.h"

#define ISENSE_I_TO_V_GAIN      2 // 0 - 2V at ISENSE means 0 - 1A Current Limit
#define VSET_OPAMP_GAIN         5
#define VSET_V_TO_V_GAIN        ((MAX_OUTPUT_VOLTAGE_V * 1000)/(VSET_OPAMP_GAIN * NUM_STEPS_VOLTAGE))      // Vset PWM value to actual voltage output multiplier


#define VSW_PWM_MIN             760
#define VSW_PWM_MAX             930
#define VSW_VOUT_DIFF           1

#define VSW_VOUT_DIFF_STEPS     (VSW_VOUT_DIFF* NUM_STEPS_PER_VOLT)       // voltage difference between VSW and VOUT

// VSW to VSW_PWM value equation constants
#define CONST_INTERCEPT         9250
#define CONST_GRADIENT          (-1)
#define EQ_MULTIPLIER           10     // Equation is multiplied to avoid floating point arithmetic


void Control_Init(void);
void Control_TimerCallback(void);
void Control_Task(void);

#endif
