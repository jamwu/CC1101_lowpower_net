/**
  ******************************************************************************
  * @file main.c
  * @brief This file contains the main function.
  * @author STMicroelectronics - MCD Application Team
  * @version V1.1.0
  * @date    09/14/2009
  ******************************************************************************
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2009 STMicroelectronics</center></h2>
  * @image html logo.bmp
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
/**
  * @addtogroup DEMO_Functions
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Evalboard I/Os configuration */
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
INT8U RF_INIT_FLAG;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
void system_init(void);
/* Public functions ----------------------------------------------------------*/

/**
  * @brief Validation firmware main entry point.
  * @par Parameters:
  * None
  * @retval void None
  * @par Required preconditions:
  * None
  * @par Library functions called:
  */
void main(void)
{
    //INT8U RF_RSSI_reg=0;
    //INT8S RF_RSSI=0;
    system_init();
    /* Executive loop, runs continually after initialization */
    while (1)
    {
        RF_Handler();
    }
}

void system_init(void)
{
    /* Select fCPU = 16MHz */
    CLK_MasterPrescalerConfig(CLK_MasterPrescaler_HSIDiv1);   
    EXGPIO_configuration();//多余IO配置
    uart_configuration();  
    led_init();     
    /* TIMER4 initialization for generic timebase */
    Init_TIM4();    
    spi_gpio_init();
    spi_configuration();    
    RF_configuration();
    CC_IRQGPIO_INIT();   
    /* Enable general interrupts */
    enableInterrupts();
    Set_RF_INIT_FLAG();
    Erase_RF_INIT_FLAG(); //擦除RF初始化标记
    CC1101SetTRMode(RX_MODE);
    RF_get_gateway_devices();//获取在线网关
    RF_check_ack_timer = 0;
    delay_ms(2000);
    RF_INIT_FLAG = Get_RF_INIT_FLAG();
    if(RF_INIT_FLAG == 0)//RF未初始化
    {
        if(Local_Device_Type == TYPE_GATEWAY) //网关自分配地址
        {
           Local_ADDR = allocate_gateway_addr();          
        }
    }
    printf("LAD:%d\r\n",Local_ADDR);
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval : None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif


/**
  * @}
  */

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
