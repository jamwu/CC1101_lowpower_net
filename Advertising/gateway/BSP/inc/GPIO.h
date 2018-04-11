/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GPIO_H
#define __GPIO_H

/* Includes ------------------------------------------------------------------*/
#include "stm8l10x.h"
#include "stm8l10x_gpio.h"
/* Exported types ------------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/
void EXGPIO_configuration(void);
void STM8_SendBusyMode(void);
void STM8_SendIdleMode(void);

#endif