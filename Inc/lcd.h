/******************************************************************************
* @file           : lcd.h
* @brief          : header file for lcd.c
******************************************************************************/

#ifndef _LCD_H_
#define _LCD_H_

/* Includes ------------------------------------------------------------------*/

/* Definitions ---------------------------------------------------------------*/

// Macros
#define CS_HIGH()       HAL_GPIO_WritePin( DISP_CS_GPIO_Port,  DISP_CS_Pin,  GPIO_PIN_SET   )
#define CS_LOW()        HAL_GPIO_WritePin( DISP_CS_GPIO_Port,  DISP_CS_Pin,  GPIO_PIN_RESET )
#define DC_HIGH()       HAL_GPIO_WritePin( DISP_DC_GPIO_Port,  DISP_DC_Pin,  GPIO_PIN_SET   )
#define DC_LOW()        HAL_GPIO_WritePin( DISP_DC_GPIO_Port,  DISP_DC_Pin,  GPIO_PIN_RESET )
#define RST_HIGH()      HAL_GPIO_WritePin( DISP_RST_GPIO_Port, DISP_RST_Pin, GPIO_PIN_SET   )
#define RST_LOW()       HAL_GPIO_WritePin( DISP_RST_GPIO_Port, DISP_RST_Pin, GPIO_PIN_RESET )

// Register Definitions
#define LCD_SWRESET             0x01
#define LCD_SLPOUT              0x11
#define LCD_PIXELFMT            0x3A
#define LCD_GAMMASET            0x26
#define LCD_GAMRSEL             0xF2
#define LCD_GAMMAPCORR          0xE0
#define LCD_GAMMANCORR          0xE1
#define LCD_FRAMERATECTRL1      0xB1
#define LCD_INVCTRL             0xB4
#define LCD_PWRCTRL1            0xC0
#define LCD_PWRCTRL2            0xC1
#define LCD_VCOMCTRL1           0xC5
#define LCD_VCOMOFFSET          0xC7
#define LCD_MACTRL              0x36
#define LCD_DISPON              0x29
#define LCD_COLSET              0x2A
#define LCD_ROWSET              0x2B
#define LCD_MEMWRITE            0x2C

// LCD Size Definition
#define LCD_WIDTH_PX            160
#define LCD_HEIGHT_PX           128

// Colour Definitions
#define	BLACK   0x0000
#define	BLUE    0x001F
#define	RED     0xF800
#define	GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

/* Function Prototypes ------------------------------------------------------*/
void LCD_Init(void);
void LCD_SetAddrWindow(uint8_t x0, uint8_t y0, uint8_t width, uint8_t height);
void LCD_FillRectangle(uint8_t x0, uint8_t y0, uint8_t width, uint8_t height, uint16_t colour);
void drawPixel(uint16_t x, uint16_t y, uint16_t color);
void LCD_SendImage(uint8_t x0, uint8_t y0, const uint16_t image[]);

#endif