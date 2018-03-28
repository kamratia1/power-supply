/******************************************************************************
* File Name          : uart.h
* Description        : header file for uart.c
******************************************************************************/

#ifndef UART_H_
#define UART_H_

/* Definitions */
#define RXBUFFERSIZE    32

/* Public Function Prototypes */
void UART_Init(void);
void UART_print(char out[]);
void UART_putchar(char c);

#endif /* UART_H_ */
