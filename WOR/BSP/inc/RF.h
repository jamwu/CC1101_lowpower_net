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
#include "protocol.h"
/* Exported types ------------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/
#define FRAMEDATA_HEAD 0XEB

#define TYPE_BROADCAST 0X00
#define TYPE_GATEWAY 0X01
#define TYPE_TERMINAL 0X02

#define FRAME_ACK 0X01
#define FRAME_CMD 0X02

#define FRAME_DATA_TRANSFER 0X01
#define FRAME_APPLY_TERMINAL_ADDR 0X02
#define FRAME_SYNC_TERMINAL_ADDRPOOL 0X03

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
extern INT16U RF_check_ack_timer;

extern INT8U Local_Device_Type;
extern INT8U Local_ADDR;
extern INT8U Target_ADDR;

extern TX_Base_DATA tx_Base_DATA;
extern RX_Base_DATA rx_Base_DATA;

void RF_configuration(void);
void RF_Reset_Check(void);

void Set_RF_INIT_FLAG(void);
void Erase_RF_INIT_FLAG(void);
INT8U Get_RF_INIT_FLAG(void);
void Set_RF_Local_DeviceType(INT8U devicetype);
INT8U Get_RF_Local_DeviceType(void);
void Set_Local_ADDR(INT8U Local_Addr);
INT8U Get_Local_ADDR(void);
void Set_Target_ADDR(INT8U Target_Addr);
INT8U Get_Target_ADDR(void);
void Reset_Addpool_EEPROM(void);
void Sync_Addpool_EEPROM(void);
void Get_Addpool_EEPROM(void);

INT8U RF_TX_DATA(INT8U *txbuffer, INT8U size, INT8U addr);

void RF_GD0_it_Handler(void);
void RF_GD2_it_Handler(void);

void RF_Handler(void);


#endif