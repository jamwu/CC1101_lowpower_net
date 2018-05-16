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
    CC_IRQGPIO_INIT(); 
    uart_configuration();  
    led_init();     
    Init_TIM4();   
    spi_gpio_init();
    spi_configuration();   
    //Local_Device_Type = TYPE_TERMINAL;
    Local_Device_Type = TYPE_GATEWAY;
    RF_Reset_Check();
    //EraseE2PDatas(2, 32);//擦除保存的地址池数据
    if(Local_Device_Type == TYPE_GATEWAY)
    {
        printf("GATEWAY\r\n");
        ReadE2PDatas(2, Device_addrpool, 32); //网关读取保存的地址池数据
    }
    else
    {
        printf("TERMINAL\r\n");
    }
    RF_configuration();
    enableInterrupts();
    if(Local_ADDR == 0x02)//未初始化的终端
    {
        RF_terminal_apply_addr();
    }
    if(Local_Device_Type == TYPE_TERMINAL)
    {
        CC1101SetSWOR();
    }
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
