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
#include "ApplicationUtility.h"
#include "ApplicationTimers.h"
#include "Thermostat.h"
#include "ManualFunction.h"

TYPE_MANUAL_FUNC _V1(mf,MAX_THERM);

long _V1(limitProtectionMinMF,MAX_THERM);
long _V1(limitProtectionMaxMF,MAX_THERM);


#define CO_MANUAL_FUNCTION
#define REMOTE_MANUAL_SETPOINT

#ifdef CO_MANUAL_FUNCTION
BYTE firstRun;
#endif     


//-----------------------------------------------------------------------------
// ManualGetSetpoint
//-----------------------------------------------------------------------------                                                   
long ManualGetSetpoint(BYTE nTherm) 
{
  if ( (_PARTHERM(nTherm).HeatingCoolingSwitchOver == HC_SWITCH_AUTOMATIC)&&
       (IS_HEATING_AND_COOLING_ENABLED(nTherm)) )
    return _V1(actualSetpoint,nTherm);
  else
    return _therm(nTherm).setpointManuale;
}

//-----------------------------------------------------------------------------
// ManualStartFunction
//-----------------------------------------------------------------------------                                                   
BYTE ManualStartFunction(BYTE nTherm) 
{
    TYPE_TR_DATA *t = &_therm(nTherm);
    
    _V1(mf,nTherm).manualActive = YES;
    
    if ( (_PARTHERM(nTherm).HeatingCoolingSwitchOver == HC_SWITCH_AUTOMATIC)&&
         (IS_HEATING_AND_COOLING_ENABLED(nTherm)) )
    {
        if ( t->heatingCoolingMode == COOLING )
        {
            t->setpointManuale -= (t->setpointComfort[COOLING] - t->setpointComfort[HEATING]);
        }
    }
    else
    {
    }
    return 1;
}

//-----------------------------------------------------------------------------
// ManualExitFromFunction
//-----------------------------------------------------------------------------                                                   
BYTE ManualExitFromFunction(BYTE nTherm) 
{
  if ( _V1(mf,nTherm).manualActive == YES )
  {
    _V1(mf,nTherm).manualActive = NO;
    EZ_StopTimer(TimerEndOfManual+nTherm); 
  }
  
  return _V1(mf,nTherm).manualActive;
}

//-----------------------------------------------------------------------------
// ManualFunctionInit
//-----------------------------------------------------------------------------                                                   
void ManualFunctionInit(void) 
{
  
   for ( BYTE nTherm = 0; nTherm < MAX_THERM; nTherm++ )
   {
       _V1(mf,nTherm).manualActive = NO;
       _V1(mf,nTherm).manualTimerEnabled = (GetEndOfManualOperation(_PARTHERM(nTherm).EndManualSetpoint) != 0);
       if (( _V1(mf,nTherm).manualActive == YES )&&(_therm(nTherm).hvacAuto != TRUE ))
       {
           _V1(mf,nTherm).manualActive = YES;  
       }
       
       if (IS_HEATING_AND_COOLING_ENABLED(nTherm) || (_V1(thermFunctions,nTherm) & FUNCT_THERM_HEATING)) 
            _V1(limitProtectionMinMF,nTherm) = _therm(nTherm).setpointProtection[HEATING]/10;
       else
            _V1(limitProtectionMinMF,nTherm) = 20; 
       
       if (IS_HEATING_AND_COOLING_ENABLED(nTherm) || (_V1(thermFunctions,nTherm) & FUNCT_THERM_COOLING)) 
            _V1(limitProtectionMaxMF,nTherm) = _therm(nTherm).setpointProtection[COOLING]/10;
       else
            _V1(limitProtectionMaxMF,nTherm) = 500; 
   }
   
#ifdef CO_MANUAL_FUNCTION
   firstRun = TRUE;
#endif     
}
//-----------------------------------------------------------------------------
// ManualFunction
//-----------------------------------------------------------------------------                                                   
BYTE ManualFunction(BYTE mode,BYTE nTherm) 
{
  WORD param;
  long setp; 
  TYPE_MANUAL_FUNC *m = &_V1(mf,nTherm);
  TYPE_TR_DATA *t = &_therm(nTherm);
  
  if ((m->manualActive == YES)&&(m->manualTimerEnabled == YES)&&
      (t->hvacAuto == TRUE))
  {
      param = GetEndOfManualOperation(_PARTHERM(nTherm).EndManualSetpoint);
      if ( param )
      {
          EZ_StartNotRunningTimer( TimerEndOfManual+nTherm, param*60L, TM_MODE_1S );
          if ( EZ_GetState(TimerEndOfManual+nTherm) )
          {
              m->manualActive = NO;
          }
          else
          {
             mode = m->manualHvacMode;
          }
      }
  }
  else
  {
      EZ_StopTimer(TimerEndOfManual+nTherm); 
  }
  

  if ( CheckUpdateFlag(CO_INX_THERM(nTherm,CO_ManualModeStatus_1)) )
  {
      if ( _OBJV1(ManualModeStatus,nTherm) == 0 )
      {
          ManualExitFromFunction(nTherm);
      }
      else
      {
          if ( (m->manualActive == NO)&&(t->hvacMode != HVAC_PROTECTION) )
          {
              ManualStartFunction(nTherm);    
          }
      }
  }


  if ( CheckUpdateFlag(CO_INX_THERM(nTherm,CO_ManualSetpoint_1))&&(t->hvacMode != HVAC_PROTECTION) )
  {
      setp = ShortF2LongI(_OBJV1(ManualSetpoint,nTherm));    
      if ( (setp > _V1(limitProtectionMaxMF,nTherm)*10)||( setp < _V1(limitProtectionMinMF,nTherm)*10) )
      {
      }
      else
      {
          if (m->manualActive == NO)
          {
              long* ptrSetpoints = (long*)&t->setpointComfort;
              BYTE modo = (t->hvacMode-1)*2 +  t->heatingCoolingMode;
              t->setpointBaseManuale = ptrSetpoints[modo];  
              t->setpointManuale = setp;
              ManualStartFunction(nTherm);              
          }
          else
          {
              t->setpointManuale = setp;
          }
      }
  }
  
  if (( m->manualActive !=  _OBJV1(ManualModeStatus,nTherm) )||( firstRun == TRUE ))
  {
      firstRun = FALSE;
      _OBJV1(ManualModeStatus,nTherm) = m->manualActive;
      LUSetTransmitRequest(CO_INX_THERM(nTherm,CO_ManualModeStatus_1));  
  }

  
  return mode;
}