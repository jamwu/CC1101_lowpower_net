/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LED_H
#define __LED_H

/* Includes ------------------------------------------------------------------*/
#include "stm8l10x.h"
#include "stm8l10x_gpio.h"
/* Exported types ------------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/
#define LED1_ON()         GPIO_ResetBits(GPIOD, GPIO_Pin_0)
#define LED2_ON()         GPIO_ResetBits(GPIOB, GPIO_Pin_0)

#define LED1_OFF()        GPIO_SetBits(GPIOD, GPIO_Pin_0)
#define LED2_OFF()        GPIO_SetBits(GPIOB, GPIO_Pin_0)

#define LED1_TOG()        GPIO_ToggleBits(GPIOD, GPIO_Pin_0)
#define LED2_TOG()        GPIO_ToggleBits(GPIOB, GPIO_Pin_0)
/* Exported macro ------------------------------------------------------------*/
void led_init(void);

#endif