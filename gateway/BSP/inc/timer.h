/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TIMER_H
#define __TIMER_H

/* Includes ------------------------------------------------------------------*/
#include "stm8l10x.h"
#include "stm8l10x_tim2.h"
#include "stm8l10x_awu.h"
#include "uart.h"
/* Exported types ------------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
void Init_TIM4(void);
void delay_us(uint16_t Delay);
void delay_ms(uint16_t Delay);
void AWU_Initializes(void);
void timer4_it_handler(void);
#endif

