/**@addtogroup SBS_Code_EK-EQ2-TP
@{
commento.....
*/
//------------------------------------------------------------------

#include "project.h"
#include "eib_stk.h"
#include "app.h"
#include EIB_DEVICE_MODEL_APP_H_FILE      
#include <math.h>
#include <stdlib.h>
#include "ApplicationUtility.h"
#include "ApplicationTimers.h"
#include "ListValues.h"
#include "TemperatureSensor.h"

long    _V1(prevTempValue,MAX_THERM);

//-------------------------------------------------------------------- 
// TemperatureSensorInit
//-------------------------------------------------------------------- 
void TemperatureSensorInit(void)
{
  WORD param;
  
  for ( BYTE t = 0; t < MAX_THERM; t++ )  
  {
      if ( param = GetCyclicSendingInterval(PARAMETER.TempMeasureRT) )
      {
          EZ_StartTimer( TimerTemperatureSensorRT+t, param, TM_MODE_1S );
      }  
    
      if (( param = GetCyclicSendingInterval(PARAMETER.TempThresholdstRT) )&&
          (( PARAMETER.TempThreshold1 != NOT_ACTIVE )||(PARAMETER.TempThreshold2 != NOT_ACTIVE))) 
      {
          EZ_StartTimer( TimerTemperatureThresholdRT+t, param, TM_MODE_1S );
      }    
      if ( (PARAMETER.TempThreshold1 != NOT_ACTIVE)&&(PARAMETER.TempThreshold1EnableLock == TRUE) )
      {
          switch (PARAMETER.TempThreshold1LockBehaviourAfterBusOn)
          {
          case PREV_AT_BUS_ON:
              break;
          case LOCK_AT_BUS_ON:
              tempThreshold1Lock = LOCK_FB;        
              break;        
          case UNLOCK_AT_BUS_ON:
              tempThreshold1Lock = UNLOCK_FB;
              break;        
          }
      }
      if ( (PARAMETER.TempThreshold2 != NOT_ACTIVE)&&(PARAMETER.TempThreshold2EnableLock == TRUE) )
      {
          switch (PARAMETER.TempThreshold2LockBehaviourAfterBusOn)
          {
          case PREV_AT_BUS_ON:
              break;
          case LOCK_AT_BUS_ON:
              tempThreshold2Lock = LOCK_FB;        
              break;        
          case UNLOCK_AT_BUS_ON:
              tempThreshold2Lock = UNLOCK_FB;
              break;        
          }
      }      
  }
}

//-------------------------------------------------------------------- 
// TemperatureSensor
//-------------------------------------------------------------------- 
void TemperatureSensor(BYTE nTherm )
{
  BYTE txflag = 0;
  BYTE threshold1 = _OBJV1(TempThreshold1Switch,nTherm);
  BYTE threshold2 = _OBJV1(TempThreshold2Switch,nTherm);
  long param;
  BYTE updateThresholds1 = NO;
  BYTE updateThresholds2 = NO;  
  
#ifndef TEMP_VALUE_FROM_BUS  
  _OBJV1(TempValue,nTherm) = LongI2ShortF(fTemperature[nTherm]);
#endif  
  /* COV */
  param = ShortF2LongI(PARAMETER.TempMeasureCOV);
  if ( param != 0 )
  {
      if ( labs( _V1(prevTempValue,nTherm) - fTemperature[nTherm] ) > param )
      {
          _V1(prevTempValue,nTherm) = fTemperature[nTherm];
          txflag = 1;
      }
  }
  /* RT */
  if (EZ_GetState(TimerTemperatureSensorRT+nTherm)) 
  {
    txflag = 1;
    EZ_StartTimer( TimerTemperatureSensorRT+nTherm, 
                   GetCyclicSendingInterval(PARAMETER.TempMeasureRT), TM_MODE_1S );
  }
  
  if ( txflag )  SetTransmitRequest( CO_INX_THERM(nTherm,CO_TempValue_1) );
  
  /* Threshold 1 */
  if ( CheckUpdateFlag(CO_TempThreshold1Value) )
  {
      param = ShortF2LongI(OBJ_VALUE->TempThreshold1Value);
      if ( param <= 5000 && param >= 0 )
      {
          tempThreshold1 = param;
      }
  }      
  param = tempThreshold1;   
  
  if ( (PARAMETER.TempThreshold1EnableLock == TRUE)&&(CheckUpdateFlag(CO_TempThreshold1Lock)) )
  {
      tempThreshold1Lock = _OBJV1(TempThreshold1Lock,nTherm);
      updateThresholds1 = YES;
  }

  if ( tempThreshold1Lock == FALSE )
  {
      switch ( PARAMETER.TempThreshold1 )
      {
      case NOT_ACTIVE:
          break;
      case BELOW:
          if ( fTemperature[nTherm] < param )
          {
              _OBJV1(TempThreshold1Switch,nTherm) = ON;
          }   
          if ( fTemperature[nTherm] > param + (PARAMETER.TempThresholdsHyst * 10) )
          {
              _OBJV1(TempThreshold1Switch,nTherm) = OFF;
          }
          break;
      case ABOVE:
          if ( fTemperature[nTherm] > param )
          {
              _OBJV1(TempThreshold1Switch,nTherm) = ON;
          }   
          if ( fTemperature[nTherm] < param - (GetHysteresis(PARAMETER.TempThresholdsHyst) * 10) )
          {
              _OBJV1(TempThreshold1Switch,nTherm) = OFF;
          }
          break;
      default:
          break;
      }      
  }
  else
  {
      switch (PARAMETER.TempThreshold1LockBehaviour)
      {
      case LOCK_NONE:
          break;
      case LOCK_SEND_OFF:
          _OBJV1(TempThreshold1Switch,nTherm) = OFF;
          break;
      case LOCK_SEND_ON:
          _OBJV1(TempThreshold1Switch,nTherm) = ON;        
          break;
      }
  }
  
  if ( (threshold1 != _OBJV1(TempThreshold1Switch,nTherm))||(updateThresholds1 == YES) ) 
      SetTransmitRequest( CO_INX_THERM(nTherm,CO_TempThreshold1Switch_1) );
  
  /* Threshold 2 */
  if ( CheckUpdateFlag(CO_TempThreshold2Value) )
  {
      param = ShortF2LongI(OBJ_VALUE->TempThreshold2Value);
      if ( param <= 5000 && param >= 0 )
      {
          tempThreshold2 = param;
      }
      else
      {
      }
  }      
  param = tempThreshold2; 
  
  if ( (PARAMETER.TempThreshold2EnableLock == TRUE)&&(CheckUpdateFlag(CO_TempThreshold2Lock)) )
  {
      tempThreshold2Lock = _OBJV1(TempThreshold2Lock,nTherm);
      updateThresholds2 = YES;
  }  
  
  if ( tempThreshold2Lock == FALSE )
  {
      switch ( PARAMETER.TempThreshold2 )
      {
      case NOT_ACTIVE:
          break;
      case BELOW:
          if ( fTemperature[nTherm] < param )
          {
              _OBJV1(TempThreshold2Switch,nTherm) = ON;
          }   
          if ( fTemperature[nTherm] > param + (GetHysteresis(PARAMETER.TempThresholdsHyst) * 10) )
          {
              _OBJV1(TempThreshold2Switch,nTherm) = OFF;
          }
          break;
      case ABOVE:
          if ( fTemperature[nTherm] > param )
          {
              _OBJV1(TempThreshold2Switch,nTherm) = ON;
          }   
          if ( fTemperature[nTherm] < param - (GetHysteresis(PARAMETER.TempThresholdsHyst) * 10) )
          {
              _OBJV1(TempThreshold2Switch,nTherm) = OFF;
          }
          break;
      default:
          break;
      }    
  }
  else
  {
      switch (PARAMETER.TempThreshold2LockBehaviour)
      {
      case LOCK_NONE:
          break;
      case LOCK_SEND_OFF:
          _OBJV1(TempThreshold2Switch,nTherm) = OFF;
          break;
      case LOCK_SEND_ON:
          _OBJV1(TempThreshold2Switch,nTherm) = ON;        
          break;
      }    
  }
  
  if ( (threshold2 != _OBJV1(TempThreshold2Switch,nTherm))||(updateThresholds2 == YES) ) 
      SetTransmitRequest( CO_INX_THERM(nTherm,CO_TempThreshold2Switch_1) );
  
  /* Threshold RT */
   if (EZ_GetState(TimerTemperatureThresholdRT+nTherm)) 
   {
      SetTransmitRequest( CO_INX_THERM(nTherm,CO_TempThreshold1Switch_1) );
      SetTransmitRequest( CO_INX_THERM(nTherm,CO_TempThreshold2Switch_1) );
      EZ_StartTimer( TimerTemperatureThresholdRT+nTherm, 
                     GetCyclicSendingInterval(PARAMETER.TempThresholdstRT), TM_MODE_1S );
     
   }
}