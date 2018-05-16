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

//LSI时钟测量
uint32_t AWU_LSIMeasurement(void)
{
    uint32_t lsi_freq_hz = 0x0;
    uint32_t fmaster = 0x0;
    uint16_t ICValue1 = 0x0;
    uint16_t ICValue2 = 0x0; 
    
    /* Get master frequency */
    fmaster = CLK_GetClockFreq();
    
    CLK_PeripheralClockConfig(CLK_Peripheral_AWU,ENABLE);
    
    /* Enable the LSI measurement: LSI clock connected to timer Input Capture 1 */
    AWU->CSR |= AWU_CSR_MSR;
    /* Measure the LSI frequency with TIMER Input Capture 1 */

    /* Enable TIM2 clock */
    CLK_PeripheralClockConfig(CLK_Peripheral_TIM2, ENABLE);
    
    /* Capture only every 8 events!!! */
    /* Enable capture of TI1 */   
    TIM2_ICInit(TIM2_Channel_1,TIM2_ICPolarity_Rising,TIM2_ICSelection_DirectTI,TIM2_ICPSC_Div8,0);
    
    /* Enable TIM2 */
    TIM2_Cmd(ENABLE); 
    
    /* wait a capture on cc1 */
    while((TIM2->SR1 & TIM2_FLAG_CC1) != TIM2_FLAG_CC1);
    /* Get CCR1 value*/
    ICValue1 = TIM2_GetCapture1();
    TIM2_ClearFlag(TIM2_FLAG_CC1);

    /* wait a capture on cc1 */
    while((TIM2->SR1 & TIM2_FLAG_CC1) != TIM2_FLAG_CC1);
    /* Get CCR1 value*/
    ICValue2 = TIM2_GetCapture1();
    TIM2_ClearFlag(TIM2_FLAG_CC1);

    /* Disable IC1 input capture */
    TIM2->CCER1 &= (uint8_t)(~TIM_CCER1_CC1E);
    /* Disable timer2 */
    TIM2_Cmd(DISABLE); 
    
    /* DISABLE TIM2 clock */
    CLK_PeripheralClockConfig(CLK_Peripheral_TIM2, DISABLE);   
    
    /* Compute LSI clock frequency */
    lsi_freq_hz = (8 * fmaster) / (ICValue2 - ICValue1);

    /* Disable the LSI measurement: LSI clock disconnected from timer Input Capture 1 */
    AWU->CSR &= (uint8_t)(~AWU_CSR_MSR);

    return (lsi_freq_hz);
}
//AWU自动唤醒
void AWU_Initializes(void)
{
  AWU_LSICalibrationConfig(AWU_LSIMeasurement());//校正LSI时钟
  AWU_Init(AWU_Timebase_1s);                     //初始化AWU
  AWU_Cmd(ENABLE);
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