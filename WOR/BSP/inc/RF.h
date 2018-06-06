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
//数据帧头
#define FRAMEDATA_HEAD 0XEB
//无线设备定义
#define BROADCAST_ADDR 0X00
#define DEFAULT_GATEWAY_ADDR 0X01
#define DEFAULT_TERMINAL_ADDR 0X80
//无线指令类型
#define RF_FRAME_ACK 0X01
#define RF_FRAME_CMD 0X02
//无线协议功能字
#define RF_FRAME_DATA_TRANSFER 0X01
#define RF_FRAME_SET_ADDR 0X02
#define RF_FRAME_GET_TARGET_RSSI 0X03
//串口本地设备表示，0X01-0XFE表示无线设备地址
#define UART_LOCAL_DEVICE 0X00
//串口功能字
#define UART_FRAME_DATA_TRANSFER 0X01
#define UART_FRAME_SET_ADDR 0X02 //串口输入功能字
#define UART_FRAME_RETURN_STATUS 0X02  //串口返回功能字
#define UART_FRAME_GET_LOCAL_ADDR 0X03
#define UART_FRAME_GET_TARGET_RSSI 0X04
#define UART_FRAME_DEBUG_SELECT 0X05
//串口返回状态字
#define UART_STATUS_OK 0X00

#define UART_ACK_SEND_TO_GATEWAY_OK 0X01
#define UART_ACK_SEND_TO_GATEWAY_FAIL 0X02

#define UART_ACK_SEND_TO_TERMINAL_OK 0X03
#define UART_ACK_SEND_TO_TERMINAL_FAIL 0X04

#define UART_ACK_GET_FROM_GATEWAY_OK 0X05
#define UART_ACK_GET_FROM_TERMINAL_OK 0X06

#define UART_RF_SET_TERMINAL_ADDR_OK 0X07
#define UART_SET_LOCAL_ADDR_OK 0X08

#define UART_RF_SEND_OK 0X09
#define UART_RF_SEND_FAIL 0X0A

#define UART_RF_SEND_CCA_BUSY 0X0B

#define UART_CMD_PERMISION_DENIED 0X0C
#define UART_SET_FORBIDDEN_ADDR 0X0D
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

extern TX_Base_DATA tx_Base_DATA;
extern RX_Base_DATA rx_Base_DATA;

extern INT8U module_ready_flag;

void RF_configuration(void);
void RF_Reset_Check(void);

void Set_Local_ADDR(INT8U Local_Addr);
INT8U Get_Local_ADDR(void);

INT8U RF_TX_DATA(INT8U *txbuffer, INT8U size, INT8U addr);

void RF_GD0_it_Handler(void);
void RF_GD2_it_Handler(void);

void RF_Handler(void);


#endif