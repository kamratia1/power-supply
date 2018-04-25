/*******************************************************************************
* File Name          : enable.c
* Description        : GPIO initialisation for PSU Output and VSW enable
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"
#include "hw_config.h"
#include "enable.h"

/* Private Function Prototypes -----------------------------------------------*/

void Enable_Init(void)
{     
  
  OUT_EN_GPIO_CLK_ENABLE();
  VSW_EN_GPIO_CLK_ENABLE();
  
  GPIO_InitTypeDef GPIO_InitStruct;
  GPIO_InitStruct.Pin = OUT_EN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(OUT_EN_GPIO_Port, &GPIO_InitStruct);
  
  // Turns out the Relay is active low assertion - wiring mistake
  Enable_Output(GPIO_PIN_SET);
  
  GPIO_InitStruct.Pin = VSW_EN_Pin;
  HAL_GPIO_Init(VSW_EN_GPIO_Port, &GPIO_InitStruct);
  Enable_VSW(GPIO_PIN_RESET);
}

void Enable_Output(GPIO_PinState state)
{
  if (state == GPIO_PIN_SET)
  {
    HAL_GPIO_WritePin(OUT_EN_GPIO_Port, OUT_EN_Pin, GPIO_PIN_RESET);
  }
  else if (state == GPIO_PIN_RESET)
  {
    HAL_GPIO_WritePin(OUT_EN_GPIO_Port, OUT_EN_Pin, GPIO_PIN_SET);
  }
}

void Enable_VSW(GPIO_PinState state)
{
  HAL_GPIO_WritePin(VSW_EN_GPIO_Port, VSW_EN_Pin, state);
}