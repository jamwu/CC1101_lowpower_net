/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __SPI_H
#define __SPI_H

/* Includes ------------------------------------------------------------------*/
#include "stm8l10x.h"
#include "stm8l10x_spi.h"
#include "stm8l10x_exti.h"
#include "stm8l10x_tim2.h"
#include "uart.h"
/* Exported types ------------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/
// SPI
#define PORT_SPI        GPIOB
#define PIN_SCLK        GPIO_Pin_5        
#define PIN_MOSI        GPIO_Pin_6
#define PIN_MISO        GPIO_Pin_7
// CC1101
#define PORT_CC_CSN     GPIOB
#define PIN_CC_CSN      GPIO_Pin_4

#define PORT_CC_GDO0     GPIOA
#define PIN_CC_GDO0      GPIO_Pin_2

#define PORT_CC_GDO2    GPIOA
#define PIN_CC_GDO2     GPIO_Pin_3

/* Exported macro ------------------------------------------------------------*/
extern uint8_t spi_timer_count;

void spi_gpio_init(void);
void CC_IRQGPIO_INIT(void);
void CC_CSN_LOW(void);
void CC_CSN_HIGH(void);
void CC_SCLK_HIGH(void);
void CC_SCLK_LOW(void);
void CC_SI_HIGH(void);
void CC_SI_LOW(void);
void CC_GDO2_Trigger_Rising(void);
void CC_GDO2_Trigger_Falling(void);
void CC_GDO2_EXTI_ENABLE(void);
void CC_GDO2_EXTI_DISABLE(void);
BitStatus CC_GDO2_READ(void);
BitStatus CC_GDO0_READ(void);
void spi_configuration(void);
uint8_t SPI_ExchangeByte(uint8_t input);

#endif