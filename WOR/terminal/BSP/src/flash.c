/* Includes ------------------------------------------------------------------*/
#include "flash.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
uint8_t ReadE2PData(uint16_t offset)
{
    uint8_t Read_Word;
    uint16_t add = 0x9800+offset;
    
    FLASH_SetProgrammingTime(FLASH_ProgramTime_Standard);
    FLASH_Unlock(FLASH_MemType_Program);
    
    Read_Word = FLASH_ReadByte(add);
    
    FLASH_WaitForLastOperation();
    FLASH_Lock(FLASH_MemType_Program); 
    
    return Read_Word;
}

void SaveE2PData(uint16_t offset, uint8_t Data)
{
    uint16_t add = 0x9800+offset;
    
    FLASH_SetProgrammingTime(FLASH_ProgramTime_Standard);
    FLASH_Unlock(FLASH_MemType_Program);
    
    FLASH_ProgramByte(add , Data);
    
    FLASH_WaitForLastOperation();  
    FLASH_Lock(FLASH_MemType_Program); 
}