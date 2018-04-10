/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __UART_H
#define __UART_H

/* Includes ------------------------------------------------------------------*/
#include "stm8l10x.h"
#include "stdio.h"
/* Exported types ------------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/
#define uart_send_temp_num 255
#define uart_receive_temp_num 255
/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
extern uint8_t uart_receive_temp[uart_receive_temp_num];
extern uint16_t uart_receive_num;
extern uint16_t uart_receive_count;

extern uint8_t uart_send_temp[uart_send_temp_num];
extern uint16_t uart_send_num;
extern uint16_t uart_send_count;

extern uint8_t uart_interrupt_flag;
extern uint8_t uart_timer_count;
extern uint8_t uart_receive_timeout_flag;
/* Exported functions --------------------------------------------------------*/
void uart_configuration(void);
int putchar(int c);
void uart_send_bits(uint8_t* str,uint16_t num);
void copy_datas(uint8_t* buff1,uint8_t* buff2,uint8_t num);
void uart_rx_it_handler(void);

#endif