/*******************************************************************************
* File Name          : ui.c
* Description        : Contains ui Task
                       Handles the buttons, encoder and display
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "rtos.h"
#include "stm32f0xx_hal.h"
#include "hw_config.h"
#include "ui.h"
#include "lcd.h"
#include "graphics.h"

/* Function Prototypes -------------------------------------------------------*/

void StartUiTask(void const * argument)
{
  
  LCD_Init();  
  fillScreen(BLACK);    
  
  
  /* Infinite loop */
  for(;;)
  {    
    
  }
}





