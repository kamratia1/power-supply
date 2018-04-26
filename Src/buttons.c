/*******************************************************************************
* File Name          : buttons.c
* Description        : Code for initialising and processing the buttons
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"
#include "hw_config.h"
#include "rtos.h"
#include "buttons.h"
#include "system_state.h"

/* Definitions----------------------------------------------------------------*/
#define DEBOUNCE_TIMER_MS               40

void Buttons_Init(void)
{
  // Initialise GPIO Strcutre
  GPIO_InitTypeDef GPIO_InitStruct;
  
  // Enable GPIO Clocks
  ENC_SW_GPIO_CLK_ENABLE(); 
  SW1_GPIO_CLK_ENABLE();
   
  // Setup GPIO Pin with Falling Edge Interrupt
  GPIO_InitStruct.Pin = ENC_SW_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  // Define pullup in hw_config.h
#ifdef PINS_PULLUP
  GPIO_InitStruct.Pull = GPIO_PULLUP;
#else
  GPIO_InitStruct.Pull = GPIO_NOPULL;
#endif
  HAL_GPIO_Init(ENC_SW_GPIO_Port, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin = SW1_Pin;
  HAL_GPIO_Init(SW1_GPIO_Port, &GPIO_InitStruct);

  // Setup Interrupts
  HAL_NVIC_SetPriority(ENC_SW_IRQn, 6, 0);
  HAL_NVIC_EnableIRQ(ENC_SW_IRQn);
  
  HAL_NVIC_SetPriority(SW1_IRQn, 6, 0);
  HAL_NVIC_EnableIRQ(SW1_IRQn);  
  
  // Create the Debounce Timer
  //xDebounceTimer = xTimerCreate("Debounce Timer", pdMS_TO_TICKS(DEBOUNCE_TIMER_MS), pdFALSE,( void * ) 0, vDebounceTimerCallback);
   
}

void Buttons_IRQ_Callback(uint16_t GPIO_Pin)
{
    
  // Disable interrupt for the relevant GPIO pin and start debounce timer
  if (GPIO_Pin == SW1_Pin)
  {   
    HAL_NVIC_DisableIRQ(SW1_IRQn);
    // start debounce timer
    //xTimerStartFromISR(xDebounceTimer, &xHigherPriorityTaskWoken);
  }
  
  if (GPIO_Pin == ENC_SW_Pin)
  {
    HAL_NVIC_DisableIRQ(ENC_SW_IRQn);
    //start debounce timer
    //xTimerStartFromISR(xDebounceTimer, &xHigherPriorityTaskWoken);
  }
}

void vDebounceTimerCallback(TimerHandle_t xTimer)
{       
  // Check if button is pressed
  if (HAL_GPIO_ReadPin(SW1_GPIO_Port, SW1_Pin) == GPIO_PIN_RESET)
  {
    // Set Button State
    OutputSw_TypeDef state;
    state = Get_OutputSwState();    
    
    if (state == OUT_DISABLE)
    {
      state = OUT_ENABLE;
    }
    else
    {
      state = OUT_DISABLE;
    }
    
    Set_OutputSwState(state);
    
  }
  
  if (HAL_GPIO_ReadPin(ENC_SW_GPIO_Port, ENC_SW_Pin) == GPIO_PIN_RESET)
  {
    // Set Button State
    EncoderSw_TypeDef state;
    state = Get_EncoderSwitchState(); 
    state++;
    
    if (state == NUM_ENCODER_SW_STATES)
    {
      state = COARSE_VOLTAGE;
    }
    
    Set_EncoderSwitchState(state);    
  }
  
  // Re-enable Interrupts
  HAL_NVIC_EnableIRQ(ENC_SW_IRQn);
  HAL_NVIC_EnableIRQ(SW1_IRQn);  
}
