/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PROTOCOL_H
#define __PROTOCOL_H

/* Includes ------------------------------------------------------------------*/
#include "RF.h"
/* Exported types ------------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
extern INT8U Device_addrpool[32];

void RF_get_ack(void);
void RF_terminal_apply_addr(void);
void wireless_protocol_handle(void);
void uart_protocol_handle(void);

#endif