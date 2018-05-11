/*******************************************************************************
* File Name          : system_state.c
* Description        : Contains structure that holds system state
                       Also has get and set methods for the state
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"
#include "system_state.h"
#include "hw_config.h"
#include "adc.h"


// Stores the Encoder Value for Voltage and Current
typedef struct
{
  uint16_t voltageStep;
  uint16_t currentStep; 
  
} EncoderVal_TypeDef;

// Stores the Measured Values for output voltage and current
typedef struct 
{
  uint32_t outputVoltage;
  uint32_t outputCurrent;
      
} MeasuredVal_TypeDef; 


// Structures that hold system state
static EncoderSw_TypeDef         State_EncoderSw;        // Encoder Push Button Switch State
static OutputSw_TypeDef          State_OutputSw;         // Output Enable Switch State      
static EncoderVal_TypeDef        State_EncoderVal;       // Encoder Value for Voltage and Curent
static MeasuredVal_TypeDef       State_MeasuredVal;      // Output Voltage and Current Measured

// Create Task State ariables
TaskState_TypeDef State_ControlTask     = TASK_NOT_READY;
TaskState_TypeDef State_UITask          = TASK_NOT_READY;      
TaskState_TypeDef State_SerialTask      = TASK_NOT_READY;  


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

uint16_t Get_EncoderCurrent(void)
{
  return State_EncoderVal.currentStep;
}

// Sets the Encoder Value for Voltage and Current. Bounding done in the IRQ Handler
void Set_EncoderVoltage(int16_t val)
{  
  State_EncoderVal.voltageStep = val;
}

void Set_EncoderCurrent(int16_t val)
{ 
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

uint32_t Get_OutputVoltage(void)
{
  return State_MeasuredVal.outputVoltage;
}

uint32_t Get_OutputCurrent(void)
{
  return State_MeasuredVal.outputCurrent;
}

void Update_OutputVoltage(void)
{
// equation values are for output voltage in microvolts
#define VOLTAGE_EQ_GRADIENT     8230
#define VOLTAGE_EQ_INTERCEPT    470190  
#define VOLTAGE_EQ_MULTIPLER    1000    // 1000 changes microvolts to millivolts
 
  uint16_t adcValue = ADC_getReading(VOUT_ADC_Pin);
  uint32_t val =   (adcValue * VOLTAGE_EQ_GRADIENT) + VOLTAGE_EQ_INTERCEPT; 
  val = val/VOLTAGE_EQ_MULTIPLER;      

  State_MeasuredVal.outputVoltage = val;   
}

void Update_OutputCurrent(void)
{
  uint16_t adcValue = ADC_getReading(I_SENSE_Pin); 
  State_MeasuredVal.outputCurrent = adcValue;
}
