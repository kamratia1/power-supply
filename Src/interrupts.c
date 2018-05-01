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
#include "serial.h"
#include "control.h"
#include "ui.h"
#include "system_state.h"

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

// Timer Interrupts
/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM interrupt took place, inside
  * HAL_TIM_IRQHandler(). 
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *TimerHandle)
{ 
  if (TimerHandle->Instance == TIM17) {
    // This makes a direct call to HAL_IncTick() to increment
    // a global variable "uwTick" used as application time base.
    HAL_IncTick();
    
    //Check for button debouncing
    if (Get_DebounceTimerFlag() != FLAG_RESET)
    {     
      if (DebounceTime_Inc() == DEBOUNCE_TIMER_MS)
      {
        Buttons_DebounceTimerCallback();
      }
    }
  }
  
  if (TimerHandle->Instance == DEBUG_TIMER_TIM)
  {
    Serial_DebugTimerCallback();
  }
  
  if (TimerHandle->Instance == CONTROL_TIMER_TIM)
  {
    Control_TimerCallback();
  }  
  
  if (TimerHandle->Instance == UI_TIMER_TIM)
  {
    UI_TimerCallback();
  }  
  
}

