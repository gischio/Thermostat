//! ADCmgr.c
//!

#include "msp430f6724.h"
#include <string.h>
#include "SHT21.h"
#include "Application.h"

unsigned char TxData[TXLENGTH];
unsigned char RxData[RXLENGTH], txCount, transmitLength, rxCount;
unsigned char *rxBufferPointer;
etSHTStatus SHTState;
unsigned char SGPState;
etI2CStatus I2CState;
unsigned long nackCnt;

unsigned int TVOC;
extern long  rCO2Value;
extern long  rRhValue;

#ifdef SHT21_SEND_RAW_VALUE
unsigned int rRhRawValue;
#endif
#ifdef SGP30_SEND_RAW_VALUE            
unsigned int rCO2RawValue;
#endif

#ifdef SGP30_BASELINE
unsigned int counter1hSGP30;
#endif

long rShtTemp;



//-----------------------------------------------------------------------------
// SHT2x_CheckCrc
//----------------------------------------------------------------------------- 
//  CRC
const unsigned int SHT_POLYNOMIAL = 0x31;  //P(x)=x^8+x^5+x^4+1 = 100110001

unsigned char sensirion_common_generate_crc(unsigned char *data, 
                                            unsigned int count,unsigned char crc8_init)
{
  unsigned char crc = crc8_init;	
  unsigned char byteCtr;
  //calculates 8-Bit checksum with given polynomial
  for (byteCtr = 0; byteCtr < count; ++byteCtr)
  { crc ^= (data[byteCtr]);
    for (unsigned char bit = 8; bit > 0; --bit)
    { if (crc & 0x80) crc = (crc << 1) ^ SHT_POLYNOMIAL;
      else crc = (crc << 1);
    }
  }
  
  return crc;

}

unsigned char SHT2x_CheckCrc(unsigned char *data, unsigned char nbrOfBytes, unsigned char checksum)
{
  if ( sensirion_common_generate_crc(data,nbrOfBytes,SHT21_CRC8_INIT) != checksum )
      return CHECKSUM_ERROR;
  
  return 0;
}

unsigned char SGP30_CheckCrc(unsigned char *data, unsigned char nbrOfBytes, unsigned char checksum)
{
  if ( sensirion_common_generate_crc(data,nbrOfBytes,SGP30_CRC8_INIT) != checksum )
      return CHECKSUM_ERROR;
  
  return 0;
}



//-----------------------------------------------------------------------------
// I2C_SendAddr
//----------------------------------------------------------------------------- 
unsigned char I2C_SendAddr( unsigned char addrRW,unsigned char rwFlag )
{
    unsigned long tout;
        
    UCB0CTLW0 |= UCSWRST;    
    UCB0I2CSA = addrRW;    
    UCB0CTLW0 &= ~I2C_W_MODE;
    UCB0CTLW0 &= ~(UCSWRST);
    UCB0CTLW0 |= (rwFlag | UCTXSTT);
    tout = 0;
    if ( rwFlag == I2C_W_MODE )
    {
        while (!(UCB0IFG & UCTXIFG0))       // Poll for transmit interrupt flag.
        {
            if ( tout++ > 10000 )
                break;
        }
    }
    else
    {
        UCB0IE |= UCRXIE0; 
        while (!(UCB0IFG & UCRXIFG0))       // Poll for receive interrupt flag.
        {
            if ( tout++ > 10000 )
                break;
        }
    }    
    if (UCB0IFG & UCNACKIFG) 
        return (ACK_ERROR);
    
    return ( tout < 10000 );
}
//-----------------------------------------------------------------------------
// I2C_Transmit
//----------------------------------------------------------------------------- 
unsigned char I2C_Transmit( unsigned char adr, unsigned char *buf, 
                            unsigned char count, unsigned char flags )
{
    unsigned long tout;
    unsigned long twait;    

    rxCount = 0;
    if ( I2C_SendAddr(adr,I2C_W_MODE) == ACK_ERROR )
        return (ACK_ERROR);
    
    transmitLength = count;
    txCount = 1;
    UCB0TXBUF = *buf++;   
    UCB0IE |= UCTXIE0;    
    
    tout = 0;
    while ((UCB0STAT & UCBBUSY)||(txCount < transmitLength))
    {
        if ( tout++ > 10000 )
            break;
    }        
    if ( I2C_WAIT_BEFORE_STOP )
    {
        twait = 0;
        while (1)
        {
            if ( twait++ > 10000 )
                break;
        }                
    }
      
    return ( tout < 10000 );
}
//-----------------------------------------------------------------------------
// I2C_Reveive
//----------------------------------------------------------------------------- 
unsigned char I2C_Receive( unsigned char adr, unsigned char *buf, unsigned char count )
{
    unsigned long tout;

    ClearRxBuffer();
    txCount = 0;
    rxBufferPointer = buf;
    rxCount = count;    
    if ( I2C_SendAddr(adr,I2C_R_MODE) == ACK_ERROR )
        return (ACK_ERROR);
    UCB0IE |= UCNACKIE;
    tout = 0;
    while (rxCount)
    {
        if ( tout++ > 10000 )
            break;
    }  
    //UCB0IE &= ~UCNACKIE;    
    return ( tout < 10000 );
}

//-----------------------------------------------------------------------------
// I2C_masterReceiveFinish
//-----------------------------------------------------------------------------  
unsigned char I2C_masterReceiveFinish( void )
{
    //unsigned long tout;
    unsigned char data = UCB0RXBUF;

    UCB0IE &= ~UCRXIE0;
    UCB0CTLW0 |= UCTXSTP;

    return (data);
}
//-----------------------------------------------------------------------------
// USCI_B0_TX
//-----------------------------------------------------------------------------  
void USCI_B0_TX( void )
{

    if (txCount < transmitLength)
    {
        UCB0TXBUF = TxData[txCount];
        txCount++;
    } 
    else   
    {
        UCB0CTLW0 |= UCTXSTP;
        UCB0IFG   &= ~(UCTXIE0);
        UCB0IE    &= ~UCTXIE0;
    }
}
//-----------------------------------------------------------------------------
// USCI_B0_RX
//-----------------------------------------------------------------------------  
void USCI_B0_RX( void )
{
    
    if (rxCount)
    {
        if (rxCount == 1)
        {
            *rxBufferPointer++ = I2C_masterReceiveFinish();
        } 
        else   
        {
            *rxBufferPointer++ = UCB0RXBUF;
        }
        rxCount--;
    } 
    else   
    {
        *rxBufferPointer = UCB0RXBUF;
    }
    
}
//-----------------------------------------------------------------------------
// USCI_B0_TXRXISR
//----------------------------------------------------------------------------- 
#pragma vector = USCI_B0_VECTOR
__interrupt void USCI_B0_TXRXISR ( void )
{
    switch (__even_in_range(UCB0IV, 30))
    {
    case USCI_I2C_UCRXIFG0:
      USCI_B0_RX();
      break;
    case USCI_I2C_UCTXIFG0:
      USCI_B0_TX();
      break;    
    case USCI_I2C_UCNACKIFG:
      nackCnt++;
      break;
    default:
      break;
    }
}

//-----------------------------------------------------------------------------
// HWResetSHT21
//----------------------------------------------------------------------------- 
void HWResetSHT21(void)
{
    P2SEL &= ~0x03;
    for (int j=0;j<1000;j++);
    P2OUT |= BIT0; 
    for (int j=0;j<1000;j++);
    P2OUT |= BIT1; 
    for (int j=0;j<1000;j++);
    P2SEL |= 0x03;   
    P2OUT &= ~(BIT1|BIT0); 
}

extern volatile unsigned long  TM_counter1ms;
//-----------------------------------------------------------------------------
// SGP30_poll
//----------------------------------------------------------------------------- 
unsigned char SGP30_poll( unsigned char reset )
{
    static BYTE counterSGP30=0;
    static unsigned long lastMs = 0;
    unsigned char rxOk;
    
    switch ( SGPState )
    {
    case SGP_INIT_AIR_QUALITY:
        counterSGP30 = 0;
        TxData[0]=0x20;
        TxData[1]=0x03;
        transmitLength = 2;     
        I2C_Transmit(SGP30_ADDR,TxData,2,0);
#ifdef SGP30_BASELINE
        SGPState = SGP_SET_BASE_LINE;    
#else        
        SGPState = SGP_GET_FEATURE_SET_VERSION_RQ;    
#endif        
        rCO2Value = -9998;
        break;
        
    case SGP_GET_FEATURE_SET_VERSION_RQ:
        TxData[0]=0x20;
        TxData[1]=0x2f;
        transmitLength = 2;     
        I2C_Transmit(SGP30_ADDR,TxData,2,0);
        SGPState = SGP_GET_FEATURE_SET_VERSION_RX;    
        break;
        
    case SGP_GET_FEATURE_SET_VERSION_RX:
        rxOk = I2C_Receive( SGP30_ADDR, RxData, 3 );
        if ( (SGP30_CheckCrc(RxData, 2, RxData[2]) != 0)||( rxOk !=1 ) )
          nackCnt++;

        SGPState = SGP_MEASURE_AIR_QUALITY_RQ;    
        break;        
        
    case SGP_GET_BASE_LINE_RQ:
        TxData[0]=0x20;
        TxData[1]=0x15;
        transmitLength = 2;     
        I2C_Transmit(SGP30_ADDR,TxData,2,0);
        SGPState = SGP_GET_BASE_LINE_RX;    
        break;        
        
    case SGP_GET_BASE_LINE_RX:
        rxOk = I2C_Receive( SGP30_ADDR, RxData, 6 );
        if ( (SGP30_CheckCrc(RxData, 2, RxData[2]) != 0)||
             (SGP30_CheckCrc(&RxData[3], 2, RxData[5]) != 0)||( rxOk !=1 ) )
        {
            nackCnt++;   
        }
        else
        {
            baselineCO2[0] = RxData[0];
            baselineCO2[1] = RxData[1];
            baselineCO2[2] = RxData[3];
            baselineCO2[3] = RxData[4];
        }
        SGPState = SGP_MEASURE_AIR_QUALITY_RQ; 
        break;        
        
    case SGP_SET_BASE_LINE:
        TxData[0]=0x20;
        TxData[1]=0x1E;
        TxData[2]=baselineCO2[2];
        TxData[3]=baselineCO2[3];
        TxData[4]=sensirion_common_generate_crc(&TxData[2], 2, SGP30_CRC8_INIT);
        TxData[5]=baselineCO2[0];
        TxData[6]=baselineCO2[1];        
        TxData[7]=sensirion_common_generate_crc(&TxData[5], 2, SGP30_CRC8_INIT);
        transmitLength = 8;     
        I2C_Transmit(SGP30_ADDR,TxData,6,0);
        SGPState = SGP_MEASURE_AIR_QUALITY_RQ;    
        break;  

    case SGP_MEASURE_AIR_QUALITY_RQ:
        lastMs = TM_counter1ms;
        //P3OUT^=BIT4;
        TxData[0]=0x20;
        TxData[1]=0x08;
        transmitLength = 2;     
        I2C_Transmit(SGP30_ADDR,TxData,2,0);
        SGPState = SGP_MEASURE_AIR_QUALITY_RX;    
        break;
    
    case SGP_MEASURE_AIR_QUALITY_RX:
        rxOk = I2C_Receive( SGP30_ADDR, RxData, 6 );
        if ( (SGP30_CheckCrc(RxData, 2, RxData[2]) != 0)||( rxOk !=1 ) )
        {
            nackCnt++;
            rCO2Value = -9998;
#ifdef SGP30_SEND_RAW_VALUE            
            rCO2RawValue = 0;
#endif
        }
        else
        {
            rCO2Value = (RxData[0]*256L + RxData[1])*10;
#ifdef SGP30_SEND_RAW_VALUE            
            rCO2RawValue = (RxData[0]*256L + RxData[1]);
#endif
        }
        if ( (SGP30_CheckCrc(&RxData[3], 2, RxData[5]) != 0)||(rxOk !=1) )
        {
            nackCnt++;
            TVOC  = 0;
        }
        else
        {
            TVOC  = RxData[3]*256L + RxData[4];          
        }
        SGPState = SGP_MEASURE_WAIT_NEXT; 
        break;  
        
    case SGP_MEASURE_WAIT_NEXT:
        if (TM_counter1ms - lastMs > 1000)
        {
#ifdef SGP30_BASELINE   
            if ( counter1hSGP30++ > 3600 )
            {
                counter1hSGP30 = 0;
                SGPState = SGP_GET_BASE_LINE_RQ; 
            }
            else
            {
                SGPState = SGP_MEASURE_AIR_QUALITY_RQ; 
            }
#else            
            SGPState = SGP_MEASURE_AIR_QUALITY_RQ; 
#endif            
            lastMs = TM_counter1ms;
            if ( counterSGP30++ > 10) 
            {
                I2CState = I2C_SHT;
                counterSGP30 = 0;
            }
        }
        break;
    }
    
    return 1;
}
//-----------------------------------------------------------------------------
// SHT21_poll
//----------------------------------------------------------------------------- 
unsigned char SHT21_poll( unsigned char reset )
{
    
    if(reset) 
    {
        SHTState = SHT_SOFT_RESET;
        SGPState = SHT_SOFT_RESET;
    }
    
    switch ( SHTState )
    {
    case SHT_SOFT_RESET: 
        rRhValue  = -9998;
        HWResetSHT21();
        transmitLength = 1;     
        TxData[0] = SOFT_RESET;   
        if ( I2C_Transmit(SHT21_ADDR,TxData,1,0) )
            SHTState = SHT_TRIG_RH;
        break;
    case SHT_TRIG_RH: 
    case SHT_READ_RH: 
        //P3OUT^=BIT5;
        transmitLength = 1;     
        TxData[0] = TRIG_RH_MEASUREMENT_POLL;   
        if ( I2C_Transmit(SHT21_ADDR,TxData,1,I2C_WAIT_BEFORE_STOP)  )
            SHTState = SHT_CONV_RH;
        break;
        
    case SHT_CONV_RH: 
#ifdef SHT21_SEND_RAW_VALUE      
        rRhRawValue = SHT21_read();
#else      
        rRhValue = ((( SHT21_read() * 1250UL ) / 65536UL) - 60); 
#endif          
#ifdef SHT21_READ_T
        SHTState = SHT_TRIG_T; 
#else        
        SHTState = SHT_TRIG_RH;
        I2CState = I2C_SGP;
#endif        
        break;
#ifdef SHT21_READ_T       
    case SHT_TRIG_T: 
    case SHT_READ_T: 
        transmitLength = 1;     
        TxData[0] = TRIG_T_MEASUREMENT_POLL;   
        if ( I2C_Transmit(SHT21_ADDR,TxData,1,I2C_WAIT_BEFORE_STOP)  )
            SHTState = SHT_CONV_T;
        break;
        
    case SHT_CONV_T: 
        rShtTemp = (( SHT21_read() * 1757UL ) / 65536UL) - 468; 
        SHTState = SHT_TRIG_RH;
        I2CState = I2C_SGP;
        break;        
#endif        
    default:
        SHTState = SHT_SOFT_RESET;
        break;
    }
    return (0);
}

//-----------------------------------------------------------------------------
// I2C_poll
//----------------------------------------------------------------------------- 
unsigned char I2C_poll( unsigned char reset )
{

    if(reset) 
    {
        SHTState = SHT_SOFT_RESET;
        SGPState = SGP_INIT_AIR_QUALITY;
        I2CState = I2C_SHT;
        rCO2Value = -9998;
        rRhValue  = -9998;
    }
    
    switch ( I2CState )
    {
    case  I2C_SHT:
        SHT21_poll(0);
    break;
    
    case  I2C_SGP:
        SGP30_poll(0);
    break;
    }
    
    return 1;
}

//-----------------------------------------------------------------------------
// SHT21_read
//----------------------------------------------------------------------------- 
unsigned long SHT21_read( void )
{
    unsigned long rh; 
    
    rh = 0UL;
    if ( I2C_Receive(SHT21_ADDR,RxData,3) != ACK_ERROR )
    {
        if ( !SHT2x_CheckCrc(RxData,2, RxData[2]) ) 
        {
            rh = RxData[0]*256UL + (RxData[1]&0xFC);
        }
    }
    return rh; 
} 

