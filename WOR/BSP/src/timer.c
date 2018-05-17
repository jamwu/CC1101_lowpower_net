/* Includes ------------------------------------------------------------------*/
#include "timer.h"
#include "spi.h"
#include "RF.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

void Init_TIM4(void)
{
    /* TimerTick = 200 us
       Warning: fcpu must be equal to 16MHz
     */
    CLK_PeripheralClockConfig(CLK_Peripheral_TIM4, ENABLE);
    
    TIM4_TimeBaseInit(TIM4_Prescaler_128, 25);

    TIM4_ITConfig(TIM4_IT_Update, ENABLE);
    
    TIM4_Cmd(ENABLE);
    
}

void delay_us(uint16_t Delay)
{
   Delay *=3;
   while(--Delay);

}

void delay_ms(uint16_t Delay)
{
    while(Delay--)
    {
        delay_us(1000);
    }
}

void timer4_it_handler(void)
{
    RF_check_timer++;
    if(RF_check_timer>65534)
    {
        RF_check_timer = 0;
    }
    //***************************************************************************************  
    RF_check_ack_timer++;
    if(RF_check_ack_timer>65534)
    {
        RF_check_ack_timer = 0;
    }
  //***************************************************************************************  
    RF_timeout_count++;
    if(RF_timeout_count > 254)
    {
        RF_timeout_count = 0;
    }
   
  //***************************************************************************************  
    spi_timer_count++;
    if(spi_timer_count > 254)
    {
        spi_timer_count = 0;
    }
//***************************************************************************************    
    if(uart_interrupt_flag)
    {
        uart_timer_count++;
    }
    if(uart_timer_count >= 5)
    {
        uart_interrupt_flag=0;
        uart_receive_num=uart_receive_count;
        uart_receive_count=0;
        uart_timer_count=0;
        uart_receive_timeout_flag=1;
        LED2_OFF();
    }
}