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
extern const uint16_t switch_open[];
extern const uint16_t switch_closed[];
// Letters are 19 width, 20 height
extern const uint16_t green_a[];
extern const uint16_t green_v[];
// Numbers are 16 width, 20 height
extern const uint16_t green_zero[];
extern const uint16_t green_one[];
extern const uint16_t green_two[];
extern const uint16_t green_three[];
extern const uint16_t green_four[];
extern const uint16_t green_five[];
extern const uint16_t green_six[];
extern const uint16_t green_seven[];
extern const uint16_t green_eight[];
extern const uint16_t green_nine[];

OutputSw_TypeDef UI_OutputState;

#define SWITCH_XPOS      85
#define SWITCH_YPOS      85
#define LETTER_WIDTH     19
#define NUMBER_WIDTH     16

// Draws the basic template
void UI_Init(void)
{
  // Fill in UI State
  UI_OutputState = Get_OutputSwState();
  
   
  // Draw Stuff  
  fillScreen(BLACK); 
  LCD_SendImage(SWITCH_XPOS, SWITCH_YPOS, switch_open);
  
  int xpos = 15;
  int ypos = 15;
  LCD_SendImage(xpos, ypos, green_zero);
  LCD_SendImage(xpos=xpos+NUMBER_WIDTH, ypos, green_one);
  LCD_SendImage(xpos=xpos+NUMBER_WIDTH, ypos, green_two);
  LCD_SendImage(xpos=xpos+NUMBER_WIDTH, ypos, green_three);
  LCD_SendImage(xpos=xpos+NUMBER_WIDTH+5, ypos, green_v);
  
  xpos = 15;
  ypos = 40;
  LCD_SendImage(xpos=xpos, ypos, green_five);
  LCD_SendImage(xpos=xpos+NUMBER_WIDTH, ypos, green_six);
  LCD_SendImage(xpos=xpos+NUMBER_WIDTH, ypos, green_seven);
  LCD_SendImage(xpos=xpos+NUMBER_WIDTH, ypos, green_nine);
  LCD_SendImage(xpos=xpos+NUMBER_WIDTH+5, ypos, green_a);
  
  
  Enable_Bklight(GPIO_PIN_SET);  
  
  

  // Start UI Timer
  UI_TimerInit();
  
}

void UI_Task(void)
{
  // Update Measured Voltage and Current
  
  // Update Desired Voltage and Current
  
  // Update Switch State if state has changed
  if (Get_OutputSwState() != UI_OutputState)
  {
    UI_OutputState = Get_OutputSwState();
    if (UI_OutputState == OUT_ENABLE)
    {
      LCD_SendImage(SWITCH_XPOS, SWITCH_YPOS, switch_closed);
    }
    else
    {
      LCD_SendImage(SWITCH_XPOS, SWITCH_YPOS, switch_open);
    }
  }
  
}

static void UI_TimerInit(void)
{
  UI_TIMER_CLK_ENABLE();
    
  // DebougTimerFreq = MCU_Clock_Freq/((Period+1)*(Prescaler+1))
  // Period =((UI_TIMER_MS * MCU_Clock)/(Prescaler+1))-1 
  
  UI_TimerHandle.Instance = UI_TIMER_TIM;
  UI_TimerHandle.Init.Prescaler = 127; // Set this to a (power of 2)-1 for integer maths
  UI_TimerHandle.Init.CounterMode = TIM_COUNTERMODE_UP;
  UI_TimerHandle.Init.Period = ((UI_TIMER_MS * SystemCoreClock)/((UI_TimerHandle.Init.Prescaler + 1)*1000)) - 1; 
  UI_TimerHandle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  UI_TimerHandle.Init.RepetitionCounter = 0;
  HAL_TIM_Base_Init(&UI_TimerHandle);
  
  HAL_NVIC_SetPriority(UI_TIMER_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(UI_TIMER_IRQn);    
  
  HAL_TIM_Base_Start_IT(&UI_TimerHandle);
}

void UI_TimerCallback(void)
{
  UI_Task();
}