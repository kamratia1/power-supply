/**
  ******************************************************************************
  * @file    stm32f0xx_it.c
  * @brief   Interrupt Service Routines.
  ******************************************************************************
  *
  * COPYRIGHT(c) 2018 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"
#include "stm32f0xx.h"
#include "stm32f0xx_it.h"
#include "hw_config.h"

/* External variables --------------------------------------------------------*/
extern UART_HandleTypeDef UART_Handle;
extern TIM_HandleTypeDef htim17;
extern TIM_HandleTypeDef DebounceTimerHandle;
extern TIM_HandleTypeDef DebugTimerHandle;
extern TIM_HandleTypeDef ControlTimerHandle;

/******************************************************************************/
/*            Cortex-M0 Processor Interruption and Exception Handlers         */ 
/******************************************************************************/

/**
* @brief This function handles Non maskable interrupt.
*/
void NMI_Handler(void)
{
}

/**
* @brief This function handles Hard fault interrupt.
*/
void HardFault_Handler(void)
{
  while (1)
  {
  }
}

/**
* @brief This function handles System tick timer.
*/
void SysTick_Handler(void)
{
  //osSystickHandler();
}

/******************************************************************************/
/* STM32F0xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f0xx.s).                    */
/******************************************************************************/

/**
* @brief This function handles TIM17 global interrupt.
*/
void TIM17_IRQHandler(void)
{
  HAL_TIM_IRQHandler(&htim17);
}

// Application Timer Intterupts
void DEBOUNCE_TIMER_IRQHandler(void)
{
  HAL_TIM_IRQHandler(&DebounceTimerHandle);
}

void DEBUG_TIMER_IRQHandler(void)
{
  HAL_TIM_IRQHandler(&DebugTimerHandle);
}

void CONTROL_TIMER_IRQHandler(void)
{
  HAL_TIM_IRQHandler(&ControlTimerHandle);
}


/**
* @brief This function handles UART interrupt.
*/
void USART1_IRQHandler(void)
{
  HAL_UART_IRQHandler(&UART_Handle);
}


void USARTx_DMA_IRQHandler(void)
{
  HAL_DMA_IRQHandler(UART_Handle.hdmatx);
  HAL_DMA_IRQHandler(UART_Handle.hdmarx);
}

void ENC_SW1_IRQHandler(void)
{
  static GPIO_PinState pinA; 
  static GPIO_PinState pinB;
  static GPIO_PinState encSw; 
  static GPIO_PinState Sw1;
  
  // Read GPIO Pins 
  pinA = HAL_GPIO_ReadPin(ENCA_GPIO_Port, ENCA_Pin);
  pinB = HAL_GPIO_ReadPin(ENCB_GPIO_Port, ENCB_Pin);
  encSw = HAL_GPIO_ReadPin(ENC_SW_GPIO_Port, ENC_SW_Pin);
  Sw1 = HAL_GPIO_ReadPin(SW1_GPIO_Port, SW1_Pin);
  
  if (encSw == GPIO_PIN_RESET)
  {
    // Handle Interrupt only on Falling edge of Encoder Switch
    HAL_GPIO_EXTI_IRQHandler(ENC_SW_Pin);
  }
  
  if (Sw1 == GPIO_PIN_RESET)
  {
    // Handle Interrupt only on Falling edge of Output Switch
    HAL_GPIO_EXTI_IRQHandler(SW1_Pin);
  }
  
  // ISN'T THIS CODE BELOW REDUNDANT
  // TODO - The below code checks if pin is high or low, which is always true
    // can probably remove the if statements
  if( pinA == GPIO_PIN_SET || pinA == GPIO_PIN_RESET )
  {
    // Handle Interrupt on Rising or falling edge of Encoder
    HAL_GPIO_EXTI_IRQHandler(ENCA_Pin);
  }
  
  if( pinB == GPIO_PIN_SET || pinB == GPIO_PIN_RESET )
  {
    // Handle Interrupt on Rising or falling edge of Encoder
    HAL_GPIO_EXTI_IRQHandler(ENCB_Pin);
  }
}