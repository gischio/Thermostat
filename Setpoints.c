/**@addtogroup SBS_Code_EK-EQ2-TP
@{
commento.....
*/
//------------------------------------------------------------------
#include <string.h>
#include "project.h"
#include "eib_stk.h"
#include "app.h"
#include "application.h"
#include EIB_DEVICE_MODEL_APP_H_FILE
#include "Application.h"
#include "ListValues.h"
#include "ApplicationTimers.h"
#include "ApplicationUtility.h"
#include "Setpoints.h"
#include "Thermostat.h"


long limit_min = 200;     // 1/100 °C
long limit_max = 5000;    // 1/100 °C

const long setpointLimits[][2] = {
  /* SINGLE SETPOINT */
  { MAX_COMFORT_SETPOINT_HEATING   , MIN_COMFORT_SETPOINT_HEATING   },
  { MAX_COMFORT_SETPOINT_COOLING   , MIN_COMFORT_SETPOINT_COOLING   },
  { MAX_STANDBY_SETPOINT_HEATING   , MIN_STANDBY_SETPOINT_HEATING   },
  { MAX_STANDBY_SETPOINT_COOLING   , MIN_STANDBY_SETPOINT_COOLING   },
  { MAX_ECONOMY_SETPOINT_HEATING   , MIN_ECONOMY_SETPOINT_HEATING   },
  { MAX_ECONOMY_SETPOINT_COOLING   , MIN_ECONOMY_SETPOINT_COOLING   },
  { MAX_BUILDPROT_SETPOINT_HEATING , MIN_BUILDPROT_SETPOINT_HEATING },
  { MAX_BUILDPROT_SETPOINT_COOLING , MIN_BUILDPROT_SETPOINT_COOLING },
  /* ABSOLUTE SETPOINT */
  { MAX_COMFORT_SETPOINT_HEATING   , MIN_COMFORT_SETPOINT_HEATING   },
  { MAX_COMFORT_SETPOINT_COOLING   , MIN_COMFORT_SETPOINT_COOLING   },
  { MAX_STANDBY_SETPOINT_HEATING   , MIN_STANDBY_SETPOINT_HEATING   },
  { MAX_STANDBY_SETPOINT_COOLING   , MIN_STANDBY_SETPOINT_COOLING   },
  { MAX_ECONOMY_SETPOINT_HEATING   , MIN_ECONOMY_SETPOINT_HEATING   },
  { MAX_ECONOMY_SETPOINT_COOLING   , MIN_ECONOMY_SETPOINT_COOLING   },
  { MAX_BUILDPROT_SETPOINT_HEATING , MIN_BUILDPROT_SETPOINT_HEATING },
  { MAX_BUILDPROT_SETPOINT_COOLING , MIN_BUILDPROT_SETPOINT_COOLING },
  /* RELATIVE SETPOINT */
  { MAX_COMFORT_SETPOINT_HEATING   , MIN_COMFORT_SETPOINT_HEATING   },
  { MAX_COMFORT_SETPOINT_COOLING   , MIN_COMFORT_SETPOINT_COOLING   },
  { MAX_STANDBY_OFFSET_HEATING     , MIN_STANDBY_OFFSET_HEATING     },
  { MAX_STANDBY_OFFSET_COOLING     , MIN_STANDBY_OFFSET_COOLING     },
  { MAX_ECONOMY_OFFSET_HEATING     , MIN_ECONOMY_OFFSET_HEATING     },
  { MAX_ECONOMY_OFFSET_COOLING     , MIN_ECONOMY_OFFSET_COOLING     },
  { MAX_BUILDPROT_SETPOINT_HEATING , MIN_BUILDPROT_SETPOINT_HEATING },
  { MAX_BUILDPROT_SETPOINT_COOLING , MIN_BUILDPROT_SETPOINT_COOLING },
};

/*
   allinea l'array dei CO setpoint con quello usato internamente dal fw

   [0] CO setpoint comfort heating -> [1] comfortSetpoint heating
   [1] CO setpoint comfort cooling -> [0] comfortSetpoint cooling
   [2] CO setpoint standby cooling -> [3] standbySetpoint heating
   [3] CO setpoint standby cooling -> [2] standbySetpoint cooling
   [4] CO setpoint economy heating -> [5] economySetpoint heating
   [5] CO setpoint economy cooling -> [4] economySetpoint cooling
   [6] CO setpoint protect heating -> [7] protectSetpoint heating
   [7] CO setpoint protect cooling -> [6] protectSetpoint cooling
*/
const BYTE transSetp[] = {
  // normal 0,1,2,3,4,5,6,7
  1,0,3,2,5,4,7,6
};


//-----------------------------------------------------------------------------
// SetpointsAdjustAfterDownload
//-----------------------------------------------------------------------------
void SetpointsAdjustAfterDownload(void)
{
  struct THERM *p;
  TYPE_TR_DATA *t;

  for ( BYTE nTherm=0; nTherm<MAX_THERM; nTherm++ )
  {
      p = (struct THERM*)&_PARTHERM(nTherm);
      t = &_therm(nTherm);

      if ( p->SetpointManagement != SETPOINT_SINGLE )
      {
          t->setpointComfort[COOLING] = ShortF2LongI(p->CoolingComfortTemperature);
          t->setpointComfort[HEATING] = ShortF2LongI(p->HeatingComfortTemperature);

          if ( p->CommandCO == CMD_CO_SEPARATED )
          {
              if ( (p->HeatingCoolingSwitchOver == HC_SWITCH_AUTOMATIC )&&
                   (p->ThermostatFunction == HEATING_COOLING_FUNCTION ) )
              {
                  t->neutralZone = (long)(int)GetIntegerConst(&p->CoolingComfortTemperature)*10;
                  t->setpointComfort[COOLING] = t->setpointComfort[HEATING] + t->neutralZone;
              }
          }
#ifdef SETPONT_STANDBY_ECONOMY_FLOAT
          t->setpointStandby[COOLING] = ShortF2LongI(p->CoolingStandbyTemperature);
          t->setpointStandby[HEATING] = ShortF2LongI(p->HeatingStandbyTemperature);
          t->setpointEconomy[COOLING] = ShortF2LongI(p->CoolingEconomyTemperature);
          t->setpointEconomy[HEATING] = ShortF2LongI(p->HeatingEconomyTemperature);
#else
          t->setpointStandby[COOLING] = (long)(int)GetIntegerConst(&p->CoolingStandbyTemperature)*10;
          t->setpointStandby[HEATING] = (long)(int)GetIntegerConst(&p->HeatingStandbyTemperature)*10;
          t->setpointEconomy[COOLING] = (long)(int)GetIntegerConst(&p->CoolingEconomyTemperature)*10;
          t->setpointEconomy[HEATING] = (long)(int)GetIntegerConst(&p->HeatingEconomyTemperature)*10;
#endif
#ifdef GESTIONE_ETS_SETPOINT_VERS1
          if ( p->SetpointManagement == SETPOINT_ABSOLUTE )
          {
              t->setpointStandby[COOLING] += t->setpointComfort[COOLING];
              t->setpointStandby[HEATING] += t->setpointComfort[HEATING];
              t->setpointEconomy[COOLING] += t->setpointComfort[COOLING];
              t->setpointEconomy[HEATING] += t->setpointComfort[HEATING];
          }
#else
          if ( p->SetpointManagement == SETPOINT_RELATIVE )
          {
              t->setpointStandby[COOLING] -= t->setpointComfort[COOLING];
              t->setpointStandby[HEATING] -= t->setpointComfort[HEATING];
              t->setpointEconomy[COOLING] -= t->setpointComfort[COOLING];
              t->setpointEconomy[HEATING] -= t->setpointComfort[HEATING];
          }
#endif
      }
      else
      {
          t->setpointComfort[COOLING] = ShortF2LongI(p->CoolingComfortTemperature);
          t->setpointComfort[HEATING] = ShortF2LongI(p->HeatingComfortTemperature);
      }

      t->setpointProtection[COOLING] = ShortF2LongI(p->CoolingBuildingProtectionTemperature);
      t->setpointProtection[HEATING] = ShortF2LongI(p->HeatingBuildingProtectionTemperature);

      if ( p->ThermostatFunction == HEATING_FUNCTION )
          t->setpointProtection[COOLING] = MAX_BUILDPROT_SETPOINT_COOLING;
      if ( p->ThermostatFunction == COOLING_FUNCTION )
          t->setpointProtection[HEATING] = MIN_BUILDPROT_SETPOINT_HEATING;
  }
}

//-----------------------------------------------------------------------------
// SetpointsLoadFromEeprom ( e altri valori di termoregolazione ... )
//-----------------------------------------------------------------------------
void SetpointsLoadFromEeprom(BYTE nTherm)
{
  memcpy( &_therm(nTherm).hvacMode, (unsigned char *)&actualEep->_therm(nTherm).hvacMode, sizeof(TYPE_TR_DATA) );
}

//-----------------------------------------------------------------------------
// SetpointsInit
//-----------------------------------------------------------------------------
void SetpointsInit(void)
{
  WORD param;
  TYPE_TR_DATA *t;

  for ( BYTE nTherm = 0; nTherm < MAX_THERM; nTherm++ )
  {
      if ( !( _V1(thermFunctions,nTherm) & FUNCT_THERMOSTAT ) ) continue;

      t = &_therm(nTherm);
      if ( param = GetCyclicSendingInterval(_PARTHERM(nTherm).SetpointRT) )
      {
          EZ_StartTimer( TimerSetpointRT+nTherm, param, TM_MODE_1S );
      }

      _OBJV2(SetpointComfort,nTherm,0)             = LongI2ShortF(t->setpointComfort[HEATING]);
      _OBJV2(SetpointComfort,nTherm,1)             = LongI2ShortF(t->setpointComfort[COOLING]);
      _OBJV2(SetpointBuildProtection,nTherm,0)     = LongI2ShortF(t->setpointProtection[HEATING]);
      _OBJV2(SetpointBuildProtection,nTherm,1)     = LongI2ShortF(t->setpointProtection[COOLING]);
      _OBJV2(SetpointStandby,nTherm,0)             = LongI2ShortF(t->setpointStandby[HEATING]);
      _OBJV2(SetpointStandby,nTherm,1)             = LongI2ShortF(t->setpointStandby[COOLING]);
      _OBJV2(SetpointEconomy,nTherm,0)             = LongI2ShortF(t->setpointEconomy[HEATING]);
      _OBJV2(SetpointEconomy,nTherm,1)             = LongI2ShortF(t->setpointEconomy[COOLING]);

      _OBJV1(ManualSetpoint,nTherm)                 = LongI2ShortF(t->setpointManuale);
  }
}
//-----------------------------------------------------------------------------
// Setpoints
//-----------------------------------------------------------------------------
void Setpoints(BYTE nTherm)
{

  /* RT solo actualSetpoint ! */
  if ( EZ_GetState(TimerSetpointRT+nTherm) )
  {
      EZ_StartTimer( TimerSetpointRT+nTherm,
                     GetCyclicSendingInterval(_PARTHERM(nTherm).SetpointRT), TM_MODE_1S );
      _OBJV1(ActualSetpoint,nTherm) = LongI2ShortF(_V1(actualSetpoint,nTherm));
      SetTransmitRequest(CO_INX_THERM(nTherm,CO_ActualSetpoint_1));
  }
  else
  {
  }
}

//-----------------------------------------------------------------------------
// SetpointsFromBus
//-----------------------------------------------------------------------------
BYTE SetpointsFromBus(BYTE nTherm)
{
  WORD* srcPnt = &_OBJV2(SetpointComfort,nTherm,0);
  long* dstPnt = (long*)&_therm(nTherm).setpointComfort[0];
  BYTE updated = 0;
  long app;
  BYTE cnt;
  TYPE_TR_DATA *t = &_therm(nTherm);

  if ((_PARTHERM(nTherm).HeatingCoolingSwitchOver == HC_SWITCH_AUTOMATIC)&&(IS_HEATING_AND_COOLING_ENABLED(nTherm)))
  {
      if (CheckUpdateFlag(CO_INX_THERM(nTherm,CO_SetpointComfort_1_1)))
      {
          app = ShortF2LongI(srcPnt[0]);
          if ( app != t->setpointComfort[HEATING] )
          {
              if (( app <= setpointLimits[HEATING][0] ) && ( app >= setpointLimits[HEATING][1] ))
              {
                  t->setpointComfort[HEATING] = app;
                  t->setpointComfort[COOLING] = t->setpointComfort[HEATING] + t->neutralZone;
                  _OBJV2(SetpointComfort,nTherm,transSetp[COOLING]) = LongI2ShortF(t->setpointComfort[COOLING]);
                  updated = 1;
              }
              else
              {
                  _OBJV2(SetpointComfort,nTherm,transSetp[HEATING]) = LongI2ShortF(t->setpointComfort[HEATING]);
              }
          }
      }
      if (CheckUpdateFlag(CO_INX_THERM(nTherm,CO_SetpointComfort_1_2)) )
      {
          app = ShortF2LongI(srcPnt[1]);
          if ( app != t->setpointComfort[COOLING] )
          {
              if (( app <= setpointLimits[COOLING][0] ) && ( app >= setpointLimits[COOLING][1] ))
              {
                  t->setpointComfort[COOLING] = app;
                  t->setpointComfort[HEATING] = t->setpointComfort[COOLING] - t->neutralZone;
                  _OBJV2(SetpointComfort,nTherm,transSetp[HEATING]) = LongI2ShortF(t->setpointComfort[HEATING]);
                  updated = 1;
              }
              else
              {
                  _OBJV2(SetpointComfort,nTherm,transSetp[COOLING]) = LongI2ShortF(t->setpointComfort[COOLING]);
              }
          }
      }
  }

  if ( _PARTHERM(nTherm).SetpointManagement == SETPOINT_SINGLE )
  {
      if ( CheckUpdateFlag(CO_INX_THERM(nTherm,CO_SetpointComfort_1_1)) )
      {
          app = ShortF2LongI(_OBJV2(SetpointComfort,nTherm,0));
          if ( app != dstPnt[t->heatingCoolingMode] )
          {
              if (( app <= t->setpointProtection[COOLING] ) && ( app >= t->setpointProtection[HEATING] ))
              {
                  dstPnt[t->heatingCoolingMode] = app;
                  updated = 1;
              }
              else
              {
                  _OBJV2(SetpointComfort,nTherm,0) = LongI2ShortF(dstPnt[t->heatingCoolingMode]);
              }
          }
      }
      for (cnt = 6; cnt < 8; cnt++)
      {
          if (CheckUpdateFlag(CO_INX_THERM(nTherm,CO_SetpointComfort_1_1+cnt)))
          {
#ifdef SINGLE_THERMOSTAT
              app = ShortF2LongI(srcPnt[cnt]);
#else
              app = ShortF2LongI((srcPnt+4*(cnt&0xFE))[cnt&0x1]);
#endif
              if ( app != dstPnt[transSetp[cnt]] )
              {
                  if (( app <= setpointLimits[cnt][0] ) && ( app >= setpointLimits[cnt][1] ))
                  {
                      dstPnt[transSetp[cnt]] = app;
                      updated = 1;
                  }
                  else
                  {
#ifdef SINGLE_THERMOSTAT
                    srcPnt[cnt] = LongI2ShortF(dstPnt[transSetp[cnt]]);
#else
                    srcPnt+4*(cnt&0xFE))[cnt&0x1] = LongI2ShortF(dstPnt[transSetp[cnt]];
#endif
                  }
              }
          }
      }
  }
  else
  {
      for (cnt = 0; cnt < 8; cnt++)
      {
          BYTE k;
          k = 8*(_PARTHERM(nTherm).SetpointManagement);
          if (CheckUpdateFlag(CO_INX_THERM(nTherm,CO_SetpointComfort_1_1+cnt)))
          {
#ifdef SINGLE_THERMOSTAT
              app = ShortF2LongI(srcPnt[cnt]);
#else
              app = ShortF2LongI((srcPnt+4*(cnt&0xFE))[cnt&0x1]);
#endif
              if ( app != dstPnt[transSetp[cnt]] )
              {
                  if (( app <= setpointLimits[cnt+k][0] ) && ( app >= setpointLimits[cnt+k][1] ))
                  {
                      dstPnt[transSetp[cnt]] = app;
                      updated = 1;
                  }
                  else
                  {
#ifdef SINGLE_THERMOSTAT
                      srcPnt[cnt] = LongI2ShortF(dstPnt[transSetp[cnt]]);
#else
                      srcPnt+4*(cnt&0xFE))[cnt&0x1] = LongI2ShortF(dstPnt[transSetp[cnt]];
#endif
                  }
              }
/***********************************/
              if ((_PARTHERM(nTherm).HeatingCoolingSwitchOver == HC_SWITCH_AUTOMATIC)&&
                  (IS_HEATING_AND_COOLING_ENABLED(nTherm))&&
                  (_PARTHERM(nTherm).SetpointManagement == SETPOINT_ABSOLUTE) )
              {
              }
/***********************************/
          }
      }
  }



  return updated;
}

//-----------------------------------------------------------------------------
// SetpointSendOnCOV
//-----------------------------------------------------------------------------
void SetpointSendOnCOV(BYTE nTherm) {
  WORD  value;
  long  *srcPnt;
  WORD  *dstPnt;
  BYTE  cnt;

  if ( _V1(thermFunctions,nTherm) & FUNCT_THERM_HEATING )
  {
      srcPnt = (long*)&_therm(nTherm).setpointComfort[HEATING];
      dstPnt = &_OBJV2(SetpointComfort,nTherm,0);
      for ( cnt = 0; cnt<8; cnt+=2,srcPnt+=2,dstPnt+=2 )
      {
        value = LongI2ShortF(*srcPnt);
        if ( *dstPnt != value)
        {
          *dstPnt = value;
          SetTransmitRequest(CO_INX_THERM(nTherm,CO_SetpointComfort_1_1+cnt));
        }
      }
  }
  if ( _V1(thermFunctions,nTherm) & FUNCT_THERM_COOLING )
  {
      srcPnt = (long*)&_therm(nTherm).setpointComfort[COOLING];
      dstPnt = &_OBJV2(SetpointComfort,nTherm,1);
      for ( cnt = 1; cnt<8; cnt+=2,srcPnt+=2,dstPnt+=2 )
      {
        value = LongI2ShortF(*srcPnt);
        if ( *dstPnt != value)
        {
          *dstPnt = value;
          SetTransmitRequest(CO_INX_THERM(nTherm,CO_SetpointComfort_1_1+cnt));
        }
      }
  }
}






