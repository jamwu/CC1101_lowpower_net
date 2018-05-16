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
    uint16_t add = 0x9FFF - offset;
    
    FLASH_SetProgrammingTime(FLASH_ProgramTime_Standard);
    FLASH_Unlock(FLASH_MemType_Program);
    
    Read_Word = FLASH_ReadByte(add);
    
    FLASH_WaitForLastOperation();
    FLASH_Lock(FLASH_MemType_Program); 
    
    return Read_Word;
}

void ReadE2PDatas(uint16_t offset, uint8_t *datatemp, uint8_t data_num)
{
    uint16_t add = 0x9FFF - offset;
    uint8_t i;
    
    FLASH_SetProgrammingTime(FLASH_ProgramTime_Standard);
    FLASH_Unlock(FLASH_MemType_Program);
    for(i=0;i<data_num;i++)
    {
        datatemp[i] = FLASH_ReadByte(add - i);
    }
    FLASH_WaitForLastOperation();
    FLASH_Lock(FLASH_MemType_Program);   
}

void SaveE2PData(uint16_t offset, uint8_t Data)
{
    uint16_t add = 0x9FFF - offset;
    
    FLASH_SetProgrammingTime(FLASH_ProgramTime_Standard);
    FLASH_Unlock(FLASH_MemType_Program);
    
    FLASH_ProgramByte(add , Data);
    
    FLASH_WaitForLastOperation();  
    FLASH_Lock(FLASH_MemType_Program); 
}

void SaveE2PDatas(uint16_t offset, uint8_t *datatemp, uint8_t data_num)
{
    uint16_t add = 0x9FFF - offset;
    uint8_t i;
    
    FLASH_SetProgrammingTime(FLASH_ProgramTime_Standard);
    FLASH_Unlock(FLASH_MemType_Program);
    for(i=0;i<data_num;i++)
    {
        FLASH_ProgramByte(add - i, datatemp[i]);
    }
    FLASH_WaitForLastOperation();  
    FLASH_Lock(FLASH_MemType_Program);     
}

void EraseE2PDatas(uint16_t offset, uint8_t num)
{
    uint16_t add = 0x9FFF - offset;
    uint8_t i;
    
    FLASH_SetProgrammingTime(FLASH_ProgramTime_Standard);
    FLASH_Unlock(FLASH_MemType_Program);
    for(i=0;i<num;i++)
    {
        FLASH_ProgramByte(add - i, 0);
    }
    FLASH_WaitForLastOperation();  
    FLASH_Lock(FLASH_MemType_Program);       
}