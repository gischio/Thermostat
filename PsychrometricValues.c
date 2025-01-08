/**@addtogroup SBS_Code_EK-EQ2-TP
@{
commento.....
*/
//------------------------------------------------------------------

#include "project.h"
#include "eib_stk.h"
#include "app.h"
#include EIB_DEVICE_MODEL_APP_H_FILE
#include "ApplicationUtility.h"
#include "ListValues.h"
#include "ApplicationTimers.h"
#include <stdlib.h>
#include <math.h>
#include "Thermostat.h"
#include "HumiditySensor.h"
#include "PsychrometricValues.h"

#ifdef ABSHUMIDITY_ENABLED
//float absHumidity[MAX_THERM];
#endif
#ifdef ENTHALPY_ENABLED  
//float enthalpy[MAX_THERM];
#endif

long  _V1(prevDewpoint,MAX_THERM);
//long  _V1(prevPerceivedTemperature,MAX_THERM);
long  _V1(dewPoint,MAX_THERM);

//-----------------------------------------------------------------------------
// PsychrometricValueRT
//-----------------------------------------------------------------------------
BYTE PsychrometricValueRT(BYTE paramRT,WORD CO_nr,WORD timer_nr,BYTE nTherm)
{
  WORD param;
  
  param = GetCyclicSendingInterval(paramRT);
  if ( param != 0 )
  {
      if ( EZ_GetState(timer_nr+nTherm ) )
      {   
          SetTransmitRequest(CO_INX_THERM(nTherm,CO_nr));
          return 1;
      }
      else
      {
          EZ_StartNotRunningTimer( timer_nr+nTherm, param, TM_MODE_1S );
      }
  }  
  
  return 0;
}


//-----------------------------------------------------------------------------
// PsychrometricValue
//
// 
//-----------------------------------------------------------------------------
void PsychrometricValue (BYTE nTherm)
{
  BYTE  serror = 0;
  float fHum;
  float fTemp;  
  float ua;


  struct THERM *p = (struct THERM*)&_PARTHERM(nTherm);
  
  if ( (p->HumidityFromBus != ENABLE) ) return;
   
  if (( _V1(temperature,nTherm) == SENSOR_ERROR) || ( _V1(humidity,nTherm) == SENSOR_ERROR ))
  {
      serror = 1;
  }
  else
  {
      fTemp = _V1(temperature,nTherm)/100.0;
      fHum = _V1(humidity,nTherm)/100.0;
  }
  
  /* DewPoint       
   --------------------------------------------- */
  if ( serror )
  {
      _V1(dewPoint,nTherm) = SENSOR_ERROR;
  }
  else
  {
      _V1(dewPoint,nTherm) = (long)(DewpointTemperature(fHum,fTemp)*100); 
  }
  _OBJV1(DewPointTemp,nTherm) = LongI2ShortF(_V1(dewPoint,nTherm));
  if ( p->DewPointTemperature == ENABLED )
  {
      PsychrometricValueRT(p->DewPointTemperatureRT,CO_DewPointTemp,TimerDewPointRT,nTherm);
      if ( labs(_V1(dewPoint,nTherm) - _V1(prevDewpoint,nTherm)) > GetTemperatureMinChangeToSend(p->DewPointTemperatureCOV)*10)
          SetTransmitRequest( CO_INX_THERM(nTherm,CO_DewPointTemp) );
      _V1(prevDewpoint,nTherm) = _V1(dewPoint,nTherm);
  }
#ifdef PERCEIVED_TEMPERATURE_ENABLED  
  /* Perceived Temperature       
   --------------------------------------------- */
  if ( serror )
  {
      perceivedTemperature[nTherm] = SENSOR_ERROR;
  }
  else
  {
      perceivedTemperature[nTherm] = (long)(PerceivedTemperature(fHum,fTemp)*100);
  }
  OBJ_VALUE->PerceivedTemperature[nTherm] = LongI2ShortF(perceivedTemperature[nTherm]);
  if ( p->PerceivedTemperature == ENABLE )
  {
      PsychrometricValueRT(p->PerceivedTemperature,CO_PerceivedTemperature,TimerPerceivedTemperatureRT,nTherm);
      if ( labs(perceivedTemperature - prevPerceivedTemperature) > GetTemperatureMinChangeToSend(p->PerceivedTemperatureCOV)*10 )
          SetTransmitRequest( CO_INX_THERM(nTherm,CO_PerceivedTemperature) );
      prevPerceivedTemperature = perceivedTemperature;
  }
#endif  
#ifdef ABSHUMIDITY_ENABLED
  /* Absolute Humidity       
   --------------------------------------------- */
  if ( serror )
  {
      ua = SENSOR_ERROR; /* Error !!! */
  }
  else
  {
      ua = AbsoluteHumidity(fHum,fTemp);
  }
#endif
#ifdef ENTHALPY_ENABLED  
  /* Enthalpy    
   --------------------------------------------- */
  if ( serror )    
  {
      enthalpy[nTherm] = SENSOR_ERROR; /* Error !!! */
  }
  else
  {
      enthalpy[nTherm] = Enthalpy(ua,fTemp);  
  }

  absHumidity[nTherm] = ua;
#endif  
}               
