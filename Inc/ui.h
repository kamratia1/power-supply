/******************************************************************************
* @file           : ui.h
* @brief          : header file for ui
******************************************************************************/

#ifndef _UI_H_
#define _UI_H_


void UI_Init(void);
void UI_TimerCallback(void);
void UI_Task(void);
void drawDigits18pt(uint16_t number, uint8_t xstart, uint8_t ypos, uint16_t colour);
void drawDigits25pt(uint16_t number, uint8_t xstart, uint8_t ypos, uint16_t colour);
uint8_t* getDigits(uint16_t number);


#endif