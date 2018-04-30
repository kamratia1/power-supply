/******************************************************************************
* File Name          : uart.c
* Description        : Code for initialising and setting up UART peripheral
******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "hw_config.h"
#include "stm32f0xx_hal.h"
#include "main.h"
#include "uart.h"
#include "printf.h"

typedef enum{
  NOT_READY = 0,
  READY=1
}State_TypeDef;

/* Private Variables ---------------------------------------------------------*/
UART_HandleTypeDef UART_Handle;
State_TypeDef UART_TxComplete = READY;


/* Private Function Prototypes*/


void UART_Init(void)
{
  // Enable GPIO and UART Clocks
  USARTx_GPIO_PORT_CLK_ENABLE();
  USARTx_CLK_ENABLE();

  // Set up UART GPIO pins
  GPIO_InitTypeDef GPIO_InitStruct;
  GPIO_InitStruct.Pin = UART_TX_Pin | UART_RX_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = UART_AF;
  HAL_GPIO_Init(UART_GPIO_Port, &GPIO_InitStruct);

  // Set Up UART
  UART_Handle.Instance = USARTx;
  UART_Handle.Init.BaudRate = BAUDRATE;
  UART_Handle.Init.WordLength = UART_WORDLENGTH_8B;
  UART_Handle.Init.StopBits = UART_STOPBITS_1;
  UART_Handle.Init.Parity = UART_PARITY_NONE;
  UART_Handle.Init.Mode = UART_MODE_TX_RX;
  UART_Handle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  UART_Handle.Init.OverSampling = UART_OVERSAMPLING_16;
  HAL_UART_Init(&UART_Handle);
  
  // Config NVIC for UART
  HAL_NVIC_SetPriority(USARTx_IRQn, 0, 1);
  HAL_NVIC_EnableIRQ(USARTx_IRQn);
  
  // Configure DMA
  static DMA_HandleTypeDef DMATX_Handle;
  static DMA_HandleTypeDef DMARX_Handle;
  
  // Enable DMA clock
  DMAx_CLK_ENABLE();
   
  // Configure the DMA handler for transmission process
  DMATX_Handle.Instance                 = USARTx_TX_DMA_CHANNEL;
  DMATX_Handle.Init.Direction           = DMA_MEMORY_TO_PERIPH;
  DMATX_Handle.Init.PeriphInc           = DMA_PINC_DISABLE;
  DMATX_Handle.Init.MemInc              = DMA_MINC_ENABLE;
  DMATX_Handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
  DMATX_Handle.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
  DMATX_Handle.Init.Mode                = DMA_NORMAL;
  DMATX_Handle.Init.Priority            = DMA_PRIORITY_MEDIUM;
  HAL_DMA_Init(&DMATX_Handle);

  // Configure the DMA handler for reception process
  DMARX_Handle.Instance                 = USARTx_RX_DMA_CHANNEL;
  DMARX_Handle.Init.Direction           = DMA_PERIPH_TO_MEMORY;
  DMARX_Handle.Init.PeriphInc           = DMA_PINC_DISABLE;
  DMARX_Handle.Init.MemInc              = DMA_MINC_ENABLE;
  DMARX_Handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
  DMARX_Handle.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
  DMARX_Handle.Init.Mode                = DMA_NORMAL;
  DMARX_Handle.Init.Priority            = DMA_PRIORITY_MEDIUM;
  HAL_DMA_Init(&DMARX_Handle);
  
  // Remap UART TX and RX DMA Channels from 2 and 3 to 4 and 5 respectively
  if (DMATX_Handle.Instance == DMA1_Channel4)
  {
    __HAL_DMA_REMAP_CHANNEL_ENABLE(DMA_REMAP_USART1_TX_DMA_CH4);
  }
  if (DMATX_Handle.Instance == DMA1_Channel5)
  {
    __HAL_DMA_REMAP_CHANNEL_ENABLE(DMA_REMAP_USART1_RX_DMA_CH5);
  }
  
   // Associate the initialized DMA handle to the UART handle
  __HAL_LINKDMA(&UART_Handle, hdmatx, DMATX_Handle);
  __HAL_LINKDMA(&UART_Handle, hdmarx, DMARX_Handle);
  
  // Configure Interrupts
  // NVIC configuration for DMA transfer complete interrupt (USART1_TX)
  HAL_NVIC_SetPriority(USARTx_DMA_TX_IRQn, 0, 1);
  HAL_NVIC_EnableIRQ(USARTx_DMA_TX_IRQn);
    
  // NVIC configuration for DMA transfer complete interrupt (USART1_RX)
  HAL_NVIC_SetPriority(USARTx_DMA_RX_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(USARTx_DMA_RX_IRQn);
  
}

//  Implemented here to use with printf library
void _putchar(char character)
{
  //HAL_UART_Transmit_DMA(&UART_Handle, (uint8_t*) character, 1);    
}


void HAL_UART_TxCpltCallback(UART_HandleTypeDef *UartHandle)
{
  /* Set transmission flag: trasfer complete*/
  UART_TxComplete = READY; 

}

// Transmit string of any length onto UART peripheral
void UART_Print(char out[])
{
  //if UART transmit is busy, then wait before transmitting character
  //while (HAL_UART_GetState(&UART_Handle) == HAL_UART_STATE_BUSY_TX);    
  //HAL_UART_Transmit(&UART_Handle, (uint8_t *) out, strlen(out), 10); 
  
  UART_TxComplete = NOT_READY;
  HAL_UART_Transmit_DMA(&UART_Handle, (uint8_t*) out, strlen(out));
  while(UART_TxComplete != READY); 
  //HAL_Delay(1);
}

void UART_Receive_DMA_Stop(void)
{
  // Stop character reception
  //HAL_UART_DMAStop(&UART_Handle);
  HAL_UART_AbortReceive(&UART_Handle);
  
}

void UART_Receive_DMA_Start(uint8_t buffer[], uint8_t len)
{
  HAL_UART_Receive_DMA(&UART_Handle, (uint8_t *)buffer, len);
}