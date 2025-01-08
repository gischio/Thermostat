/**@addtogroup SBS_Code_EK-EQ2-TP
@{
commento.....
*/
//------------------------------------------------------------------
#include <string.h>
#include "project.h"
#include "eib_stk.h"
#include "app.h"
#include EIB_DEVICE_MODEL_APP_H_FILE      
#include "Application.h"
#include "ListValues.h"
#include "ApplicationTimers.h"
#include "ApplicationUtility.h"
#include "HumiDehumiSetpoints.h"
#include "Thermostat.h"

//-----------------------------------------------------------------------------
// HumiDehumiSetpointsInit
//-----------------------------------------------------------------------------                                                   
void HumiDehumiSetpointsInit(void) 
{

  for ( BYTE nTherm = 0; nTherm < MAX_THERM; nTherm++ )
  {
      _OBJV1(HumiditySetpointForHum,nTherm)   = LongI2ShortF(_therm(nTherm).setpointHumidification*100L);
      _OBJV1(HumiditySetpointForDehum,nTherm) = LongI2ShortF(_therm(nTherm).setpointDehumidification*100L);

      _OBJV1(HumidControlDisable,nTherm) = ContactFromBus(_therm(nTherm).humidificationDisable, 
                                                          _PARTHERM(nTherm).HumidificationInvertDisableSignal);
      _OBJV1(DehumControlDisable,nTherm) = ContactFromBus(_therm(nTherm).dehumidificationDisable, 
                                                          _PARTHERM(nTherm).DehumidificationInvertDisableSignal);      
  }
}
//-----------------------------------------------------------------------------
// HumiDehumiSetpoints
//-----------------------------------------------------------------------------                                                   
void HumiDehumiSetpoints(BYTE nTherm) 
{
  long param;
  
  if (CheckUpdateFlag(CO_INX_THERM(nTherm,CO_HumiditySetpointForHum_1))) 
  {
    param = (ShortF2LongI(_OBJV1(HumiditySetpointForHum,nTherm)));
    if ( param < MIN_SP_HUMI ) param = MIN_SP_HUMI;
    else if ( param > MAX_SP_HUMI ) param = MAX_SP_HUMI;
    _therm(nTherm).setpointHumidification = param/100;
    _OBJV1(HumiditySetpointForHum,nTherm)   = LongI2ShortF(_therm(nTherm).setpointHumidification*100L);    
  }
  if (CheckUpdateFlag(CO_INX_THERM(nTherm,CO_HumiditySetpointForDehum_1))) 
  {
    param = (ShortF2LongI(_OBJV1(HumiditySetpointForDehum,nTherm)));
    if ( param < MIN_SP_DEHUMI ) param = MIN_SP_DEHUMI;
    else if ( param > MAX_SP_DEHUMI ) param = MAX_SP_DEHUMI;
    _therm(nTherm).setpointDehumidification = param/100;
    _OBJV1(HumiditySetpointForDehum,nTherm) = LongI2ShortF(_therm(nTherm).setpointDehumidification*100L);    
  }  
}

