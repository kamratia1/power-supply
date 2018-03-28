/******************************************************************************
* File Name          : uart.c
* Description        : Code for initialising and setting up UART peripheral
******************************************************************************/

#include "hw_config.h"
#include "stm32f0xx_hal.h"
#include "main.h"
#include "uart.h"
#include "string.h"

/* External Variables */
extern UART_HandleTypeDef UartHandle;

/* Private Function Prototypes*/


void UART_Init( void )
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
  UartHandle.Instance = USARTx;
  UartHandle.Init.BaudRate = BAUDRATE;
  UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
  UartHandle.Init.StopBits = UART_STOPBITS_1;
  UartHandle.Init.Parity = UART_PARITY_NONE;
  UartHandle.Init.Mode = UART_MODE_TX_RX;
  UartHandle.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  UartHandle.Init.OverSampling = UART_OVERSAMPLING_16;
  HAL_UART_Init(&UartHandle);
  
  // Config NVIC for UART
  HAL_NVIC_SetPriority(USARTx_IRQn, 0, 1);
  HAL_NVIC_EnableIRQ(USARTx_IRQn);
  
  // Configure DMA
  static DMA_HandleTypeDef hdma_tx;
  static DMA_HandleTypeDef hdma_rx;
  
  // Enable DMA clock
  DMAx_CLK_ENABLE();
  
  // Configure the DMA handler for reception process
  hdma_tx.Instance                 = USARTx_TX_DMA_CHANNEL;
  hdma_tx.Init.Direction           = DMA_MEMORY_TO_PERIPH;
  hdma_tx.Init.PeriphInc           = DMA_PINC_DISABLE;
  hdma_tx.Init.MemInc              = DMA_MINC_ENABLE;
  hdma_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
  hdma_tx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
  hdma_tx.Init.Mode                = DMA_NORMAL;
  hdma_tx.Init.Priority            = DMA_PRIORITY_LOW;
  HAL_DMA_Init(&hdma_tx);

  // Configure the DMA handler for reception process
  hdma_rx.Instance                 = USARTx_RX_DMA_CHANNEL;
  hdma_rx.Init.Direction           = DMA_PERIPH_TO_MEMORY;
  hdma_rx.Init.PeriphInc           = DMA_PINC_DISABLE;
  hdma_rx.Init.MemInc              = DMA_MINC_ENABLE;
  hdma_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
  hdma_rx.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
  hdma_rx.Init.Mode                = DMA_NORMAL;
  hdma_rx.Init.Priority            = DMA_PRIORITY_HIGH;
  HAL_DMA_Init(&hdma_rx);
  
   // Associate the initialized DMA handle to the UART handle
  __HAL_LINKDMA(&UartHandle, hdmatx, hdma_tx);
  __HAL_LINKDMA(&UartHandle, hdmarx, hdma_rx);
  
  // Configure Interrupts
  // NVIC configuration for DMA transfer complete interrupt (USART1_TX)
  HAL_NVIC_SetPriority(USARTx_DMA_TX_IRQn, 0, 1);
  HAL_NVIC_EnableIRQ(USARTx_DMA_TX_IRQn);
    
  // NVIC configuration for DMA transfer complete interrupt (USART1_RX)
  HAL_NVIC_SetPriority(USARTx_DMA_RX_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(USARTx_DMA_RX_IRQn);
  
}

// Transmit string of any length onto UART peripheral
void UART_print(char out[])
{
  //if UART transmit is busy, then wait before transmitting character
  while (HAL_UART_GetState(&UartHandle) == HAL_UART_STATE_BUSY_TX);    
  HAL_UART_Transmit(&UartHandle, (uint8_t *) out, strlen(out), 10); 
}

