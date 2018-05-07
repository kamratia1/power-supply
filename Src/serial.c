/*******************************************************************************
* File Name          : serial.c
* Description        : Contains serial Task
                      This task allows PC debugging
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"
#include "string.h"
#include "enable.h"
#include "pwm.h"
#include "hw_config.h"
#include "adc.h"
#include "uart.h"
#include "serial.h"
#include "printf.h"

/* Definitions ---------------------------------------------------------------*/
#define RX_BUFFER_SIZE       16

enum COMMANDS
{
  SET_VSW=1,
  SET_V,
  SET_I,
  OUT_EN,
  VSW_EN,
  DISP_BKLIGHT_EN,
  
  CMD_NUMBER
};

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef DebugTimerHandle;
uint8_t RxBuffer[RX_BUFFER_SIZE];
char str[64];
uint16_t adc_values[6];

/* Private Function Prototypes -----------------------------------------------*/
static void ProcessCommand(void);
static void Debug_TimerInit(void);
static void Debug_Task(void);

static void Debug_Task(void)
{
   ProcessCommand();    // Process Command
   ADC_PrintReadings();  // Print all ADC values
}

void Debug_TimerInit(void)
{
  DEBUG_TIMER_CLK_ENABLE();
    
  // DebougTimerFreq = MCU_Clock_Freq/((Period+1)*(Prescaler+1))
  // Period =((DEBOUNCE_TIMER_MS * MCU_Clock)/(Prescaler+1))-1 
  
  DebugTimerHandle.Instance = DEBUG_TIMER_TIM;
  DebugTimerHandle.Init.Prescaler = 127; // Set this to a (power of 2)-1 for integer maths
  DebugTimerHandle.Init.CounterMode = TIM_COUNTERMODE_UP;
  DebugTimerHandle.Init.Period = ((DEBUG_TIMER_MS * SystemCoreClock)/((DebugTimerHandle.Init.Prescaler + 1)*1000)) - 1; 
  DebugTimerHandle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  DebugTimerHandle.Init.RepetitionCounter = 0;
  HAL_TIM_Base_Init(&DebugTimerHandle);
  
  HAL_NVIC_SetPriority(DEBUG_TIMER_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(DEBUG_TIMER_IRQn);    
  
  HAL_TIM_Base_Start_IT(&DebugTimerHandle);
  
}

void SerialDebug_Init(void)
{
  UART_Print("\r\nPower Supply!\r\nKishan Amratia\r\nBuild Date 28 March 2018\r\n");
  
  // Set UART DMA to receive into RxBuffer
  UART_Receive_DMA_Start((uint8_t *)RxBuffer, RX_BUFFER_SIZE);
  
  Debug_TimerInit();
}

void Serial_DebugTimerCallback(void)
{
  Debug_Task();
}

void ProcessCommand(void)
{
  int cmd_number = 0;           // command number
  int val = 0;                  // value to set for command
  
  // stop character reception
  UART_Receive_DMA_Stop();
  
  // Process the command buffer
  for(int i=0; i<RX_BUFFER_SIZE; i++)
  {
    if (RxBuffer[i] == '+')
    {
      // Parse the command buffer and extract the command number and value
      sscanf((char*)RxBuffer, "cmd,%d,%d,+", &cmd_number, &val);        
      
      // decide what to do based on command
      switch(cmd_number)
      {
        case SET_VSW:
          PWM_setDuty(VSW_PWM_Pin, val);
          break;
        case SET_V:
          PWM_setDuty(VSET_PWM_Pin, val);
          break;
        case SET_I:
          PWM_setDuty(ISET_PWM_Pin, val);
          break;
        case VSW_EN:
          Enable_VSW((GPIO_PinState) val);
          break;
        case OUT_EN:
          Enable_Output((GPIO_PinState) val);          
          break;
        case DISP_BKLIGHT_EN:
          Enable_Bklight((GPIO_PinState) val); 
          break;
      }
      
      // print out command back onto serial or specify invalid command
      if (cmd_number > 0 && cmd_number <= CMD_NUMBER)
      {
        sprintf(str, "ok,cmd=%d val=%d\r\n", cmd_number, val);
      }
      else
      {
        sprintf(str, "Invalid Command!\r\n");
      }        
      UART_Print(str);
    }
  }
  
  // Reset the Receive Buffer and restart DMA
  memset(RxBuffer, 0, RX_BUFFER_SIZE);
  UART_Receive_DMA_Start((uint8_t *)RxBuffer, RX_BUFFER_SIZE);          
}

