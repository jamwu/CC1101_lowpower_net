/**
  ********************************************************************************
  * @file stm8l10x_iwdg.h
  * @brief This file contains all functions prototypes and macros for the IWDG peripheral.
  * @author STMicroelectronics - MCD Application Team
  * @version V1.1.0
  * @date    09/14/2009
  ******************************************************************************
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2009 STMicroelectronics</center></h2>
  * @image html logo.bmp
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM8L10x_IWDG_H
#define __STM8L10x_IWDG_H

/* Includes ------------------------------------------------------------------*/
#include "stm8l10x.h"

/* Exported variables ------------------------------------------------------- */
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/** @addtogroup IWDG_Exported_Constants
  * @{
  */

/**
  * @brief Define used to prevent watchdog reset
  */
#define IWDG_KEY_REFRESH    ((uint8_t)0xAA)  /*!<  This value written in the Key register prevent the watchdog reset */
/**
  * @brief Define used to start the watchdog counter down
  */
#define IWDG_KEY_ENABLE     ((uint8_t)0xCC)  /*!<  This value written in the Key register start the watchdog counting down*/

/**
 * @}
 */

/* Exported macros -----------------------------------------------------------*/
/* Private macros ------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/

/** @addtogroup IWDG_Exported_Types
  * @{
  */

/**
  *@brief IWDG write  acces enumeration
  */
typedef enum
{
    IWDG_WriteAccess_Enable  = (uint8_t)0x55, /*!< Code 0x55 in Key register, allow write access to Prescaler and Reload registers */
    IWDG_WriteAccess_Disable = (uint8_t)0x00  /*!< Code 0x00 in Key register, not allow write access to Prescaler and Reload registers */
} IWDG_WriteAccess_TypeDef;


/**
  *@brief IWDG prescaler enumaration
  */
typedef enum
{
    IWDG_Prescaler_4   = (uint8_t)0x00, /*!< Used to set prescaler register to 4 */
    IWDG_Prescaler_8   = (uint8_t)0x01, /*!< Used to set prescaler register to 8 */
    IWDG_Prescaler_16  = (uint8_t)0x02, /*!< Used to set prescaler register to 16 */
    IWDG_Prescaler_32  = (uint8_t)0x03, /*!< Used to set prescaler register to 32 */
    IWDG_Prescaler_64  = (uint8_t)0x04, /*!< Used to set prescaler register to 64 */
    IWDG_Prescaler_128 = (uint8_t)0x05, /*!< Used to set prescaler register to 128 */
    IWDG_Prescaler_256 = (uint8_t)0x06  /*!< Used to set prescaler register to 256 */
} IWDG_Prescaler_TypeDef;
/**
  * @}
  */
/** @addtogroup IWDG_Private_Macros
  * @{
  */

/**
  * @brief Macro used by the assert function to check the different functions parameters.
  */

/**
  * @brief Macro used by the assert function in order to check the different
  * values of the prescaler.
  */
#define IS_IWDG_PRESCALER_VALUE(VALUE) \
   (((VALUE) == IWDG_Prescaler_4)   || \
    ((VALUE) == IWDG_Prescaler_8)   || \
    ((VALUE) == IWDG_Prescaler_16)  || \
    ((VALUE) == IWDG_Prescaler_32)  || \
    ((VALUE) == IWDG_Prescaler_64)  || \
    ((VALUE) == IWDG_Prescaler_128) || \
    ((VALUE) == IWDG_Prescaler_256))

/**
  * @brief Macro used by the assert function in order to check the different
  * values of the counter register.
  */
#define IS_IWDG_WRITEACCESS_MODE(MODE) \
   (((MODE) == IWDG_WriteAccess_Enable) || \
	  ((MODE) == IWDG_WriteAccess_Disable))

/**
  * @}
  */

/* Exported functions ------------------------------------------------------- */
/** @addtogroup IWDG_Exported_Functions
  * @{
  */

void IWDG_WriteAccessCmd(IWDG_WriteAccess_TypeDef IWDG_WriteAccess);
void IWDG_SetPrescaler(IWDG_Prescaler_TypeDef IWDG_Prescaler);
void IWDG_SetReload(uint8_t IWDG_Reload);
void IWDG_ReloadCounter(void);
void IWDG_Enable(void);

/**
  * @}
  */

#endif /* __STM8L10x_IWDG_H */

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
