/*******************************************************************************
* File Name          : system_state.h
* Description        : System State header file
*******************************************************************************/

#ifndef _SYSTEM_STATE_H_
#define _SYSTEM_STATE_H_

/* Definitions -------------------------------------------------------------- */
#define MAX_OUTPUT_VOLTAGE_V            15
#define MAX_OUTPUT_CURRENT_MA           1000
#define VOLTAGE_COURSE_RESOLUTION_MV    100
#define VOLTAGE_FINE_RESOLUTION_MV      10
#define CURRENT_COURSE_RESOLUTION_MA    5

#define NUM_STEPS_VOLTAGE               ((MAX_OUTPUT_VOLTAGE_V * 1000) / VOLTAGE_FINE_RESOLUTION_MV )
#define NUM_STEPS_CURRENT               ( MAX_OUTPUT_CURRENT_MA / CURRENT_COURSE_RESOLUTION_MA )
#define VOLTAGE_STEPS_FINE_MULTIPLIER   ( VOLTAGE_COURSE_RESOLUTION_MV / VOLTAGE_FINE_RESOLUTION_MV )

#define NUM_STEPS_PER_VOLT              (NUM_STEPS_VOLTAGE/MAX_OUTPUT_VOLTAGE_V)         

typedef enum
{
  FLAG_RESET = 0,
  FLAG_SET = 1
    
}Flag_TypeDef;

typedef enum
{
  COARSE_VOLTAGE        = 0,
  FINE_VOLTAGE          = 1,
  CURRENT               = 2,
  
  NUM_ENCODER_SW_STATES   
    
}EncoderSw_TypeDef;

typedef enum
{
  CONSTANT_VOLTAGE = 0,
  CONSTANT_CURRENT = 1
    
}PowerSupplyState_TypeDef;

typedef enum
{
  OUT_DISABLE = 0,
  OUT_ENABLE  = 1  
    
}OutputSw_TypeDef;

// TODO - Maybe add something for I_LIMIT

/* Function Prototypes -------------------------------------------------------*/
void SystemState_Init(void);

uint16_t          Get_EncoderVoltage(void);
uint16_t          Get_EncoderCurrent(void);
EncoderSw_TypeDef Get_EncoderSwitchState(void);
OutputSw_TypeDef  Get_OutputSwState(void);

void Set_EncoderVoltage(int16_t val);
void Set_EncoderCurrent(int16_t val);
void Set_EncoderSwitchState(EncoderSw_TypeDef State);
void Set_OutputSwState(OutputSw_TypeDef State);

#endif