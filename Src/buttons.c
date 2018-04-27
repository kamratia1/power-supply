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

/* Function prototypes -------------------------------------------------------*/
void DebounceTimer_Init(void);
void DebounceTimer_Start(void);
void DebounceTimer_Stop(void);

TIM_HandleTypeDef DebounceTimerHandle;


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
  HAL_NVIC_SetPriority(ENC_SW_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(ENC_SW_IRQn);
  
  HAL_NVIC_SetPriority(SW1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(SW1_IRQn);  
  
  // Initialize the Debounce Timer
  DebounceTimer_Init();
   
}

void DebounceTimer_Init(void)
{
    DEBOUNCE_TIMER_CLK_ENABLE();
    
    // _DebounceTimerFreq = MCU_Clock_Freq/((Period+1)*(Prescaler+1))
    // Period =((DEBOUNCE_TIMER_MS * MCU_Clock)/(Prescaler+1))-1 
    
    DebounceTimerHandle.Instance = DEBOUNCE_TIMER_TIM;
    DebounceTimerHandle.Init.Prescaler = 127; // Set this to a (power of 2)-1 for integer maths
    DebounceTimerHandle.Init.CounterMode = TIM_COUNTERMODE_UP;
    DebounceTimerHandle.Init.Period = ((DEBOUNCE_TIMER_MS * SystemCoreClock)/((DebounceTimerHandle.Init.Prescaler + 1)*1000)) - 1; 
    DebounceTimerHandle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    DebounceTimerHandle.Init.RepetitionCounter = 0;
    HAL_TIM_Base_Init(&DebounceTimerHandle);
    
    HAL_NVIC_SetPriority(DEBOUNCE_TIMER_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(DEBOUNCE_TIMER_IRQn);    
    
}

void DebounceTimer_Start(void)
{
  // Clear the update flag and start the timer
  __HAL_TIM_CLEAR_IT(&DebounceTimerHandle, TIM_IT_UPDATE);
  HAL_TIM_Base_Start_IT(&DebounceTimerHandle);
}

void DebounceTimer_Stop(void)
{
  // stop the timer
  HAL_TIM_Base_Stop_IT(&DebounceTimerHandle);
}


void Buttons_IRQ_Callback(uint16_t GPIO_Pin)
{
  DebounceTimer_Start();
  
  // Disable interrupt for the relevant GPIO pin and start debounce timer
  if (GPIO_Pin == SW1_Pin)
  {   
    HAL_NVIC_DisableIRQ(SW1_IRQn);   
  }
  
  if (GPIO_Pin == ENC_SW_Pin)
  {
    HAL_NVIC_DisableIRQ(ENC_SW_IRQn); 
  }
}

void Buttons_DebounceTimerCallback(void)
{      
   
  // Stop the debounce timer
  DebounceTimer_Stop();
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
