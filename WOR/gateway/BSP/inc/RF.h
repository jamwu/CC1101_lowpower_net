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

#define TYPE_GATEWAY 0X01
#define TYPE_TERMINAL 0X02

#define FRAME_ACK 0X01
#define FRAME_GET_GATEWAY_ADDR 0X02
#define FRAME_GET_TERMINAL_ADDR 0X03
#define FRAME_APPLY_TERMINAL_ADDR 0X04
#define FRAME_RETURN_TERMINAL_ADDR 0X05
#define FRAME_DATA_TRANSFER 0X06

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

extern INT8U Local_ADDR;
extern INT8U Target_ADDR;

extern TX_Base_DATA tx_Base_DATA;
extern RX_Base_DATA rx_Base_DATA;

void RF_configuration(void);
void Set_Local_ADDR(INT8U Local_Addr);
INT8U Get_Local_ADDR(void);
void Set_Target_ADDR(INT8U Target_Addr);
INT8U Get_Target_ADDR(void);
INT8U RF_TX_DATA(INT8U *txbuffer, INT8U size, INT8U addr);

void RF_GD0_it_Handler(void);
void RF_GD2_it_Handler(void);

void RF_Handler(void);


#endif