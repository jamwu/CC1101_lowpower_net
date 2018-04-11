/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __RF_H
#define __RF_H

/* Includes ------------------------------------------------------------------*/
#include "stm8l10x.h"
#include "CC1101.h"
#include "flash.h"
#include "uart.h"
#include "led.h"
#include "timer.h"
#include "GPIO.h"
#include "led.h"  
/* Exported types ------------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/

typedef struct TX_Base_DATA {
    INT8U size;                 
    INT8U Target_addr;
    INT8U *payload;          
}TX_Base_DATA;

typedef struct RX_Base_DATA {
    INT8U size;                 
    INT8U Target_addr;
    INT8U *payload;        
    INT8U RSSI;           
    INT8U LQI;                 
    INT8U CRC;     
}RX_Base_DATA;

extern INT16U RF_timeout_count;
extern INT16U RF_check_timer;
extern INT8U RF_check_status;

void RF_configuration(void);
void Set_Local_ADDR(INT8U Local_Addr);
INT8U Get_Local_ADDR(void);
void Set_Target_ADDR(INT8U Target_Addr);
INT8U Get_Target_ADDR(void);
//发送任意长度
void RF_TX_InfiniteDATA(INT8U *txbuffer, INT16U size, INT8U addr);

void RF_GD0_it_Handler(void);
void RF_GD2_it_Handler(void);

void RF_Handler(void);


#endif