/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PROTOCOL_H
#define __PROTOCOL_H

/* Includes ------------------------------------------------------------------*/
#include "RF.h"
/* Exported types ------------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/

void RF_get_ack(void);
void wireless_protocol_handle(void);
void uart_protocol_handle(void);
void uart_return(INT8U target_addr,INT8U function_byte,INT8U state_byte);

#endif