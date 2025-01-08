#include <string.h>
#include "project.h"
#include "eib_stk.h"
#include "app.h"
#include "ApplicationUtility.h"
#include "ApplicationTimers.h"
#include "ListValues.h"
#include EIB_DEVICE_MODEL_APP_H_FILE
#include <math.h>
#include <stdlib.h>

//------------------------------------------------------------------
// CalcCRC16
//------------------------------------------------------------------
WORD CalcCRC16(BYTE *src,WORD nbyte)
{
  WORD app=0;
  
  for (WORD cnt=0; cnt < nbyte; cnt++)
  {
    app+=*src;
    src++;
  }
  return app; 
}
//------------------------------------------------------------------
// PageErased
//------------------------------------------------------------------
BYTE PageErased(BYTE *p)
{
  
  for ( WORD cnt=0; cnt < 512; cnt++ )
  {
    if ( *p != 0xFF ) return 1;
    p++;
  }
  return 0;
}

//------------------------------------------------------------------
// ErasePage
//------------------------------------------------------------------
BYTE ErasePage( BYTE *p)
{
    unsigned char   filler=0xFF;
    
    if ( PageErased(p) )
    {
        UT_CheckNvMem (p, (unsigned short)512);
        UT_FillNvMem(filler,p, 512);
        return 1;
    }
    
    return 0;
}

//------------------------------------------------------------------
// ErasePage
//------------------------------------------------------------------
extern const DM_SEEPROM cpyDM_eeprom;
struct APP_Flash * GetActivePage(void)
{
  
  if ( (EEPROM_VAR.page1.marker == 0xFF55) &&
       (CalcCRC16((BYTE*)&EEPROM_VAR.page1.SAVE_START,sizeof(struct APP_Flash)-4) == EEPROM_VAR.page1.param_crc16) )
    return (struct APP_Flash*)&EEPROM_VAR.page1;
  else if ( (EEPROM_VAR.page2.marker == 0xFF55) &&
            (CalcCRC16((BYTE*)&EEPROM_VAR.page2.SAVE_START,sizeof(struct APP_Flash)-4) == EEPROM_VAR.page2.param_crc16) )
    return (struct APP_Flash*)&EEPROM_VAR.page2;
  else
    return (struct APP_Flash*)&cpyDM_eeprom.eeprom.page1; 
  
}
//------------------------------------------------------------------
// SaveUserData
//------------------------------------------------------------------
struct APP_Flash* SaveUserData(struct APP_Flash *shadow_eeprom)
{
  
  if ( (actualEep != &EEPROM_VAR.page1)&&(actualEep != &EEPROM_VAR.page2) )
    return actualEep; // non deve succedere!!!!

  shadow_eeprom->marker = 0xFF55;
  shadow_eeprom->param_crc16 = CalcCRC16(&shadow_eeprom->SAVE_START,sizeof(struct APP_Flash)-4);
  
  if ( !UT_CompareMem((unsigned char *)shadow_eeprom,actualEep, sizeof(struct APP_Flash)) )
  {  
      UT_CopyMem ((unsigned char *)shadow_eeprom, emptyEep, sizeof(struct APP_Flash) );
      
      if ( actualEep == &EEPROM_VAR.page1)
      {
          UT_WriteByte ((unsigned char *) &EEPROM_VAR.page1.marker, 0x44);
          return (struct APP_Flash*)&EEPROM_VAR.page2;
      }
      else 
      {
          UT_WriteByte ((unsigned char *) &EEPROM_VAR.page2.marker, 0x44);
          return (struct APP_Flash*)&EEPROM_VAR.page1;
      }  
  }
  else
  {
      return actualEep;
  }      
}

//-------------------------------------------------------------------- 
// ShortF2LongI
//-------------------------------------------------------------------- 
long ShortF2LongI( short inval) {
  short exp;
  long val;
  
  
  exp = (inval & 0x0078) >> 3; 
  val = (inval & 0x0000FF00)>>8 | ((inval & 0x00000087)<<8); 
  if ( val & 0x8000) val |= 0xFFFFF800;
  val = val << exp; 
  
  return (val);
}
//-------------------------------------------------------------------- 
// ShortF2LongI
//-------------------------------------------------------------------- 
short LongI2ShortF( long inval) {
    WORD        exp, sign;
    DWORD       val;
    
    if ( inval < 0 )
    {
        val = (DWORD)(-inval);
        sign = 1;
    }
    else
    {
        val = inval;
        sign = 0;
    }
    
    exp = 0;
    while ( val >= 2048 )
    {
        exp++;
        val /= 2;
    }
    if ( sign )
        val = 0-val;
    inval = (sign << 7) + (exp << 3) + ((val>>8) & 7) + ((val<<8)&0xFF00);
  
 
  return (inval);
  
}
//-------------------------------------------------------------------- 
// calcoli psicometrici ....
//-------------------------------------------------------------------- 
//-------------------------------------------------------------------- 
// DewpointTemperature
//-------------------------------------------------------------------- 
float DewpointTemperature(float h,float t) 
{ 
  float k,dew_point ; 
  
  k = (log10(h)-2)/0.4343 + (17.62*t)/(243.12+t); 
  dew_point = 243.12*k/(17.62-k); 
  
  return dew_point; 
} 
//-------------------------------------------------------------------- 
// PerceivedTemperature
//-------------------------------------------------------------------- 
float PerceivedTemperature(float h,float t)
{
  float index;
  
  index =  t + 0.5555 * ( (h/100) * 6.112 * pow(10,((7.5*t)/(237.3+t)))-10);
  
  return index;
}
//-------------------------------------------------------------------- 
// Psat
//-------------------------------------------------------------------- 
float Psat(float t)
{
  float ps;
  
  ps = 610.5*exp((17.269*t)/(237.3+t));
  
  return ps;
}
//-------------------------------------------------------------------- 
// AbsoluteHumidity g/kg
//-------------------------------------------------------------------- 
float AbsoluteHumidity(float h,float t)
{
  static float ah;
  float ps;
  
  ps = Psat(t);
  ah = 622*((h/100)*ps/(101325 - (h/100)*ps));
  
  return ah;
}
//-------------------------------------------------------------------- 
// Enthalpy  kj/kg
//-------------------------------------------------------------------- 
float Enthalpy(float ua,float t)
{
  float h;
  
  h = 1.005*t+ua*(2500+1.87*t)/1000;
  
  return h;
}


//------------------------------------------------------------------  
// ContactFromBus
//------------------------------------------------------------------  
BYTE ContactFromBus(BYTE value, BYTE inverted )
{
  return ( inverted ? !value : value );
}
#define CALC_DEFAULT_CRC16
//-------------------------------------------------------------------
// Initialize the EEPROM 
//-------------------------------------------------------------------
extern const DM_SEEPROM cpyDM_eeprom;
void ResetEEPROM( void )
{
#ifdef CALC_DEFAULT_CRC16  
#warning ========================================================
#warning CALCOLO CRC16 VALORI DI DEFAULT
#warning ========================================================  
    static WORD crc16;
    
    crc16 = CalcCRC16((BYTE*)&cpyDM_eeprom.eeprom.page1.SAVE_START,sizeof(struct APP_Flash)-4);
#endif    
    UT_CopyMem( (unsigned char *) &cpyDM_eeprom, (unsigned char *) &DM_eeprom, sizeof(DM_eeprom) );
    WDG_STOP();
}                                               //  ResetEEPROM


//BYTE Ext_Input_Type(BYTE a)
//{
  ////return (((PARAMETER.u__0010 << 4*a)&0xF0)>>4);
  ////return ( PARAMETER.ExtInput[a].InputType );
//  return 0;
//}
//-------------------------------------------------------------------
// Initialize the EEPROM 
//-------------------------------------------------------------------
unsigned long ReverseFloat(unsigned long *val)
{
 
  static unsigned long val1 = 0;
  
  val1 =  (*val & 0x000000FFUL) << 24;
  val1 |= (*val & 0x0000FF00UL) << 8;
  val1 |= (*val & 0x00FF0000UL) >> 8;
  val1 |= (*val & 0xFF000000UL) >> 24;
  
  return (val1);

}

//-------------------------------------------------------------------
// Initialize the EEPROM 
//-------------------------------------------------------------------
void ReverseBits(BYTE *b)
{
  BYTE appB = *b;
  BYTE mask1;
  BYTE mask2;  
  
  *b=0;
  mask1=0x08;
  mask2=0x10;  
  for ( BYTE cnt=1; cnt<8; cnt+=2, mask1>>=1, mask2<<=1)
  {
    *b |= ((appB&mask1)<<cnt)|((appB&mask2)>>cnt);   
  }
}

//------------------------------------------------------------------
// mDelay
//------------------------------------------------------------------
void mDelay( DWORD _delay )
{
    _delay *= 1450;
    while( _delay-- > 0 );
}
//------------------------------------------------------------------
// uDelay
//------------------------------------------------------------------
void uDelay( WORD _delay )
{
    while( _delay-- > 1 )
    {
      asm("nop");
      asm("nop");
      asm("nop");      
      asm("nop");      
      asm("nop");
      asm("nop");
      asm("nop");      
      asm("nop");
      asm("nop");      
      asm("nop");            
    }
}

//------------------------------------------------------------------
// Memory Functions
//------------------------------------------------------------------
void copyOutput( unsigned char * sourceOut, unsigned char * destOut, int length )
{
    if ( memcmp(sourceOut,destOut,length) == 0 )
        return;

    MSP_source = sourceOut;
    MSP_dest = destOut;
    MSP_count = length;
    MSP_WriteBlock(0);
}
//------------------------------------------------------------------                                   