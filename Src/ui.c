/*******************************************************************************
* File Name          : ui.c
* Description        : Contains ui Task
                       Handles the display
*******************************************************************************/

#include "stm32f0xx_hal.h"
#include "hw_config.h"
#include "lcd.h"
#include "graphics.h"
#include "ui.h"
#include "enable.h"
#include "system_state.h"

/* Function Prototypes -------------------------------------------------------*/
static void UI_TimerInit(void);
static void UI_Task(void);

/* Private Variables ---------------------------------------------------------*/
TIM_HandleTypeDef UI_TimerHandle;


// Draws the basic template
void UI_Init(void)
{
  fillScreen(BLACK); 
  
  // Draw Stuff
  
  // Start UI Timer
  //UI_TimerInit();
  Enable_Bklight(GPIO_PIN_SET);  
}

void UI_Task(void)
{
  
}

static void UI_TimerInit(void)
{
  
}

void UI_TimerCallback(void)
{
  UI_Task();
}
