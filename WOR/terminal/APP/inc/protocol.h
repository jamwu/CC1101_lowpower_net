/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PROTOCOL_H
#define __PROTOCOL_H

/* Includes ------------------------------------------------------------------*/
#include "RF.h"
/* Exported types ------------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
void RF_gateway_get_ack(void);
INT8U allocate_gateway_addr(void);
INT8U select_best_gateway(void);
void RF_get_gateway_devices(void);
void wireless_protocol_handle(void);
void uart_protocol_handle(void);

#endif