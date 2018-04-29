/*******************************************************************************
* File Name          : lcd.c
* Description        : Display driver low-level initialisation
                       Display driver is the ILI9163
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm32f0xx_hal.h"
#include "hw_config.h"
#include "lcd.h"
#include "enable.h"

/* Private Variables ---------------------------------------------------------*/
SPI_HandleTypeDef SPI_Handle;

struct CmdBuf_struct {
  uint8_t command;   // command byte
  uint8_t delay;     // ms delay after
  uint8_t len;       // length of parameter data
  uint8_t data[16];  // parameter data
};

static const struct CmdBuf_struct LCD_InitArray[] = {
  { LCD_SWRESET,        0, 0,  0 },       
  { LCD_SLPOUT,         0, 0,  0 },
  { LCD_PIXELFMT,       0, 1,  {0x05} },
  { LCD_GAMMASET,       0, 1,  {0x04} },
  { LCD_GAMRSEL,        0, 1,  {0x01} },
  { LCD_GAMMAPCORR,     0, 15, {0x3F, 0x25, 0x1C, 0x1E, 0x20, 0x12, 0x2A, 0x90,0x24, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00} },
  { LCD_GAMMANCORR,     0, 15, {0x20, 0x20, 0x20, 0x20, 0x05, 0x00, 0x15, 0xA7,0x3D, 0x18, 0x25, 0x2A, 0x2B, 0x2B, 0x3A} },
  { LCD_FRAMERATECTRL1, 0, 2,  {0x08, 0x09} },
  { LCD_INVCTRL,        0, 1,  {0x07} },
  { LCD_PWRCTRL1,       0, 2,  {0x0A, 0x02} },
  { LCD_PWRCTRL2,       0, 1,  {0x02} },
  { LCD_VCOMCTRL1,      0, 2,  {0x50, 0x50} },
  { LCD_VCOMOFFSET,     0, 1,  {0x40} },
  { LCD_COLSET,         0, 4,  {0x00, 0x00, 0x00, 0x7F} },
  { LCD_ROWSET,         0, 4,  {0x00, 0x00, 0x00, 0x9F} },
  { LCD_MACTRL,         0, 1,  {0x68} },
  { LCD_DISPON,         0, 0,  0 },
  { 0,                  0, 0,  0 }
};      


/* Private Function Prototypes -----------------------------------------------*/
static void LCD_GPIO_Pins_Init(void);
static void LCD_SPI_Init(void);
static void delay_ms(uint16_t ms);
static void SPI_Write(uint8_t byte);
static void LCD_SendCommand(uint8_t cmd);
static void LCD_SendData(uint8_t data);


static void LCD_GPIO_Pins_Init(void)
{      
  GPIO_InitTypeDef GPIO_InitStruct;
  
  // Pins Clock Enable
  DISP_RST_GPIO_CLK_ENABLE();
  DISP_DC_GPIO_CLK_ENABLE();
  DISP_CS_GPIO_CLK_ENABLE();
  DISP_SCK_GPIO_CLK_ENABLE();
  DISP_MOSI_GPIO_CLK_ENABLE();
  
  // SPI Peripheral Clock Enable
  SPIx_PERIPH_CLK_ENABLE();
    
  // GPIO Initialisation
  // RESET Pin Configuration
  GPIO_InitStruct.Pin = DISP_RST_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(DISP_RST_GPIO_Port, &GPIO_InitStruct);
  
  // DC Pin Configuration
  GPIO_InitStruct.Pin = DISP_DC_Pin;
  HAL_GPIO_Init(DISP_DC_GPIO_Port, &GPIO_InitStruct);
  
  // CS Pin Configuration
  GPIO_InitStruct.Pin = DISP_CS_Pin;
  HAL_GPIO_Init(DISP_CS_GPIO_Port, &GPIO_InitStruct);
    
  // SCK Pin Configuration
  GPIO_InitStruct.Pin = DISP_SCK_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Alternate = DISP_SCK_Alternate_Function;
  HAL_GPIO_Init(DISP_SCK_GPIO_Port, &GPIO_InitStruct);
    
  // MOSI Pin Configuration
  GPIO_InitStruct.Pin = DISP_MOSI_Pin;
  GPIO_InitStruct.Alternate = DISP_MOSI_Alternate_Function;
  HAL_GPIO_Init(DISP_MOSI_GPIO_Port, &GPIO_InitStruct); 
}


static void LCD_SPI_Init(void)
{   
  SPI_Handle.Instance               = DISP_SPIx;
  SPI_Handle.Init.Mode              = SPI_MODE_MASTER;
  SPI_Handle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
  SPI_Handle.Init.Direction         = SPI_DIRECTION_2LINES;
  SPI_Handle.Init.CLKPhase          = SPI_PHASE_1EDGE;
  SPI_Handle.Init.CLKPolarity       = SPI_POLARITY_LOW;
  SPI_Handle.Init.DataSize          = SPI_DATASIZE_8BIT;
  SPI_Handle.Init.FirstBit          = SPI_FIRSTBIT_MSB;
  SPI_Handle.Init.TIMode            = SPI_TIMODE_DISABLE;
  SPI_Handle.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
  SPI_Handle.Init.CRCPolynomial     = 7;
  SPI_Handle.Init.CRCLength         = SPI_CRC_LENGTH_8BIT;
  SPI_Handle.Init.NSS               = SPI_NSS_SOFT;
  SPI_Handle.Init.NSSPMode          = SPI_NSS_PULSE_DISABLE;        
  HAL_SPI_Init(&SPI_Handle);
  
  // Config NVIC for SPI
  /*HAL_NVIC_SetPriority(DISP_SPI_IRQn, 4, 0);
  HAL_NVIC_EnableIRQ(DISP_SPI_IRQn);
  
  // Configure DMA
  DMA_HandleTypeDef SPIDMA_Handle;
  SPIDMA_Handle.Instance                 = USARTx_TX_DMA_CHANNEL;
  SPIDMA_Handle.Init.Direction           = DMA_MEMORY_TO_PERIPH;
  SPIDMA_Handle.Init.PeriphInc           = DMA_PINC_DISABLE;
  SPIDMA_Handle.Init.MemInc              = DMA_MINC_ENABLE;
  SPIDMA_Handle.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
  SPIDMA_Handle.Init.MemDataAlignment    = DMA_MDATAALIGN_BYTE;
  SPIDMA_Handle.Init.Mode                = DMA_NORMAL;
  SPIDMA_Handle.Init.Priority            = DMA_PRIORITY_MEDIUM;
  HAL_DMA_Init(&SPIDMA_Handle);
  
   // Associate the initialized DMA handle to the SPI handle
  __HAL_LINKDMA(&SPI_Handle, hdmatx, SPIDMA_Handle);
  
  // Configure Interrupts
  // NVIC configuration for DMA transfer complete interrupt (USART1_TX)
  HAL_NVIC_SetPriority(USARTx_DMA_TX_IRQn, 0, 1);
  HAL_NVIC_EnableIRQ(USARTx_DMA_TX_IRQn);
  */
}



void LCD_Init(void)
{     
  // Initialise GPIO Pins and SPI peripheral
  LCD_GPIO_Pins_Init();
  LCD_SPI_Init();  
  
  CS_HIGH();
  DC_HIGH();
  
  // Hardware Reset of the display
  RST_HIGH();
  delay_ms(5);
  RST_LOW();
  delay_ms(20);
  RST_HIGH();
  delay_ms(150);
  
  const struct CmdBuf_struct *cmd;
  
  // Sends the commands and data for initialising the LCD
  for (cmd=LCD_InitArray; cmd->command; cmd++)
  {   
    LCD_SendCommand(cmd->command);
    if (cmd->len)
    {
      for(int j=0; j<cmd->len; j++)
      {
        LCD_SendData(cmd->data[j]);
      }     
    }
    if (cmd->delay)
    {
      delay_ms(cmd->delay);
    }
  }
}

void LCD_SetAddrWindow(uint8_t x0, uint8_t y0, uint8_t width, uint8_t height)
{
  LCD_SendCommand(LCD_COLSET);
  LCD_SendData(0x00);
  LCD_SendData(x0);
  LCD_SendData(0x00);
  LCD_SendData(x0+height);
  
  LCD_SendCommand(LCD_ROWSET);
  LCD_SendData(0x00);
  LCD_SendData(y0);
  LCD_SendData(0x00);
  LCD_SendData(y0+width);
  
  LCD_SendCommand(LCD_MEMWRITE);
  
}

void drawPixel(uint16_t x, uint16_t y, uint16_t color)
{
  LCD_SetAddrWindow(x, y, x+1, y+1);
  LCD_SendData(color >> 8);
  LCD_SendData(color);
  
}

void LCD_FillRectangle(uint8_t x0, uint8_t y0, uint8_t width, uint8_t height, uint16_t colour)
{
  LCD_SetAddrWindow(x0, y0, height, width);
  
  DC_HIGH();
  CS_LOW();
  for(int i=0; i<(width*height); i++)
  {
    SPI_Write(colour >> 8);
    SPI_Write(colour);    
  }
  CS_HIGH();
}

static void delay_ms(uint16_t ms)
{
    HAL_Delay(ms);
}

static void SPI_Write(uint8_t byte)
{          
  HAL_SPI_Transmit(&SPI_Handle, (uint8_t*) &byte, 1, 1000);
}

static void LCD_SendCommand(uint8_t cmd)
{
  CS_LOW();
  DC_LOW();
  SPI_Write(cmd);
  CS_HIGH();  
}

static void LCD_SendData(uint8_t data)
{
  CS_LOW();
  DC_HIGH();
  SPI_Write(data);
  CS_HIGH();
}

