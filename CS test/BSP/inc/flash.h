/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FLASH_H
#define __FLASH_H

/* Includes ------------------------------------------------------------------*/
#include "stm8l10x.h"
#include "stm8l10x_flash.h"
/* Exported types ------------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/

/* Exported macro ------------------------------------------------------------*/
uint8_t ReadE2PData(uint16_t offset);
void SaveE2PData(uint16_t offset, uint8_t Data);

#endif