/******************************************************************************
* @file           : hw_config.h
* @brief          : contains hardware pin configuration
******************************************************************************/

#ifndef __PINS_CONFIG_H__
#define __PINS_CONFIG_H__

// Function prototype for microsecond Delay
void Delay_us(int us); // defined in system_state.c

/* ADC Pins ------------------------------------------------------------------*/
#define ADC_DMA_CHANNEL                                 DMA1_Channel1
#define ADC_INSTANCE                                    ADC1

#define VREF_FILT_Pin 					GPIO_PIN_0
#define VREF_FILT_GPIO_Port 			        GPIOA
#define VREF_FILT_ADC_CHANNEL                           ADC_CHANNEL_0

#define VIN_ADC_Pin 					GPIO_PIN_1
#define VIN_ADC_GPIO_Port 				GPIOA
#define VIN_ADC_CHANNEL                                 ADC_CHANNEL_1

#define VSW_ADC_Pin 					GPIO_PIN_2
#define VSW_ADC_GPIO_Port 				GPIOA
#define VSW_ADC_CHANNEL                                 ADC_CHANNEL_2

#define VOUT_ADC_Pin 					GPIO_PIN_3
#define VOUT_ADC_GPIO_Port 				GPIOA
#define VOUT_ADC_CHANNEL                                ADC_CHANNEL_3

#define I_SENSE_Pin 					GPIO_PIN_4
#define I_SENSE_GPIO_Port 				GPIOA
#define I_SENSE_ADC_CHANNEL                             ADC_CHANNEL_4

#define VINL_ADC_Pin 					GPIO_PIN_5
#define VINL_ADC_GPIO_Port 				GPIOA
#define VINL_ADC_CHANNEL                                ADC_CHANNEL_5

#define ISRC_ADC_Pin 					GPIO_PIN_6
#define ISRC_ADC_GPIO_Port 				GPIOA
#define ISRC_ADC_CHANNEL                                ADC_CHANNEL_6

#define ADC_GPIO_CLK_ENABLE()                           __HAL_RCC_GPIOA_CLK_ENABLE()
#define ADC_CLK_ENABLE()                                __HAL_RCC_ADC1_CLK_ENABLE()
#define ADC_GPIO_Port                                   GPIOA

/* Display Pins --------------------------------------------------------------*/
#define DISP_BKLIGHT_GPIO_CLK_ENABLE()                  __HAL_RCC_GPIOA_CLK_ENABLE()
#define DISP_BKLIGHT_Pin 				GPIO_PIN_7
#define DISP_BKLIGHT_GPIO_Port 			        GPIOA

#define DISP_RST_GPIO_CLK_ENABLE()                      __HAL_RCC_GPIOB_CLK_ENABLE()
#define DISP_RST_Pin 					GPIO_PIN_0
#define DISP_RST_GPIO_Port 				GPIOB

#define DISP_DC_GPIO_CLK_ENABLE()                       __HAL_RCC_GPIOB_CLK_ENABLE()
#define DISP_DC_Pin 					GPIO_PIN_1
#define DISP_DC_GPIO_Port 				GPIOB

#define DISP_CS_GPIO_CLK_ENABLE()                       __HAL_RCC_GPIOB_CLK_ENABLE()
#define DISP_CS_Pin 					GPIO_PIN_2
#define DISP_CS_GPIO_Port 				GPIOB

#define DISP_SPIx                                       SPI1
#define SPIx_PERIPH_CLK_ENABLE()                        __HAL_RCC_SPI1_CLK_ENABLE()
#define DISP_SCK_Pin 					GPIO_PIN_13
#define DISP_SCK_GPIO_Port 				GPIOB
#define DISP_SCK_GPIO_CLK_ENABLE()                      __HAL_RCC_GPIOB_CLK_ENABLE()
#define DISP_SCK_Alternate_Function                     GPIO_AF0_SPI1      

#define DISP_MOSI_Pin 					GPIO_PIN_15
#define DISP_MOSI_GPIO_Port 			        GPIOB
#define DISP_MOSI_GPIO_CLK_ENABLE()                     __HAL_RCC_GPIOB_CLK_ENABLE()
#define DISP_MOSI_Alternate_Function                    GPIO_AF0_SPI1

#define DISP_MISO_Pin 					GPIO_PIN_14
#define DISP_MISO_GPIO_Port 			        GPIOB

#define DISP_SPI_IRQn                                   SPI1_IRQn
#define DISP_SPI_IRQHandler                             SPI1_IRQHandler
#define DISP_SPI_DMA_CHANNEL                            DMA1_Channel3

/* PWM Pins ------------------------------------------------------------------*/
#define TIM_PWM_CLK_ENABLE()                           __HAL_RCC_TIM1_CLK_ENABLE()
#define TIM_PWM_AF                                     GPIO_AF2_TIM1

#define VSET_PWM_Pin 					GPIO_PIN_8
#define VSET_PWM_GPIO_Port 				GPIOA
#define VSET_PWM_GPIO_CLK_ENABLE()                      __HAL_RCC_GPIOA_CLK_ENABLE()
#define VSET_PWM_TIM_CHANNEL                            TIM_CHANNEL_1

#define ISET_PWM_Pin 					GPIO_PIN_9
#define ISET_PWM_GPIO_Port 				GPIOA
#define ISET_PWM_GPIO_CLK_ENABLE()                      __HAL_RCC_GPIOA_CLK_ENABLE()
#define ISET_PWM_TIM_CHANNEL                            TIM_CHANNEL_2

#define VSW_PWM_Pin 					GPIO_PIN_10
#define VSW_PWM_GPIO_Port 				GPIOA
#define VSW_PWM_GPIO_CLK_ENABLE()                       __HAL_RCC_GPIOA_CLK_ENABLE()
#define VSW_PWM_TIM_CHANNEL                             TIM_CHANNEL_3

/* Encoder and Switch Pins ---------------------------------------------------*/
#define ENCB_Pin 					GPIO_PIN_11
#define ENCB_GPIO_Port 					GPIOA
#define ENCB_GPIO_CLK_ENABLE()                          __HAL_RCC_GPIOA_CLK_ENABLE()
#define ENCA_Pin 					GPIO_PIN_12
#define ENCA_GPIO_Port 					GPIOA
#define ENCA_GPIO_CLK_ENABLE()                          __HAL_RCC_GPIOA_CLK_ENABLE()
#define ENCA_B_IRQn                                     EXTI4_15_IRQn

#define ENC_SW_Pin 					GPIO_PIN_6
#define ENC_SW_GPIO_Port 				GPIOF
#define ENC_SW_GPIO_CLK_ENABLE()                        __HAL_RCC_GPIOF_CLK_ENABLE()
#define ENC_SW_IRQn                                     EXTI4_15_IRQn

#define SW1_Pin 					GPIO_PIN_7
#define SW1_GPIO_Port 					GPIOF
#define SW1_GPIO_CLK_ENABLE()                           __HAL_RCC_GPIOF_CLK_ENABLE()
#define SW1_IRQn                                        EXTI4_15_IRQn
#define ENC_SW1_IRQHandler                              EXTI4_15_IRQHandler

// Comment out if PULLUP is external
//#define PINS_PULLUP   

/* Timer Configurations ------------------------------------------------------*/
#define UI_TIMER_TIM                                    TIM14
#define UI_TIMER_CLK_ENABLE()                           __HAL_RCC_TIM14_CLK_ENABLE()
#define UI_TIMER_IRQn                                   TIM14_IRQn
#define UI_TIMER_IRQHandler                             TIM14_IRQHandler
#define UI_TIMER_MS                                     50

#define DEBOUNCE_TIMER_MS                               40

#define DEBUG_TIMER_TIM                                 TIM16
#define DEBUG_TIMER_CLK_ENABLE()                        __HAL_RCC_TIM16_CLK_ENABLE()
#define DEBUG_TIMER_IRQn                                TIM16_IRQn
#define DEBUG_TIMER_IRQHandler                          TIM16_IRQHandler
#define DEBUG_TIMER_MS                                  50

#define CONTROL_TIMER_TIM                               TIM3
#define CONTROL_TIMER_CLK_ENABLE()                      __HAL_RCC_TIM3_CLK_ENABLE()
#define CONTROL_TIMER_IRQn                              TIM3_IRQn
#define CONTROL_TIMER_IRQHandler                        TIM3_IRQHandler
#define CONTROL_TIMER_MS                                5

/* 
  TIM1   |      PWM
  TIM3   |      Control
  TIM14  |      UI
  TIM16  |      Debug
  TIM17  |      System Tick
  
*/

/* Enable Pins ---------------------------------------------------------------*/
#define OUT_EN_Pin 					GPIO_PIN_15
#define OUT_EN_GPIO_Port 				GPIOA
#define OUT_EN_GPIO_CLK_ENABLE()                        __HAL_RCC_GPIOA_CLK_ENABLE()

#define VSW_EN_Pin 					GPIO_PIN_9
#define VSW_EN_GPIO_Port 				GPIOB
#define VSW_EN_GPIO_CLK_ENABLE()                        __HAL_RCC_GPIOB_CLK_ENABLE()

/* UART Config ---------------------------------------------------------------*/
#define BAUDRATE                                        115200
#define USARTx                                          USART1
#define UART_TX_Pin 					GPIO_PIN_6
#define UART_GPIO_Port 					GPIOB
#define UART_RX_Pin 					GPIO_PIN_7
#define UART_AF                     	                GPIO_AF0_USART1

#define USARTx_IRQn                              	USART1_IRQn
#define USARTx_IRQHandler                        	USART1_IRQHandler
#define USARTx_DMA_TX_IRQn                              DMA1_Channel4_5_IRQn
#define USARTx_DMA_RX_IRQn                              DMA1_Channel4_5_IRQn

#define USARTx_GPIO_PORT_CLK_ENABLE()                   __HAL_RCC_GPIOB_CLK_ENABLE()
#define USARTx_CLK_ENABLE()                             __HAL_RCC_USART1_CLK_ENABLE()
#define DMAx_CLK_ENABLE()                               __HAL_RCC_DMA1_CLK_ENABLE()

#define USARTx_TX_DMA_CHANNEL                           DMA1_Channel4 
#define USARTx_RX_DMA_CHANNEL                           DMA1_Channel5
#define USARTx_DMA_IRQHandler                           DMA1_Channel4_5_IRQHandler

#endif
