/*******************************************************************************
* File Name          : encoder.h
* Description        : header file for encoder.c
*******************************************************************************/

#ifndef _ENCODER_H_
#define _ENCODER_H_

/* Definitions ---------------------------------------------------------------*/
#define R_START                 0x00
#define DIR_NONE                0x00
#define DIR_CW                  0x10
#define DIR_CCW                 0x20
#define R_CW_FINAL              0x01
#define R_CW_BEGIN              0x02
#define R_CW_NEXT               0x03
#define R_CCW_BEGIN             0x04
#define R_CCW_FINAL             0x05
#define R_CCW_NEXT              0x06

/* Function Prototypes -------------------------------------------------------*/
void Encoder_Init(void);
void Encoder_IRQ_Callback(void);


#endif
