/**@addtogroup SBS_Code_EK-EQ2-TP
@{
commento.....
*/
//------------------------------------------------------------------

#include "project.h"
#include "eib_stk.h"
#include "app.h"
#include EIB_DEVICE_MODEL_APP_H_FILE  
#include "Application.h"
#include "ListValues.h"
#include "ApplicationUtility.h"
#include "ApplicationTimers.h"
#include "Thermostat.h"
#include "HumiditySensor.h"
#include "Humidification.h"

BYTE    fistTimeHumi = 0;

//-----------------------------------------------------------------------------
// HumReadCOEnable
//----------------------------------------------------------------------------- 
BYTE HumReadCOEnable(BYTE humidificationDisable,BYTE nTherm)
{
  
    if ( CheckUpdateFlag(CO_INX_THERM(nTherm,CO_HumidControlDisable_1)) )
        humidificationDisable = ContactFromBus(_OBJV1(HumidControlDisable,nTherm) , 
                                               _PARTHERM(nTherm).HumidificationInvertDisableSignal);
        
    return humidificationDisable;    
}

//-----------------------------------------------------------------------------
// humValueErr
//-----------------------------------------------------------------------------   
BYTE humValueErr(BYTE nTherm)
{
  
    if ( _V1(humidity,nTherm) == SENSOR_ERROR )
        return 1;

    return 0;
}

//-----------------------------------------------------------------------------
// HumidificationReady
//-----------------------------------------------------------------------------   
BYTE HumidificationReady(BYTE hvacMode,BYTE nTherm)
{
  if ( (_therm(nTherm).humidificationDisable != 1)&&(hvacMode != HVAC_PROTECTION) )
  {
      if ( 
           (_PARTHERM(nTherm).Humidification == HD_COOLING_AND_HEATING)||
           ( (_therm(nTherm).heatingCoolingMode == COOLING)&&(_PARTHERM(nTherm).Humidification == HD_COOLING_ONLY) )||
           ( (_therm(nTherm).heatingCoolingMode == HEATING)&&(_PARTHERM(nTherm).Humidification == HD_HEATING_ONLY)) 
         )
      {
        if ( ! humValueErr(nTherm) )  return 1;  
      }
  }
  
  return 0;  
}

//-----------------------------------------------------------------------------
// HumidificationInit
//-----------------------------------------------------------------------------                                                   
void HumidificationInit(void)
{
  WORD param;

  fistTimeHumi = 0;
  
  for ( BYTE nTherm = 0; nTherm < MAX_THERM; nTherm++ )
  {
      if (!HUMIDIFICATION_ENABLE(nTherm) ) continue;
      fistTimeHumi |= (1L<<nTherm);
      if ( param = GetCyclicSendingInterval(_PARTHERM(nTherm).HumidificationControlRT) )
      {
          EZ_StartTimer( TimerHumCommandRT+nTherm, param, TM_MODE_1S );
      }
  }
}

//-----------------------------------------------------------------------------
// Humidification
//-----------------------------------------------------------------------------                                                   
BYTE Humidification(BYTE hvacMode,BYTE nTherm) 
{
  BYTE flagTx = 0;
  
  if (!HUMIDIFICATION_ENABLE(nTherm) ) return 0;
  
  if ( _PARTHERM(nTherm).HumidificationDisableFromBus == YES )
      _therm(nTherm).humidificationDisable = HumReadCOEnable(_therm(nTherm).humidificationDisable,nTherm);
  else
      _therm(nTherm).humidificationDisable = 0;
  
  
  if ( HumidificationReady(hvacMode,nTherm) )
  {
      if ( _V1(humidity,nTherm) > (_therm(nTherm).setpointHumidification*100) )
      {
          if ( _OBJV1(HumidCommand,nTherm) != 0 )
          {
              _OBJV1(HumidCommand,nTherm) = 0;
              flagTx = 1;
          }
      }
      else if ( _V1(humidity,nTherm) < ( (_therm(nTherm).setpointHumidification*100) - 
                                      GetHumidityHysteresis(_PARTHERM(nTherm).HumidificationHysteresis)*10 ) )
      {
           if ( _OBJV1(HumidCommand,nTherm) != 1 )
          {
              _OBJV1(HumidCommand,nTherm) = 1;
              flagTx = 1;
          }        
      }
  }
  else
  { // disattivo l'uscita
      if ( _OBJV1(HumidCommand,nTherm) != 0 )
      {
          _OBJV1(HumidCommand,nTherm) = 0;
          flagTx = 1;
      }
  }

  // RT
  if ( EZ_GetState(TimerHumCommandRT+nTherm) ) 
  {
      EZ_StartTimer( TimerHumCommandRT+nTherm, 
                     GetCyclicSendingInterval(_PARTHERM(nTherm).HumidificationControlRT), TM_MODE_1S );
      flagTx = 1;
  }
  
  if (( flagTx )||(fistTimeHumi&(1L<<nTherm)) ) 
      SetTransmitRequest( CO_INX_THERM(nTherm,CO_HumidCommand_1) );
  
  fistTimeHumi &= ~(1L<<nTherm);

  
  return 1;
}