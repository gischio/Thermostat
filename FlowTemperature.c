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
#include "SensorFromBus.h"
#include "FlowTemperature.h"


long _V1(flowTemp,MAX_THERM);

//-----------------------------------------------------------------------------
// FlowTemperatureRead
//-----------------------------------------------------------------------------                                                   
long FlowTemperatureRead(BYTE nTherm)
{

#ifdef PIPPO  /* non ci somo ingressi sulla morsettiera !!! */
  /* questo va bene ma non è ottimizzato ! */
  for ( BYTE inx = 0; inx < EXTIN_MAX_INPUTS; inx++)
  {
      if ( EXT_INPUT_TYPE(inx) == EXTIN_FLOW_TEMP )
       return (ExtInput[inx].ai);
  }
#endif
  
  if (( !__IsSensorFromBusTimeout(nTherm,FROMBUS_FLOW_TEMP) )&&
      ( __IsSensorFromBusUpdated(nTherm,FROMBUS_FLOW_TEMP) ))
  {
      return ( ShortF2LongI(_OBJV1(FlowTempFB,nTherm)) );  
  }
  
  return SENSOR_ERROR; /* errore ...  */
  
}

//-----------------------------------------------------------------------------
// FlowTemperatureInit
//-----------------------------------------------------------------------------                                                   
void FlowTemperatureInit(void)
{
  for ( BYTE nTherm = 0; nTherm < MAX_THERM; nTherm++ )
  {
      _V1(flowTemp,nTherm) = SENSOR_ERROR;
  }
}
//-----------------------------------------------------------------------------
// FlowTemperature
//-----------------------------------------------------------------------------                                                   
BYTE FlowTemperature(BYTE nTherm)
{
  if ( _PARTHERM(nTherm).FlowTempFromBus != ENABLED ) return 0;
  
  _V1(flowTemp,nTherm) = FlowTemperatureRead(nTherm);
  
  return 1;
}