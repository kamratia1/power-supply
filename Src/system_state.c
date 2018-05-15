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
  Set_EncoderCurrent(CURRENT_INIT_VALUE);  
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

uint16_t Get_DesiredVoltage(void)
{
  return Get_EncoderVoltage() * VOLTAGE_FINE_RESOLUTION_MV;
}

uint16_t Get_DesiredCurrent(void)
{
  return Get_EncoderCurrent() * CURRENT_COURSE_RESOLUTION_MA;
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
  
  static int32_t adcValueFilt = 0;  // filtered ADC value
  
  uint16_t adcValue = ADC_getReading(VOUT_ADC_Pin);
  adcValueFilt = (adcValueFilt << 2) - adcValueFilt;
  adcValueFilt += adcValue;
  adcValueFilt = adcValueFilt >> 2;
    
  uint32_t val =   (adcValueFilt * VOLTAGE_EQ_GRADIENT) + VOLTAGE_EQ_INTERCEPT; 
  val = val/VOLTAGE_EQ_MULTIPLER; 
  
  int32_t diff = (Get_EncoderVoltage() * VOLTAGE_FINE_RESOLUTION_MV) - val;
  
  // Display the encoder setting unless the difference is 100mV or less
  if (diff < 100 || diff < -100 || val < VOLTAGE_EQ_INTERCEPT/VOLTAGE_EQ_MULTIPLER)
  {    
    val = Get_EncoderVoltage() * VOLTAGE_FINE_RESOLUTION_MV;
  }
  
  // Round value to nearest 10
  val = (val/10);
  State_MeasuredVal.outputVoltage = val * 10;  
}

void Update_OutputCurrent(void)
{
  uint16_t adcValue = ADC_getReading(I_SENSE_Pin); 
  
  static int32_t adcValueFiltI = 0;  // filtered ADC value
  
  if (Get_OutputSwState() == OUT_ENABLE)
  {
    adcValueFiltI = (adcValueFiltI << 2) - adcValueFiltI;
    adcValueFiltI += adcValue;
    adcValueFiltI = adcValueFiltI >> 2;
    adcValue = adcValueFiltI;
  }
  
  // Round the Current to the nearest 10mA
  adcValue = (adcValue/10); 
  State_MeasuredVal.outputCurrent = adcValue * 10;
}
