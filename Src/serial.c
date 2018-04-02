/*******************************************************************************
* File Name          : serial.c
* Description        : Contains serial Task
                      This task allows PC debugging
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "rtos.h"
#include "uart.h"
#include "stm32f0xx_hal.h"
#include "string.h"

/* Private variables ---------------------------------------------------------*/
#define COMMAND_LENGTH       16

UART_HandleTypeDef UartHandle;
uint8_t RxBuffer[RXBUFFERSIZE];
uint8_t CmdBuffer[COMMAND_LENGTH];
char str[64];

enum COMMANDS
{
  SET_VSW=1,
  SET_V,
  SET_I,
  VSW_EN,
  OUT_EN,
  DISP_BKLIGHT_EN,
  
  CMD_NUMBER
};

static void process_command(void);

/* StartDefaultTask function */
void StartSerialTask(void const * argument)
{
  sprintf(str, "\r\nPower Supply!\r\nKishan Amratia\r\nBuild Date 28 March 2018\r\n");
  UART_print(str);
  
  // Set UART DMA to receive into CmdBuffer
  HAL_UART_Receive_DMA(&UartHandle, (uint8_t *)CmdBuffer, COMMAND_LENGTH);
  
  /* Infinite loop */
  for(;;)
  {    
    process_command();
      
    // print out adc values and button states
    
    
    // delay in ms
    osDelay(50);
  }
}

void process_command(void)
{
  int cmd_number = 0;           // command number
  int val = 0;                  // value to set for command
  
  // stop character reception
  HAL_UART_DMAStop(&UartHandle);
  
  // Process the command buffer
  for(int i=0; i<COMMAND_LENGTH; i++)
  {
    if (CmdBuffer[i] == '+')
    {
      // Parse the command buffer and extract the command number and value
      sscanf((char*)CmdBuffer, "cmd,%d,%d,+", &cmd_number, &val);
      
      // decide what to do based on command
      switch(cmd_number)
      {
        case SET_VSW:
          break;
        case SET_V:
          break;
        case SET_I:
          break;
        case VSW_EN:
          break;
        case OUT_EN:
          break;
        case DISP_BKLIGHT_EN:
          break;
      }
      
      // print out command back onto serial or specify invalid command
      if (cmd_number > 0 && cmd_number <= CMD_NUMBER)
      {
        sprintf(str, "ok,cmd=%d val=%d\r\n", cmd_number, val);
      }
      else
      {
        sprintf(str, "invalid command!\r\n");
      }
      UART_print(str);          
    }
  }
  
  // Reset the Receive Buffer and restart DMA
  memset(CmdBuffer, 0, COMMAND_LENGTH);
  HAL_UART_Receive_DMA(&UartHandle, (uint8_t *)CmdBuffer, COMMAND_LENGTH);          
}

