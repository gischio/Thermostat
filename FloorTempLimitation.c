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
#include "SensorFromBus.h"
#include "Thermostat.h"
#include "FloorTempLimitation.h"


long _V1(FloorTemp,MAX_THERM);
TYPE_FLOORTEMPLIMIT _V1(ftc,MAX_THERM);

//-----------------------------------------------------------------------------
// FloorTemperatureRead
//-----------------------------------------------------------------------------                                                   
long FloorTemperatureRead(BYTE nTherm)
{
#ifdef EXT_IN_PRESENT
  /* questo va bene ma non è ottimizzato ! */
  for ( BYTE inx = 0; inx < EXTIN_MAX_INPUTS; inx++)
  {
      if ( EXT_INPUT_TYPE(inx) == EXTIN_FLOOR_TEMP )
       return (ExtInput[inx].ai);
  }
#endif
  if (( !__IsSensorFromBusTimeout(nTherm,FROMBUS_FLOOR_TEMP) )&&
      ( __IsSensorFromBusUpdated(nTherm,FROMBUS_FLOOR_TEMP) ))
  {
      return ( ShortF2LongI(_OBJV1(FloorTempFB,nTherm)) );  
  }
  
  return SENSOR_ERROR; /* errore ...  */
  
}
//-----------------------------------------------------------------------------
// FloorTemperatureLimitation
//-----------------------------------------------------------------------------
BYTE FloorTemperatureLimitation (BYTE nTherm)
{
  long app;
  
  if ( !FLOOR_TEMP_LIMIT_ENABLE(nTherm) ) 
  {
      _V1(ftc,nTherm).heatingTempLimitActivated = NO;
      return 0;
  }
  
  _V1(FloorTemp,nTherm) = FloorTemperatureRead(nTherm);
  
  if ( (_PARTHERM(nTherm).HeatingFloorTempLimit == DISABLED) ||
       (_V1(FloorTemp,nTherm) == SENSOR_ERROR) ||
       (_therm(nTherm).heatingCoolingMode != HEATING) ) 
    
  {
      _V1(ftc,nTherm).heatingTempLimitActivated = NO;
      return 0;
  }
  
  app = ShortF2LongI(_PARTHERM(nTherm).HeatingTempLimitValue);
  if ( _V1(FloorTemp,nTherm) > app )
      _V1(ftc,nTherm).heatingTempLimitActivated = YES;
  else if ( _V1(FloorTemp,nTherm) < app - GetHysteresis(_PARTHERM(nTherm).TempLimitHysteresisRange)*10 )
      _V1(ftc,nTherm).heatingTempLimitActivated = NO;
 
  
  return 0;
}


