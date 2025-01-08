/**@addtogroup SBS_Code_EK-EQ2-TP
@{
commento.....
*/
//------------------------------------------------------------------

#include "project.h"
#include "eib_stk.h"
#include "app.h"
#include EIB_DEVICE_MODEL_APP_H_FILE    
#include "stdlib.h"
#include "ListValues.h"
#include "ApplicationUtility.h"
#include "ApplicationTimers.h"
#include "CO2Functions.h"

#define CO2_THRESHOLDS_NR       3
#define CO2_LEVELS_NR           4

#define CO_TYPE_1BIT            0
#define CO_TYPE_UBYTE           1
#define CO_TYPE_PERCENTAGE      2

#define CO2_4_SINGLE_BITS       0
#define CO2_UNSIGNED_BYTE       1
#define CO2_PERCENTAGE          2
#define CO2_PERCENTAGE_CONT     3

#define ABOVE_BELOW             3


#define SM_FIRST_RUN            0
#define SM_NORMAL_RUN           1
#define SM_ENTER_LOCK           2
#define SM_LOCK                 3
#define SM_EXIT_LOCK            4

WORD p1x;
WORD p2x;    
BYTE p1y;
BYTE p2y; 
WORD hyst; 
float m;  

BYTE CO2ThrLevel;
BYTE prevCO2ThrLevel;
BYTE CO2ThresholdsStatus;
BYTE prevCO2ThresholdsStatus;
BYTE CO2VentOut;
BYTE prevCO2VentOut;
BYTE lastCO2VentOut;

struct {
    BITS    actual      :4;  
    BITS    previous    :4;  
} statusCO2ContVent;

BYTE thresholdValueOut[CO2_THRESHOLDS_NR];

long CO2Value;

BYTE * const pntCO2Treshold[] ={ 
  &OBJ_VALUE->CO2Treshold1Percentage,
  &OBJ_VALUE->CO2Treshold2Percentage,
  &OBJ_VALUE->CO2Treshold3Percentage
};

BYTE * const pntCO2TresholdLock[] ={ 
  &OBJ_VALUE->CO2Threshold1Lock,
  &OBJ_VALUE->CO2Threshold2Lock,
  &OBJ_VALUE->CO2Threshold3Lock
};

//-----------------------------------------------------------------------------
// CO2ThresholdsValueToSend
//-----------------------------------------------------------------------------
BYTE CO2ThresholdsValueToSend(BYTE value,BYTE cnt)
{  
  switch ( PARAMETER.CO2Thresholds[cnt].CO2FunctThreshold1COType )
  {
  case CO_TYPE_1BIT:
    value = (value != 0);
  break;
  
  case CO_TYPE_UBYTE:
  break;
  
  case CO_TYPE_PERCENTAGE:
    value = ( value * 255L ) /100;
  break;
  }
  
  return value;
}

//-----------------------------------------------------------------------------
// CO2ThresholdsSendCO
//-----------------------------------------------------------------------------
BYTE CO2ThresholdsSendCO(BYTE cnt)
{
  BYTE value=0;
  
  if ( CO2ThresholdsStatus & 1<<cnt )
  { // above
      value = PARAMETER.CO2Thresholds[cnt].CO2FunctThreshold1AboveValueToSend;
  }
  else
  { // below
      value = PARAMETER.CO2Thresholds[cnt].CO2FunctThreshold1BelowValueToSend;
  }
  
  value = CO2ThresholdsValueToSend(value,cnt);
  
  return value;
  
}

//-----------------------------------------------------------------------------
// CO2Thresholds
//-----------------------------------------------------------------------------
void CO2Thresholds(void)
{
    BYTE flagTx;
    

    CO2ThrLevel = 0;
    if ( CO2Value != 0 )
    {
        for ( BYTE cnt=0; cnt < CO2_THRESHOLDS_NR; cnt++ )
        {
            if (CO2Value >= GetIntegerConst(&PARAMETER.CO2Thresholds[cnt].CO2FunctThreshold1Value) )
            {
                CO2ThresholdsStatus |= (0x1<<cnt);
            }
            if (CO2Value <= GetIntegerConst(&PARAMETER.CO2Thresholds[cnt].CO2FunctThreshold1Value) -
                            GetCO2ThresholdHysteresis(PARAMETER.CO2Thresholds[cnt].CO2FunctThreshold1Hyst) )
                CO2ThresholdsStatus &= ~(0x1<<cnt);
            
            if ( CO2ThresholdsStatus & (0x1<<cnt) )
               CO2ThrLevel++;
        }
    }
    else
    {
        CO2ThresholdsStatus = 0;
        CO2ThrLevel = 0;
    }
            
    for ( BYTE cnt=0; cnt < CO2_THRESHOLDS_NR; cnt++ )
    {
        flagTx = DONOT_SEND_CO;      
        if ( PARAMETER.CO2Thresholds[cnt].CO2FunctThreshold1Type == DISABLE )
            continue;
        
        if ( CO2ThresholdLock[cnt] != *pntCO2TresholdLock[cnt] )
        {
            flagTx = SEND_CO_ON_LOCK_CHANGE;
            CO2ThresholdLock[cnt] = *pntCO2TresholdLock[cnt]; 
        }
        
        if ( EZ_GetState(TimerCO2Threshold1RT+cnt) )
        {
            flagTx = SEND_CO_ON_RT;
            EZ_StartTimer( TimerCO2Threshold1RT+cnt, 
                           GetCyclicSendingInterval(PARAMETER.CO2Thresholds[cnt].CO2FunctThreshold1RT), TM_MODE_1S );
        }  
        
        if ( CO2ThresholdLock[cnt] == UNLOCK_FB ) 
        {
            if ( (PARAMETER.CO2Thresholds[cnt].CO2FunctThreshold1Type == ABOVE)||
                 (PARAMETER.CO2Thresholds[cnt].CO2FunctThreshold1Type == ABOVE_BELOW) )
            {
                if (  (CO2ThresholdsStatus & (0x01<<cnt))&&
                     !(prevCO2ThresholdsStatus & (0x01<<cnt)) )
                    flagTx = SEND_CO_ON_COV;
            }
            if ( (PARAMETER.CO2Thresholds[cnt].CO2FunctThreshold1Type == BELOW)||
                 (PARAMETER.CO2Thresholds[cnt].CO2FunctThreshold1Type == ABOVE_BELOW) )
            {
                if ( !(CO2ThresholdsStatus & (0x01<<cnt)) &&
                      (prevCO2ThresholdsStatus & (0x01<<cnt)) )
                    flagTx = SEND_CO_ON_COV;
            }
            
            thresholdValueOut[cnt] = CO2ThresholdsSendCO(cnt);
        }
        else
        {
            switch (PARAMETER.CO2Thresholds[cnt].CO2FunctThreshold1LockBehaviour)
            {
            case LOCK_NONE:
                break;
            case LOCK_SEND_VALUE:
                thresholdValueOut[cnt] = CO2ThresholdsValueToSend(
                       PARAMETER.CO2Thresholds[cnt].CO2FunctThreshold1LockValue,cnt);
                break;
            }
        }       
        if ( (*pntCO2Treshold[cnt] != thresholdValueOut[cnt])||(flagTx != DONOT_SEND_CO) )
        {
                *pntCO2Treshold[cnt] = thresholdValueOut[cnt];
                SetTransmitRequest(CO_CO2Treshold1Percentage+(cnt*2));
        }        
    }

    prevCO2ThresholdsStatus = CO2ThresholdsStatus;
}
//-----------------------------------------------------------------------------
// CO2VentilationOUT
//-----------------------------------------------------------------------------
void CO2VentilationOUT(BYTE ventOut)
{
    BYTE *p;
    
    switch ( PARAMETER.CO2VentilationCOType )
    {
    case CO2_4_SINGLE_BITS:
        p = &OBJ_VALUE->CO2VentilationBit1;
        for (BYTE cnt=0; cnt < CO2_LEVELS_NR; cnt++)
        {
            p[cnt] = ((ventOut&(0x80>>cnt))!=0);
            SetTransmitRequest(CO_CO2VentilationBit1+cnt);
        }      
    break;
    
    case CO2_UNSIGNED_BYTE:
        OBJ_VALUE->CO2VentilationPercentage = ventOut;
        SetTransmitRequest(CO_CO2VentilationPercentage);
    break;

    case CO2_PERCENTAGE:
        OBJ_VALUE->CO2VentilationPercentage = (ventOut * 255L)/100;
        SetTransmitRequest(CO_CO2VentilationPercentage);
    break;
    
    case CO2_PERCENTAGE_CONT:
        OBJ_VALUE->CO2VentilationPercentage = (ventOut * 255L)/100;
        SetTransmitRequest(CO_CO2VentilationPercentage);
    break;    
    }
}

//-----------------------------------------------------------------------------
// GetCO2VentContOutValue
//-----------------------------------------------------------------------------
BYTE GetCO2VentContOutValue(BYTE out)
{
  float val;

  val = ( CO2Value - p1x ) * m + p1y;
  
  if ( (CO2Value < p1x)||((CO2Value <= p1x + hyst)&&(out == 0)) )
  {
    val = 0;
    statusCO2ContVent.actual = CONT_VENT_MIN;
  } 
  else
  {
      if ( CO2Value > p2x ) 
      {
          val = p2y;
          statusCO2ContVent.actual = CONT_VENT_MAX;
      }
      else
      {
          statusCO2ContVent.actual = CONT_VENT_REG;
      }
  }
  
  return (BYTE)val;
}

//-----------------------------------------------------------------------------
// GetCO2VentOutValue
//-----------------------------------------------------------------------------
BYTE GetCO2VentOutValue(BYTE level)
{
  
    BYTE retVal;
   
    if ( PARAMETER.CO2VentilationCOType == CO2_PERCENTAGE_CONT )
        retVal = GetCO2VentContOutValue(CO2VentOut);
    else
        retVal = PARAMETER.CO2VentilationValueToSend[level];
  
    return retVal;
}

//-----------------------------------------------------------------------------
// CO2Ventilation
//-----------------------------------------------------------------------------
void CO2Ventilation(void)
{
    BYTE flagTx = DONOT_SEND_CO;
    
    if ( OBJ_VALUE->CO2VentilationLock == UNLOCK_FB ) 
    {
        CO2VentOut = GetCO2VentOutValue(CO2ThrLevel);
        
        if ( PARAMETER.CO2VentilationCOType == CO2_PERCENTAGE_CONT )
        {
            if ( (abs(lastCO2VentOut - CO2VentOut) > PARAMETER.CO2VentilationCOV)&&
                 (PARAMETER.CO2VentilationCOV != 0) )
            {
                flagTx = SEND_CO_ON_COV;
                lastCO2VentOut = CO2VentOut;
            }
            if ( statusCO2ContVent.actual != statusCO2ContVent.previous )
            {
                flagTx = SEND_CO_ON_COV;
                statusCO2ContVent.previous =  statusCO2ContVent.actual;
            }
        }
        else
        {
            if ( CO2ThrLevel != prevCO2ThrLevel )
                flagTx = SEND_CO_ON_COV;
        }        
    }
    else
    {
        switch (PARAMETER.CO2VentilationLockBehaviour)
        {
        case LOCK_NONE:
            CO2VentOut = prevCO2VentOut;
            break;
        case LOCK_SEND_VALUE:
            CO2VentOut = PARAMETER.CO2VentilationLockValue;  
            break;
        }
    }

    if ( EZ_GetState(TimerCO2VentilationRT) )
    {
        flagTx = SEND_CO_ON_RT;
        EZ_StartTimer( TimerCO2VentilationRT, 
                       GetCyclicSendingInterval(PARAMETER.CO2VentilationOutRT), TM_MODE_1S );
    }

    if ( CO2VentilationLock != OBJ_VALUE->CO2VentilationLock )
    {
        flagTx = SEND_CO_ON_LOCK_CHANGE;
        CO2VentilationLock = OBJ_VALUE->CO2VentilationLock;
    }    
    
    /* Disable from bus */
    if ( CO2VentilationDisableFB != ContactFromBus(OBJ_VALUE->CO2VentilationDisableFB,
                                                   PARAMETER.CO2VentilationDisableInverted) )
    {
        flagTx = SEND_CO_ON_DISABLE_CHANGE;
        CO2VentilationDisableFB = ContactFromBus(OBJ_VALUE->CO2VentilationDisableFB,
                                                 PARAMETER.CO2VentilationDisableInverted);
        if ( CO2VentilationDisableFB == NO && CO2VentilationLock == ENABLE )
            flagTx = SEND_CO_ON_LOCK_CHANGE;
    }
    
    if ( CO2VentilationDisableFB == YES )
    {
        CO2VentOut = 0;
    }
    
    
    
    if ( flagTx != DONOT_SEND_CO )
    {
        if ( CO2VentilationLock == ENABLE && flagTx != SEND_CO_ON_LOCK_CHANGE &&
             CO2VentilationDisableFB == NO )
        {
        }
        else
        {
            CO2VentilationOUT(CO2VentOut);
            if ( CO2VentilationDisableFB == NO ) prevCO2VentOut = CO2VentOut;
        }
    }
}
//-----------------------------------------------------------------------------
// CO2FunctionsInit
//-----------------------------------------------------------------------------
void CO2FunctionsInit(void)
{

  if ( PARAMETER.CO2FunctEnable != ENABLE ) return;
  
  if ( PARAMETER.CO2VentilationDisableFB == ENABLE )
      OBJ_VALUE->CO2VentilationDisableFB = ContactFromBus(CO2VentilationDisableFB,
                                                          PARAMETER.CO2VentilationDisableInverted);
  else      
      CO2VentilationDisableFB = NO;
  
  if ( PARAMETER.CO2VentilationEnableFunct == ENABLE )
  {
      prevCO2ThrLevel = 0xFF;
      //prevCO2VentOut = CO2VentilationValueAtBusOff;
      
      if ( PARAMETER.CO2VentilationEnableLock == ENABLED )
      {
          switch (PARAMETER.CO2VentilationBehaviourAfterBusOn)
          {
          case PREV_AT_BUS_ON:
              OBJ_VALUE->CO2VentilationLock = CO2VentilationLock&1;
              CO2VentilationLock = 0;
              break;
          case LOCK_AT_BUS_ON:
              OBJ_VALUE->CO2VentilationLock = LOCK_FB;
              //CO2VentilationLock = LOCK_FB;
              break;        
          case UNLOCK_AT_BUS_ON:
              OBJ_VALUE->CO2VentilationLock = UNLOCK_FB;
              //CO2VentilationLock = UNLOCK_FB;
              break;        
          }
      }
      else
      {
          OBJ_VALUE->CO2VentilationLock = UNLOCK_FB;
          //CO2VentilationLock = UNLOCK_FB;
      }
      
      if ( PARAMETER.CO2VentilationOutRT )
      {        
          EZ_StartTimer( TimerCO2VentilationRT, 
                         GetCyclicSendingInterval(PARAMETER.CO2VentilationOutRT), TM_MODE_1S );
      }
  }
  
  for ( BYTE cnt=0; cnt < CO2_THRESHOLDS_NR; cnt++ )
  {
      if ( PARAMETER.CO2Thresholds[cnt].CO2FunctThreshold1Type == DISABLE )
          continue;    
      
      if ( PARAMETER.CO2Thresholds[cnt].CO2FunctThreshold1RT )
      {        
          EZ_StartTimer( TimerCO2Threshold1RT+cnt, 
                         GetCyclicSendingInterval(PARAMETER.CO2Thresholds[cnt].CO2FunctThreshold1RT), TM_MODE_1S );
      }
      if ( PARAMETER.CO2Thresholds[cnt].CO2FunctThreshold1EnableLock == ENABLED )
      {
          switch (PARAMETER.CO2Thresholds[cnt].CO2FunctThreshold1LockBehaviourAfterBusOn)
          {
          case PREV_AT_BUS_ON:
              break;
          case LOCK_AT_BUS_ON:
              CO2ThresholdLock[cnt] = LOCK_FB;        
              break;        
          case UNLOCK_AT_BUS_ON:
              CO2ThresholdLock[cnt] = UNLOCK_FB;
              break;        
          }
      }
      else
      {
          CO2ThresholdLock[cnt] = UNLOCK_FB;
      }
      *pntCO2TresholdLock[cnt] = CO2ThresholdLock[cnt];       
  }
  
  //p1x  = ShortF2LongI(PARAMETER.CO2VentilationP1X)/100;
  //p2x  = ShortF2LongI(PARAMETER.CO2VentilationP2X)/100;    
  p1x  = GetIntegerConst(&PARAMETER.CO2VentilationP1X);
  p2x  = GetIntegerConst(&PARAMETER.CO2VentilationP2X);      
  p1y  = PARAMETER.CO2VentilationP1Y;
  p2y  = PARAMETER.CO2VentilationP2Y; 
  hyst = GetCO2ThresholdHysteresis(PARAMETER.CO2VentilationHyst);  
  m = (float)( p2y - p1y ) / ( p2x - p1x );   
  
}

//-----------------------------------------------------------------------------
// CO2Functions
//-----------------------------------------------------------------------------
BYTE CO2Functions(void)
{
  
#ifdef CO2_VALUE_FROM_BUS  
  CO2Value = ShortF2LongI(OBJ_VALUE->CO2Value)/100;
#else
  CO2Value = (fCO2Value/100);
#endif  
  
  if ( PARAMETER.CO2FunctEnable != ENABLE ) return 0;
  
  CO2Thresholds();
  
  if ( PARAMETER.CO2VentilationEnableFunct == ENABLE )
      CO2Ventilation();
  
  prevCO2ThrLevel = CO2ThrLevel;
  CO2VentilationValueAtBusOff = prevCO2VentOut;
  
  return 0;
}                                                   

