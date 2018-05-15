/*******************************************************************************
* File Name          : encoder.c
* Description        : Code for initialising and processing the encoder
                      Encoder code taken from: 
              https://github.com/buxtronix/arduino/tree/master/libraries/Rotary
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"
#include "hw_config.h"
#include "encoder.h"
#include "system_state.h"

/* Private Function Prototypes -----------------------------------------------*/
static unsigned char Encoder_Process(void);

/* Private Variables ---------------------------------------------------------*/
static unsigned char encoderState;

const unsigned char transitionTable[7][4] = {
  {R_START,    R_CW_BEGIN,  R_CCW_BEGIN, R_START},              // R_START
  {R_CW_NEXT,  R_START,     R_CW_FINAL,  R_START | DIR_CW},     // R_CW_FINAL
  {R_CW_NEXT,  R_CW_BEGIN,  R_START,     R_START},              // R_CW_BEGIN  
  {R_CW_NEXT,  R_CW_BEGIN,  R_CW_FINAL,  R_START},              // R_CW_NEXT  
  {R_CCW_NEXT, R_START,     R_CCW_BEGIN, R_START},              // R_CCW_BEGIN
  {R_CCW_NEXT, R_CCW_FINAL, R_START,     R_START | DIR_CCW},    // R_CCW_FINAL
  {R_CCW_NEXT, R_CCW_FINAL, R_CCW_BEGIN, R_START},              // R_CCW_NEXT
};


void Encoder_Init(void)
{
  // Initialise GPIO Strcutre
  GPIO_InitTypeDef GPIO_InitStruct;
  
  // Enable GPIO Clocks
  ENCA_GPIO_CLK_ENABLE();
  ENCB_GPIO_CLK_ENABLE();
  
  // Setup GPIO pin with Rising and Falling Edge Interrupt
  GPIO_InitStruct.Pin = ENCA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  // Define pullup in hw_config.h
#ifdef PINS_PULLUP
  GPIO_InitStruct.Pull = GPIO_PULLUP;
#else
  GPIO_InitStruct.Pull = GPIO_NOPULL;
#endif
  HAL_GPIO_Init(ENCA_GPIO_Port, &GPIO_InitStruct);
  
  GPIO_InitStruct.Pin = ENCB_Pin;
  HAL_GPIO_Init(ENCB_GPIO_Port, &GPIO_InitStruct); 
  
  // Set up Interrupts 
  HAL_NVIC_SetPriority(ENCA_B_IRQn, 6, 0);
  HAL_NVIC_EnableIRQ(ENCA_B_IRQn);  
  
  // Initialise encoder state variable
  encoderState = R_START;
}


void Encoder_IRQ_Callback(void)
{
  static int16_t encoderVoltage = 0;
  static int16_t encoderCurrent = CURRENT_INIT_VALUE;    
  unsigned char result = Encoder_Process(); 
  EncoderSw_TypeDef EncoderState = Get_EncoderSwitchState();
  
  switch(EncoderState)
  {
    case COARSE_VOLTAGE:
      if (result == DIR_CW)     encoderVoltage += VOLTAGE_STEPS_FINE_MULTIPLIER;
      if (result == DIR_CCW)    encoderVoltage -= VOLTAGE_STEPS_FINE_MULTIPLIER;  
      break;
    
    case FINE_VOLTAGE:
      if (result == DIR_CW)     encoderVoltage++;
      if (result == DIR_CCW)    encoderVoltage--;  
      break;
        
    case CURRENT:
      if (result == DIR_CW)     encoderCurrent++;
      if (result == DIR_CCW)    encoderCurrent--;
      break;
  }   
   
  // Set State. Bound the values
  if (encoderVoltage <= 0)                       encoderVoltage = 0;  
  if (encoderVoltage >= NUM_STEPS_VOLTAGE)       encoderVoltage = NUM_STEPS_VOLTAGE;
  if (encoderCurrent <= CURRENT_INIT_VALUE)      encoderCurrent = CURRENT_INIT_VALUE;    
  if (encoderCurrent >= NUM_STEPS_CURRENT)       encoderCurrent = NUM_STEPS_CURRENT;
  
  Set_EncoderVoltage(encoderVoltage);
  Set_EncoderCurrent(encoderCurrent);
  
}

static unsigned char Encoder_Process(void)
{
    GPIO_PinState encoderPinA = HAL_GPIO_ReadPin(ENCA_GPIO_Port, ENCA_Pin);
    GPIO_PinState encoderPinB = HAL_GPIO_ReadPin(ENCB_GPIO_Port, ENCB_Pin);
    
    unsigned char pinState = (encoderPinB << 1) | encoderPinA;
    
    // Determine new state from the pins and state table.
    encoderState = transitionTable[encoderState & 0xf][pinState];
    
    // Return emit bits, ie the generated event.
    return encoderState & 0x30;
}