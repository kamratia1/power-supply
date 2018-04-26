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
extern UART_HandleTypeDef UART_Handle;
uint8_t RxBuffer[RX_BUFFER_SIZE];
char str[64];
uint16_t adc_values[6];

/* Private Function Prototypes -----------------------------------------------*/
static void process_command(void);


void StartSerialDebug(void const * argument)
{
  UART_Print("\r\nPower Supply!\r\nKishan Amratia\r\nBuild Date 28 March 2018\r\n");
  
  // Set UART DMA to receive into RxBuffer
  UART_Receive_DMA_Start((uint8_t *)RxBuffer, RX_BUFFER_SIZE);
  
  /* Infinite loop */
  for(;;)
  {    
    process_command();    // Process Command
    ADC_PrintReadings();  // Print all ADC values      
    HAL_Delay(50);          // 50ms delay
  }
}

void process_command(void)
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
      sscanf((char*)RxBuffer, "cmd,%d,%d,+", &cmd_number, &val);        // TODO - rewrite for lower flash footprint
      
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
          break;
      }
      
      // print out command back onto serial or specify invalid command
      if (cmd_number > 0 && cmd_number <= CMD_NUMBER)
      {
        sprintf(str, "ok,cmd=%d val=%d\r\n", cmd_number, val);  // TODO - rewrite for lower flash footprint
      }
      else
      {
        UART_Print("Invalid Command!\r\n");
      }        
    }
  }
  
  // Reset the Receive Buffer and restart DMA
  memset(RxBuffer, 0, RX_BUFFER_SIZE);
  UART_Receive_DMA_Start((uint8_t *)RxBuffer, RX_BUFFER_SIZE);          
}

