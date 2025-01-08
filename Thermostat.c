 /**@addtogroup Termostato
@{
commento.....
*/
//------------------------------------------------------------------
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "project.h"
#include "eib_stk.h"
#include "app.h"
#include EIB_DEVICE_MODEL_APP_H_FILE     
#include "ApplicationTimers.h"
#include "ApplicationUtility.h"
#include "ManualFunction.h"
#include "ScenesHVAC.h"
#include "Anticondensation.h"
#include "WindowsContact.h"
#include "CardHolder.h"
#include "ListValues.h"
#include "ValveProtection.h"
#include "FloorTempLimitation.h"
#include "Setpoints.h"
#include "Fancoil.h"
#include "PresenceSensor.h"
#include "Thermostat.h"

#define PI_STOP         0
#define PI_RUN          1
#define PERCENT         255

static long _param;

extern unsigned int Time1MS;

unsigned char thermFirstRun;
unsigned char firstHCStatusOut;

long          _V1(prevActualSetpoint,MAX_THERM);
BYTE          _V1(prevHvacMode,MAX_THERM);
BYTE          _V1(prevHvacAuto,MAX_THERM);
long          _V1(temperature,MAX_THERM);
long          _V1(actualSetpoint,MAX_THERM);
BYTE          _V1(heatingOut,MAX_THERM);                  
BYTE          _V1(auxiliaryHeatingOut,MAX_THERM);
BYTE          _V1(coolingOut,MAX_THERM);
BYTE          _V1(auxiliaryCoolingOut,MAX_THERM);
BYTE          _V1(heatingCoolingChanged,MAX_THERM);
BYTE          _V1(thermLock,MAX_THERM);
WORD          _V1(thermFunctions,MAX_THERM);
BYTE          _V1(statoRegolazioneCooling,MAX_THERM);
BYTE          _V1(statoRegolazioneHeating,MAX_THERM);
BYTE          _V1(coolingControlType,MAX_THERM);

BYTE          _V2(outPWM,MAX_THERM,2);
WORD          _V2(cyclePWM,MAX_THERM,2); 
WORD          _V2(appValuePWM,MAX_THERM,2); 
float         _V2(I,MAX_THERM,2); 
long          _V1(updatePWMOut,MAX_THERM);
BYTE          _V1(spChanged,MAX_THERM);

/*********************************************************************//**
 * @brief	
 *
 * @param[in]	
 * @return 	
 **********************************************************************/
void SetThermFunctions(void)
{
  struct THERM *p; 
   
  for ( BYTE nTherm = 0; nTherm < MAX_THERM; nTherm++ )
  {
      _V1(thermFunctions,nTherm) = 0;
#ifdef EXTERNAL_SENSOR        
      if ( (PARAMETER.data[nTherm/2].AorB[nTherm&1].Input_AoB == INPUT_NTC)&&
           (PARAMETER.data[nTherm/2].AorB[nTherm&1].ThermostatEnabled == ENABLED) ) 
#else
      if  (PARAMETER.TempEnableSensor == YES)        
#endif        
      {
          _V1(thermFunctions,nTherm) |= FUNCT_THERMOSTAT;
          p = (struct THERM*)&_PARTHERM(nTherm);

          if ( (p->ThermostatFunction == HEATING_COOLING_FUNCTION )||
               (p->ThermostatFunction == HEATING_FUNCTION ))
          {
              _V1(thermFunctions,nTherm) |= FUNCT_THERM_HEATING;
          }

          if ( (p->ThermostatFunction == HEATING_COOLING_FUNCTION )||
               (p->ThermostatFunction == COOLING_FUNCTION ))
          {
              _V1(thermFunctions,nTherm) |= FUNCT_THERM_COOLING;
          }
          
          if (( _V1(thermFunctions,nTherm) & FUNCT_THERM_HEATING )&&(p->AuxiliaryHeating == ENABLED ))
          {
              _V1(thermFunctions,nTherm) |= FUNCT_THERM_AUXHEATING;
          }     
          if (( _V1(thermFunctions,nTherm) & FUNCT_THERM_COOLING )&&(p->AuxiliaryCooling == ENABLED ))
          {
              _V1(thermFunctions,nTherm) |= FUNCT_THERM_AUXCOOLING;
          }          
      }
  }
}

/*********************************************************************//**
 * @brief	
 *
 * @param[in]	
 * @return 	
 **********************************************************************/
BYTE PIController(char stato, long Setpoint, BYTE heatingCooling, BYTE nTherm)
{
  float error;
  float appo;
  BYTE integrTime;
  float pBand;
  float P = 0;
  float PI;
  unsigned char updateI = 0;
  struct THERM *p = (struct THERM*)&_PARTHERM(nTherm);  
  
  if ( stato == PI_STOP ) {
    _V2(I,nTherm,heatingCooling) = 0;
    return (0);
  }
  
#ifdef TEST_PWM_PID    
  static unsigned long _update[MAX_THERM];
  if  ( (unsigned long)(TM_counter1ms - _update[nTherm]) > 100 ) {
    _update[nTherm] = TM_counter1ms;
    updateI = 1;
  }
#else  
  static long _update[MAX_THERM];
  if  ( (unsigned long)(TM_counter1ms - _update[nTherm]) > 1000 ) {
    _update[nTherm] = TM_counter1ms;
    updateI = 1;
  }  
#endif  
  
  if ( heatingCooling == HEATING )
  {
      pBand = (p->HeatingProportionalBand / 10.0F);
      integrTime = p->HeatingIntegralTime;
      error = (Setpoint - _V1(temperature,nTherm))/100.0F;
  }
  else
  {
      pBand = (p->CoolingProportionalBand / 10.0F);
      integrTime = p->CoolingIntegralTime;
      error = (_V1(temperature,nTherm) - Setpoint)/100.0F;
  }

  if ( updateI ) 
  {
    if ((fabs(error) <= pBand )&&( integrTime != 0 ))
    {
        appo = pBand * integrTime;
        _V2(I,nTherm,heatingCooling) += (( error / appo)/ 60.0F);
    }
    else
    {
      _V2(I,nTherm,heatingCooling) = 0;
    }
  }
  P = ( error) / pBand;

  if ( _V2(I,nTherm,heatingCooling) > 1 ) 
    _V2(I,nTherm,heatingCooling) = 1;
  if ( _V2(I,nTherm,heatingCooling) < -1) 
    _V2(I,nTherm,heatingCooling) = -1;

  PI = ( P + _V2(I,nTherm,heatingCooling) ) * PERCENT ;

  if ( PI > PERCENT )  
  {
      PI = PERCENT;
  } else  if ( PI < 0 ) 
  {       
      PI = 0;
      _V2(I,nTherm,heatingCooling) = 0;
  }
  
  return ( (BYTE)PI );
    
  
}
/*********************************************************************//**
 * @brief	
 *
 * @param[in]	
 * @return 	
 **********************************************************************/
void PWMStopController(BYTE heatingCooling, BYTE nTherm)
{
  _V2(outPWM,nTherm,heatingCooling) = 0;  
  _V2(outPWM,nTherm,heatingCooling) = 0;
  _V2(appValuePWM,nTherm,heatingCooling) = 0;
  _V2(cyclePWM,nTherm,heatingCooling) = 0;
}
/*********************************************************************//**
 * @brief	
 *
 * @param[in]	
 * @return 	
 **********************************************************************/
BYTE PWMController(BYTE value, BYTE heatingCooling, BYTE nTherm)
{
  
  if (( _V1(spChanged,nTherm) ) || (_V1(heatingCoolingChanged,nTherm)) )
  {
    PWMStopController(heatingCooling,nTherm);
    _V1(spChanged,nTherm) = NO;
  }
  
#ifdef TEST_PWM_PID  
  if ((TM_counter1ms - updatePWMOut[nTherm]) < 16 )
#else    
  if ((unsigned long)(TM_counter1ms - _V1(updatePWMOut,nTherm)) < 1000 )
#endif    
    return _V2(outPWM,nTherm,heatingCooling);  
  
  _V1(updatePWMOut,nTherm) = TM_counter1ms;
  
  if ( value == 0 ) 
  {
      _V2(outPWM,nTherm,heatingCooling) = 0;  
  }
  else
  {
      if ( _V2(cyclePWM,nTherm,heatingCooling) == 0 )
      {
        if( heatingCooling == HEATING )
          _V2(cyclePWM,nTherm,heatingCooling) = GetIntegerConst(&_PARTHERM(nTherm).HeatingPWMCycleTime) * 60;
        else
          _V2(cyclePWM,nTherm,heatingCooling) = GetIntegerConst(&_PARTHERM(nTherm).CoolingPWMCycleTime) * 60;    
        
        if ( value > 0 )
        {
          _V2(appValuePWM,nTherm,heatingCooling) = ((DWORD)_V2(cyclePWM,nTherm,heatingCooling) * value / 255);
          _V2(outPWM,nTherm,heatingCooling) = 1;
        }
      }
      else
      { 
        _V2(cyclePWM,nTherm,heatingCooling)--;
        if ( _V2(appValuePWM,nTherm,heatingCooling) > 0 ) 
        {
          _V2(appValuePWM,nTherm,heatingCooling)--;
        }
        else
        {
          _V2(outPWM,nTherm,heatingCooling) = 0;
        }
      }
  }
  
  return _V2(outPWM,nTherm,heatingCooling);
}

/*********************************************************************//**
 * @brief	Azzera gli oggetti di comunicazione utilizzati come comandi
 *              per il riscaldamento.
 * @param[in]	
 * @return 	
 **********************************************************************/
void StopHeating(BYTE nTherm) {
  
  _V1(heatingOut,nTherm) = 0;
  PWMStopController(HEATING, nTherm);
  PIController(PI_STOP, 0, HEATING, nTherm);
}
/*********************************************************************//**
 * @brief	Azzera gli oggetti di comunicazione utilizzati come comandi
 *              per il riscaldamento.
 * @param[in]	
 * @return 	
 **********************************************************************/
void StopAuxHeating(BYTE nTherm) {

   _V1(auxiliaryHeatingOut,nTherm) = 0;
}
/*********************************************************************//**
* @brief	Azzera gli oggetti di comunicazione utilizzati come comandi
 *              per il raffrescamento.
 * @param[in]	
 * @return 	
 **********************************************************************/
void StopCooling(BYTE nTherm) {
  
  _V1(coolingOut,nTherm) = 0;
  PWMStopController(COOLING, nTherm);
  PIController(PI_STOP, 0, COOLING, nTherm);
}
/*********************************************************************//**
* @brief	Azzera gli oggetti di comunicazione utilizzati come comandi
 *              per il raffrescamento.
 * @param[in]	
 * @return 	
 **********************************************************************/
void StopAuxCooling(BYTE nTherm) {
  
  _V1(auxiliaryCoolingOut,nTherm) = 0;
}
//------------------------------------------------------------------
// SendHVACMode
//------------------------------------------------------------------          
BYTE SendHVACMode(BYTE hvacModeUsed,BYTE prevHvacMode, BYTE nTherm)
{
    if ( prevHvacMode != hvacModeUsed )
    {
        _OBJV1(HVACModeOut,nTherm) = hvacModeUsed;
        SetTransmitRequest(CO_INX_THERM(nTherm,CO_HVACModeOut_1));
    }
    
    return hvacModeUsed;
}

/*********************************************************************//**
 * @brief	SwitchOver tra heating e cooling + RT 
 *
 * @param[in]	heatingCoolingModeStatus attuale.
 * @return 	heatingCoolingModeStatus modificato in base alla logica dello
 *              SwitchOver.
 **********************************************************************/
BYTE HeatingCoolingSwitchOver(BYTE hCoolingModeStatus, BYTE nTherm) {
    long spC;
    long spH;
    TYPE_TR_DATA *t = &_therm(nTherm);
    
    _V1(heatingCoolingChanged,nTherm) = 0;
    

    switch (_PARTHERM(nTherm).HeatingCoolingSwitchOver) {
    case HC_SWITCH_FROM_BUS:
        if (CheckUpdateFlag(CO_INX_THERM(nTherm,CO_HeatingCoolingStatusIn_1)))
        {
            if ( hCoolingModeStatus != _OBJV1(HeatingCoolingStatusIn,nTherm) )
            {
                _param = GetIntegerConst(&_PARTHERM(nTherm).DelayAfterModeChange)*10;
                if ( _param > 0) 
                    EZ_StartTimer(TimerDelayHCModeChange+nTherm,_param,TM_MODE_1MS);
                else
                    EZ_StartTimer(TimerDelayHCModeChange+nTherm,100,TM_MODE_1MS);
            }
            else
            {
                EZ_StopTimer(TimerDelayHCModeChange+nTherm);
            }
        }
        if ( EZ_GetState(TimerDelayHCModeChange+nTherm)  )
        {
            hCoolingModeStatus = _OBJV1(HeatingCoolingStatusIn,nTherm);
            _OBJV1(HeatingCoolingStatusOut,nTherm) = hCoolingModeStatus;
            _V1(heatingCoolingChanged,nTherm) = 1;
            SetTransmitRequest(CO_INX_THERM(nTherm,CO_HeatingCoolingStatusOut_1));
        }
        
        break;
        
    case HC_SWITCH_AUTOMATIC:
        if ( _V1(temperature,nTherm) == SENSOR_ERROR ) break;
        
        if ( _V1(mf,nTherm).manualActive == NO )
        {
            spH = (long)t->setpointComfort[HEATING];
            spC = (long)t->setpointComfort[COOLING];
        }
        else
        {
            spH = t->setpointManuale;               
            spC = t->setpointManuale + (t->setpointComfort[COOLING] -
                                        t->setpointComfort[HEATING]);              
        }
        if (( _V1(temperature,nTherm) <= spH )&&( hCoolingModeStatus != HEATING ))
        {
            hCoolingModeStatus = HEATING;
            _OBJV1(HeatingCoolingStatusOut,nTherm) = hCoolingModeStatus;
            SetTransmitRequest(CO_INX_THERM(nTherm,CO_HeatingCoolingStatusOut_1));
            _V1(heatingCoolingChanged,nTherm) = 1;
        }
        else
        {
            if (( _V1(temperature,nTherm) >= spC )&&( hCoolingModeStatus != COOLING ))
            {
              hCoolingModeStatus = COOLING;
              _OBJV1(HeatingCoolingStatusOut,nTherm) = hCoolingModeStatus;
              SetTransmitRequest(CO_INX_THERM(nTherm,CO_HeatingCoolingStatusOut_1));
              _V1(heatingCoolingChanged,nTherm) = 1;
            }
        }
        break;
    }

    if (( _V1(heatingCoolingChanged,nTherm) != 0 )/*&&( t->setpointOffset == 0)*/)
    {
        //t->setpointManuale = 0;
    }

    if ( EZ_GetState(TimerHeatingCoolingRT+nTherm) || THERM_CHK_HCFIRSTRUN(nTherm) ) 
    {
        _OBJV1(HeatingCoolingStatusOut,nTherm) = hCoolingModeStatus;
        SetTransmitRequest(CO_INX_THERM(nTherm,CO_HeatingCoolingStatusOut_1));
        _param = GetCyclicSendingInterval(_PARTHERM(nTherm).HeatingCoolingRT);
        if ( _param != 0 )
            EZ_StartTimer( TimerHeatingCoolingRT+nTherm, _param, TM_MODE_1S );
        THERM_RST_HCFIRSTRUN(nTherm);
    }

    return (hCoolingModeStatus);
}

//------------------------------------------------------------------
// ActualSetpoint
//------------------------------------------------------------------                                          
DWORD ActualSetpoint(DWORD actual, BYTE modo, BYTE nTherm)
{
  DWORD setp;
  TYPE_TR_DATA *t = &_therm(nTherm);
  BYTE appHC = t->heatingCoolingMode;

  if (!IS_ENABLE_HC(nTherm)) //return actual;
  {
      if ( _PARTHERM(nTherm).ThermostatFunction == HEATING_FUNCTION )
          appHC = HEATING;
      else
          appHC = COOLING;
  }
  
  if ( ( _V1(mf,nTherm).manualActive == YES )&&(_V1(winc,nTherm).windowsProtectionActive == NO) )
  {
      setp = t->setpointManuale;
      if ( (_PARTHERM(nTherm).HeatingCoolingSwitchOver == HC_SWITCH_AUTOMATIC)&&
           (IS_HEATING_AND_COOLING_ENABLED(nTherm)) )
      {
          if ( appHC == COOLING )
          {
              setp += (t->setpointComfort[COOLING] - t->setpointComfort[HEATING]);
          }
      }        
  }
  else
  {
      long* ptrSetpoints = (long*)&t->setpointComfort;
      if ( _PARTHERM(nTherm).SetpointManagement == SETPOINT_RELATIVE )  
      {
          if (( modo > 1 ) && ( modo < 4 ))
              setp = t->setpointComfort[appHC];
          else
              setp = 0;
          modo = (modo-1)*2 +  appHC;
          setp += ptrSetpoints[modo];  
      }        
      else
      {
          modo = (modo-1)*2 + appHC;
          setp = ptrSetpoints[modo];      
      }
  }
 
  /* COV */  
  if ( actual != setp )
  {
      _OBJV1(ActualSetpoint,nTherm) = LongI2ShortF(setp);
      SetTransmitRequest(CO_INX_THERM(nTherm,CO_ActualSetpoint_1));
      _V1(spChanged,nTherm) = YES;
  }
  else
  {
      _V1(spChanged,nTherm) = NO;
  }
  
  return (setp);
}

//------------------------------------------------------------------
// HeatingHysteresis
//------------------------------------------------------------------
BYTE HeatingHysteresis(DWORD setPoint, BYTE range, BYTE out, BYTE nTherm) {
#ifdef SETPOINT_CENTRATO
  if ( _V1(temperature,nTherm) > (long)(setPoint + range*5) ) 
    out = 0;
  if ( _V1(temperature,nTherm) < (long)(setPoint - range*5) ) 
    out = 1;
#else
  if ( _V1(temperature,nTherm) >= (long)(setPoint) )
    out = 0;
  if ( _V1(temperature,nTherm) <= (long)(setPoint - range*10) ) 
    out = 1;
#endif  
  return out;
}
//------------------------------------------------------------------
// HeatingHysteresisReverse
//------------------------------------------------------------------
BYTE HeatingHysteresisReverse(DWORD setPoint, BYTE range, BYTE out, BYTE nTherm) {

  if ( _V1(temperature,nTherm) >= (long)(setPoint + range*10) )
    out = 0;
  if ( _V1(temperature,nTherm) <= (long)(setPoint) ) 
    out = 1;

  return out;
}
//------------------------------------------------------------------
// CoolingHysteresis
//------------------------------------------------------------------
BYTE CoolingHysteresis(DWORD setPoint, BYTE range, BYTE out, BYTE nTherm) {
#ifdef SETPOINT_CENTRATO
  if ( _V1(temperature,nTherm) > (long)(setPoint + range*5) ) 
    out = 1;
  if ( _V1(temperature,nTherm) < (long)(setPoint - range*5) ) 
    out = 0;
#else
  if ( _V1(temperature,nTherm) >= (long)(setPoint+ range*10) ) 
    out = 1;
  if ( _V1(temperature,nTherm) <= (long)(setPoint) ) 
    out = 0;
#endif  
  return out;
}
//------------------------------------------------------------------
// CoolingHysteresisReverse
//------------------------------------------------------------------
BYTE CoolingHysteresisReverse(DWORD setPoint, BYTE range, BYTE out, BYTE nTherm) {

  if ( _V1(temperature,nTherm) >= (long)(setPoint) ) 
    out = 1;
  if ( _V1(temperature,nTherm) <= (long)(setPoint - range*10) ) 
    out = 0;

  return out;
}
//------------------------------------------------------------------
// Heating
//------------------------------------------------------------------
void Heating(BYTE nTherm) {
  BYTE out;
  BYTE addOut;
  WORD controlV;
  BYTE outValue;
  struct THERM *p = (struct THERM*)&_PARTHERM(nTherm); 
  
 
  switch ( p->HeatingControlType )
  {
  case CONTINUOUS:
  case PWM:
      out = PIController(PI_RUN, _V1(actualSetpoint,nTherm), HEATING, nTherm);
      controlV = (p->HeatingMinControlValue*255)/100;
      if ( out < controlV )
      {
        out = 0;
      }
      else 
      {
        controlV = (p->HeatingMaxControlValue*255)/100;
        if ( out > controlV )
          out = controlV;
      }
      outValue = out;
      if ( p->HeatingControlType == PWM )
        out = PWMController(out,HEATING, nTherm);
      break;      
      
  case _2POINTS:
      if (p->HeatingHystPosition == HYST_BELOW)
        out = HeatingHysteresis(_V1(actualSetpoint,nTherm),GetHysteresis(p->HeatingHysteresisRange),_V1(heatingOut,nTherm),nTherm);
      else
        out = HeatingHysteresisReverse(_V1(actualSetpoint,nTherm),GetHysteresis(p->HeatingHysteresisRange),_V1(heatingOut,nTherm),nTherm);
      outValue = out;
      break;    
  }

  if ( _V1(heatingOut,nTherm) != out )
  {
      _V1(heatingOut,nTherm) = out;
  }      
  
  if ( outValue != 0) _V1(statoRegolazioneHeating,nTherm) = ON;
  else _V1(statoRegolazioneHeating,nTherm) = OFF;
  
  if ( p->AuxiliaryHeating == ENABLED )
  {
    if (( p->AuxHeatingDiabledFromKNX == ENABLED ) && ( _OBJV1(AuxiliaryHeatingDisable,nTherm) == 1))
    {
      addOut = 0;
    }
    else
    {
      addOut = HeatingHysteresis( _V1(actualSetpoint,nTherm) - GetOffsetFromSetpoint(p->AuxHeatingOffset)*10,
                                  GetHysteresis(p->AuxHeatingHysteresisRange),
                                  _V1(auxiliaryHeatingOut,nTherm),nTherm );
    }
  }  
  
  if ( _V1(auxiliaryHeatingOut,nTherm)  != addOut )
  {
      _V1(auxiliaryHeatingOut,nTherm) = addOut;
  }
}
//------------------------------------------------------------------
// AuxHeating
//------------------------------------------------------------------
void AuxHeating(BYTE nTherm) {
  BYTE addOut;
  struct THERM *p = (struct THERM*)&_PARTHERM(nTherm); 
  
  if ( p->AuxiliaryHeating == ENABLED )
  {
    if (( p->AuxHeatingDiabledFromKNX == ENABLED ) && ( _OBJV1(AuxiliaryHeatingDisable,nTherm) == 1))
    {
      addOut = 0;
    }
    else
    {
      addOut = HeatingHysteresis(_V1(actualSetpoint,nTherm) - GetOffsetFromSetpoint(p->AuxHeatingOffset)*10,
                                  GetHysteresis(p->AuxHeatingHysteresisRange),
                                  _V1(auxiliaryHeatingOut,nTherm),nTherm );
    }
  }  
  
  if ( _V1(auxiliaryHeatingOut,nTherm)  != addOut )
  {
      _V1(auxiliaryHeatingOut,nTherm) = addOut;
  }
}
//------------------------------------------------------------------
// Cooling
//------------------------------------------------------------------
void Cooling(BYTE nTherm) {
  BYTE out;
  BYTE addOut;
  WORD controlV;
  BYTE outValue;
  struct THERM *p = (struct THERM*)&_PARTHERM(nTherm); 
 
  switch (_V1(coolingControlType,nTherm) )
  {
  case CONTINUOUS:
  case PWM:
      out = PIController(PI_RUN, _V1(actualSetpoint,nTherm), COOLING, nTherm);
      controlV = (p->CoolingMinControlValue*255)/100;
      if ( out < controlV )
      {
        out = 0;
      }
      else 
      {
        controlV = (p->CoolingMaxControlValue*255)/100;
        if ( out > controlV )
          out = controlV;
      }
      outValue = out;
      if ( _V1(coolingControlType,nTherm) == PWM )
        out = PWMController(out,COOLING,nTherm);
      break;      
      
  case _2POINTS:
      if (p->CoolingHystPosition == HYST_ABOVE)
        out = CoolingHysteresis(_V1(actualSetpoint,nTherm),GetHysteresis(p->CoolingHysteresisRange),
                                _V1(coolingOut,nTherm),nTherm);
      else
        out = CoolingHysteresisReverse(_V1(actualSetpoint,nTherm),GetHysteresis(p->CoolingHysteresisRange),
                                       _V1(coolingOut,nTherm),nTherm);
      outValue = out;
      break;      
  }

  if ( _V1(coolingOut,nTherm) != out )
  {
    _V1(coolingOut,nTherm) = out;
  }     
  
  if ( outValue != 0) _V1(statoRegolazioneCooling,nTherm) = ON;
  else _V1(statoRegolazioneCooling,nTherm) = OFF;
 
  if ( p->AuxiliaryCooling == ENABLED )
  {
    if (( p->AuxCoolingDiabledFromKNX == ENABLED ) && ( _OBJV1(AuxiliaryCoolingDisable,nTherm) == 1))
    {
      addOut = 0;
    }
    else
    {
      addOut = CoolingHysteresis(_V1(actualSetpoint,nTherm) + GetOffsetFromSetpoint(p->AuxCoolingOffset)*10,
                                 GetHysteresis(p->AuxCoolingHysteresisRange),
                                 _V1(auxiliaryCoolingOut,nTherm),nTherm);
    }
  }  
  
  if ( _V1(auxiliaryCoolingOut,nTherm)  != addOut )
  {
      _V1(auxiliaryCoolingOut,nTherm) = addOut;
  }
}
//------------------------------------------------------------------
// AuxCooling
//------------------------------------------------------------------
void AuxCooling(BYTE nTherm) {
  BYTE addOut;
  struct THERM *p = (struct THERM*)&_PARTHERM(nTherm); 
  
  if ( p->AuxiliaryCooling == ENABLED )
  {
    if (( p->AuxCoolingDiabledFromKNX == ENABLED ) && ( _OBJV1(AuxiliaryCoolingDisable,nTherm) == 1))
    {
      addOut = 0;
    }
    else
    {
      addOut = CoolingHysteresis(_V1(actualSetpoint,nTherm) + GetOffsetFromSetpoint(p->AuxCoolingOffset)*10,
                                 GetHysteresis(p->AuxCoolingHysteresisRange),
                                 _V1(auxiliaryCoolingOut,nTherm),nTherm);
    }
  }  
  
  if ( _V1(auxiliaryCoolingOut,nTherm)  != addOut )
  {
      _V1(auxiliaryCoolingOut,nTherm) = addOut;
  }
}

//------------------------------------------------------------------
// CoolingSetpointMatched
//------------------------------------------------------------------
BYTE CoolingSetpointMatched(BYTE nTherm) {
  BYTE out=0;
  
  if (_therm(nTherm).heatingCoolingMode == COOLING) 
  {
      if ((_PARTHERM(nTherm).CoolingHystPosition == HYST_ABOVE)||(_V1(coolingControlType,nTherm) != _2POINTS))
          out = CoolingHysteresis(_V1(actualSetpoint,nTherm),GetHysteresis(_PARTHERM(nTherm).CoolingHysteresisRange),
                                  _V1(coolingOut,nTherm),nTherm);
      else
          out = CoolingHysteresisReverse(_V1(actualSetpoint,nTherm),GetHysteresis(_PARTHERM(nTherm).CoolingHysteresisRange),
                                         _V1(coolingOut,nTherm),nTherm);
  }
  
  return !out;
}  

//------------------------------------------------------------------
// ThermostatInit
//------------------------------------------------------------------
void ThermostatInit(void)
{
  WORD param;
  
  
  for ( BYTE nTherm = 0; nTherm < MAX_THERM; nTherm++ )
  {  
      _V1(prevHvacMode,nTherm) = 99;
      _V1(prevActualSetpoint,nTherm) = 0;
      
      if ( !(_V1(thermFunctions,nTherm) & FUNCT_THERMOSTAT) ) continue;
      
    
      param = GetCyclicSendingInterval(_PARTHERM(nTherm).HeatingCoolingRT);
      if ( param )
      {
          EZ_StartTimer( TimerHeatingCoolingRT+nTherm, param, TM_MODE_1S );
      }
                                       
      _OBJV1(AuxiliaryHeatingDisable,nTherm) = 1;
      _OBJV1(AuxiliaryCoolingDisable,nTherm) = 1;  
    
      if ( _PARTHERM(nTherm).AuxHeatingDiabledFromKNX == ENABLED )
      {
          _OBJV1(AuxiliaryHeatingDisable,nTherm) = _therm(nTherm).auxiliaryHeatingDisabled;
      }
      if ( _PARTHERM(nTherm).AuxCoolingDiabledFromKNX == ENABLED )
      {
          _OBJV1(AuxiliaryCoolingDisable,nTherm) = _therm(nTherm).auxiliaryCoolingDisabled;
      }  
      
      _OBJV1(HeatingCoolingStatusOut,nTherm) = _therm(nTherm).heatingCoolingMode & 0x01;
      THERM_SET_HCFIRSTRUN(nTherm);
      THERM_SET_FIRSTRUN(nTherm);
      
      _V1(statoRegolazioneCooling,nTherm) = OFF;
      _V1(statoRegolazioneHeating,nTherm) = OFF;
      
      if ( IS_ENABLE_HC(nTherm) )
          _OBJV1(RoomTempControllerStatus,nTherm) = !_therm(nTherm).DisableTempController;
      else  
          _OBJV1(RoomTempControllerStatus,nTherm) = DISABLED;
      
      _V1(prevHvacAuto,nTherm) = 99;
  }
}

//------------------------------------------------------------------
// Thermostat
//------------------------------------------------------------------

BYTE Thermostat(BYTE pHvacMode,BYTE nTherm)
{
  BYTE pHvacModeUsed;
  BYTE _cardholderOut;
  struct THERM *p = (struct THERM*)&_PARTHERM(nTherm); 
  TYPE_TR_DATA *t = &_therm(nTherm);
  BYTE tX = 0;
  
  if ( CheckUpdateFlag(CO_INX_THERM(nTherm,CO_ThermalGeneratorLock_1)) )
      _V1(thermLock,nTherm) = _OBJV1(ThermalGeneratorLock,nTherm);
  
  
  if ( p->DisableTempContrFB == YES )
  {
      if ( CheckUpdateFlag(CO_INX_THERM(nTherm,CO_DisableTempController_1)) )
      {
          t->DisableTempController = ContactFromBus(_OBJV1(DisableTempController,nTherm),
                                                    p->DisableTempContrSignal);
      }
  }
  
  if ( !IS_ENABLE_HC(nTherm) )
  {
      if  (_OBJV1(RoomTempControllerStatus,nTherm) != DISABLED)
      {
          _OBJV1(RoomTempControllerStatus,nTherm) = DISABLED;
          tX = 1;
      }
  }  
  else if (_OBJV1(RoomTempControllerStatus,nTherm) == t->DisableTempController)
  {
      _OBJV1(RoomTempControllerStatus,nTherm) = !t->DisableTempController;  
      tX = 1;
  }
  if ( THERM_CHK_FIRSTRUN(nTherm) ) 
  {
      tX = 1;
  }   
  if ( tX == 1 ) 
      LUSetTransmitRequest(CO_INX_THERM(nTherm,CO_RoomTempControllerStatus_1));

  if ( p->SetpointManagement != SETPOINT_SINGLE )
  {
      if ( CheckUpdateFlag(CO_INX_THERM(nTherm,CO_HVACModeIn_1)) )
      {
          if (( _OBJV1(HVACModeIn,nTherm) < 5 )&&( _OBJV1(HVACModeIn,nTherm) > 0) )
          {
              t->hvacModeFromBus = _OBJV1(HVACModeIn,nTherm);
          }
          else
          {
              _OBJV1(HVACModeIn,nTherm) = t->hvacModeFromBus;
          }
          if (( p->EndManualSetpoint == NO )&&(t->hvacAuto == TRUE))
              ManualExitFromFunction(nTherm);       
      }
      if ( CheckUpdateFlag(CO_INX_THERM(nTherm,CO_HVACManualMode_1)) )
      {
          if (( _OBJV1(HVACManualMode,nTherm) == HVAC_AUTO ))
          {
              t->hvacAuto = TRUE; 
          }
          else
          {
              if ( _OBJV1(HVACManualMode,nTherm) < 5 )
              {
                  t->hvacAuto = FALSE; 
                  t->hvacModeManual = _OBJV1(HVACManualMode,nTherm);
              }
          }
          ManualExitFromFunction(nTherm);  
      }
      if ( CheckUpdateFlag(CO_INX_THERM(nTherm,CO_ChronoModeStatus_1)) )
      {
          t->hvacAuto = (_OBJV1(ChronoModeStatus,nTherm) == 1);
      }  
      else
      {
          if ( t->hvacAuto != _V1(prevHvacAuto,nTherm) )
          {
              _V1(prevHvacAuto,nTherm) = t->hvacAuto;
              _OBJV1(ChronoModeStatus,nTherm) = (t->hvacAuto == 1);
              SetTransmitRequest(CO_INX_THERM(nTherm,CO_ChronoModeStatus_1));
          }
      }    
      pHvacMode = ( t->hvacAuto == TRUE ) ? t->hvacModeFromBus : t->hvacModeManual;
  }
  else
  {
      pHvacMode = HVAC_COMFORT; t->hvacAuto = TRUE;
  }
  
  if ( (_cardholderOut = CardHolder(pHvacMode,nTherm)) != pHvacMode )
  {
      pHvacMode = _cardholderOut;
      ManualExitFromFunction(nTherm);
      PresenceRestartTimer(nTherm);
  }  
  
  pHvacModeUsed = pHvacMode;
  pHvacModeUsed = ManualFunction(pHvacModeUsed,nTherm);
  pHvacModeUsed = PresenceStatus(pHvacModeUsed,nTherm); 
  
  SetpointsFromBus(nTherm);
  
  t->heatingCoolingMode = HeatingCoolingSwitchOver(t->heatingCoolingMode,nTherm);  

  if ( CheckUpdateFlag(CO_INX_THERM(nTherm,CO_HVACForcedModeIn_1)) )  
  {
      if ( (t->hvacModeForced != _OBJV1(HVACForcedModeIn,nTherm))&&(_OBJV1(HVACForcedModeIn,nTherm) <5) )
          t->hvacModeForced = _OBJV1(HVACForcedModeIn,nTherm);
      ManualExitFromFunction(nTherm);
      PresenceRestartTimer(nTherm);
  }

  if ( t->hvacModeForced != HVAC_AUTO ) 
      pHvacModeUsed = t->hvacModeForced;
  
  pHvacModeUsed = WindowsContactStatus(pHvacModeUsed,nTherm);  
  _V1(actualSetpoint,nTherm) = ActualSetpoint(_V1(actualSetpoint,nTherm),pHvacModeUsed,nTherm);
  
  Setpoints(nTherm);
  _V1(prevHvacMode,nTherm) = SendHVACMode(pHvacModeUsed,_V1(prevHvacMode,nTherm),nTherm);

  BYTE stopAll = NO;
  if ((_V1(temperature,nTherm) == SENSOR_ERROR )||(_V1(thermLock,nTherm) == YES )||
      (t->DisableTempController == YES)||
      (!IS_ENABLE_HC(nTherm)) )
  { 
      _V1(statoRegolazioneCooling,nTherm) = OFF;
      _V1(statoRegolazioneHeating,nTherm) = OFF;
      StopHeating(nTherm); StopAuxHeating(nTherm);
      StopCooling(nTherm); StopAuxCooling(nTherm);
      stopAll = YES;
  }
  
  FloorTemperatureLimitation(nTherm);

  PassiveAnticondensation(stopAll,CoolingSetpointMatched(nTherm),nTherm);
  ActiveAnticondensation(stopAll,CoolingSetpointMatched(nTherm),nTherm);
  ActiveAnticondensationNoProbe(stopAll,CoolingSetpointMatched(nTherm),nTherm);
  
  if ( stopAll == NO )      
  {    
      if (t->heatingCoolingMode == HEATING) 
      {
          _V1(statoRegolazioneCooling,nTherm) = OFF; 
          StopCooling(nTherm); StopAuxCooling(nTherm);
          if (_V1(ftc,nTherm).heatingTempLimitActivated == YES)
          { 
              _V1(statoRegolazioneHeating,nTherm) = OFF;
              StopHeating(nTherm); 
          }
          else  
          {
              Heating(nTherm);
          }
          AuxHeating(nTherm);
      }
      else
      {
          _V1(statoRegolazioneHeating,nTherm) = OFF;
          StopHeating(nTherm); StopAuxHeating(nTherm);
          if ( (  _ac(nTherm).passiveAnticondActivated == YES )||( _ac(nTherm).activeAnticondActivated== YES ))
          {
              StopCooling(nTherm);
          }
          else
          {
              Cooling(nTherm);
          }
          AuxCooling(nTherm);
      }
  }
  
  FancoilFunction(stopAll,nTherm);
  
  ValveOutStatusAdj(nTherm);  
  ValveProtection(nTherm);
  ValveOutput(nTherm);
  ValveRT(nTherm);
  ValveStatus(nTherm);

  if ( t->hvacMode != pHvacMode ) t->hvacMode = pHvacMode;
  
  if ( _V1(mf,nTherm).manualActive == YES )
  {
      if ( _OBJV1(HVACProtectionModeStatus,nTherm) == 1 ) 
        
      {
          _OBJV1(HVACProtectionModeStatus,nTherm) = 0;
          LUSetTransmitRequest(CO_INX_THERM(nTherm,CO_HVACProtectionModeStatus_1));      
      } 
  }
  else
  {
      if ( pHvacModeUsed == 4 && _OBJV1(HVACProtectionModeStatus,nTherm) == 0 )
      {
          _OBJV1(HVACProtectionModeStatus,nTherm) = 1;
          LUSetTransmitRequest(CO_INX_THERM(nTherm,CO_HVACProtectionModeStatus_1));      
      }
      if ( pHvacModeUsed != 4 && _OBJV1(HVACProtectionModeStatus,nTherm) == 1 ) 
        
      {
          _OBJV1(HVACProtectionModeStatus,nTherm) = 0;
          LUSetTransmitRequest(CO_INX_THERM(nTherm,CO_HVACProtectionModeStatus_1));      
      } 
  }
  
  if ( THERM_CHK_FIRSTRUN(nTherm) ) 
  {
      LUSetTransmitRequest(CO_INX_THERM(nTherm,CO_HVACProtectionModeStatus_1));      
      THERM_RST_FIRSTRUN(nTherm);
  }

  
  return pHvacModeUsed;  
}

/**
 * @}
 */