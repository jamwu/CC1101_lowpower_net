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
void ReadE2PDatas(uint16_t offset, uint8_t *datatemp, uint8_t data_num);
void SaveE2PData(uint16_t offset, uint8_t Data);
void SaveE2PDatas(uint16_t offset, uint8_t *datatemp, uint8_t data_num);
void EraseE2PDatas(uint16_t offset, uint8_t num);
#endif