/* Includes ------------------------------------------------------------------*/
#include "RF.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
INT8S CS_Threshold = -60;//RSSI = -60dBm
INT8U Local_ADDR;
INT8U Target_ADDR;

TRMODE RF_TRX_MODE=RX_MODE;

INT8U RF_RX_status[2]={0};
INT8U RF_RX_temp[64]={0};
INT8U RF_TX_temp[64]={0};

INT16U RF_timeout_count=0;  //GDIO超时计时
INT16U RF_check_timer=0;    //载波防碰撞超时计时
INT16U RF_check_ack_timer = 0;  //接收ACK超时计时

TX_Base_DATA tx_Base_DATA = {
  0,
  0,
  RF_TX_temp
};

RX_Base_DATA rx_Base_DATA = {
  0,
  0,
  RF_RX_temp,
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
  CC1101WORInit();
  Set_Local_ADDR(0X0B);
  Set_Target_ADDR(0X01);
  Local_ADDR = Get_Local_ADDR();
  Target_ADDR = Get_Target_ADDR();
  CC1101SetAddress(Local_ADDR, BROAD_0);    //BROAD_0 
  CC1101SetSWOR();
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
INT8U RF_TX_DATA(INT8U *txbuffer, INT8U size, INT8U addr)
{
    INT8S rssi=0;
    INT8U status = 1;
    CC1101ClrTXBuff();
    RF_TRX_MODE=TX_MODE;
    CC1101SetTRMode( RX_MODE );
    delay_ms(1);
    rssi=CC1101ReadStatus(CC1101_RSSI);
    rssi=CC1101_RSSI_Caculate(rssi);
    if(rssi < CS_Threshold)
    {
        CC1101SetTRMode( TX_MODE ); 
           
        CC1101WriteReg( CC1101_TXFIFO, size + 1);
        CC1101WriteReg( CC1101_TXFIFO, addr);
        CC1101WriteMultiReg( CC1101_TXFIFO, txbuffer, size);
        
        RF_timeout_count=0;
        while(CC_GDO0_READ() == 0)
        {
            if(RF_timeout_count >= 100)//20ms
            {
              status = 0;
              break;
            }     
        }
        RF_timeout_count=0;
        while(CC_GDO0_READ() != 0)
        {
            if(RF_timeout_count >= 100)//20ms
            {
              status = 0;
              break;
            }            
        }
    }
    else
    {
        status = 0;
        //printf("CCA\n");
    }
    CC1101ClrTXBuff(); 
    CC1101SetSWOR();
    RF_TRX_MODE = RX_MODE;
    return status;
}

void RF_GD0_it_Handler(void)
{
    INT8U rx_bytes;
    if(RF_TRX_MODE == RX_MODE)//接收模式
    {
        if(CC_GDO0_READ() == 0)
        {      
            if(CC1101ReadStatus(CC1101_MARCSTATE) != 17) //not in RXFIFO_OVERFLOW status
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
                            RF_gateway_get_ack();
                            RF_received_flag = 1;                                 
                        }
                    } 
                }
            }
            LED1_OFF();
            CC1101ClrRXBuff();
            CC1101SetSWOR();
        }
        else//接收到sync  
        {     
             LED1_ON();
        }
    }
    else//发送模式
    {
        if(CC_GDO0_READ() == 0)
        {
            if(CC1101ReadStatus(CC1101_MARCSTATE) == 22)//TXFIFO_UNDERFLOW 
            {
                CC1101ClrTXBuff();
            }
        }
        else//已发送sync
        {
          
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
            uart_receive_timeout_flag = 0;
            uart_protocol_handle();
        }
    }
    else  //接收完一帧无线数据
    {
        RF_received_flag = 0;  
        wireless_protocol_handle();      
    }
}
