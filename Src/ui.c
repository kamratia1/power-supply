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

/* Private Variables ---------------------------------------------------------*/
TIM_HandleTypeDef UI_TimerHandle;
extern const uint16_t switch_open[];
extern const uint16_t switch_closed[];
extern const uint16_t A_25[];
extern const uint16_t V_25[];
extern const uint16_t A_18[];
extern const uint16_t V_18[];
extern const uint16_t zero_25_1bit[];
extern const uint16_t one_25_1bit[];
extern const uint16_t two_25_1bit[];
extern const uint16_t three_25_1bit[];
extern const uint16_t four_25_1bit[];
extern const uint16_t five_25_1bit[];
extern const uint16_t six_25_1bit[];
extern const uint16_t seven_25_1bit[];
extern const uint16_t eight_25_1bit[];
extern const uint16_t nine_25_1bit[];

extern TaskState_TypeDef State_UITask;

OutputSw_TypeDef UI_OutputState;

#define SWITCH_XPOS      95
#define SWITCH_YPOS      85

// Draws the basic template
void UI_Init(void)
{
  // Fill in UI State
  UI_OutputState = Get_OutputSwState();
  
   
  // Draw Stuff  
  fillScreen(BLACK);   
  
  LCD_SendImage(SWITCH_XPOS, SWITCH_YPOS, switch_open);
  
  int xstart = 10;
  int ypos = 8;
#define POINT_OFFSET    5
  
  LCD_SendMonoImage(xstart,    ypos, one_25_1bit, GREEN);
  LCD_SendMonoImage(xstart+16, ypos, zero_25_1bit, GREEN);
  // decimal point
  LCD_FillRectangle(xstart+34, ypos+21, 4, 4, GREEN);
  LCD_SendMonoImage(xstart+32+POINT_OFFSET, ypos, two_25_1bit, GREEN);
  LCD_SendMonoImage(xstart+48+POINT_OFFSET, ypos, three_25_1bit, GREEN);
  LCD_SendMonoImage(xstart+64+POINT_OFFSET, ypos, four_25_1bit, GREEN);  
  LCD_SendImageColoured(xstart+94, ypos, V_25, GREEN);
  
  ypos = 42;
  //LCD_SendMonoImage(xstart,    ypos, five_25_1bit, GREEN);
#define COLOUR  RED     
  
  LCD_SendMonoImage(xstart+16, ypos, zero_25_1bit, COLOUR);
  // decimal point
  LCD_FillRectangle(xstart+34, ypos+21, 4, 4, COLOUR);
  LCD_SendMonoImage(xstart+32+POINT_OFFSET, ypos, three_25_1bit, COLOUR);
  LCD_SendMonoImage(xstart+48+POINT_OFFSET, ypos, four_25_1bit, COLOUR);
  LCD_SendMonoImage(xstart+64+POINT_OFFSET, ypos, nine_25_1bit, COLOUR);
  LCD_SendImageColoured(xstart+94, ypos, A_25, COLOUR);
  
  ypos = 78;
  LCD_SendImage(xstart+65, ypos, V_18);
  
  ypos = 103;
  LCD_SendImage(xstart+65, ypos, A_18);
  
  
  
  
  //Enable_Bklight(GPIO_PIN_SET);     

  // Start UI Timer
  UI_TimerInit();
  
}

void UI_Task(void)
{
  if(State_UITask == TASK_READY)
  {
      State_UITask = TASK_NOT_READY;
      
      // Update Desired Voltage and Current
      
      // Update Measured Voltage and Current
      

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
}


static void UI_TimerInit(void)
{
  UI_TIMER_CLK_ENABLE();
    
  // Freq = MCU_Clock_Freq/((Period+1)*(Prescaler+1))
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
  State_UITask = TASK_READY;
}