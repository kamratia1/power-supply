/*******************************************************************************
* File Name          : control.c
* Description        : Contains control Task
                       Handles the Power supply main control system
*******************************************************************************/

#include "stm32f0xx_hal.h"
#include "hw_config.h"
#include "buttons.h"
#include "encoder.h"
#include "adc.h"
#include "control.h"
#include "system_state.h"
#include "enable.h"

/* Function Prototypes -------------------------------------------------------*/
static void Control_TimerInit(void);
static void Control_Task(void);
static void SetOutputSw(void);

/* Private Variables ---------------------------------------------------------*/
TIM_HandleTypeDef ControlTimerHandle;

static void Control_Task(void)
{  
  // Check the state of the Output Switch and set accordingly
  SetOutputSw();        
  
}

// Initialise the Control Task 
// Set up any variables ...etc
void Control_Init(void)
{
  Control_TimerInit();
}

// Initialise the Control Timer
static void Control_TimerInit(void)
{
  CONTROL_TIMER_CLK_ENABLE();
    
  // DebougTimerFreq = MCU_Clock_Freq/((Period+1)*(Prescaler+1))
  // Period =((CONTROL_TIMER_MS * MCU_Clock)/(Prescaler+1))-1 
  
  ControlTimerHandle.Instance = CONTROL_TIMER_TIM;
  ControlTimerHandle.Init.Prescaler = 127; // Set this to a (power of 2)-1 for integer maths
  ControlTimerHandle.Init.CounterMode = TIM_COUNTERMODE_UP;
  ControlTimerHandle.Init.Period = ((CONTROL_TIMER_MS * SystemCoreClock)/((ControlTimerHandle.Init.Prescaler + 1)*1000)) - 1; 
  ControlTimerHandle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  ControlTimerHandle.Init.RepetitionCounter = 0;
  HAL_TIM_Base_Init(&ControlTimerHandle);
  
  HAL_NVIC_SetPriority(CONTROL_TIMER_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(CONTROL_TIMER_IRQn);    
  
  HAL_TIM_Base_Start_IT(&ControlTimerHandle);
}


void Control_TimerCallback(void)
{
  Control_Task();  
}

static void SetOutputSw(void)
{
  if (Get_OutputSwState() == OUT_ENABLE)
  {
    Enable_Output(GPIO_PIN_SET);
  }
  else
  {
    Enable_Output(GPIO_PIN_RESET);
  }  
}
