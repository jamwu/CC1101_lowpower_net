/* Includes ------------------------------------------------------------------*/
#include "RF.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
INT8U Local_ADDR;
INT8U Target_ADDR;

TRMODE RF_TRX_MODE=RX_MODE;

INT8U RF_RX_status[2]={0};
INT8U RF_RX_temp[64]={0};
INT8U RF_TX_temp[64]={0};

INT16U RF_timeout_count=0;
INT16U RF_check_timer=0;
INT8U RF_check_status=0;

INT8U RF_status=0;

TX_Base_DATA tx_Base_DATA = {
  0,
  0,
  RF_TX_temp
};

RX_Base_DATA rx_Base_DATA = {
  0,
  0,
  RF_TX_temp,
  0,
  0,
  0  
};

INT8U RF_received_flag = 0; 
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
void RF_configuration(void)
{
  CC1101Init();
  Set_Local_ADDR(0X01);
  Set_Target_ADDR(0X02);
  Local_ADDR = Get_Local_ADDR();
  Target_ADDR = Get_Target_ADDR();
  CC1101SetAddress(Local_ADDR, BROAD_0);    //BROAD_0 
  CC1101SetTRMode( RX_MODE );
  RF_TRX_MODE=RX_MODE;
}

void Set_Local_ADDR(INT8U Local_Addr)
{
    SaveE2PData(0, Local_Addr);
}

INT8U Get_Local_ADDR(void)
{
    return ReadE2PData(0);
}

void Set_Target_ADDR(INT8U Target_Addr)
{
    SaveE2PData(1, Target_Addr);
}

INT8U Get_Target_ADDR(void)
{
    return ReadE2PData(1);
}

//长度不能超过60字节
void RF_TX_DATA(INT8U *txbuffer, INT8U size, INT8U addr)
{
    CC1101ClrTXBuff();
    RF_TRX_MODE=TX_MODE;
    
    CC1101SetTRMode( TX_MODE ); 
    while(1);
    delay_ms(100);
    CC1101WriteReg( CC1101_TXFIFO, size + 1);
    CC1101WriteReg( CC1101_TXFIFO, addr);
    CC1101WriteMultiReg( CC1101_TXFIFO, txbuffer, size);
    
    RF_timeout_count=0;
    while(CC_GDO0_READ() == 0)
    {
        if(RF_timeout_count >= 100)//20ms
        {
          break;
        }     
    }
    RF_timeout_count=0;
    while(CC_GDO0_READ() != 0)
    {
        if(RF_timeout_count >= 100)//20ms
        {
          break;
        }            
    }
}

//任意长度
void RF_TX_InfiniteDATA(INT8U *txbuffer, INT16U size, INT8U addr)
{
    INT16U i;
    INT16U size_remainder = size % 60;//余数
    INT16U size_aliquot = size / 60;//商
    if(size < 60)
    {
        RF_TX_DATA(txbuffer, size, addr);
    }
    else
    {
        for(i=0;i<size_aliquot;i++)
        {
            delay_ms(1);
            RF_TX_DATA(txbuffer+i*60, 60, addr);
        }
        if(size_remainder != 0)
        {
            delay_ms(1);
            RF_TX_DATA(txbuffer+size_aliquot*60, size_remainder, addr);      
        }
    }
    CC1101ClrTXBuff(); 
    CC1101SetTRMode( RX_MODE );
    RF_TRX_MODE = RX_MODE;
}

void RF_GD0_it_Handler(void)
{
    INT8U rx_bytes;
    if(RF_TRX_MODE == RX_MODE)//接收模式
    {
        if(CC_GDO0_READ() == 0)//接收完成
        {           
            rx_bytes=CC1101ReadReg(CC1101_RXBYTES);
            if((rx_bytes & 0x7f) != 0)
            {
                rx_Base_DATA.size=CC1101ReadReg(CC1101_RXFIFO);
                if(rx_Base_DATA.size != 0)
                {
                   rx_Base_DATA.size -= 1;
                   rx_Base_DATA.Target_addr=CC1101ReadReg(CC1101_RXFIFO);
                    CC1101ReadMultiReg(CC1101_RXFIFO, rx_Base_DATA.payload, rx_Base_DATA.size); 
                    CC1101ReadMultiReg(CC1101_RXFIFO, RF_RX_status, 2);   // Read  status bytes     
                    rx_Base_DATA.RSSI=RF_RX_status[0];
                    rx_Base_DATA.CRC=RF_RX_status[1] & CRC_OK;
                    rx_Base_DATA.LQI=RF_RX_status[1] & 0x7f;
                    if( rx_Base_DATA.CRC )
                    {
                        RF_received_flag = 1;                                 
                    }
                }
            }
            LED1_OFF();
            CC1101ClrRXBuff();
            CC1101SetTRMode( RX_MODE );
        }
        else//接收到sync  
        {     
             LED1_ON();
        }
    } 
}

void RF_GD2_it_Handler(void)
{

}

void RF_Handler(void)
{
    if(RF_received_flag == 0)//没有接收完无线数据(优先处理无线数据)
    {
        if(uart_receive_timeout_flag == 1)//接收完一帧串口数据       
        {
            uart_send_bits(uart_receive_temp,uart_receive_num); //回传
            RF_TX_InfiniteDATA(uart_receive_temp, uart_receive_num, Target_ADDR);
            uart_receive_timeout_flag = 0;
        }
    }
    else  //接收完一帧无线数据
    {
        uart_send_bits(rx_Base_DATA.payload,rx_Base_DATA.size); //发送到串口          
        RF_received_flag = 0;      
    }
}
