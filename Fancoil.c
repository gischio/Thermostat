/**@addtogroup SBS_Code_EK-EQ2-TP
@{
commento.....
*/
//------------------------------------------------------------------
#include <stdlib.h>
#include "project.h"
#include "eib_stk.h"
#include "app.h"
#include EIB_DEVICE_MODEL_APP_H_FILE      
#include "ListValues.h"
#include "ApplicationTimers.h"
#include "ApplicationUtility.h"
#include "fancoil.h"
#include "Thermostat.h"
#include "SensorFromBus.h"

#define FAN_HYSTERESIS

#define MAN_SPEEDOFF_ACTIVE       0
#define MAN_SPEEDOFF_NOT_ACTIVE   1


BYTE prevContFanOut[MAX_THERM];

BYTE firstRunFancoil;
BYTE antistrat;

TYPE_VENTILATION _fc(MAX_THERM);
BYTE             _fancoilFunctions(MAX_THERM);

//-----------------------------------------------------------------------------
// SetFancoilFunctions
//-----------------------------------------------------------------------------  
void SetFancoilFunctions(void)
{
  struct THERM *p; 
  
  for ( BYTE nTherm = 0; nTherm < MAX_THERM; nTherm++ )
  {  
      _fancoilFunctions(nTherm) = 0;

      if ( !(_V1(thermFunctions,nTherm) & FUNCT_THERMOSTAT) ) continue;

      p = (struct THERM*)&_PARTHERM(nTherm);
      
      if ((_V1(thermFunctions,nTherm) & FUNCT_THERM_HEATING )&&( p->HeatingType == FANCOIL ))
      {
          _fancoilFunctions(nTherm) |= FUNCT_FANCOIL_HEATING;
      }
      
      if ((_V1(thermFunctions,nTherm) & FUNCT_THERM_AUXHEATING )&&( p->AuxHeatingVentilation == ENABLED ))
      {
          _fancoilFunctions(nTherm) |= FUNCT_FANCOIL_AUXHEATING;
      }
      
      if ((_V1(thermFunctions,nTherm) & FUNCT_THERM_COOLING )&&( p->CoolingType == FANCOIL ))
      {
          _fancoilFunctions(nTherm) |= FUNCT_FANCOIL_COOLING;
      }
      
      if ((_V1(thermFunctions,nTherm) & FUNCT_THERM_AUXCOOLING )&&( p->AuxCoolingVentilation == ENABLED ))
      {
          _fancoilFunctions(nTherm) |= FUNCT_FANCOIL_AUXCOOLING;
      }
      
      if (( _fancoilFunctions(nTherm) & (FUNCT_FANCOIL_HEATING|FUNCT_FANCOIL_AUXHEATING|
                                   FUNCT_FANCOIL_COOLING)|FUNCT_FANCOIL_AUXCOOLING) )
      {
          _V1(thermFunctions,nTherm) |= FUNCT_FANCOIL;
      }
      
      if ( (_fancoilFunctions(nTherm) & (FUNCT_FANCOIL_AUXHEATING|FUNCT_FANCOIL_HEATING))&&
           (p->AntistratTempFromBus == ENABLE)&&(p->FanAntistratification == ENABLE) )
      {
          _fancoilFunctions(nTherm) |= FUNCT_ANTISTRAT;    
      }
      
      if ( (_V1(thermFunctions,nTherm) & FUNCT_FANCOIL)&&(p->CoilTempFromBus == ENABLE) )
      {
          _fancoilFunctions(nTherm) |= FUNCT_COIL_BATTERY;
      }
  }
}

//-----------------------------------------------------------------------------
// FanGetNrOfManualSpeed
//-----------------------------------------------------------------------------   
BYTE FanGetNrOfManualSpeed(BYTE fanControlType)
{
  if ( fanControlType < 3 )
    return fanControlType+1;
  else
    return 5;
}

//-----------------------------------------------------------------------------
// FanManualSpeedPercentage
//-----------------------------------------------------------------------------   
BYTE FanManualSpeedPercentage(struct THERM *therm, BYTE manualFanSpeed)
{
  BYTE nrSpeed;

  nrSpeed = FanGetNrOfManualSpeed(therm->FanControlType);

  return (255*manualFanSpeed/nrSpeed);
}

//-----------------------------------------------------------------------------
// OutFanSpeed
//-----------------------------------------------------------------------------   
BYTE OutFanSpeed(BYTE fanOut,BYTE nTherm)
{
  BYTE mask = 1;
  struct THERM *p = (struct THERM*)&_PARTHERM(nTherm); 

   if ( p->FanControlType == FANS_CONTINUOUS )
   {
      if ( (fanOut == 0)||
           ( abs( fanOut - prevContFanOut[nTherm] ) >= (p->FanSpeedCOV*255)/100 ) )
      {
          _OBJV1(FanContinuousSpeed,nTherm) = fanOut;
          LUSetTransmitRequest(CO_INX_THERM(nTherm,CO_FanContinuousSpeed_1));
          prevContFanOut[nTherm] = fanOut;
      }
   }
   else
   {
      _OBJV1(FanContinuousSpeed,nTherm) =  ( fanOut*255 )/(p->FanControlType+1) ;
      LUSetTransmitRequest(CO_INX_THERM(nTherm,CO_FanContinuousSpeed_1));
      _OBJV1(FanStepSpeed,nTherm) = fanOut;
      LUSetTransmitRequest(CO_INX_THERM(nTherm,CO_FanStepSpeed_1));
      if ( fanOut != OFF ) 
      {
        fanOut = 1<<(fanOut-1);
      }
      for ( BYTE cnt = 0; cnt <= p->FanControlType; cnt++ , mask <<= 1) 
      {
        if ( fanOut & mask ) 
        {
            _OBJV2(FanSpeed,nTherm,cnt) = ON;
            LUSetTransmitRequest(CO_INX_THERM(nTherm,CO_FanSpeed_1_1+cnt));
        }
        if ( fanOut == 0 )
        {
            _OBJV2(FanSpeed,nTherm,cnt) = OFF;
            SetTransmitRequest(CO_INX_THERM(nTherm,CO_FanSpeed_1_1+cnt));
        }
      }     
   }

   return fanOut; 
}

//-----------------------------------------------------------------------------
// FanCoilOut
//-----------------------------------------------------------------------------   
BYTE FanCoilOut(BYTE fanOut,BYTE nTherm)
{
  long sp,hy;
  BYTE appFanSpeed;
  struct THERM *p = (struct THERM*)&_PARTHERM(nTherm); 
  TYPE_TR_DATA *t = &_therm(nTherm);

  if ( t->fanSpeed == FAN_SPEED_AUTO )
  { 
     sp = 0;
     if (( t->heatingCoolingMode == HEATING )&&( _fancoilFunctions(nTherm) & FUNCT_FANCOIL_AUXHEATING ))
        sp = GetOffsetFromSetpoint(p->AuxHeatingOffset)*10;  
     if (( t->heatingCoolingMode == COOLING )&&( _fancoilFunctions(nTherm) & FUNCT_FANCOIL_AUXCOOLING ))
        sp = GetOffsetFromSetpoint(p->AuxCoolingOffset)*10;    
    
     return(AutoFansSpeed(fanOut,sp,nTherm));
  }
  else
  {
      appFanSpeed = ManualFansSpeed(p,t->fanSpeed);
      if ( p->FanManualOperation == FANS_TEMP_DEPENDING )
      {
          sp = _V1(actualSetpoint,nTherm);
          if ( t->heatingCoolingMode == HEATING )
          {
              if ( _fancoilFunctions(nTherm) & FUNCT_FANCOIL_AUXHEATING )
              {
                  sp = sp - GetOffsetFromSetpoint(p->AuxHeatingOffset)*10;
                  hy = GetHysteresis(p->AuxHeatingHysteresisRange)*10;
              }
              else
              {
                  hy = GetHysteresis(p->HeatingHysteresisRange)*10;
              }
              if ( _V1(temperature,nTherm) < (sp-hy) )
                  return (appFanSpeed);
              if ( _V1(temperature,nTherm) >= sp )
                  return (OFF);
              return (fanOut);
          }
          else
          {
              if ( _fancoilFunctions(nTherm) & FUNCT_FANCOIL_AUXCOOLING )
              {
                  sp = sp - GetOffsetFromSetpoint(p->AuxCoolingOffset)*10;
                  hy = GetHysteresis(p->AuxCoolingHysteresisRange)*10;
              }
              else
              {
                  hy = GetHysteresis(p->CoolingHysteresisRange)*10;
              }
              if ( _V1(temperature,nTherm) >= (sp+hy) )
                  return (appFanSpeed);
              if ( _V1(temperature,nTherm) <= sp )
                  return (OFF);
              return (fanOut);
          }
      }
      else
      {
          return (appFanSpeed);
      }
  }
  
  //return 0;
}

//-----------------------------------------------------------------------------
// AutoFansSpeed
//----------------------------------------------------------------------------- 
BYTE AutoFansSpeed(BYTE fansSpeed,long sp,BYTE nTherm) {
  long pb;
  BYTE out = fansSpeed;
  long app;
  BYTE nrOfSpeed;
  struct THERM *p = (struct THERM*)&_PARTHERM(nTherm); 
  TYPE_TR_DATA *t = &_therm(nTherm);

  if (t->heatingCoolingMode == HEATING)
    pb = _V1(actualSetpoint,nTherm) - _V1(temperature,nTherm) - sp;
  else
  {
    pb = _V1(temperature,nTherm) - _V1(actualSetpoint,nTherm) - sp;
  }
  
  if ( p->FanControlType == FANS_CONTINUOUS )
  {
     if ( pb < 0) pb = 0;
     app = (255 *  pb) / (p->FanSpeedProportionalBand * 10); /* FansProportionalBand x0,1°C */
     if ( app > 255 ) out = 255;
     else out = app;
  }
  else
  {
    nrOfSpeed = p->FanControlType + 1;  
    BYTE zona = 0;
#ifdef FAN_HYSTERESIS   
    WORD hyst = GetHysteresis(p->FanSpeedControlHysteresis)*10;
    BYTE vS,vD;
    BYTE *threshold = (BYTE*)&p->FanFirstThreshold;
    if ( pb > 0 ) // valore sotto/sopra il setpoint impostato !
    {
      out = fansSpeed;
      vD = 0;
      vS = nrOfSpeed;
      for ( zona = 0; zona < nrOfSpeed; zona++ ) {
        if ( pb >= threshold[zona]*10 + hyst )
            vD++;
#ifdef SOGLIA1_OFF_A_SETPOINT       
        /* vel 1 non ha isteresi ..... stacca a Sepoint raggiunto */
        if (( pb <= threshold[zona]*10 )&&(zona !=0))
            vS--;
#else
        if ( pb <= threshold[zona]*10 )
            vS--;
#endif        
      }
      if ( fansSpeed < vD ) 
        out = vD;
      if ( fansSpeed > vS )
        out = vS;
      if ( vS == vD ) 
      {
        out = vS;
      }
    }
    else
    {
      out = 0;
    }
  }
#else    
    if ( pb > 0 ) // valore sotto/sopra il setpoint impostato !
    {
      for ( zona = 1; zona < nrOfSpeed+1; zona++ ) {
        if ( pb < p->FansThreshold[zona-1]*10 )
          break;
      }
    }
    if ( zona <= t->fansSpeed )
    { // up
      out = zona;
      if  ( out > nrOfSpeed )
        out = nrOfSpeed;
    }
    else
    { // down
      if (  zona > 0 )
        out = zona - 1;
      else
        out = zona;
    }
  }
#endif  
  return out;
}
//-----------------------------------------------------------------------------
// ManualFansSpeed
//-----------------------------------------------------------------------------
BYTE ManualFansSpeed(struct THERM *therm, BYTE fansSpeed) {

    if ( therm->FanControlType == FANS_CONTINUOUS )
      return ((fansSpeed * 255) / 5);
    else
      return ( fansSpeed );
}

//-----------------------------------------------------------------------------
// AntistratificationSensorRead
//
// 0 -> non attiva , nessuna formazione di condensa
// 1 -> attiva , possibile formazione di condensa
//
//-----------------------------------------------------------------------------                                                   
long AntistratificationSensorRead(BYTE nTherm)
{
#ifdef EXT_IN_PRESENT
  /* questo va bene ma non è ottimizzato ! */
  for ( BYTE inx = 0; inx < EXTIN_MAX_INPUTS; inx++)
  {
      if ( EXT_INPUT_TYPE(inx) == EXTIN_ANTISTRATIFICATION )
       return (ExtInput[inx].ai);
  }
#endif
  if (( !__IsSensorFromBusTimeout(nTherm,FROMBUS_ANTISTRAT_TEMP) )&&
      (  __IsSensorFromBusUpdated(nTherm,FROMBUS_ANTISTRAT_TEMP) ))
  {
      return ( ShortF2LongI(_OBJV1(AntistratificationTempFB,nTherm)) );  
  }
  
  return SENSOR_ERROR; /* errore ...  */
  
}

//-----------------------------------------------------------------------------
// AntistratificationFunction
//-----------------------------------------------------------------------------                                                   
BYTE AntistratificationFunction(BYTE fanOut,BYTE nTherm)
{
  long temp;
  struct THERM *p = (struct THERM*)&_PARTHERM(nTherm); 
  TYPE_TR_DATA *t = &_therm(nTherm);
  
  if (!(_fancoilFunctions(nTherm) & FUNCT_ANTISTRAT)||(fanOut != 0)||(t->heatingCoolingMode != HEATING)||
       (t->fanSpeed == FAN_SPEED_OFF ))
  {
      FANCOIL_RST_ANTISTRAT(nTherm);
      return fanOut;
  }
  
  if ( (temp = AntistratificationSensorRead(nTherm)) != SENSOR_ERROR )
  {
      temp = temp - _V1(temperature,nTherm);
      if ( temp >= GetTemperatureDifferential(p->FanAntistratificationDifferential) )
      {
          FANCOIL_SET_ANTISTRAT(nTherm);
      }
      if ( temp + GetHysteresis(p->AntistratificationHysteresis)*10 <= 
           GetTemperatureDifferential(p->FanAntistratificationDifferential) )  
      {
          FANCOIL_RST_ANTISTRAT(nTherm);
      }
  }
  else
  {
      FANCOIL_RST_ANTISTRAT(nTherm);
  }
  
  if ( FANCOIL_CHK_ANTISTRAT(nTherm) )
      if ( p->FanControlType == FANS_CONTINUOUS )
          return (255/5);
      else  
          return (1);
  else
      return (0);
}

//-----------------------------------------------------------------------------
// CoilBatteryTemperatureRead
//-----------------------------------------------------------------------------                                                   
long CoilBatteryTemperatureRead(BYTE nTherm)
{
#ifdef EXT_IN_PRESENT  
  /* questo va bene ma non è ottimizzato ! */
  for ( BYTE inx = 0; inx < EXTIN_MAX_INPUTS; inx++)
  {
      if ( EXT_INPUT_TYPE(inx) == EXTIN_COIL_BATTERY )
          return (ExtInput[inx].ai);
  }
#endif
  if   ( (!__IsSensorFromBusTimeout(nTherm,FROMBUS_COIL_TEMP)) &&
         ( __IsSensorFromBusUpdated(nTherm,FROMBUS_COIL_TEMP)) ) 
  {
      return ( ShortF2LongI(_OBJV1(CoilTempFB,nTherm)) );  
  }

  return SENSOR_ERROR; 

}
//-----------------------------------------------------------------------------
// FanDelay
//-----------------------------------------------------------------------------      
BYTE FanDelay(BYTE fanOut,BYTE nTherm)
{
  WORD app;
  struct THERM *p = (struct THERM*)&_PARTHERM(nTherm); 
  TYPE_VENTILATION *f = &_fc(nTherm);
  TYPE_TR_DATA *t = &_therm(nTherm);
  
  /* Ritardo spegnimento ventilatore */
  if ((fanOut != 0)&&(f->actualFanOut == 0)&&( t->fanSpeed == FAN_SPEED_AUTO )&&
      (!FANCOIL_CHK_ANTISTRAT(nTherm)) )
  {
      app = GetVentilationDelay(p->FanStartDelay);
      if (( app )&& !EZ_GetState(TimerFanOnDelay+nTherm))
      {
        f->fanOnDelay = 1;
        fanOut = 0;
        EZ_StartNotRunningTimer( TimerFanOnDelay+nTherm, app, TM_MODE_1S );
      }
      else
      {
        f->fanOnDelay = 0;
      }
  }
  else
  {
      EZ_StopTimer( TimerFanOnDelay+nTherm );
  }
  /* Ritardo accensione ventilatore */
  if ((fanOut == 0)&&(f->actualFanOut != 0)&&( t->fanSpeed == FAN_SPEED_AUTO )&&
      (!FANCOIL_CHK_ANTISTRAT(nTherm)) )
  {
      app = GetVentilationDelay(p->FanStopDelay);
      if ( ( app )&& !EZ_GetState(TimerFanOffDelay+nTherm) )
      {
        f->fanOffDelay = 1;
        fanOut = f->actualFanOut;
        EZ_StartNotRunningTimer( TimerFanOffDelay+nTherm, app, TM_MODE_1S );
      }
      else
      {
        f->fanOffDelay = 0;
      }
  }
  else
  {
      EZ_StopTimer( TimerFanOffDelay+nTherm );
  }  
  
  return fanOut;
}
//-----------------------------------------------------------------------------
// FansHotStart
//-----------------------------------------------------------------------------      
BYTE FanHotStart(BYTE fancoilOut,BYTE nTherm)
{
  struct THERM *p = (struct THERM*)&_PARTHERM(nTherm); 
  TYPE_VENTILATION *f = &_fc(nTherm);
  
  if ( _fancoilFunctions(nTherm) & FUNCT_COIL_BATTERY )
  {
      f->coilBatteryTemperature = CoilBatteryTemperatureRead(nTherm);
  }
  else
  {
      f->hotStartFunctionActivated = NO;
      goto EXIT_HOTSTART;
  }
  
  if ( (p->FanHotStart == NO )||(_therm(nTherm).heatingCoolingMode == COOLING)||
       (f->coilBatteryTemperature == SENSOR_ERROR) )
  {
      f->hotStartFunctionActivated = NO;
  }
  else
  {
      if ( f->coilBatteryTemperature/10 > p->FanMinTemperatureToStart*10 )
         f->hotStartFunctionActivated = NO;
      if ( f->coilBatteryTemperature/10 < (p->FanMinTemperatureToStart*10 - 50) )
         f->hotStartFunctionActivated = YES;
  }
EXIT_HOTSTART:  
  if (f->hotStartFunctionActivated == YES ) fancoilOut = 0;
    
  return fancoilOut;
}

//-----------------------------------------------------------------------------
// FancoilInit
//-----------------------------------------------------------------------------   
void FancoilInit( void )
{
  WORD app;
  struct THERM *p; 
  TYPE_VENTILATION *f;

  for ( BYTE nTherm = 0; nTherm < MAX_THERM; nTherm++ )
  {  
      p = (struct THERM*)&_PARTHERM(nTherm);
      f = &_fc(nTherm);
      
      if ( !(_V1(thermFunctions,nTherm) & FUNCT_FANCOIL) ) {
        f->fanControlDisabled = YES;
        f->fanManualOperation = NO;
        f->fan_allowed = NO;
        continue;
      }
      if ( p->FanDisableVentilationFromBus == NO )
          _therm(nTherm).fansDisabled = NO;
      
      f->fanCoilFirstRun = NO;
      f->fanControlDisabled = _therm(nTherm).fansDisabled;
      f->fanManualOperation = NO;
      if ( (app = GetCyclicSendingInterval(p->FanControlRT)) != 0)
          EZ_StartTimer( TimerFanOutRT+nTherm, app, TM_MODE_1S );
      
      prevContFanOut[nTherm] = 0;
      FANCOIL_SET_FIRSTRUN(nTherm); // trasmette se fancoil manuale/automatico a bus on      
  }
  
  
  
}

//-----------------------------------------------------------------------------
// FanChkCOSpeedIn
//-----------------------------------------------------------------------------   
BYTE FanChkCOSpeedIn(BYTE fanContinuousSpeed,BYTE nTherm)
{
  BYTE nrOfSpeed;
  
  nrOfSpeed = FanGetNrOfManualSpeed(_PARTHERM(nTherm).FanControlType);  
  
  if ( fanContinuousSpeed > nrOfSpeed )
    fanContinuousSpeed = nrOfSpeed;
  
  return fanContinuousSpeed;
  
}


//-----------------------------------------------------------------------------
// FancoilFunction
//-----------------------------------------------------------------------------   
void FancoilFunction( BYTE stopAll, BYTE nTherm )
{
  BYTE fancoilOut = 0;
  BYTE fanEnabled = NO;
  BYTE amChange = NO;
  BYTE manFanSpeedChange = NO;  
  struct THERM *p = (struct THERM*)&_PARTHERM(nTherm); 
  TYPE_VENTILATION *f = &_fc(nTherm);
  TYPE_TR_DATA *t = &_therm(nTherm);
  
/*
  if ( CheckUpdateFlag(CO_INX_THERM(nTherm,CO_FanManualSpeedPercentage_1)) )
  {
      _OBJV1(FanManualSpeedPercentage,nTherm) = FanChkCOSpeedIn(_OBJV1(FanManualSpeedPercentage,nTherm),nTherm);
      if ( t->fanSpeed != _OBJV1(FanManualSpeedPercentage,nTherm) )
      {
          t->fanSpeed = _OBJV1(FanManualSpeedPercentage,nTherm);
      }
  }
*/
  if ( CheckUpdateFlag(CO_INX_THERM(nTherm,CO_FanStepManualSpeed_1)) )
  {
      _OBJV1(FanStepManualSpeed,nTherm) = FanChkCOSpeedIn(_OBJV1(FanStepManualSpeed,nTherm),nTherm);
      if ( t->fanSpeed != _OBJV1(FanStepManualSpeed,nTherm) )
      {
          t->fanSpeed = _OBJV1(FanStepManualSpeed,nTherm);
      }
  }

  if ( (t->fanSpeed != 6)&&(t->fanSpeed != t->manualFanSpeed) )
  {
      t->manualFanSpeed = t->fanSpeed;
      manFanSpeedChange = YES;
  }
  
  if ( CheckUpdateFlag(CO_INX_THERM(nTherm,CO_VentManualOperation_1)) )
  {
     if ( f->fanManualOperation != _OBJV1(FanManualOperation,nTherm) ) 
     {
        if ( _OBJV1(FanManualOperation,nTherm))
        {
            t->fanSpeed = _therm(nTherm).manualFanSpeed;
        }
        else
        {
            t->fanSpeed = FAN_SPEED_AUTO;
        }
     }
  }
  
  if ( !(_V1(thermFunctions,nTherm) & FUNCT_FANCOIL) ) return;

  if (( p->FanDisableVentilationFromBus )&&( CheckUpdateFlag(CO_INX_THERM(nTherm,CO_FanControlDisable_1)) ))
  {
        if ( ContactFromBus(_OBJV1(FanControlDisable,nTherm) ,p->FanDisableVentilationFromBusInverted) )
        {
            f->fanControlDisabled = 1;
        }
        else
        {
            f->fanControlDisabled = 0;
        }
  }
  
  if (f->fanControlDisabled == NO)
  {
        /* heating or auxheating ? */
      if ( t->heatingCoolingMode == HEATING )
      {
           if (( _fancoilFunctions(nTherm) & FUNCT_FANCOIL_HEATING )||
               ( _fancoilFunctions(nTherm) & FUNCT_FANCOIL_AUXHEATING ))
           {
              fanEnabled = YES;
           }
      }
      /* cooling or auxcooling ? */
      if ( t->heatingCoolingMode == COOLING )
      {
           if (( _fancoilFunctions(nTherm) & FUNCT_FANCOIL_COOLING )||
               ( _fancoilFunctions(nTherm) & FUNCT_FANCOIL_AUXCOOLING ))
           {
               fanEnabled = YES;
           }
      }
  }
  
  
  if ( (f->fanControlDisabled == NO)&&(_V1(temperature,nTherm) != SENSOR_ERROR)&&(_V1(thermLock,nTherm) == NO)&&
       (stopAll == NO) )
  {
      /* heating or auxheating ? */
      if ( t->heatingCoolingMode == HEATING )
      {
           if ( _fancoilFunctions(nTherm) & FUNCT_FANCOIL_HEATING )
           {
             fancoilOut = FanCoilOut(f->actualFanOut,nTherm);
             fancoilOut = FanHotStart(fancoilOut,nTherm);
           }
           if ( _fancoilFunctions(nTherm) & FUNCT_FANCOIL_AUXHEATING )
           {
             fancoilOut = FanCoilOut(f->actualFanOut,nTherm);
             fancoilOut = FanHotStart(fancoilOut,nTherm);
           }     
      }
      /* cooling or auxcooling ? */
      if ( t->heatingCoolingMode == COOLING )
      {
           if ( _fancoilFunctions(nTherm) & FUNCT_FANCOIL_COOLING )
           {
               fancoilOut = FanCoilOut(f->actualFanOut,nTherm); 
           }
           if ( _fancoilFunctions(nTherm) & FUNCT_FANCOIL_AUXCOOLING )
           {
               fancoilOut = FanCoilOut(f->actualFanOut,nTherm);      
           }     
      }
      
      fancoilOut = FanDelay(fancoilOut,nTherm);
      fancoilOut = AntistratificationFunction(fancoilOut,nTherm);  
  }
  else
  {
      fancoilOut = OFF;
  }
  
  if ( (fancoilOut != f->actualFanOut )||(f->fanCoilFirstRun == NO) )
  {
      f->fanCoilFirstRun = YES;
      if ( p->FanControlType == FANS_CONTINUOUS )
      {
          EZ_StartTimer( TimerFanSpeedChange+nTherm,10, TM_MODE_1MS );
      }
      else
      {
          OutFanSpeed(OFF,nTherm);
          if ( fancoilOut != OFF )
              EZ_StartTimer( TimerFanSpeedChange+nTherm,1000, TM_MODE_1MS );
      }
      f->actualFanOut = fancoilOut; 
  }
  if ( EZ_GetState(TimerFanSpeedChange+nTherm) )
  {
       OutFanSpeed(f->actualFanOut,nTherm);
  }
  
  f->fanCoilEnabled = fanEnabled;
  t->fansDisabled = f->fanControlDisabled;
  
  f->fan_allowed = ( f->fanControlDisabled ? 0 : f->fanCoilEnabled );
  
  if ( f->fanManualOperation != (t->fanSpeed != FAN_SPEED_AUTO ) )
  {
      amChange = YES;
  }  
  
  if ( f->fan_allowed == YES )    
  {
      if ( (f->prevFan_allowed != f->fan_allowed)||(FANCOIL_CHK_FIRSTRUN(nTherm))||(amChange == YES)||
           (manFanSpeedChange == YES) )
      {
          f->fanManualOperation = (t->fanSpeed != FAN_SPEED_AUTO);  
          if ( f->fanManualOperation == TRUE )
          {
              _therm(nTherm).fanSpeed = t->manualFanSpeed;
              _OBJV1(FanManualOperation,nTherm) = t->manualFanSpeed;
              _OBJV1(FanManualSpeedPercentage,nTherm) = FanManualSpeedPercentage(p,t->manualFanSpeed);
              _OBJV1(FanManualOffStatus,nTherm) = (t->manualFanSpeed == 0 ) ? MAN_SPEEDOFF_ACTIVE:MAN_SPEEDOFF_NOT_ACTIVE;
              LUSetTransmitRequest(CO_INX_THERM(nTherm,CO_FanManualOffStatus_1));
              LUSetTransmitRequest(CO_INX_THERM(nTherm,CO_FanManualStepSpeed_1));
              LUSetTransmitRequest(CO_INX_THERM(nTherm,CO_FanManualSpeedPercentage_1));
          }
          else
          {
              _OBJV1(FanManualOffStatus,nTherm) = MAN_SPEEDOFF_NOT_ACTIVE;
              LUSetTransmitRequest(CO_INX_THERM(nTherm,CO_FanManualOffStatus_1));
          }
          _OBJV1(FanManualOperation,nTherm) = f->fanManualOperation;
          LUSetTransmitRequest(CO_INX_THERM(nTherm,CO_VentManualOperation_1));
      }
  }
  else
  {
      if ( (f->prevFan_allowed != f->fan_allowed)||(FANCOIL_CHK_FIRSTRUN(nTherm)) )
      {
          _OBJV1(FanManualOffStatus,nTherm) = MAN_SPEEDOFF_ACTIVE;
          LUSetTransmitRequest(CO_INX_THERM(nTherm,CO_FanManualOffStatus_1));
          _OBJV1(FanManualOperation,nTherm) = NO;
          LUSetTransmitRequest(CO_INX_THERM(nTherm,CO_VentManualOperation_1));
      }
  }
  
  
  f->prevFan_allowed = f->fan_allowed;  
  
  if ( p->FanControlType == FANS_CONTINUOUS )
  {
      f->visFanLevel = (f->actualFanOut*5)/255;
      if (( f->visFanLevel == 0)&&(f->actualFanOut > 0))
          f->visFanLevel = 1;
  }
  else  
  {
      f->visFanLevel = f->actualFanOut;
  }
  
  /* RT */
  if ( EZ_GetState(TimerFanOutRT+nTherm) ) 
  {
      LUSetTransmitRequest(CO_INX_THERM(nTherm,CO_FanSpeed_1_1));
      LUSetTransmitRequest(CO_INX_THERM(nTherm,CO_FanSpeed_1_2));
      LUSetTransmitRequest(CO_INX_THERM(nTherm,CO_FanSpeed_1_3));
      LUSetTransmitRequest(CO_INX_THERM(nTherm,CO_FanContinuousSpeed_1));
      LUSetTransmitRequest(CO_INX_THERM(nTherm,CO_VentManualOperation_1));
      EZ_StartTimer( TimerFanOutRT+nTherm, 
                     GetCyclicSendingInterval(p->FanControlRT), 
                     TM_MODE_1S );
  }

  FANCOIL_RST_FIRSTRUN(nTherm); 
  
}


                                                   
