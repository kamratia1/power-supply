/*******************************************************************************
* File Name          : graphics.h
* Description        : header file for graphics.h
*******************************************************************************/

#include "stm32f0xx_hal.h"


/* Function Prototypes -------------------------------------------------------*/
void startWrite(void);
void endWrite(void);

void drawFastVLine(uint16_t x, uint16_t y, uint16_t h, uint16_t color);
void drawFastHLine(uint16_t x, uint16_t y, uint16_t w, uint16_t color);
void fillRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
void fillScreen(uint16_t color);
void drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);
void drawRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
void drawCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color);
void drawCircleHelper(uint16_t x0, uint16_t y0, uint16_t r, uint8_t cornername, uint16_t color);
void fillCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color);
void fillCircleHelper(uint16_t x0, uint16_t y0, uint16_t r, uint8_t cornername,uint16_t delta, uint16_t color);
void drawTriangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
void fillTriangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
void drawRoundRect(uint16_t x0, uint16_t y0, uint16_t w, uint16_t h, uint16_t radius, uint16_t color);
void fillRoundRect(uint16_t x0, uint16_t y0, uint16_t w, uint16_t h, uint16_t radius, uint16_t color);