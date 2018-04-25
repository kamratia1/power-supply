/******************************************************************************
* File Name          : uart.h
* Description        : header file for uart.c
******************************************************************************/

#ifndef UART_H_
#define UART_H_

/* Definitions */

/* Public Function Prototypes */
void UART_Init(void);
void UART_Print(char out[]);
void UART_Receive_DMA_Stop(void);
void UART_Receive_DMA_Start(uint8_t buffer[], uint8_t len);

#endif /* UART_H_ */
