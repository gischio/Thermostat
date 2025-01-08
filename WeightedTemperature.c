/**@addtogroup SBS_Code_EK-EQ2-TP
@{
commento.....
*/
//------------------------------------------------------------------

#include "project.h"
#include "eib_stk.h"
#include "app.h"
#include EIB_DEVICE_MODEL_APP_H_FILE
#include <stdlib.h>
#include "ApplicationUtility.h"
#include "ApplicationTimers.h"
#include "ListValues.h"
#include "SensorFromBus.h"
#include "WeightedTemperature.h"
#include "Thermostat.h"

long _V1(wTemperature,MAX_THERM);
long _V1(prevwTemperature,MAX_THERM);

//-------------------------------------------------------------------- 
// RoomTemperature
//-------------------------------------------------------------------- 
long RoomTemperature(BYTE nTherm)
{
  
  if ( _PARTHERM(nTherm).RoomTempFromBus == ENABLED )
      return CalculationOfWeightedTemperature(nTherm);
  else
      return fTemperature[nTherm];
}

//-------------------------------------------------------------------- 
// ReadAddMainRoomTemperature
//-------------------------------------------------------------------- 
long ReadAddMainRoomTemperature(BYTE tempSrc,BYTE nTherm)
{
  long temp = SENSOR_ERROR;

  switch ( tempSrc )
  {
  case SENSOR_INTERNAL:
    temp = fTemperature[nTherm];
    break;
  case SENSOR_INPUT:
#ifdef EXT_IN_PRESENT    
    /* questo va bene ma non è ottimizzato ! */
    for ( BYTE inx = 0; inx < EXTIN_MAX_INPUTS; inx++)
    {
        if ( EXT_INPUT_TYPE(inx) == EXTIN_ROOM_TEM )
        {
            temp = ExtInput[inx].ai;
            break;
        }
    }
#endif    
    break;
  case SENSOR_FROM_BUS:
    if ( !__IsSensorFromBusTimeout(nTherm,FROMBUS_ROOM_TEMP)&& __IsSensorFromBusUpdated(nTherm,FROMBUS_ROOM_TEMP) )
        temp = ShortF2LongI(_OBJV1(RoomTempFB,nTherm));
    else
        temp = SENSOR_ERROR;
    break;
  default:
    break;
  }  
  
  return temp;
  
}

//-------------------------------------------------------------------- 
// TransmissionOfWeightedTemperature
//-------------------------------------------------------------------- 
void TransmissionOfWeightedTemperature(BYTE nTherm)
{
  BYTE txflag = 0;
  long change;

  change = ShortF2LongI(_PARTHERM(nTherm).ChangeOfValueWeightedTemp);
  _OBJV1(WeightedTemp,nTherm) = LongI2ShortF(_V1(wTemperature,nTherm));
  if (( labs( _V1(prevwTemperature,nTherm) - _V1(wTemperature,nTherm) ) > change )&&( change != 0 ))
  {
      _V1(prevwTemperature,nTherm) = _V1(wTemperature,nTherm);
      txflag = 1;
  }
  else
  {
      if (EZ_GetState(TimerWeightedTemperatureRT+nTherm)) 
      {
        EZ_StartTimer( TimerWeightedTemperatureRT+nTherm, 
                      GetCyclicSendingInterval(_PARTHERM(nTherm).RepetitionTimeWeightedTemp), TM_MODE_1S );
        txflag = 1;
      }
  }
  
  if ( txflag )  SetTransmitRequest( CO_INX_THERM(nTherm,CO_WeightedTemp_1) );

}

//-------------------------------------------------------------------- 
// WeightedTemperatureInit
//-------------------------------------------------------------------- 
void WeightedTemperatureInit(void)
{
  WORD param;
  
  for ( BYTE nTherm=0; nTherm < MAX_THERM; nTherm++ )
  {
      if ( _PARTHERM(nTherm).RoomTempFromBus != 1 )
          continue;
      if ( param = GetCyclicSendingInterval(_PARTHERM(nTherm).RepetitionTimeWeightedTemp) )
      {
          EZ_StartTimer( TimerWeightedTemperatureRT+nTherm, param, TM_MODE_1S );
      }       
  }

}
//-------------------------------------------------------------------- 
// CalculationOfWeightedTemperature
//-------------------------------------------------------------------- 
long CalculationOfWeightedTemperature(BYTE nTherm)
{
  long val;
  BYTE w;
  long addTemperature = SENSOR_ERROR;  
  long mainTemperature = SENSOR_ERROR;
  
  w = GetRelativeWeight( _PARTHERM(nTherm).RelativeWeight ); 

  mainTemperature = ReadAddMainRoomTemperature(SENSOR_INTERNAL,nTherm);
  addTemperature  = ReadAddMainRoomTemperature(SENSOR_FROM_BUS,nTherm);

  
  if ( (( mainTemperature == SENSOR_ERROR )&&(tRelativeWeight[w][0] != 0))||
       (( addTemperature  == SENSOR_ERROR )&&(tRelativeWeight[w][1] != 0)) )
  {
      val = SENSOR_ERROR;
  }
  else
  {    
      val = (mainTemperature * tRelativeWeight[w][0]) / 100 + (addTemperature * tRelativeWeight[w][1]) / 100;
  }
  
  _V1(wTemperature,nTherm) = val;
  _OBJV1(WeightedTemp,nTherm) = LongI2ShortF(val);
  
  if ( (tRelativeWeight[w][1] == 0)||(tRelativeWeight[w][0] == 0) )
  {
  }
  else     
  {
      TransmissionOfWeightedTemperature(nTherm);
  }
  
  return val;
}
                                                   
