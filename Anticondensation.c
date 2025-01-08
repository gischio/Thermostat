/**@addtogroup SBS_Code_EK-EQ2-TP
@{
commento.....
*/
//------------------------------------------------------------------

#include "project.h"
#include "eib_stk.h"
#include "app.h"
#include EIB_DEVICE_MODEL_APP_H_FILE    
#include "ListValues.h"
#include "ApplicationTimers.h"
#include "Thermostat.h"
#include "SensorFromBus.h"
#include "FlowTemperature.h"
#include "PsychrometricValues.h"
#include "Anticondensation.h"


TYPE_ANTICONDENSATION _ac(MAX_THERM);

//-----------------------------------------------------------------------------
// AnticondensationSensorRead
//
// 0 -> non attiva , nessuna formazione di condensa
// 1 -> attiva , possibile formazione di condensa
//
//-----------------------------------------------------------------------------                                                   
BYTE AnticondensationSensorRead(BYTE nTherm)
{

#ifdef EXT_IN_PRESENT
  /* questo va bene ma non è ottimizzato ! */
  for ( BYTE inx = 0; inx < EXTIN_MAX_INPUTS; inx++)
  {
      if ( EXT_INPUT_TYPE(inx) == EXTIN_ANTICONDENSATION )
       return (ExtInput[inx].di&0x1);
  }
#endif
  if (( !__IsSensorFromBusTimeout(nTherm,FROMBUS_ANTICONDESATION) )&&
      ( __IsSensorFromBusUpdated(nTherm,FROMBUS_ANTICONDESATION) ))
  {
      return ( _OBJV1(AnticondensationFB,nTherm));  
  }
  
  return OFF; /* errore ...   forza il controllo in disattivato ! */
  
}

//-----------------------------------------------------------------------------
// AnticondensationInit
//-----------------------------------------------------------------------------
void AnticondensationInit(void)
{
  struct THERM *p;
  
  for ( BYTE nTherm = 0; nTherm < MAX_THERM; nTherm++ )
  {
      EZ_StopTimer( TimerPassiveAnticond+nTherm );
      EZ_StopTimer( TimerActiveAnticond+nTherm );
      _ac(nTherm).passiveAnticondActivated = NO;
      _ac(nTherm).activeAnticondActivated = NO;
      
      p = (struct THERM*)&_PARTHERM(nTherm); //PARAMETER._therm(nTherm);
      if ( p->AnticondensationFromBus == ENABLE )
      {
          if ((( p->CoolingType == FLOOR_RADIANT )||( p->CoolingType == CEILING_RADIANT ))&&
               ( _V1(thermFunctions,nTherm) & FUNCT_THERM_COOLING )&&(p->AnticondensationWithProbe == ENABLED))
                 _V1(thermFunctions,nTherm) |= FUNCT_PASS_ANTICOND;
      } 
      
      if ( p->HumidityFromBus == ENABLE )
      {
          if ((( p->CoolingType == FLOOR_RADIANT )||( p->CoolingType == CEILING_RADIANT ))&&
               ( _V1(thermFunctions,nTherm) & FUNCT_THERM_COOLING ) )
          {
              if (p->ActiveAnticondensation == ENABLE_PROBE )
              {
                  _V1(thermFunctions,nTherm) |= FUNCT_ACTIVE_ANTICOND;
              }
              else
              {
                  if (p->ActiveAnticondensation == ENABLE_TPROJ )                
                      _V1(thermFunctions,nTherm) |= FUNCT_ACTIVE_ANTICOND_NOPROB;
              }
          }
      }       
  }
}

//-----------------------------------------------------------------------------
// PassiveAnticondensation
//-----------------------------------------------------------------------------
BYTE PassiveAnticondensation (BYTE thermFail,BYTE setpointMatched,BYTE nTherm)
{
  BYTE anticondStatus;
  WORD param;
  
  if (!( _V1(thermFunctions,nTherm) & FUNCT_PASS_ANTICOND )) return _ac(nTherm).passiveAnticondActivated;
 
  if (( _therm(nTherm).heatingCoolingMode != COOLING )||(thermFail == 1)||
      ((statoRegolazioneCooling == OFF)&&(_ac(nTherm).passiveAnticondActivated == 0))||
        ((setpointMatched == YES)&&(_ac(nTherm).passiveAnticondActivated == 1)) ) 
      anticondStatus = OFF;
  else 
      anticondStatus = AnticondensationSensorRead(nTherm);     
        
  if ( anticondStatus != _ac(nTherm).passiveAnticondActivated )
  {
      if ( anticondStatus == 1 )
      {
         param = GetCyclicSendingInterval(_PARTHERM(nTherm).AnticondAlarmADelay);
         if ( param != 0 )
            EZ_StartNotRunningTimer( TimerPassiveAnticond+nTherm, param, TM_MODE_1S );
         else
            EZ_StartNotRunningTimer( TimerPassiveAnticond+nTherm, 1, TM_MODE_1S );         
      }
      else
      {        
          EZ_StopTimer( TimerPassiveAnticond+nTherm );
      }
      if (( EZ_GetState(TimerPassiveAnticond+nTherm) ) || (anticondStatus == 0))
      {
          _ac(nTherm).passiveAnticondActivated = anticondStatus;  
          _OBJV1(AnticondensationAlarm,nTherm) = _ac(nTherm).passiveAnticondActivated;
          SetTransmitRequest(CO_INX_THERM(nTherm,CO_AnticondensationAlarm));
      }
  }
 
  return anticondStatus;
}
//-----------------------------------------------------------------------------
// ActiveAnticondensation NON ESISTE NELLA VERSIONE BASE
//-----------------------------------------------------------------------------
BYTE ActiveAnticondensation (BYTE thermFail,BYTE setpointMatched,BYTE nTherm)
{
  BYTE anticondStatus = _ac(nTherm).activeAnticondActivated;
  WORD param;
  
  if (!( _V1(thermFunctions,nTherm) & FUNCT_ACTIVE_ANTICOND )) return _ac(nTherm).activeAnticondActivated;
 
  if (( _therm(nTherm).heatingCoolingMode != COOLING )||(thermFail == 1)||
      ((_V1(statoRegolazioneCooling,nTherm) == OFF)&&(_ac(nTherm).activeAnticondActivated == 0))||
       (_V1(dewPoint,nTherm) == SENSOR_ERROR)||(_V1(flowTemp,nTherm) == SENSOR_ERROR)||
       ((setpointMatched == YES)&&(_ac(nTherm).activeAnticondActivated == 1)) ) 
  {
      anticondStatus = OFF;
  }
  else
  {
      if ( flowTemp  > dewPoint )
          anticondStatus = OFF;
      else if ( _V1(flowTemp,nTherm) + GetHysteresis(_PARTHERM(nTherm).ActiveAnticondensationHystRange)*10 <= _V1(dewPoint,nTherm) )
          anticondStatus = ON;
  }
    
  if (anticondStatus != _ac(nTherm).activeAnticondActivated)
  {
      if ( anticondStatus == 1 ) 
      {
         param = GetCyclicSendingInterval(_PARTHERM(nTherm).AnticondAlarmADelay);
         if ( param != 0 )
            EZ_StartNotRunningTimer( TimerActiveAnticond+nTherm, param, TM_MODE_1S );
         else
            EZ_StartNotRunningTimer( TimerActiveAnticond+nTherm, 1, TM_MODE_1S );         
      }
      else 
      {
          EZ_StopTimer( TimerActiveAnticond+nTherm );
      }
      if (( EZ_GetState(TimerActiveAnticond+nTherm) ) || (anticondStatus == 0))
      {
          _ac(nTherm).activeAnticondActivated = anticondStatus;
          _OBJV1(AnticondensationAlarm,nTherm) = _ac(nTherm).activeAnticondActivated;
          SetTransmitRequest(CO_INX_THERM(nTherm,CO_AnticondensationAlarm));
      }
  }
  
  return anticondStatus;
}                                                   

//-----------------------------------------------------------------------------
// ActiveAnticondensationNoProbe NON ESISTE NELLA VERSIONE BASE
//-----------------------------------------------------------------------------
BYTE ActiveAnticondensationNoProbe (BYTE thermFail,BYTE setpointMatched,BYTE nTherm)
{
  BYTE anticondStatus = _ac(nTherm).activeAnticondActivated;
  WORD param;
  int flowT;
  
  if (!( _V1(thermFunctions,nTherm) & FUNCT_ACTIVE_ANTICOND_NOPROB )) return _ac(nTherm).activeAnticondActivated;
 
  flowT = GetTemperatureAnticondensation(_PARTHERM(nTherm).DesignFlowTemperature);
  
  if (( _therm(nTherm).heatingCoolingMode != COOLING )||(thermFail == 1)||
      ((_V1(statoRegolazioneCooling,nTherm) == OFF)&&(_ac(nTherm).activeAnticondActivated == 0))||
       (_V1(dewPoint,nTherm) == SENSOR_ERROR)||
       ((setpointMatched == YES)&&(_ac(nTherm).activeAnticondActivated == 1)) ) 
  {
      anticondStatus = OFF;
  }
  else
  {
      if ( flowT  > _V1(dewPoint,nTherm) )
          anticondStatus = OFF;
      else if ( flowT + 30 <= _V1(dewPoint,nTherm) )
          anticondStatus = ON;
  }
    
  if (anticondStatus != _ac(nTherm).activeAnticondActivated)
  {
      if ( anticondStatus == 1 ) 
      {
         param = GetCyclicSendingInterval(_PARTHERM(nTherm).AnticondAlarmADelay);
         if ( param != 0 )
            EZ_StartNotRunningTimer( TimerActiveAnticond+nTherm, param, TM_MODE_1S );
         else
            EZ_StartNotRunningTimer( TimerActiveAnticond+nTherm, 1, TM_MODE_1S );         
      }
      else 
      {
          EZ_StopTimer( TimerActiveAnticond+nTherm );
      }
      if (( EZ_GetState(TimerActiveAnticond+nTherm) ) || (anticondStatus == 0))
      {
          _ac(nTherm).activeAnticondActivated = anticondStatus;
          _OBJV1(AnticondensationAlarm,nTherm) = _ac(nTherm).activeAnticondActivated;
          SetTransmitRequest(CO_INX_THERM(nTherm,CO_AnticondensationAlarm));
      }
  }
  
  return anticondStatus;
}                                                   
