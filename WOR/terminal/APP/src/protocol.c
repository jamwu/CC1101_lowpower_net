/* Includes ------------------------------------------------------------------*/
#include "protocol.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
//*************0x00~0x0A,其中0x00为广播地址，0x01~0x0A为网关地址**************//
//********************0x0B~0XFE为终端地址,共255个地址************************//
INT8U Device_addrpool[255]={0};//地址分配池
INT8U Device_aliveadd[255]={0};//设备在线池
/* Private variables ---------------------------------------------------------*/
INT8U RF_ack_get_flag = 0;
INT8U send_device_type;
INT8U receive_device_type;
INT8U send_addr;
INT8U receive_addr;
INT8U function_byte;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
void frame_head(INT8U target_addr,INT8U function_byte,INT8U cmd_type_byte)
{
    tx_Base_DATA.payload[0] = FRAMEDATA_HEAD;
    tx_Base_DATA.payload[1] = Local_ADDR;
    tx_Base_DATA.payload[2] = target_addr;
    tx_Base_DATA.Target_addr = target_addr;
    tx_Base_DATA.payload[3] = function_byte;      
    tx_Base_DATA.payload[4] = cmd_type_byte;
}

INT8U wireless_addr_type(INT8U addr)
{
    INT8U device_type = 0;
    if((addr >= 0x01)&&(addr <= 0x0A))//gateway addr 
    {
        device_type = TYPE_GATEWAY;
    }
    else if((addr >= 0x0B)&&(addr <= 0xFE))//terminal addr 
    {
        device_type = TYPE_TERMINAL;
    }
    else
    {
        device_type = TYPE_BROADCAST;
    }
    return device_type;  
}

void wireless_addr_mark(INT8U addr)
{
    Device_addrpool[addr] = 1;//地址占用标记
    Device_aliveadd[addr]++;
    if(Device_aliveadd[addr] >= 3)
    {
        Device_aliveadd[addr] = 3;
    }
}

void wireless_addr_demark(INT8U addr)
{
    if(Device_aliveadd[addr] > 0)
    {
        Device_aliveadd[addr]--;
    }
}

INT8U select_best_gateway(void)
{
    INT8U i,temp=0;
    for(i=0;i<11;i++)
    {
        if(Device_aliveadd[i] > temp)
        {
            temp = i;
        }
    }
    return temp;
}

INT8U allocate_gateway_addr(void)
{
    INT8U i,temp=0;
    for(i=1;i<11;i++)
    {
        if(Device_addrpool[i] == 0)
        {
            temp = i;
            Device_addrpool[i] = 1;
            break;
        }
    }    
    return temp;
}

INT8U allocate_terminal_addr(void)
{
    INT8U i,temp=0;
    for(i=11;i<255;i++)
    {
        if(Device_addrpool[i] == 0)
        {
            temp = i;
            break;
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

void RF_terminal_send_ACK(INT8U target_addr,INT8U function_byte)
{
    uint8_t status;
    frame_head(target_addr,function_byte,FRAME_ACK);
    tx_Base_DATA.size = 5;
    status = RF_terminal_send_data(tx_Base_DATA.payload, tx_Base_DATA.size, tx_Base_DATA.Target_addr);
    if(status == 1)
    {
        uart_send_bits("TCK_S\r\n",7);
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
            delay_ms(50);//50ms
        }
    }
    RF_ack_get_flag = 0;
    return status;
}

void RF_gateway_send_ACK(INT8U target_addr,INT8U function_byte)
{
    uint8_t status;
    frame_head(target_addr,function_byte,FRAME_ACK);
    tx_Base_DATA.size = 5;
    //uart_send_bits(tx_Base_DATA.payload,tx_Base_DATA.size);   
    status = RF_gateway_send_data(tx_Base_DATA.payload, tx_Base_DATA.size, tx_Base_DATA.Target_addr);
    if(status == 1)
    {
        uart_send_bits("GCK_S\r\n",7);
    }  
    else
    {
        wireless_addr_demark(tx_Base_DATA.Target_addr);
    }
}

void RF_gateway_send_terminal_allocate_addr(INT8U target_addr)
{
    INT8U status = 0;
    INT8U allocate_addr = allocate_terminal_addr();
    frame_head(target_addr,FRAME_RETURN_TERMINAL_ADDR,FRAME_ACK);
    tx_Base_DATA.payload[5] = allocate_addr;
    tx_Base_DATA.size = 6;
    status = RF_gateway_send_data(tx_Base_DATA.payload, tx_Base_DATA.size, tx_Base_DATA.Target_addr);
    if(status == 1)
    {
        uart_send_bits("GSD_S\r\n",7);
    }
}

void RF_gateway_get_ack(void)
{
    if(rx_Base_DATA.payload[0] == FRAMEDATA_HEAD)
    {
        send_addr = rx_Base_DATA.payload[1];
        wireless_addr_mark(send_addr);
        if(rx_Base_DATA.payload[4] == FRAME_ACK)
        {
            RF_ack_get_flag = 1;
        }
    }
}

void RF_get_gateway_devices(void)
{
    uint8_t status;
    frame_head(TYPE_BROADCAST,FRAME_GET_GATEWAY_ADDR,FRAME_CMD);
    tx_Base_DATA.size = 5;
    status = RF_terminal_send_data(tx_Base_DATA.payload, tx_Base_DATA.size, tx_Base_DATA.Target_addr);
    if(status == 1)
    {
        uart_send_bits("GGD_S\r\n",7);
    }    
}

void recive_ack_handle(void)
{
    uart_send_bits("ACK_G\r\n",7);
    if(receive_device_type == TYPE_TERMINAL)//若为终端接受的ACK
    { 
        if(function_byte == FRAME_RETURN_TERMINAL_ADDR)//返回终端申请的地址
        {
            Local_ADDR = rx_Base_DATA.payload[5];
            send_addr = Local_ADDR; 
        }
        RF_terminal_send_ACK(send_addr,function_byte); //终端再返回一个ACK
    }    
}

void recive_cmd_handle(void)
{
    if(receive_device_type == TYPE_BROADCAST)//收到广播消息
    {
        if(function_byte == FRAME_GET_GATEWAY_ADDR)//获取网关地址命令
        {
            if(Local_Device_Type == TYPE_GATEWAY)
            {
                RF_gateway_send_ACK(TYPE_BROADCAST,function_byte);
            }
        }
        else if(function_byte == FRAME_GET_TERMINAL_ADDR)//获取终端地址命令
        {
            if(Local_Device_Type == TYPE_TERMINAL)
            {
                RF_terminal_send_ACK(TYPE_BROADCAST,function_byte);
            }                    
        }
    }
    else
    {
        if(function_byte == FRAME_DATA_TRANSFER)//数据传输
        {
            if(send_device_type == TYPE_TERMINAL)//发送ACK
            {
                //发送端是终端
                RF_gateway_send_ACK(send_addr,function_byte);
            }
            else
            {
                RF_terminal_send_ACK(send_addr,function_byte);
            } 
            uart_send_bits(&rx_Base_DATA.payload[5],rx_Base_DATA.size - 5); //发送到串口  
        }
        else if(function_byte == FRAME_APPLY_TERMINAL_ADDR)//终端向网关申请地址
        {
            RF_gateway_send_terminal_allocate_addr(send_addr);//也是一种ACK
        }
    }   
}

void wireless_protocol_handle(void)
{
    receive_addr = rx_Base_DATA.payload[2];
    function_byte = rx_Base_DATA.payload[3];
    receive_device_type = wireless_addr_type(receive_addr);
    send_device_type = wireless_addr_type(send_addr);
    if(rx_Base_DATA.payload[0] == FRAMEDATA_HEAD)
    {
        if(rx_Base_DATA.payload[4] == FRAME_ACK)//若为ACK
        {
            recive_ack_handle();
        }
        else //若为CMD
        {
            recive_cmd_handle();    
        }
    }
}

void uart_protocol_handle(void)
{
    uint8_t status = 0;
    uart_send_bits(uart_receive_temp,uart_receive_num); //回传
    Target_ADDR = select_best_gateway();
    status = RF_terminal_send_data(uart_receive_temp, uart_receive_num, Target_ADDR);
    if(status == 0)//发送不成功
    {
        wireless_addr_demark(Target_ADDR);
    }
    else
    {
        uart_send_bits("Data_S\r\n",8);
    }
}


