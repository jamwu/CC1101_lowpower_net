/* Includes ------------------------------------------------------------------*/
#include "protocol.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
//*************0x00~0x0A,其中0x00为广播地址，0x01~0x0A为网关地址**************//
INT8U Gateway_addrpool[11] = {0};
//************************0x0B~0XFE,共244个终端地址***************************//
INT8U Terminal_addrpool[244]={FALSE};
/* Private variables ---------------------------------------------------------*/
INT8U RF_ack_get_flag = 0;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
void frame_head(INT8U target_addr,INT8U function_byte)
{
    tx_Base_DATA.payload[0] = FRAMEDATA_HEAD;
    tx_Base_DATA.payload[1] = Local_ADDR;
    tx_Base_DATA.payload[2] = target_addr;
    tx_Base_DATA.Target_addr = target_addr;
    tx_Base_DATA.payload[3] = function_byte;
}

INT8U wireless_addr_type(INT8U addr)
{
    INT8U device_type;
    if((addr >= 0x01)&&(addr <= 0x0A))//gateway addr 
    {
        device_type = TYPE_GATEWAY;
    }
    else if((addr >= 0x0B)&&(addr <= 0xFE))//terminal addr 
    {
        device_type = TYPE_TERMINAL;
    }
    return device_type;  
}

void wireless_addr_mark(INT8U addr)
{
    if((addr >= 0x01)&&(addr <= 0x0A))//gateway addr 
    {
        Gateway_addrpool[addr]++;
        if(Gateway_addrpool[addr] >= 3)
        {
            Gateway_addrpool[addr] = 3;
        }
    }
    else if((addr >= 0x0B)&&(addr <= 0xFE))//terminal addr 
    {
        Terminal_addrpool[addr-0x0B] = TRUE;
    }
}

void wireless_addr_demark(INT8U addr)
{
    if((addr >= 0x01)&&(addr <= 0x0A))
    {
        if(Gateway_addrpool[addr] > 0)
        {
            Gateway_addrpool[addr]--;
        }
    }
    else if((addr >= 0x0B)&&(addr <= 0xFE))
    {
        Terminal_addrpool[addr-0x0B] = FALSE;
    } 
}

INT8U select_best_gateway(void)
{
    INT8U i,temp=0;
    for(i=0;i<11;i++)
    {
        if(Gateway_addrpool[i] > temp)
        {
            temp = i;
        }
    }
    return temp;
}

uint8_t RF_terminal_send_data(INT8U *txbuffer, INT8U size, INT8U addr)
{
    uint8_t status = 0;
    RF_check_timer = 0;
    while(status != 1)
    {
        status = RF_TX_DATA(txbuffer, size, addr);
        if(RF_check_timer >= 5000) //1s
        {
            break;
        }
        if(status != 1)
        {
            delay_ms(10);//10ms
        }
    }
    RF_check_timer = 0;
    return status;
}

void RF_terminal_send_ACK(INT8U target_addr)
{
    uint8_t status;
    frame_head(target_addr,FRAME_ACK);
    tx_Base_DATA.size = 4;
    status = RF_terminal_send_data(tx_Base_DATA.payload, tx_Base_DATA.size, tx_Base_DATA.Target_addr);
    if(status == 1)
    {
        uart_send_bits("ACK_S\r\n",7);
    }
}

uint8_t RF_gateway_send_data(INT8U *txbuffer, INT8U size, INT8U addr)
{
    uint8_t status = 0;
    RF_check_ack_timer = 0;
    while(RF_ack_get_flag != 1)
    {
        status = RF_terminal_send_data(txbuffer, size, addr);
        if(RF_check_ack_timer >= 10000) //2s
        {
            status = 0;
            break;
        }
        if(RF_ack_get_flag != 1)
        {
            delay_ms(30);//30ms
        }
    }
    RF_ack_get_flag = 0;
    return status;
}

void RF_gateway_send_ACK(INT8U target_addr)
{
    uint8_t status;
    frame_head(target_addr,FRAME_ACK);
    tx_Base_DATA.size = 4;
    //uart_send_bits(tx_Base_DATA.payload,tx_Base_DATA.size);   
    status = RF_gateway_send_data(tx_Base_DATA.payload, tx_Base_DATA.size, tx_Base_DATA.Target_addr);
    if(status == 1)
    {
        uart_send_bits("ACK_S\r\n",7);
    }    
}

void RF_gateway_get_ack(void)
{
    if(rx_Base_DATA.payload[0] == FRAMEDATA_HEAD)
    {
        if(rx_Base_DATA.payload[3] == FRAME_ACK)
        {
            RF_ack_get_flag = 1;
        }
    }
}

void wireless_protocol_handle(void)
{
    INT8U send_device_type,receive_device_type;
    INT8U send_addr = rx_Base_DATA.payload[1];
    INT8U receive_addr = rx_Base_DATA.payload[2];
    wireless_addr_mark(send_addr);
    receive_device_type = wireless_addr_type(receive_addr);
    send_device_type = wireless_addr_type(send_addr);
    if(rx_Base_DATA.payload[0] == FRAMEDATA_HEAD)
    {
        if(rx_Base_DATA.payload[3] == FRAME_ACK)//若为ACK
        {
            uart_send_bits("ACK_G\r\n",7);
            if(receive_device_type == TYPE_TERMINAL)//若为终端接受的ACK
            {
                RF_terminal_send_ACK(send_addr); //终端再返回一个ACK
            }
        }
        else //若为数据
        {
            if(send_device_type == TYPE_TERMINAL)//发送ACK
            {
                //发送端是终端
                RF_gateway_send_ACK(send_addr);
            }
            else
            {
                RF_terminal_send_ACK(send_addr);
            }  
            uart_send_bits(rx_Base_DATA.payload,rx_Base_DATA.size); //发送到串口  
        }
    }
}

void uart_protocol_handle(void)
{
    uint8_t status = 0;
    uart_send_bits(uart_receive_temp,uart_receive_num); //回传
    status = RF_terminal_send_data(uart_receive_temp, uart_receive_num, Target_ADDR);
    if(status)
    {
        uart_send_bits("Data_S\r\n",8);
    }
}


