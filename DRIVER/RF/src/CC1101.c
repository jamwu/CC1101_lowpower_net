/*
================================================================================
Copyright   : Ebyte electronic co.,LTD
Website     : http://yh-ebyte.taobao.com
              http://yiheliyong.cn.alibaba.com
Description : This module contains the low level operations for CC1101
================================================================================
*/
#include "CC1101.h"

//10, 7, 5, 0, -5, -10, -15, -20,-30 dbm output power
INT8U PaTabel[] = { 0xc0, 0xC8, 0x84, 0x60, 0x68, 0x34, 0x1D, 0x0E, 0x12};

// Sync word qualifier mode = 16/16 sync word bits detected 
// CRC autoflush = false 
// Channel spacing = 199.951172KHZ
// Data format = Normal mode 
// Data rate = 249.939KB
// RX filter BW = 541.666667KHZ
// PA ramping = false 
// Preamble count = 24 
// Whitening = false 
// Address config = address check 
// Carrier frequency = 400.199890
// Device address = 1 
// TX power = 10 
// Manchester enable = false 
// CRC enable = true 
// Deviation = 127KHZ
// Packet length mode = Variable packet length mode. Packet length configured by the first byte after sync word 
// Packet length = 255 
// Modulation format = GFSK 
// Base frequency = 399.999939 
// Modulated = true 
// Channel number = 1 

static const INT8U CC1101InitData[35][2]= 
{
  {CC1101_IOCFG2,      0x00}, 
  {CC1101_IOCFG0,      0x06}, 
  {CC1101_FIFOTHR,     0x4D}, 
  {CC1101_PKTCTRL0,    0x05},
  {CC1101_CHANNR,      0x01},
  /*******************IF frequency************************///(中频381KHZ)
  {CC1101_FSCTRL1,     0x0F},
  /*******************base frequency************************///可更改
  {CC1101_FREQ2,       0x0F},
  {CC1101_FREQ1,       0x62},
  {CC1101_FREQ0,       0x76},
  /*******************filter BW*****************************///可更改
  {CC1101_MDMCFG4,     0x2D},
  /*******************data rate*****************************///可更改
  {CC1101_MDMCFG3,     0x3B},
  /************************GFSK*****************************/  
  {CC1101_MDMCFG2,     0x16},
  /***********************preamble bytes********************/
  {CC1101_MDMCFG1,     0x72},  
  /**********************Channel spacing********************///可更改
  {CC1101_MDMCFG0,     0xF8}, 
  /**********************Deviation**************************///可更改
  {CC1101_DEVIATN,     0x62},
  {CC1101_MCSM2,       0x07},
  /*********************************************************///可更改
  {CC1101_MCSM1,       0x3F},
  {CC1101_MCSM0,       0x18},
  {CC1101_FOCCFG,      0x16},
  {CC1101_BSCFG,       0x6C},
  /*********************************************************///可更改
  {CC1101_AGCCTRL2,    0x43},
  {CC1101_AGCCTRL1,    0x49},
  {CC1101_AGCCTRL0,    0x91},
  {CC1101_WOREVT1,     0x87},
  {CC1101_WOREVT0,     0x6B},
  {CC1101_WORCTRL,     0xF8},
  {CC1101_FREND1,      0x56},
  {CC1101_FREND0,      0x10},
  {CC1101_FSCAL3,      0xEA},
  {CC1101_FSCAL2,      0x2A},
  {CC1101_FSCAL1,      0x00},
  {CC1101_FSCAL0,      0x1F},
  {CC1101_TEST2,       0x88},
  {CC1101_TEST1,       0x31},
  {CC1101_TEST0,       0x0B},
};
/*
================================================================================
Function : CC1101WORInit( )
    Initialize the WOR function of CC1101
INPUT    : None
OUTPUT   : None
================================================================================
*/
void  CC1101WORInit( void )
{

    CC1101WriteReg(CC1101_MCSM0,0x38);
    // Main Radio Control State Machine Configuration 
    //[5:4]=11,When to perform automatic calibration = Every 4th time When going from IDLE to RX or TX
    //[3:2]=10,Timeout after XOSC start = Approx. 149 – 155 μs 
    //[1]=0,Disables the pin radio control option 
    //[0]=0,Force the XOSC to stay on in the SLEEP state =  Disable
    CC1101WriteReg(CC1101_WORCTRL,0x78); 
    // Wake On Radio Control 
    //[7]=0,Power down signal to RC oscillator.When written to 0,automatic initial calibration will be performed. 
    //[6:4]=111,Event 1 timeout = (1.333 – 1.385 ms) ,clock periods after Event 0 before Event 1 times out. 
    //[3]=1,Enables the RC oscillator calibration. 
    //[1:0]=00,WOR_RES =00,Controls the Event 0 resolution as well as maximum timeout of the WOR module 
    //and maximum timeout under normal RX operation: 
    CC1101WriteReg(CC1101_MCSM2,0x06);
    //Main Radio Control State Machine Configuration 
    //[4]=0,Direct RX termination based on RSSI measurement 
    //[3]=0,When the RX_TIME timer expires, the chip checks if sync word is found when RX_TIME_QUAL=0
    //[2:0]=110,Timeout for sync word search in RX for both WOR mode and normal RX 
    //operation. The timeout is relative to the programmed EVENT0 timeout.  
    //RX timeout = 2ms
    CC1101WriteReg(CC1101_WOREVT1,0x8C);
    // High Byte Event0 
    CC1101WriteReg(CC1101_WOREVT0,0xA0);
    // Low Byte Event0 
    //Event0 = 36000
    //EVENT0 timeout = 1s
    CC1101WriteCmd( CC1101_SWORRST );
    //Reset real time clock to Event1 value. 
}
/*
================================================================================
Function : CC1101ReadReg( )
    read a byte from the specified register
INPUT    : addr, The address of the register
OUTPUT   : the byte read from the rigister
================================================================================
*/
INT8U CC1101ReadReg( INT8U addr )
{
    INT8U i;
    CC_CSN_LOW( );
    SPI_ExchangeByte( addr | READ_SINGLE);
    i = SPI_ExchangeByte( 0xFF );
    CC_CSN_HIGH( );
    return i;
}
/*
================================================================================
Function : CC1101ReadMultiReg( )
    Read some bytes from the rigisters continously
INPUT    : addr, The address of the register
           buff, The buffer stores the data
           size, How many bytes should be read
OUTPUT   : None
================================================================================
*/
void CC1101ReadMultiReg( INT8U addr, INT8U *buff, INT8U size )
{
    INT8U i, j;
    CC_CSN_LOW( );
    SPI_ExchangeByte( addr | READ_BURST);
    for( i = 0; i < size; i ++ )
    {
        for( j = 0; j < 20; j ++ );
        *( buff + i ) = SPI_ExchangeByte( 0xFF );
    }
    CC_CSN_HIGH( );
}
/*
================================================================================
Function : CC1101ReadStatus( )
    Read a status register
INPUT    : addr, The address of the register
OUTPUT   : the value read from the status register
================================================================================
*/
INT8U CC1101ReadStatus( INT8U addr )
{
    INT8U i;
    CC_CSN_LOW( );
    SPI_ExchangeByte( addr | READ_BURST);
    i = SPI_ExchangeByte( 0xFF );
    CC_CSN_HIGH( );
    return i;
}
/*
================================================================================
Function : CC1101WriteReg( )
    Write a byte to the specified register
INPUT    : addr, The address of the register
           value, the byte you want to write
OUTPUT   : None
================================================================================
*/
void CC1101WriteReg( INT8U addr, INT8U value )
{
    CC_CSN_LOW( );
    SPI_ExchangeByte( addr );
    SPI_ExchangeByte( value );
    CC_CSN_HIGH( );
}
/*
================================================================================
Function : CC1101WriteMultiReg( )
    Write some bytes to the specified register
INPUT    : addr, The address of the register
           buff, a buffer stores the values
           size, How many byte should be written
OUTPUT   : None
================================================================================
*/
void CC1101WriteMultiReg( INT8U addr, INT8U *buff, INT8U size )
{
    INT8U i;
    CC_CSN_LOW( );
    SPI_ExchangeByte( addr | WRITE_BURST );
    for( i = 0; i < size; i ++ )
    {
        SPI_ExchangeByte( *( buff + i ) );
    }
    CC_CSN_HIGH( );
}
/*
================================================================================
Function : CC1101WriteCmd( )
    Write a command byte to the device
INPUT    : command, the byte you want to write
OUTPUT   : None
================================================================================
*/
void CC1101WriteCmd( INT8U command )
{
    CC_CSN_LOW( );
    SPI_ExchangeByte( command );
    CC_CSN_HIGH( );
}
/*
================================================================================
Function : CC1101Reset( )
    Reset the CC1101 device
INPUT    : None
OUTPUT   : None
================================================================================
*/
void CC1101Reset( void )
{
    INT8U x;

    CC_CSN_HIGH( );
    CC_CSN_LOW( );
    CC_CSN_HIGH( );
    for( x = 0; x < 100; x ++ );
    CC1101WriteCmd( CC1101_SRES ); //Reset chip. 
}
/*
================================================================================
Function : CC1101SetTRMode( )
    Set the device as TX mode or RX mode
INPUT    : mode selection
OUTPUT   : None
================================================================================
*/
void CC1101SetTRMode( TRMODE mode )
{
    if( mode == TX_MODE )
    {
        CC1101WriteCmd( CC1101_STX );//In IDLE state: Enable TX
    }
    else if( mode == RX_MODE )
    {
        CC1101WriteCmd( CC1101_SRX );//Enable RX
    }
}
/*
================================================================================
Function : CC1101SetIdle( )
    Set the CC1101 into IDLE mode
INPUT    : None
OUTPUT   : None
================================================================================
*/
void CC1101SetIdle( void )
{
    //Exit RX / TX, turn off frequency synthesizer and exit Wake-On-Radio mode if applicable. 
    CC1101WriteCmd(CC1101_SIDLE);
}
/*
================================================================================
Function : CC1101SetSWOR( )
    Set the CC1101 into SWOR mode
INPUT    : None
OUTPUT   : None
================================================================================
*/
void CC1101SetSWOR( void )
{
    //Start automatic RX polling sequence (Wake-on-Radio)
    CC1101WriteCmd(CC1101_SWOR);
}
/*
================================================================================
Function : CC1101SetSPWD( )
    Set the CC1101 into SPWD mode
INPUT    : None
OUTPUT   : None
================================================================================
*/
void CC1101SetSPWD( void )
{
    //Enter power down mode when CSn goes high. 
    CC1101WriteCmd(CC1101_SPWD);
}

/*
================================================================================
Function : CC1101ClrTXBuff( )
    Flush the TX buffer of CC1101
INPUT    : None
OUTPUT   : None
================================================================================
*/
void CC1101ClrTXBuff( void )
{
    CC1101SetIdle();//MUST BE IDLE MODE
    CC1101WriteCmd( CC1101_SFTX );
}
/*
================================================================================
Function : CC1101ClrRXBuff( )
    Flush the RX buffer of CC1101
INPUT    : None
OUTPUT   : None
================================================================================
*/
void CC1101ClrRXBuff( void )
{
    CC1101SetIdle();//MUST BE IDLE MODE
    CC1101WriteCmd( CC1101_SFRX );
}
/*
================================================================================
Function : CC1101GetRXCnt( )
    Get received count of CC1101
INPUT    : None
OUTPUT   : How many bytes hae been received
================================================================================
*/
INT8U CC1101GetRXCnt( void )
{
    return ( CC1101ReadStatus( CC1101_RXBYTES )  & BYTES_IN_RXFIFO );
}
/*
================================================================================
Function : CC1101SetAddress( )
    Set the address and address mode of the CC1101
INPUT    : address, The address byte
           AddressMode, the address check mode
OUTPUT   : None
================================================================================
*/
void CC1101SetAddress( INT8U address, ADDR_MODE AddressMode)
{
    INT8U btmp = CC1101ReadReg( CC1101_PKTCTRL1 ) & ~0x03;
    CC1101WriteReg(CC1101_ADDR, address);
    if     ( AddressMode == BROAD_ALL )     {}
    else if( AddressMode == BROAD_NO  )     { btmp |= 0x01; }
    else if( AddressMode == BROAD_0   )     { btmp |= 0x02; }
    else if( AddressMode == BROAD_0AND255 ) { btmp |= 0x03; }   
    CC1101WriteReg( CC1101_PKTCTRL1,btmp );
}
/*
================================================================================
Function : CC1101SetSYNC( )
    Set the SYNC bytes of the CC1101
INPUT    : sync, 16bit sync 
OUTPUT   : None
================================================================================
*/
void CC1101SetSYNC( INT16U sync )
{
    CC1101WriteReg(CC1101_SYNC1, 0xFF & ( sync>>8 ) );
    CC1101WriteReg(CC1101_SYNC0, 0xFF & sync ); 
}
/*
================================================================================
Function : CC1101Init( )
    Initialize the CC1101, User can modify it
INPUT    : None
OUTPUT   : None
================================================================================
*/
void CC1101Init( void )
{
    volatile INT8U i, j;
     
    CC1101Reset( );    
    
    for( i = 0; i < 35; i++ )
    {
        CC1101WriteReg(CC1101InitData[i][0], CC1101InitData[i][1]);
    }
    CC1101SetSYNC( 0x8564 );
       
    CC1101WriteMultiReg(CC1101_PATABLE, PaTabel, 8 );
    
    i = CC1101ReadStatus( CC1101_VERSION );//for test, refer to the datasheet
}

void IOCFG2_RX_Configuration(void)
{
    CC1101WriteReg(CC1101_IOCFG2, 0x00);
}

void IOCFG2_TX_Configuration(void)
{
    CC1101WriteReg(CC1101_IOCFG2, 0x02);
}
/*
================================================================================
------------------------------------THE END-------------------------------------
================================================================================
*/
