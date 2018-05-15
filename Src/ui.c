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
extern uint16_t zero_25_1bit[];
extern uint16_t one_25_1bit[];
extern uint16_t two_25_1bit[];
extern uint16_t three_25_1bit[];
extern uint16_t four_25_1bit[];
extern uint16_t five_25_1bit[];
extern uint16_t six_25_1bit[];
extern uint16_t seven_25_1bit[];
extern uint16_t eight_25_1bit[];
extern uint16_t nine_25_1bit[];

extern uint16_t zero_18_1bit[];
extern uint16_t one_18_1bit[];
extern uint16_t two_18_1bit[];
extern uint16_t three_18_1bit[];
extern uint16_t four_18_1bit[];
extern uint16_t five_18_1bit[];
extern uint16_t six_18_1bit[];
extern uint16_t seven_18_1bit[];
extern uint16_t eight_18_1bit[];
extern uint16_t nine_18_1bit[];

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
  
  // Measured Voltage and Current
  int xstart = 10;
  int ypos = 8; 
  drawDigits25pt(Get_OutputVoltage(), xstart, ypos, GREEN);
  LCD_FillRectangle(xstart+34, ypos+21, 4, 4, GREEN);     // decimal point
  LCD_SendImageColoured(xstart+94, ypos, V_25, GREEN);
  
  ypos = 42;
  drawDigits25pt(Get_OutputCurrent(), xstart, ypos, RED);
  LCD_FillRectangle(xstart+34, ypos+21, 4, 4, RED);
  LCD_SendImageColoured(xstart+94, ypos, A_25, RED);
  
  // Set Voltage and Current Text
  xstart = 10;
  ypos = 78; 
  drawDigits18pt(Get_DesiredVoltage(), xstart, ypos, WHITE);
  LCD_FillRectangle(xstart+24, ypos+15, 3, 3, WHITE);     // decimal point
  LCD_SendImage(xstart+65, ypos, V_18);  
  
  ypos = 103;
  drawDigits18pt(Get_DesiredCurrent(), xstart, ypos, WHITE);
  LCD_FillRectangle(xstart+24, ypos+15, 3, 3, WHITE);     // decimal point
  LCD_SendImage(xstart+65, ypos, A_18);
    
  Enable_Bklight(GPIO_PIN_SET);     

  // Start UI Timer
  UI_TimerInit();  
}

void UI_Task(void)
{
  static uint16_t ui_desiredVoltage;
  static uint16_t ui_desiredCurrent;
  static uint16_t ui_actualVoltage;
  static uint16_t ui_actualCurrent;
  
  if(State_UITask == TASK_READY)
  {
      State_UITask = TASK_NOT_READY;
      
    // Update Desired Voltage and Current
     if (Get_DesiredVoltage() != ui_desiredVoltage)
     {
        ui_desiredVoltage = Get_DesiredVoltage();
        drawDigits18pt(ui_desiredVoltage, 10, 78, WHITE);
     }

     if (Get_DesiredCurrent() != ui_desiredCurrent)
     {
        ui_desiredCurrent = Get_DesiredCurrent();
        drawDigits18pt(ui_desiredCurrent, 10, 103, WHITE);
     }
     
      // Update Measured Voltage and Current
     if (Get_OutputVoltage() != ui_actualVoltage)
     {
       //int16_t diff = Get_OutputVoltage() - ui_actualVoltage;
       //if (diff > 10 || diff < -10)
       //{
          ui_actualVoltage = Get_OutputVoltage();        
          drawDigits25pt(ui_actualVoltage, 10, 8, GREEN);
       //}
     }
     
     if (Get_OutputCurrent() != ui_actualCurrent)
     {
        ui_actualCurrent = Get_OutputCurrent();
        drawDigits25pt(ui_actualCurrent, 10, 42, RED);
     }
      

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

uint8_t* getDigits(uint16_t number)
{
  // digits array has to be static in order to return it
  static uint8_t digits_array[5];
  
  // set array to zero
  for(int i=0; i<5; i++)
  {
    digits_array[i] = 0;
  }
  
  uint8_t index = 4;
  while(number > 0)
  {
     digits_array[index] = number % 10;
     number /= 10;
     index--;
  }
  
  return digits_array;
}

void drawDigits25pt(uint16_t number, uint8_t xstart, uint8_t ypos, uint16_t colour)
{
    uint8_t* digits;
    digits = getDigits(number);
    
    uint8_t xpos = 0;
    uint8_t digit = 0;
    uint8_t offset[] = {0, 16, 32, 48, 64}; 
    uint8_t point_offset = 0;
    
    for(uint8_t i=0; i<5; i++)
    {
      digit = *(digits + i);
      
      if (i>=2) point_offset = 5;      // adding the decimal point offset        
      xpos = xstart + offset[i] + point_offset;
      
      // if first digit is zero, blank out that area of the screen
      if (i == 0 && digit == 0)
      {
        LCD_FillRectangle(xpos, ypos, 16, 25, BLACK);
      }
      else
      {
        switch(digit)
        {
          case 0:
            LCD_SendMonoImage(xpos, ypos, zero_25_1bit, colour);
            break;
          case 1:
            LCD_SendMonoImage(xpos, ypos, one_25_1bit, colour);
            break;
          case 2:
            LCD_SendMonoImage(xpos, ypos, two_25_1bit, colour);
            break;
          case 3:
            LCD_SendMonoImage(xpos, ypos, three_25_1bit, colour);
            break;
          case 4:
            LCD_SendMonoImage(xpos, ypos, four_25_1bit, colour);
            break;
          case 5:
            LCD_SendMonoImage(xpos, ypos, five_25_1bit, colour);
            break;
          case 6:
            LCD_SendMonoImage(xpos, ypos, six_25_1bit, colour);
            break;
          case 7:
            LCD_SendMonoImage(xpos, ypos, seven_25_1bit, colour);
            break;
          case 8:
            LCD_SendMonoImage(xpos, ypos, eight_25_1bit, colour);
            break;
          case 9:
            LCD_SendMonoImage(xpos, ypos, nine_25_1bit, colour);
            break;
        }
      }
    }     
}


void drawDigits18pt(uint16_t number, uint8_t xstart, uint8_t ypos, uint16_t colour)
{
    uint8_t* digits;
    digits = getDigits(number);
    
    uint8_t xpos = 0;
    uint8_t digit = 0;
    uint8_t offset[] = {0, 11, 22, 33, 44}; 
    uint8_t point_offset = 0;
    
    for(uint8_t i=0; i<5; i++)
    {
      digit = *(digits + i);
      
      if (i>=2) point_offset = 4;      // adding the decimal point offset        
      xpos = xstart + offset[i] + point_offset;
      
      // if first digit is zero, blank out that area of the screen
      if (i == 0 && digit == 0)
      {
        LCD_FillRectangle(xpos, ypos, 11, 18, BLACK);
      }
      else
      {
        switch(digit)
        {
          case 0:
            LCD_SendMonoImage(xpos, ypos, zero_18_1bit, colour);
            break;
          case 1:
            LCD_SendMonoImage(xpos, ypos, one_18_1bit, colour);
            break;
          case 2:
            LCD_SendMonoImage(xpos, ypos, two_18_1bit, colour);
            break;
          case 3:
            LCD_SendMonoImage(xpos, ypos, three_18_1bit, colour);
            break;
          case 4:
            LCD_SendMonoImage(xpos, ypos, four_18_1bit, colour);
            break;
          case 5:
            LCD_SendMonoImage(xpos, ypos, five_18_1bit, colour);
            break;
          case 6:
            LCD_SendMonoImage(xpos, ypos, six_18_1bit, colour);
            break;
          case 7:
            LCD_SendMonoImage(xpos, ypos, seven_18_1bit, colour);
            break;
          case 8:
            LCD_SendMonoImage(xpos, ypos, eight_18_1bit, colour);
            break;
          case 9:
            LCD_SendMonoImage(xpos, ypos, nine_18_1bit, colour);
            break;
        }
      }
    }     
}