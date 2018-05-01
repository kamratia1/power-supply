/*******************************************************************************
* File Name          : system_state.c
* Description        : Contains structure that holds system state
                       Also has get and set methods for the state
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"
#include "system_state.h"
#include "adc.h"

// Stores the Encoder Value for Voltage and Current
typedef struct
{
  uint16_t voltageStep;
  uint16_t currentStep; 
  
} EncoderVal_TypeDef;

// Stores the Measured Values from all the ADC pins
typedef struct 
{
  uint32_t inputVoltage;
  uint32_t switchingRegOutputVoltage;
  uint32_t linearRegInputVoltage;
  uint32_t isrcCurrent;
  uint32_t isenseCurrent;
  uint32_t outputVoltage;
  uint32_t outputCurrent;
      
} MeasuredVal_TypeDef; 


typedef struct
{
  uint16_t outputVoltage;
  uint16_t outputCurrent;  
  
} SetVal_TypeDef;


// Structures that hold system state
static EncoderSw_TypeDef         State_EncoderSw;        // Encoder Push Button Switch State
static OutputSw_TypeDef          State_OutputSw;         // Output Enable Switch State      
static EncoderVal_TypeDef        State_EncoderVal;       // Encoder Value for Voltage and Curent
static MeasuredVal_TypeDef       State_MeasuredVal;      // Output Voltage and Current Measured

void SystemState_Init(void)
{
  Set_OutputSwState(OUT_DISABLE);
  Set_EncoderSwitchState(COARSE_VOLTAGE);
  Set_EncoderVoltage(0);
  Set_EncoderCurrent(0);  
}


// Get Encoder State
uint16_t Get_EncoderVoltage(void)
{
  return State_EncoderVal.voltageStep;
}

uint16_t Get_EncodeCurrent(void)
{
  return State_EncoderVal.currentStep;
}

// Sets the Encoder Value for Voltage and Current with appropriate boundings
void Set_EncoderVoltage(int16_t val)
{  
  if (val <= 0)                       val = 0;  
  if (val >= NUM_STEPS_VOLTAGE)       val = NUM_STEPS_VOLTAGE;
  
  State_EncoderVal.voltageStep = val;
}

void Set_EncoderCurrent(int16_t val)
{
  if (val <= 0)                       val = 0;    
  if (val >= NUM_STEPS_CURRENT)       val = NUM_STEPS_CURRENT;
  
  State_EncoderVal.currentStep = val;
}

// Get and Set the Encoder Switch State
EncoderSw_TypeDef Get_EncoderSwitchState(void)
{
  return State_EncoderSw;
}

void Set_EncoderSwitchState(EncoderSw_TypeDef State)
{
  State_EncoderSw = State;  
}

// Get and Set the Output Enable Switch State
OutputSw_TypeDef Get_OutputSwState(void)
{
  return State_OutputSw;
}

void Set_OutputSwState(OutputSw_TypeDef State)
{
  State_OutputSw = State;  
}

