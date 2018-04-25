/*******************************************************************************
* File Name          : adc.c
* Description        : ADC initialisation
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"
#include "hw_config.h"
#include "adc.h"
#include "uart.h"

#define NUM_ADC_CHANNELS        7

/* Private Function Prototypes -----------------------------------------------*/
static void ADC_GPIO_Init(void);

/* Private Variables ---------------------------------------------------------*/
ADC_HandleTypeDef ADC_Handle;
DMA_HandleTypeDef DMA_ADC_Handle;
uint16_t adcData[NUM_ADC_CHANNELS];

uint16_t ADC_getReading(uint16_t GPIO_Pin)
{
  uint16_t value = 0;
  
  switch(GPIO_Pin)
  {
    case VREF_FILT_Pin:
      value = adcData[0];  
      break;
    case VIN_ADC_Pin:
      value = adcData[1];  
      break;
    case VSW_ADC_Pin:
      value = adcData[2];  
      break;
    case VOUT_ADC_Pin:
      value = adcData[3];  
      break;
    case I_SENSE_Pin:
      value = adcData[4];  
      break;
    case VINL_ADC_Pin:
      value = adcData[5];  
      break;
    case ISRC_ADC_Pin:
      value = adcData[6];  
      break;    
  }
  
  return value;
}

void ADC_PrintReadings(void)
{
  char str[64];
  sprintf(str, "k,%d,%d,%d,%d,%d,%d,%d\n", adcData[0], adcData[1], adcData[2], adcData[3], adcData[4], adcData[5], adcData[6]);
  UART_Print(str);
}


static void ADC_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
  
  ADC_GPIO_CLK_ENABLE();
  
  GPIO_InitStruct.Pin = VREF_FILT_Pin|VIN_ADC_Pin|VSW_ADC_Pin|VOUT_ADC_Pin|I_SENSE_Pin|VINL_ADC_Pin|ISRC_ADC_Pin; 
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(ADC_GPIO_Port, &GPIO_InitStruct);
}

void ADC_Init(void)
{     
  ADC_ChannelConfTypeDef sConfig;
  
  // Enable ADC Clock
  ADC_CLK_ENABLE();    
  
  //Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion) 
  ADC_Handle.Instance = ADC1;
  ADC_Handle.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
  ADC_Handle.Init.Resolution = ADC_RESOLUTION_12B;
  ADC_Handle.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  ADC_Handle.Init.ScanConvMode = ADC_SCAN_DIRECTION_FORWARD;
  ADC_Handle.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  ADC_Handle.Init.LowPowerAutoWait = DISABLE;
  ADC_Handle.Init.LowPowerAutoPowerOff = DISABLE;
  ADC_Handle.Init.ContinuousConvMode = ENABLE;
  ADC_Handle.Init.DiscontinuousConvMode = DISABLE;
  ADC_Handle.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  ADC_Handle.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  ADC_Handle.Init.DMAContinuousRequests = ENABLE;
  ADC_Handle.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;
  
  //Configure GPIO Pins
  ADC_GPIO_Init();


  // ADC1 DMA Init
  // ADC Init
  DMA_ADC_Handle.Instance = DMA1_Channel1;
  DMA_ADC_Handle.Init.Direction = DMA_PERIPH_TO_MEMORY;
  DMA_ADC_Handle.Init.PeriphInc = DMA_PINC_DISABLE;
  DMA_ADC_Handle.Init.MemInc = DMA_MINC_ENABLE;
  DMA_ADC_Handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
  DMA_ADC_Handle.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
  DMA_ADC_Handle.Init.Mode = DMA_CIRCULAR;
  DMA_ADC_Handle.Init.Priority = DMA_PRIORITY_MEDIUM;
  HAL_DMA_Init(&DMA_ADC_Handle);

  __HAL_LINKDMA(&ADC_Handle,DMA_Handle,DMA_ADC_Handle);
    
  HAL_ADC_Init(&ADC_Handle);
  
  // Configure ADC Channels
  sConfig.Channel = VREF_FILT_ADC_CHANNEL;
  sConfig.Rank = ADC_RANK_CHANNEL_NUMBER;       // ADC Channel 0
  sConfig.SamplingTime = ADC_SAMPLETIME_28CYCLES_5;
  HAL_ADC_ConfigChannel(&ADC_Handle, &sConfig);

  sConfig.Channel = VIN_ADC_CHANNEL;    // ADC Channel 1
  HAL_ADC_ConfigChannel(&ADC_Handle, &sConfig);

  sConfig.Channel = VSW_ADC_CHANNEL;    // ADC Channel 2
  HAL_ADC_ConfigChannel(&ADC_Handle, &sConfig);

  sConfig.Channel = VOUT_ADC_CHANNEL;   // ADC Channel 3
  HAL_ADC_ConfigChannel(&ADC_Handle, &sConfig);

  sConfig.Channel = I_SENSE_ADC_CHANNEL;        // ADC Channel 4
  HAL_ADC_ConfigChannel(&ADC_Handle, &sConfig);

  sConfig.Channel = VINL_ADC_CHANNEL;   // ADC Channel 5
  HAL_ADC_ConfigChannel(&ADC_Handle, &sConfig);

  sConfig.Channel = ISRC_ADC_CHANNEL;   // ADC Channel 6
  HAL_ADC_ConfigChannel(&ADC_Handle, &sConfig);
  
  HAL_ADC_Start_DMA(&ADC_Handle, (uint32_t *) adcData, NUM_ADC_CHANNELS);
}