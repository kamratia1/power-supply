/*******************************************************************************
* File Name          : control.c
* Description        : Contains control Task
                       Handles the Power supply main control system

@Reference: Interpolation code taken from: 
https://www.electro-tech-online.com/threads/linear-interpolation-and-lookup-tables-c.147507/
*******************************************************************************/

#include "stm32f0xx_hal.h"
#include "hw_config.h"
#include "buttons.h"
#include "encoder.h"
#include "adc.h"
#include "control.h"
#include "system_state.h"
#include "enable.h"
#include "pwm.h"

/* Function Prototypes -------------------------------------------------------*/
static void Control_TimerInit(void);
static void SetOutputSw(void);
static void SetCurrentLimit(void);
static void SetOutputVoltage(void);

/* Private Variables ---------------------------------------------------------*/
TIM_HandleTypeDef ControlTimerHandle;
extern TaskState_TypeDef State_ControlTask;

void Control_Task(void)
{  

  if(State_ControlTask == TASK_READY)
    {
        State_ControlTask = TASK_NOT_READY;
        
        // Check the state of the Output Switch and set accordingly
        SetOutputSw();        

        // Set Output Voltage and current based on the encoder 
        SetCurrentLimit();
        SetOutputVoltage();
        Update_OutputVoltage();
        Update_OutputCurrent();
    }
}



// Initialise the Control Task 
void Control_Init(void)
{
  PWM_setDuty(VSW_PWM_Pin, VSW_PWM_MAX);
  Control_TimerInit();
}

// Initialise the Control Timer
static void Control_TimerInit(void)
{
  CONTROL_TIMER_CLK_ENABLE();
    
  // DebougTimerFreq = MCU_Clock_Freq/((Period+1)*(Prescaler+1))
  // Period =((CONTROL_TIMER_MS * MCU_Clock)/(Prescaler+1))-1 
  
  ControlTimerHandle.Instance = CONTROL_TIMER_TIM;
  ControlTimerHandle.Init.Prescaler = 127; // Set this to a (power of 2)-1 for integer maths
  ControlTimerHandle.Init.CounterMode = TIM_COUNTERMODE_UP;
  ControlTimerHandle.Init.Period = ((CONTROL_TIMER_MS * SystemCoreClock)/((ControlTimerHandle.Init.Prescaler + 1)*1000)) - 1; 
  ControlTimerHandle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  ControlTimerHandle.Init.RepetitionCounter = 0;
  HAL_TIM_Base_Init(&ControlTimerHandle);
  
  HAL_NVIC_SetPriority(CONTROL_TIMER_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(CONTROL_TIMER_IRQn);    
  
  HAL_TIM_Base_Start_IT(&ControlTimerHandle);
}


void Control_TimerCallback(void)
{
  //Control_Task();  
  State_ControlTask = TASK_READY;
}

static void SetOutputSw(void)
{
  if (Get_OutputSwState() == OUT_ENABLE)
  {
    Enable_Output(GPIO_PIN_SET);
  }
  else
  {
    Enable_Output(GPIO_PIN_RESET);
  }  
}

static void SetCurrentLimit(void)
{
  static uint16_t pwmValueCurrent = 0;

  // Only set pwm value if encoder value has changed
  if (pwmValueCurrent != Get_EncoderCurrent())
  {
    pwmValueCurrent = Get_EncoderCurrent() * CURRENT_COURSE_RESOLUTION_MA * ISENSE_I_TO_V_GAIN;; 
    PWM_setDuty(ISET_PWM_Pin, pwmValueCurrent);   
  }
}

static void SetOutputVoltage(void)
{  
  static uint16_t pwmVsetVoltage = 0;
  static int16_t pwmVswVoltage = 0;
  
  // Only set pwm value if encoder value has changed
  if (pwmVsetVoltage != Get_EncoderVoltage())
  {      
    pwmVsetVoltage = Get_EncoderVoltage(); // voltage value at vset
    
    // Calculate VSW_Set voltage using equation
    pwmVswVoltage = (CONST_GRADIENT*(pwmVsetVoltage + VSW_VOUT_DIFF_STEPS)) + CONST_INTERCEPT;
    pwmVswVoltage /= EQ_MULTIPLIER;
    
    // Set limits to the switching converter output voltage
    if (pwmVswVoltage <= VSW_PWM_MIN)   pwmVswVoltage = VSW_PWM_MIN;
    if (pwmVswVoltage >= VSW_PWM_MAX)   pwmVswVoltage = VSW_PWM_MAX;
    
    PWM_setDuty(VSET_PWM_Pin, pwmVsetVoltage * VSET_V_TO_V_GAIN);
    PWM_setDuty(VSW_PWM_Pin , pwmVswVoltage);
    
  }
  
}

