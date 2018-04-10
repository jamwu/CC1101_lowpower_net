/* Includes ------------------------------------------------------------------*/
#include "spi.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
uint8_t spi_timer_count=0;
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
void spi_gpio_init(void)
{
     // SPI相关IO口配置
    GPIO_Init(PORT_SPI, PIN_MISO, GPIO_Mode_In_PU_No_IT);       // MISO (PB7)
    GPIO_Init(PORT_SPI, PIN_SCLK, GPIO_Mode_Out_PP_High_Slow);  // SCLK (PB5)
    GPIO_Init(PORT_SPI, PIN_MOSI, GPIO_Mode_Out_PP_High_Slow);  // MOSI (PB6)
    
    GPIO_Init(PORT_CC_CSN, PIN_CC_CSN, GPIO_Mode_Out_PP_High_Fast);  // CSN  (PB4)   
    
    GPIO_SetBits(PORT_CC_CSN, PIN_CC_CSN);   
}

void spi_configuration(void)
{
    CLK_PeripheralClockConfig(CLK_Peripheral_SPI, ENABLE);
    
    SPI_DeInit();

    // 配置SPI相关参数,2分频（8MHZ）
    SPI_Init(SPI_FirstBit_MSB, SPI_BaudRatePrescaler_4,
             SPI_Mode_Master, SPI_CPOL_Low, SPI_CPHA_1Edge,
             SPI_Direction_2Lines_FullDuplex, SPI_NSS_Soft);  
    
    SPI_Cmd(ENABLE); 
}

void CC_IRQGPIO_INIT(void)
{ 
    // 配置CC1101相关控制引脚
    GPIO_Init(PORT_CC_GDO2, PIN_CC_GDO2,GPIO_Mode_In_FL_IT);
    EXTI_SetPinSensitivity(EXTI_Pin_3, EXTI_Trigger_Rising_Falling);
    ITC_SetSoftwarePriority(EXTI3_IRQn,ITC_PriorityLevel_1);  
    
    GPIO_Init(PORT_CC_GDO0, PIN_CC_GDO0,GPIO_Mode_In_FL_IT);
    EXTI_SetPinSensitivity(EXTI_Pin_2, EXTI_Trigger_Rising_Falling);
    ITC_SetSoftwarePriority(EXTI2_IRQn,ITC_PriorityLevel_1);      
}

void CC_CSN_LOW(void)
{
    GPIO_ResetBits(PORT_CC_CSN, PIN_CC_CSN);
    spi_timer_count=0;
    while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7)!=0)
    {     
       if(spi_timer_count >= 5)
       {
          break;
       }        
    }   
}

void CC_CSN_HIGH(void)
{
    GPIO_SetBits(PORT_CC_CSN, PIN_CC_CSN); 
}

void CC_SCLK_HIGH(void)
{
    GPIO_SetBits(PORT_SPI, PIN_SCLK);
}

void CC_SCLK_LOW(void)
{
    GPIO_ResetBits(PORT_SPI, PIN_SCLK);
}

void CC_SI_HIGH(void)
{
    GPIO_SetBits(PORT_SPI, PIN_MOSI);
}

void CC_SI_LOW(void)
{
    GPIO_ResetBits(PORT_SPI, PIN_MOSI);
}

void CC_GDO2_Trigger_Rising(void)
{
    EXTI_SetPinSensitivity(EXTI_Pin_3, EXTI_Trigger_Rising);
}

void CC_GDO2_Trigger_Falling(void)
{
    EXTI_SetPinSensitivity(EXTI_Pin_3, EXTI_Trigger_Falling);
}

void CC_GDO2_EXTI_ENABLE(void)
{
    PORT_CC_GDO2->CR2 |= PIN_CC_GDO2;
}

void CC_GDO2_EXTI_DISABLE(void)
{
    PORT_CC_GDO2->CR2 &= (uint8_t)(~(PIN_CC_GDO2));
}

BitStatus CC_GDO2_READ(void)
{
    return GPIO_ReadInputDataBit(PORT_CC_GDO2, PIN_CC_GDO2);
}

BitStatus CC_GDO0_READ(void)
{
    return GPIO_ReadInputDataBit(PORT_CC_GDO0, PIN_CC_GDO0);
}
/*===========================================================================
* 函数 ：SPI_ExchangeByte() => 通过SPI进行数据交换                          * 
* 输入 ：需要写入SPI的值                                                    * 
* 输出 ：通过SPI读出的值                                                    * 
============================================================================*/
uint8_t SPI_ExchangeByte(uint8_t input)
{
        spi_timer_count=0;
	while (RESET == SPI_GetFlagStatus(SPI_FLAG_TXE))   // 等待数据传输完成	
        {           
            if(spi_timer_count >= 5)
            {
              break;
            }         
        }
	SPI_SendData(input);
        spi_timer_count=0;
	while (RESET == SPI_GetFlagStatus(SPI_FLAG_RXNE))  // 等待数据接收完成
        {            
            if(spi_timer_count >= 5)
            {
              break;
            }          
        }
	return (SPI_ReceiveData());
}

