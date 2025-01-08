/**@addtogroup SBS_Code_EK-EQ2-TP
@{
commento.....
*/
//------------------------------------------------------------------
// extension_enabled = YES se passaggio da comfort ad altro tramite 
// messaggio da bus

#include "project.h"
#include "eib_stk.h"
#include "app.h"
#include EIB_DEVICE_MODEL_APP_H_FILE      
#include "ApplicationTimers.h"
#include "ManualFunction.h"
#include "Thermostat.h"
#include "SensorFromBus.h"
#include "PresenceSensor.h"

TYPE_PRESENCE_SENSOR _V1(presence,MAX_THERM);
static BYTE _V1(prevHvacMode,MAX_THERM);

//-----------------------------------------------------------------------------
// PresenceRead
//
// 0 -> not occupied
// 1 -> occupied
// 0x80 -> error !!!! timeout CO o CO non ancora aggiornato
//
// lo stato del sensore presenza può provenire da due oggitti di comunicazione
// se entrambi gli oggetti sono configurati , viene eseguito l' or degli stati.
//
//-----------------------------------------------------------------------------                                                   
BYTE PresenceRead(BYTE nTherm)
{
  BYTE val = 0xFF;
  BYTE presence = 0;
  
  if ( __IsSensorFromBusEnabled(nTherm,FROMBUS_PRESENCE1) )
  {
      if ( (!__IsSensorFromBusTimeout(nTherm,FROMBUS_PRESENCE1))&&
           ( __IsSensorFromBusUpdated(nTherm,FROMBUS_PRESENCE1)) )
      {
          val = _OBJV1(PresenceSensor1FB,nTherm)&0x01;
          presence = (_PARTHERM(nTherm).PresenceSensor1FromBusInv == YES) ? (val^0x01) : val;
      }
  }

  if ( __SetSensorFromBusEnabled(nTherm,FROMBUS_PRESENCE2) )  
  {
      if ( (!__IsSensorFromBusTimeout(nTherm,FROMBUS_PRESENCE2))&&
           ( __IsSensorFromBusUpdated(nTherm,FROMBUS_PRESENCE2)) )
      {
          val = _OBJV1(PresenceSensor2FB,nTherm)&0x01;
          presence |= (_PARTHERM(nTherm).PresenceSensor2FromBusInv == YES) ? (val^0x01) : val;
      }
  }    
  
  return ((val == 0xFF) ? 0x80 : presence);
}
//------------------------------------------------------------------
// PresenceExitFromFunction
//------------------------------------------------------------------
void PresenceRestartTimer(BYTE nTherm)
{
   TYPE_PRESENCE_SENSOR *ps = &_V1(presence,nTherm);
   
   EZ_StopTimer(TimerAbsenceTime+nTherm);
   ps->functionActivated = NO;
   ps->functionStop = YES;
   ps->extension = NO;
   ps->extension_enabled = NO;
}
//------------------------------------------------------------------
// PresenceStatus
//------------------------------------------------------------------
BYTE PresenceStatus( BYTE _hvacMode,BYTE nTherm ) 
{
  BYTE status = 0;
  BYTE mode = _hvacMode;
  WORD param;
  struct THERM *p = (struct THERM*)&_PARTHERM(nTherm);
  TYPE_PRESENCE_SENSOR *ps = &_V1(presence,nTherm);
  
  if ( p->PresenceSensorFunctionality == DISABLED ) return _hvacMode;
  
  status = PresenceRead(nTherm);
 
  if ( (status & 0x80 )||(_V1(prevHvacMode,nTherm) != _hvacMode ))
  {   
      PresenceRestartTimer(nTherm);
      if (( _V1(prevHvacMode,nTherm) == HVAC_COMFORT ))  
      {
          ps->extension_enabled = YES;
      }
      if ( _hvacMode == HVAC_COMFORT )
      {
          ps->absenceStatus = OCCUPIED;
          ps->prevAbsenceStatus = OCCUPIED;
      }
      _V1(prevHvacMode,nTherm) = _hvacMode;
  }
  
  if ( status == NOT_OCCUPIED ) 
  {
    if  ( ( ps->prevAbsenceStatus != NOT_OCCUPIED) )
    {
        ps->prevAbsenceStatus = NOT_OCCUPIED;
        param = GetIntegerConst(&p->PresenceSensTimeBeforeActivation);
        EZ_StartTimer( TimerAbsenceTime+nTherm, param, TM_MODE_1S );
    }
    else
    {
      if ( EZ_GetState( TimerAbsenceTime+nTherm ) ) 
      {
          ps->absenceStatus = NOT_OCCUPIED;
      }
    }
  }
  else
  {
      EZ_StopTimer( TimerAbsenceTime+nTherm );
      ps->absenceStatus = OCCUPIED;
      ps->prevAbsenceStatus = OCCUPIED;
  }
  
  if ( _hvacMode == HVAC_COMFORT )  
      ps->extension_enabled = YES;
  else if (ps->absenceStatus != OCCUPIED)
  {
      ps->extension_enabled = NO;
      EZ_StopTimer(TimerAbsenceTime+nTherm);
  }
  
  /* COMFORT_LIMITATION 
   */
  if (( p->PresenceSensorsUse == COMFORT_LIMITATION ) || 
      ( p->PresenceSensorsUse == COMFORT_EXTENSION_LIMITATION ))
  {
    if ( (_hvacMode == HVAC_COMFORT)&&(ps->absenceStatus == NOT_OCCUPIED) )
    {
      if ( p->SetpointTemperatureLevels == COMFORT_STANDBY )
        _hvacMode = HVAC_STANDBY;
      if ( p->SetpointTemperatureLevels == COMFORT_ECONOMY )
        _hvacMode = HVAC_ECONOMY;
    }
  }
  /* COMFORT_EXTENSION 
  */
  if (( p->PresenceSensorsUse == COMFORT_EXTENSION ) || 
      ( p->PresenceSensorsUse == COMFORT_EXTENSION_LIMITATION ))
  {
        if ( (_hvacMode != HVAC_COMFORT)&&(ps->absenceStatus == OCCUPIED)
            &&(ps->extension_enabled == YES ) )
        {
            _hvacMode = HVAC_COMFORT;
        }
  }

  if ( mode != _hvacMode )
  {
    ps->functionActivated = YES;
    ps->extension = (ps->absenceStatus == OCCUPIED) ? YES : NO ;
    ManualExitFromFunction(nTherm);
  }
  else  
  {
    ps->functionActivated = NO;
    ps->extension = NO;    
  }

  return _hvacMode;
}
