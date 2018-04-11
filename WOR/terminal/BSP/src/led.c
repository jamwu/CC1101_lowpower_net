/* Includes ------------------------------------------------------------------*/
#include "led.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
void led_init(void)
{
  /*** Initialize I/Os in Output Mode Push-Pull ***/
  GPIO_Init(GPIOD, GPIO_Pin_0,GPIO_Mode_Out_PP_High_Fast);
  GPIO_Init(GPIOB, GPIO_Pin_0,GPIO_Mode_Out_PP_High_Fast);
  GPIO_SetBits(GPIOD, GPIO_Pin_0);
  GPIO_SetBits(GPIOB, GPIO_Pin_0);
}



