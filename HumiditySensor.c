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
#include "Application.h"
#include "ApplicationUtility.h"
#include "ApplicationTimers.h"
#include "ListValues.h"
#include "SensorFromBus.h"
#include "HumiditySensor.h"

#define INT_HUMI_PRESENT

long    _V1(prevHumValue,MAX_THERM);
long    _V1(humidity,MAX_THERM);

//long    intHumidity;

//-------------------------------------------------------------------- 
// HumiditySensorInit
//-------------------------------------------------------------------- 
void HumiditySensorInit(void)
{
#ifdef INT_HUMI_PRESENT    
  WORD param;
#endif  
  
  for ( BYTE nTherm = 0; nTherm < MAX_THERM; nTherm++ )
  {
      _V1(humidity,nTherm) = SENSOR_ERROR;
      
      //if (!( _PARTHERM(nTherm).HumidityFromBus == ENABLED )) continue;
    
#ifdef INT_HUMI_PRESENT    
      if ( PARAMETER.RhEnableSensor == ENABLED )
      {
          if ( param = GetCyclicSendingInterval(PARAMETER.RhMeasureRT) )
          {
              EZ_StartTimer( TimerRhSensorRT+nTherm, param, TM_MODE_1S );
          }  
        
          if (( param = GetCyclicSendingInterval(PARAMETER.RhThresholdsRT) )&&
              (( PARAMETER.RhThreshold1 != NOT_ACTIVE )||(PARAMETER.RhThreshold2 != NOT_ACTIVE))) 
          {
              EZ_StartTimer( TimerRhThresholdsRT+nTherm, param, TM_MODE_1S );
          }    
          if ( (PARAMETER.RhThreshold1 != NOT_ACTIVE)&&(PARAMETER.RhThreshold1EnableLock == TRUE) )
          {
              switch (PARAMETER.RhThreshold1LockBehaviourAfterBusOn)
              {
              case PREV_AT_BUS_ON:
                  break;
              case LOCK_AT_BUS_ON:
                  rhThreshold1Lock = LOCK_FB;        
                  break;        
              case UNLOCK_AT_BUS_ON:
                  rhThreshold1Lock = UNLOCK_FB;
                  break;        
              }
          }
          if ( (PARAMETER.RhThreshold2 != NOT_ACTIVE)&&(PARAMETER.RhThreshold2EnableLock == TRUE) )
          {
              switch (PARAMETER.RhThreshold2LockBehaviourAfterBusOn)
              {
              case PREV_AT_BUS_ON:
                  break;
              case LOCK_AT_BUS_ON:
                  rhThreshold2Lock = LOCK_FB;        
                  break;        
              case UNLOCK_AT_BUS_ON:
                  rhThreshold2Lock = UNLOCK_FB;
                  break;        
              }
          }          
      }
#endif // INT_HUMI_PRESENT    
  }
}

//-------------------------------------------------------------------- 
// HumiditySensor
//-------------------------------------------------------------------- 
void HumiditySensor(BYTE nTherm)
{
  BYTE txflag = 0;
  long param;
  BYTE threshold1 = OBJ_VALUE->RhThreshold1Switch;
  BYTE threshold2 = OBJ_VALUE->RhThreshold2Switch;  
  BYTE updateThresholds1 = NO;
  BYTE updateThresholds2 = NO;  
 

  if ( PARAMETER.RhEnableSensor == ENABLED )
  { 
    
      _V1(humidity,nTherm) = fRhValue;
      if ( _V1(humidity,nTherm) != SENSOR_ERROR )
      {
          _V1(humidity,nTherm) += STARATURA_RH;
          if ( _V1(humidity,nTherm) > 10000) _V1(humidity,nTherm) = 10000;
          OBJ_VALUE->RhValue_1byte = (255*_V1(humidity,nTherm)/100)/100;
      }
      else
      {
          OBJ_VALUE->RhValue_1byte = 0; // errore sonda !
      }
      
#ifdef SHT21_SEND_RAW_VALUE      
      extern unsigned int rRhRawValue;
      OBJ_VALUE->RhValue_2bytes = rRhRawValue;
#else      
#ifdef RH_VALUE_FROM_BUS
      _V1(humidity,nTherm) = ShortF2LongI(OBJ_VALUE->RhValue_2bytes);
#endif     
      OBJ_VALUE->RhValue_2bytes = LongI2ShortF(_V1(humidity,nTherm));
#endif      
    
      
      /* COV */
      param = PARAMETER.RhMeasureCOV * 100;
      if ( param != 0 )
      {
          if ( labs( prevHumValue - _V1(humidity,nTherm) ) > param )
          {
              prevHumValue = _V1(humidity,nTherm);
              txflag = 1;
          }
      }
      if (EZ_GetState(TimerRhSensorRT)) 
      {
          txflag = 1;
          EZ_StartTimer( TimerRhSensorRT, 
                         GetCyclicSendingInterval(PARAMETER.RhMeasureRT), TM_MODE_1S );
      }
      
      if ( txflag )  
      {
          SetTransmitRequest( CO_RhValue_1byte );
          SetTransmitRequest( CO_RhValue_2bytes );
      }
      
      /* Threshold 1 */
      if ( CheckUpdateFlag(CO_RhThreshold1_1byteValue) )
      {
          param = ((OBJ_VALUE->RhThreshold1_1byteValue*100/255L)*100);
          if ( param <= 10000 && param >= 0 )
          {
              rhThreshold1 = param;
          }
      }            
      if ( CheckUpdateFlag(CO_RhThreshold1_2bytesValue) )
      {
          param = ShortF2LongI(OBJ_VALUE->RhThreshold1_2bytesValue);
          if ( param <= 10000 && param >= 0 )
          {
              rhThreshold1 = param;
          }
      }      
      param = rhThreshold1; 
      
      if ( (PARAMETER.RhThreshold1EnableLock == TRUE)&&(CheckUpdateFlag(CO_RhThreshold1Lock)) )
      {
          rhThreshold1Lock = _OBJV1(RhThreshold1Lock,nTherm);
          updateThresholds1 = YES;
      }

      if ( rhThreshold1Lock == FALSE )
      {      
          switch ( PARAMETER.RhThreshold1 )
          {
          case NOT_ACTIVE:
              break;
          case BELOW:
              if ( _V1(humidity,nTherm) < param )
              {
                  OBJ_VALUE->RhThreshold1Switch = ON;
              }   
              if ( _V1(humidity,nTherm) > param + (GetHumidityHysteresis(PARAMETER.RhThresholdsHyst) * 10) )
              {
                  OBJ_VALUE->RhThreshold1Switch = OFF;
              }
              break;
          case ABOVE:
              if ( _V1(humidity,nTherm) > param )
              {
                  OBJ_VALUE->RhThreshold1Switch = ON;
              }   
              if ( _V1(humidity,nTherm) < param - (GetHumidityHysteresis(PARAMETER.RhThresholdsHyst) * 10) )
              {
                  OBJ_VALUE->RhThreshold1Switch = OFF;
              }
              break;
          default:
              break;
          }      
      }
      else
      {
          switch (PARAMETER.RhThreshold1LockBehaviour)
          {
          case LOCK_NONE:
              break;
          case LOCK_SEND_OFF:
              _OBJV1(RhThreshold1Switch,nTherm) = OFF;
              break;
          case LOCK_SEND_ON:
              _OBJV1(RhThreshold1Switch,nTherm) = ON;        
              break;
          }        
      }
      if ( (threshold1 != OBJ_VALUE->RhThreshold1Switch )||(updateThresholds1 == YES) )  
          SetTransmitRequest( CO_RhThreshold1Switch );
      
      /* Threshold 2 */
      if ( CheckUpdateFlag(CO_RhThreshold2_1byteValue) )
      {
          param = ((OBJ_VALUE->RhThreshold2_1byteValue*100/255L)*100);
          if ( param <= 10000 && param >= 0 )
          {
              rhThreshold2 = param;
          }
      }       
      if ( CheckUpdateFlag(CO_RhThreshold2_2bytesValue) )
      {
          param = ShortF2LongI(OBJ_VALUE->RhThreshold2_2bytesValue);
          if ( param <= 10000 && param >= 0 )
          {
              rhThreshold2 = param;
          }
      }        
      
      param = rhThreshold2;
      
      if ( (PARAMETER.RhThreshold2EnableLock == TRUE)&&(CheckUpdateFlag(CO_RhThreshold2Lock)) )
      {
          rhThreshold2Lock = _OBJV1(RhThreshold2Lock,nTherm);
          updateThresholds2 = YES;
      }      
      
      if ( rhThreshold2Lock == FALSE )
      {        
          switch ( PARAMETER.RhThreshold2 )
          {
          case NOT_ACTIVE:
              break;
          case BELOW:
              if ( _V1(humidity,nTherm) < param )
              {
                  OBJ_VALUE->RhThreshold2Switch = ON;
              }   
              if ( _V1(humidity,nTherm) > param + (GetHumidityHysteresis(PARAMETER.RhThresholdsHyst) * 10) )
              {
                  OBJ_VALUE->RhThreshold2Switch = OFF;
              }
              break;
          case ABOVE:
              if ( _V1(humidity,nTherm) > param )
              {
                  OBJ_VALUE->RhThreshold2Switch = ON;
              }   
              if ( _V1(humidity,nTherm) < param - (GetHumidityHysteresis(PARAMETER.RhThresholdsHyst) * 10) )
              {
                  OBJ_VALUE->RhThreshold2Switch = OFF;
              }
              break;
          default:
              break;
          }      
      }
      else
      {
          switch (PARAMETER.RhThreshold2LockBehaviour)
          {
          case LOCK_NONE:
              break;
          case LOCK_SEND_OFF:
              _OBJV1(RhThreshold2Switch,nTherm) = OFF;
              break;
          case LOCK_SEND_ON:
              _OBJV1(RhThreshold2Switch,nTherm) = ON;        
              break;
          }        
      }
      
      if ( (threshold2 != OBJ_VALUE->RhThreshold2Switch )||(updateThresholds2 == YES) )          
          SetTransmitRequest( CO_RhThreshold2Switch );
      /* Threshold RT */
       if (EZ_GetState(TimerRhThresholdsRT)) 
       {
          SetTransmitRequest( CO_RhThreshold1Switch );
          SetTransmitRequest( CO_RhThreshold2Switch );
          EZ_StartTimer( TimerRhThresholdsRT, 
                         GetCyclicSendingInterval(PARAMETER.RhThresholdsRT), TM_MODE_1S );
         
       }
  }
  else
  { // sensor from bus
      txflag = (__IsSensorFromBusEnabled(nTherm,FROMBUS_REL_HUMIDITY2) != 0);
      if ( __IsSensorFromBusUpdated(nTherm,FROMBUS_REL_HUMIDITY1+txflag) )
      {
          if ( __IsSensorFromBusFail(nTherm,FROMBUS_REL_HUMIDITY1+txflag))
          {
              _V1(humidity,nTherm) = SENSOR_ERROR;
          }
          else
          {
              if ( txflag )
              { // 2 byte value
                  _V1(humidity,nTherm) = ShortF2LongI(_OBJV1(Humidity2bytesFB,nTherm));
              }
              else
              { // 1 byte value
                  _V1(humidity,nTherm) = (_OBJV1(Humidity1byteFB,nTherm)*100UL/255UL)*100UL;
              }
          }
      }
  }

}