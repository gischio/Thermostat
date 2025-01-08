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
#include "OutdoorTemperature.h"


long outdoorTemp[MAX_THERM];

//-----------------------------------------------------------------------------
// OutdoorTemperatureRead
//-----------------------------------------------------------------------------                                                   
long OutdoorTemperatureRead(BYTE nTherm)
{

#ifdef EXT_IN_PRESENT
  /* questo va bene ma non è ottimizzato ! */
  for ( BYTE inx = 0; inx < EXTIN_MAX_INPUTS; inx++)
  {
      if ( EXT_INPUT_TYPE(inx) == EXTIN_OUTDOOR_TEMP )
       return (ExtInput[inx].ai);
  }
#endif
  if (( !__IsSensorFromBusTimeout(nTherm,FROMBUS_OUTDOOR_TEMP) )&&
      ( __IsSensorFromBusUpdated(nTherm,FROMBUS_OUTDOOR_TEMP) ))
  {
      return ( ShortF2LongI(_OBJV1(OutdoorTempFB,nTherm)) );  
  }
  
  return SENSOR_ERROR; /* errore ...  */
  
}


//-----------------------------------------------------------------------------
// OutdoorTemperatureInit
//-----------------------------------------------------------------------------
void OutdoorTemperatureInit(void)
{
  
  for ( BYTE nTherm = 0; nTherm < MAX_THERM; nTherm++ ) 
  {
      outdoorTemp[nTherm] = SENSOR_ERROR;
  }
}

//-----------------------------------------------------------------------------
// OutdoorTemperature
//-----------------------------------------------------------------------------
BYTE OutdoorTemperature(BYTE nTherm)
{
  
  if ( _PARTHERM(nTherm).OutdoorTempFromBus != ENABLED ) return 0;
  
  outdoorTemp[nTherm] = OutdoorTemperatureRead(nTherm);
  
  return 0;
}


