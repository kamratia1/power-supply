/*******************************************************************************
* File Name          : interrupts.c
* Description        : Contains all HAL callback functions that further calls 
                       specific module level functions.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "interrupts.h"
#include "stm32f0xx_hal.h"
#include "hw_config.h"
#include "buttons.h"
#include "encoder.h"

// External GPIO Interrupts
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if (GPIO_Pin == SW1_Pin || GPIO_Pin == ENC_SW_Pin)
  {
    Buttons_IRQ_Callback(GPIO_Pin);
  }
  
  if (GPIO_Pin == ENCA_Pin || GPIO_Pin == ENCB_Pin)
  {
    Encoder_IRQ_Callback();
  }
}


