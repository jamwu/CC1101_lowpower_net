/*===========================================================================
* 网址 ：http://www.cdebyte.com/   http://yhmcu.taobao.com/                 *
* 作者 ：李勇  原 亿和电子工作室  现 亿佰特电子科技有限公司                 * 
* 邮件 ：yihe_liyong@126.com                                                *
* 电话 ：18615799380                                                        *
============================================================================*/

#ifndef _CC1101_H_
#define _CC1101_H_

#include "STM8l10x_conf.h"
#include "stm8l10x_itc.h"
#include "mytypedef.h"
#include "CC1101_REG.h"
#include "spi.h"
/*===========================================================================
------------------------------Internal IMPORT functions----------------------
you must offer the following functions for this module
1. INT8U SPI_ExchangeByte(INT8U input); // SPI Send and Receive function
2. CC_CSN_LOW();                        // Pull down the CSN line
3. CC_CSN_HIGH();                       // Pull up the CSN Line
===========================================================================*/
/*===========================================================================
----------------------------------macro definitions--------------------------
============================================================================*/
typedef enum { TX_MODE, RX_MODE } TRMODE;
typedef enum { BROAD_ALL, BROAD_NO, BROAD_0, BROAD_0AND255 } ADDR_MODE;
typedef enum { BROADCAST, ADDRESS_CHECK} TX_DATA_MODE;

/*===========================================================================
-------------------------------------exported APIs---------------------------
============================================================================*/
/*Initialize the WOR function of CC1101*/
void  CC1101WORInit(void);

/*read a byte from the specified register*/
INT8U CC1101ReadReg(INT8U addr);

/*Read some bytes from the rigisters continously*/
void CC1101ReadMultiReg( INT8U addr, INT8U *buff, INT8U size );

/*Read a status register*/
INT8U CC1101ReadStatus(INT8U addr);

/*Write a byte to the specified register*/
void CC1101WriteReg( INT8U addr, INT8U value );

/*Write some bytes to the specified register*/
void CC1101WriteMultiReg( INT8U addr, INT8U *buff, INT8U size );

/*Write a command byte to the device*/
void CC1101WriteCmd(INT8U command);

/*Reset the CC1101 device*/
void CC1101Reset( void );

/*Set the device as TX mode or RX mode*/
void CC1101SetTRMode(TRMODE mode);

/*Set the CC1101 into IDLE mode*/
void CC1101SetIdle(void);

/*Set the CC1101 into SWOR mode*/
void CC1101SetSWOR(void);

/*Set the CC1101 into SPWD mode*/
void CC1101SetSPWD(void);

/*Flush the TX buffer of CC1101*/
void CC1101ClrTXBuff( void );

/*Flush the RX buffer of CC1101*/
void CC1101ClrRXBuff( void );

/*Get received count of CC1101*/
INT8U CC1101GetRXCnt( void );

/*Set the address and address mode of the CC1101*/
void CC1101SetAddress(INT8U address, ADDR_MODE AddressMode);

/*Set the SYNC bytes of the CC1101*/
void CC1101SetSYNC(INT16U sync);

INT8S CC1101_RSSI_Caculate(INT8U RSSI_reg);

/*Initialize the CC1101, User can modify it*/
void CC1101Init(void);

void IOCFG2_RX_Configuration(void);

void IOCFG2_TX_Configuration(void);

#endif // _CC1101_H_

/*===========================================================================
-----------------------------------文件结束----------------------------------
===========================================================================*/
