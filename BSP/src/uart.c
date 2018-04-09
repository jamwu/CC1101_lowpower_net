/* Includes ------------------------------------------------------------------*/
#include "uart.h"
#include "stm8l10x_usart.h"
#include "stm8l10x_gpio.h"
#include "led.h"  
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t uart_receive_temp[uart_receive_temp_num]={0};
uint16_t uart_receive_num=0;
uint16_t uart_receive_count=0;

uint8_t uart_send_temp[uart_send_temp_num]={0};
uint16_t uart_send_num=0;
uint16_t uart_send_count=0;

uint8_t uart_interrupt_flag=0;
uint8_t uart_timer_count=0;
uint8_t uart_receive_timeout_flag=0;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
void uart_configuration(void)
{
    /* Enable USART clock */
    CLK_PeripheralClockConfig(CLK_Peripheral_USART, ENABLE);
    
    /* Configure USART Rx as alternate function push-pull  (software pull up)*/
    GPIO_ExternalPullUpConfig(GPIOC, GPIO_Pin_2, ENABLE);
    
    /* Configure USART Tx as alternate function push-pull  (software pull up)*/
    GPIO_ExternalPullUpConfig(GPIOC, GPIO_Pin_3, ENABLE);
    
    // 配置外部唤醒控制引脚 PB1
    GPIO_Init(GPIOB, GPIO_Pin_1, GPIO_Mode_In_PU_IT);
    EXTI_SetPinSensitivity(EXTI_Pin_1, EXTI_Trigger_Falling);
    ITC_SetSoftwarePriority(EXTI1_IRQn,ITC_PriorityLevel_1);  
    
    /*波特率115200，8位数据位，1位停止位，无校验*/
    USART_Init((uint32_t)115200,USART_WordLength_8D,USART_StopBits_1,USART_Parity_No,(USART_Mode_TypeDef)(USART_Mode_Tx | USART_Mode_Rx));  
    
    /* Enable the USART Receive interrupt: this interrupt is generated when the USART
       receive data register is not empty */
    USART_ITConfig(USART_IT_RXNE, ENABLE);
    
    USART_ClearITPendingBit();
    
    /* Enable USART */  
    USART_Cmd(ENABLE);
  
}

/**
   * @brief Retargets the C library printf function to the USART.
   * @param[in] c Character to send
   * @retval char Character sent
   * @par Required preconditions:
   * - None
   */
int putchar(int c)
{  
    /* Loop until the end of transmission */
    while (USART_GetFlagStatus(USART_FLAG_TC) == RESET);

    /* Write a character to the USART */
    USART_SendData8(c);

    return (c);
}

void uart_send_bits(uint8_t* str,uint16_t num)
{
    uint16_t i=0;
    for(i=0;i<num;i++)
    {
         /* Loop until the end of transmission */
         while (USART_GetFlagStatus(USART_FLAG_TC) == RESET);
         
         /* Write a character to the USART */
         USART_SendData8(*str++);
    }
}

void copy_datas(uint8_t* buff1,uint8_t* buff2,uint8_t num)
{
    uint8_t i=0;
    for(i=0;i<num;i++)
    {
        *(buff1+i)=*(buff2+i);
    }
}

void uart_rx_it_handler(void)
{
    LED2_ON();
    uart_timer_count=0;
    uart_interrupt_flag=1;
    if(uart_receive_count < uart_receive_temp_num)
    {
        uart_receive_temp[uart_receive_count++]=USART_ReceiveData8();
    }
    else
    {
        uart_receive_num=uart_receive_count;
        uart_receive_count=0;
        uart_receive_timeout_flag=1;
        LED2_OFF();
    }
}