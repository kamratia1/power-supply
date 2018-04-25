/*******************************************************************************
* File Name          : pwm.c
* Description        : PWM initialisation and duty cycle setting functions
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"
#include "hw_config.h"
#include "pwm.h"

/* Private Variables ---------------------------------------------------------*/
TIM_HandleTypeDef TIM1_Handle;
TIM_OC_InitTypeDef sConfigOC;


void PWM_Init(void)
{ 
  // All of this initialisation code taken from Cube MX auto Generated
  // PWM Freq = Clock_Freq/((Period+1)*(Prescaler+1))
  // Duty Cycle is set by the OC Pulse variable and can be any value between 0 and Period
  // To maximize PWM resolution, the period is kept as high as possible, while keeping the prescaler low
  // PWM Frequency is set to 20kHz as it is on the limit of human hearing, MCU Clock_Freq is 48MHz
  
  TIM_PWM_CLK_ENABLE();
  
  TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_MasterConfigTypeDef sMasterConfig;
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig;
  
  TIM1_Handle.Instance = TIM1;
  TIM1_Handle.Init.Prescaler = 0;
  TIM1_Handle.Init.CounterMode = TIM_COUNTERMODE_UP;
  TIM1_Handle.Init.Period = 2399;
  TIM1_Handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  TIM1_Handle.Init.RepetitionCounter = 0;
  TIM1_Handle.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  HAL_TIM_Base_Init(&TIM1_Handle);

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  HAL_TIM_ConfigClockSource(&TIM1_Handle, &sClockSourceConfig);

  HAL_TIM_PWM_Init(&TIM1_Handle);

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  HAL_TIMEx_MasterConfigSynchronization(&TIM1_Handle, &sMasterConfig);

  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  
  HAL_TIM_PWM_ConfigChannel(&TIM1_Handle, &sConfigOC, VSET_PWM_TIM_CHANNEL);
  HAL_TIM_PWM_ConfigChannel(&TIM1_Handle, &sConfigOC, ISET_PWM_TIM_CHANNEL);
  HAL_TIM_PWM_ConfigChannel(&TIM1_Handle, &sConfigOC, VSW_PWM_TIM_CHANNEL);

  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = 0;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  HAL_TIMEx_ConfigBreakDeadTime(&TIM1_Handle, &sBreakDeadTimeConfig);
  
  HAL_TIM_PWM_Start(&TIM1_Handle, VSET_PWM_TIM_CHANNEL);
  HAL_TIM_PWM_Start(&TIM1_Handle, ISET_PWM_TIM_CHANNEL);
  HAL_TIM_PWM_Start(&TIM1_Handle, VSW_PWM_TIM_CHANNEL);
  
  // GPIO Initialisation
  VSET_PWM_GPIO_CLK_ENABLE();
  ISET_PWM_GPIO_CLK_ENABLE();
  VSW_PWM_GPIO_CLK_ENABLE();
  
  GPIO_InitTypeDef GPIO_InitStruct;
  GPIO_InitStruct.Pin = VSET_PWM_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = TIM_PWM_AF;
  HAL_GPIO_Init(VSET_PWM_GPIO_Port, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin = ISET_PWM_Pin;
  HAL_GPIO_Init(ISET_PWM_GPIO_Port, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin = VSW_PWM_Pin;
  HAL_GPIO_Init(VSW_PWM_GPIO_Port, &GPIO_InitStruct);
  
}

void PWM_setDuty(uint16_t GPIO_Pin, uint16_t val)
{
  sConfigOC.Pulse = val;
  
  switch(GPIO_Pin)
  {
    case VSET_PWM_Pin:
      HAL_TIM_PWM_ConfigChannel(&TIM1_Handle, &sConfigOC, VSET_PWM_TIM_CHANNEL);
      HAL_TIM_PWM_Start(&TIM1_Handle, VSET_PWM_TIM_CHANNEL);
      break;
      
    case ISET_PWM_Pin:
      HAL_TIM_PWM_ConfigChannel(&TIM1_Handle, &sConfigOC, ISET_PWM_TIM_CHANNEL);
      HAL_TIM_PWM_Start(&TIM1_Handle, ISET_PWM_TIM_CHANNEL);
      break;
      
    case VSW_PWM_Pin:
      HAL_TIM_PWM_ConfigChannel(&TIM1_Handle, &sConfigOC, VSW_PWM_TIM_CHANNEL);
      HAL_TIM_PWM_Start(&TIM1_Handle, VSW_PWM_TIM_CHANNEL);
      break;      
  }
  
  
}