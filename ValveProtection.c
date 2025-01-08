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
#include "ValveProtection.h"
#include "Anticondensation.h"
#include "FloorTempLimitation.h"
#include "Thermostat.h"


BYTE _V1(prevHeatingOut,MAX_THERM);
BYTE _V1(prevCoolingOut,MAX_THERM);


const T_VALVE_OUT_RT valveOutRTList[] = 
{
  { CO_HeatingOut_1 ,           TimerHeatingOutRT ,     &_PARTHERM(0).HCntlRT } ,
  { CO_CoolingOut_1 ,           TimerCoolingOutRT ,     &_PARTHERM(0).CCntlRT } ,
  { CO_AuxiliaryHeatingOut_1 ,  TimerAuxHeatingOutRT ,  &_PARTHERM(0).HAuxCntlRT } ,
  { CO_AuxiliaryCoolingOut_1 ,  TimerAuxCoolingOutRT ,  &_PARTHERM(0).CAuxCntlRT } 
};

TYPE_VALVE_PROTECTION _vp(MAX_THERM);

//-----------------------------------------------------------------------------
// ValveProtectionInit
//-----------------------------------------------------------------------------      
void ValveProtectionInit(void)
{
  TYPE_VALVE_PROTECTION *v;

  for ( BYTE nTherm = 0; nTherm < MAX_THERM; nTherm++ )
  {
      v = &_vp(nTherm);
      if ( _V1(thermFunctions,nTherm) & FUNCT_THERMOSTAT ) 
      {
          if ( (_V1(thermFunctions,nTherm) & FUNCT_THERM_HEATING) )
          {
              v->valveEnabled |= VALVE_HEATING;
          }

          if ( _V1(thermFunctions,nTherm) & FUNCT_THERM_COOLING )
          {
              if ( _V1(thermFunctions,nTherm) & FUNCT_THERM_HEATING )
              {
                  if ( _PARTHERM(nTherm).CommandCO == CMD_CO_SEPARATED )
                    v->valveEnabled |= VALVE_COOLING;
                  else
                    v->valveCoupled |= VALVE_COUPLED_PRIMARY;
              }
              else
              {
                  v->valveEnabled |= VALVE_COOLING;
              }
          }
          
          if ( _V1(thermFunctions,nTherm) & FUNCT_THERM_AUXHEATING )
          {
              v->valveEnabled |= VALVE_AUXHEATING;
          }     
          if ( _V1(thermFunctions,nTherm) & FUNCT_THERM_AUXCOOLING )
          {
              if ( _V1(thermFunctions,nTherm) & FUNCT_THERM_AUXHEATING )
              {
                  if ( _PARTHERM(nTherm).AuxHeatingCommandCO == CMD_CO_SEPARATED )
                    v->valveEnabled |= VALVE_AUXCOOLING;
                  else
                    v->valveCoupled |= VALVE_COUPLED_AUX;
              }
              else
              {
                  v->valveEnabled |= VALVE_AUXCOOLING;
              }        
          } 
          if ( v->valveCoupled & VALVE_COUPLED_PRIMARY )
          {
              _V1(coolingControlType,nTherm) = _PARTHERM(nTherm).HeatingControlType;
          }
          else
          {
              _V1(coolingControlType,nTherm) = _PARTHERM(nTherm).CoolingControlType;
          }           
          
      }
  }  
}

//-----------------------------------------------------------------------------
// ValveProtection
//-----------------------------------------------------------------------------      
void ValveProtection(BYTE nTherm)
{
  BYTE mask=1;
  TYPE_VALVE_PROTECTION *v = &_vp(nTherm);

  if ( _PARTHERM(nTherm).ValveProtection != ENABLED) return;
  
  for ( BYTE cnt=0; cnt<VALVE_NR; cnt++,mask<<=1 )
  {
      if ( !( v->valveEnabled & mask ) ) continue;
      if ( v->thermostatValveReq & mask ) v->protectionValveFase[cnt] = 0;
    
      switch (v->protectionValveFase[cnt]) 
      {
      case 0:  /* riavvia il conteggio */
          v->protectionValveCounter[cnt] = GetValveProtectionFrequency(_PARTHERM(nTherm).ValveProtectionFrequency);
          if ( v->protectionValveReq & mask )
              EZ_StopTimer( TimerValveProtection+cnt );
          v->protectionValveFase[cnt] = 1;
          v->protectionValveReq &= ~mask;
          break;
          
      case 1: /* conteggio per attivazione protezione */
          if ( (tm.elapsed1min)&&(v->protectionValveCounter[cnt]) ) v->protectionValveCounter[cnt]--;
          if ( v->protectionValveCounter[cnt] == 0 ) v->protectionValveFase[cnt] = 2;
          break;
          
      case 2: /* inizo attivazione protezione */
          EZ_StartTimer( TimerValveProtection+cnt,GetValveProtectionTime(_PARTHERM(nTherm).ValveProtectionTime), TM_MODE_1S );
          v->protectionValveReq |= mask;
          v->protectionValveFase[cnt] = 3;
          break; 
          
      case 3: /* controllo scadenza tempo di protezione */
          if ( EZ_GetState(TimerValveProtection+cnt) )
          {
            v->protectionValveReq &= ~mask;
            v->protectionValveCounter[cnt] = GetValveProtectionFrequency(_PARTHERM(nTherm).ValveProtectionFrequency);
            v->protectionValveFase[cnt] = 1;
          }
          break;   
      
      default:
        break;     
      }
  }
}        
//-----------------------------------------------------------------------------
// ProtectionOutputValue
//-----------------------------------------------------------------------------                                 
BYTE ProtectionOutputValue(BYTE controlType)
{
  if  ( controlType == CONTINUOUS ) return 255;
  else return 1;  
}

//-----------------------------------------------------------------------------
// ValveRT
//-----------------------------------------------------------------------------                                 
void ValveRT(BYTE nTherm)
{
  BYTE mask=1;
    
  for ( BYTE cnt=0; cnt < VALVE_NR; cnt++,mask<<=1 )
  {
      if ( !( _vp(nTherm).valveEnabled & mask ) ) continue;
      if ( EZ_GetState( valveOutRTList[cnt].timer+nTherm ) )
      {
         SetTransmitRequest(CO_INX_THERM(nTherm,valveOutRTList[cnt].CO));
         EZ_StartTimer( valveOutRTList[cnt].timer+nTherm, 
                        GetCyclicSendingInterval( (*(valveOutRTList[cnt].rt+sizeof(struct THERM)*nTherm) & 0xF0) >> 4 )
                                                  , TM_MODE_1S );
      }
        
  }
}

//-----------------------------------------------------------------------------
// ValveOutStatusAdj
//-----------------------------------------------------------------------------                                 
void ValveOutStatusAdj(BYTE nTherm)
{
  TYPE_VALVE_PROTECTION *v = &_vp(nTherm);
  
  v->cpyThermostatValveReq = 0;
  
  if ( _V1(heatingOut,nTherm) != 0 ) 
      v->cpyThermostatValveReq |= VALVE_HEATING;
  if ( _V1(coolingOut,nTherm) != 0 ) 
      v->cpyThermostatValveReq |= VALVE_COOLING;
  if ( _V1(auxiliaryHeatingOut,nTherm) != 0 ) 
      v->cpyThermostatValveReq |= VALVE_AUXHEATING;
  if ( _V1(auxiliaryCoolingOut,nTherm) != 0 ) 
      v->cpyThermostatValveReq |= VALVE_AUXCOOLING;  
  
  
  v->thermostatValveReq = v->cpyThermostatValveReq;
  
  if ( v->valveCoupled & VALVE_COUPLED_PRIMARY )
  {
      v->thermostatValveReq |=  ( v->cpyThermostatValveReq & VALVE_COOLING ) >> 1;
      v->thermostatValveReq &= ~( v->cpyThermostatValveReq & VALVE_COOLING );
  }

  if ( v->valveCoupled & VALVE_COUPLED_AUX )
  {
      v->thermostatValveReq |=  ( v->cpyThermostatValveReq & VALVE_AUXCOOLING ) >> 1;
      v->thermostatValveReq &= ~( v->cpyThermostatValveReq & VALVE_AUXCOOLING );
  }  
}
  

//-----------------------------------------------------------------------------
// ValveInit
//-----------------------------------------------------------------------------                                 
void ValveInit(void)
{
  BYTE mask=1;
  WORD param;
  
  for ( BYTE nTherm=0; nTherm < MAX_THERM; nTherm++ )
  {
      mask=1;
      for ( BYTE cnt=0; cnt < VALVE_NR; cnt++,mask<<=1 )
      {
          if ( !( _vp(nTherm).valveEnabled & mask ) ) continue;
          param = GetCyclicSendingInterval( (*(valveOutRTList[cnt].rt+sizeof(struct THERM)*nTherm) & 0xF0) >> 4);
          if ( param )
          {
              EZ_StartTimer( valveOutRTList[cnt].timer+nTherm,param, TM_MODE_1S );
          }
      }
      _vp(nTherm).valveFirstRun = NO;
      
      _V1(prevHeatingOut,nTherm) = 0;
      _V1(prevCoolingOut,nTherm) = 0;
  }
}

//-----------------------------------------------------------------------------
// ValveOutput
//-----------------------------------------------------------------------------                                 
void ValveOutput(BYTE nTherm)
{
  BYTE valueOut;
  BYTE mask=1;
  BYTE *objPnt;
  WORD coNr;
  BYTE *pntVH   = &_V1(heatingOut,nTherm);
  BYTE *pntVC   = &_V1(coolingOut,nTherm);
  BYTE *pntVAH  = &_V1(auxiliaryHeatingOut,nTherm);
  BYTE *pntVAC  = &_V1(auxiliaryCoolingOut,nTherm);  
  BYTE oksend;
  BYTE covControl = 0;
  TYPE_VALVE_PROTECTION *v = &_vp(nTherm);
  
  if ( v->valveCoupled & VALVE_COUPLED_PRIMARY )
  {
      if ( (_V1(heatingOut,nTherm) == 0)&&(_V1(coolingOut,nTherm) != 0) )
            pntVH = &_V1(coolingOut,nTherm);
  }

  if ( v->valveCoupled & VALVE_COUPLED_AUX )
  {
      if ( (auxiliaryHeatingOut == 0)&&(auxiliaryCoolingOut != 0) )
          pntVAH = &_V1(auxiliaryCoolingOut,nTherm);
  }  
  
  v->valveOutput = 0;
  for ( BYTE cnt=0; cnt < VALVE_NR; cnt++,mask<<=1 )
  {
      oksend = 1;
      if ( !(v->valveEnabled & mask) ) continue;
      if ( (v->thermostatValveReq & mask)||(v->protectionValveReq & mask) )
      {
          switch (mask)
          {
          case VALVE_HEATING: // e cooling se accoppiata
               valueOut = (v->protectionValveReq & mask) ? ProtectionOutputValue( _PARTHERM(nTherm).HeatingControlType ) : *pntVH ;
               if ((_PARTHERM(nTherm).HeatingControlType == CONTINUOUS )&&
                   (!(v->protectionValveReq & mask)) )
               {
                  if (_therm(nTherm).heatingCoolingMode == HEATING) 
                      covControl = (_PARTHERM(nTherm).HeatingControlCOV*255)/100;
                  else
                      covControl = (_PARTHERM(nTherm).CoolingControlCOV*255)/100;                  
                  if ( (( (BYTE)abs(valueOut - _V1(prevHeatingOut,nTherm)) < covControl )&&
                       ( valueOut != 0 )) || (covControl == 0) )
                  {
                      oksend = 0;
                  }
                  else
                  { 
                      _V1(prevHeatingOut,nTherm) = valueOut;
                  }
               }
               objPnt = &(_OBJV1(HeatingOut,nTherm));
               coNr = CO_INX_THERM(nTherm,CO_HeatingOut_1);
               break;
               
          case VALVE_AUXHEATING:  // e cooling se accoppiata
               valueOut = (v->protectionValveReq & mask) ? 1 : *pntVAH ;
               objPnt = &(_OBJV1(AuxiliaryHeatingOut,nTherm));
               coNr = CO_INX_THERM(nTherm,CO_AuxiliaryHeatingOut_1);               
               break;

          case VALVE_COOLING:      
               valueOut = (v->protectionValveReq & mask) ? ProtectionOutputValue( _V1(coolingControlType,nTherm) ) : *pntVC ;
               if ((coolingControlType == CONTINUOUS )&&
                   (!(v->protectionValveReq & mask)) )
               {
                  if ( (( (BYTE)abs(valueOut - _V1(prevCoolingOut,nTherm)) < (_PARTHERM(nTherm).CoolingControlCOV*255)/100 )&&
                       ( valueOut != 0 )) || (_PARTHERM(nTherm).CoolingControlCOV == 0) )
                  {
                      oksend = 0;
                  }     
                  else
                  { 
                      _V1(prevCoolingOut,nTherm) = valueOut;
                  }                  
               }
               objPnt = &(_OBJV1(CoolingOut,nTherm));
               coNr = CO_INX_THERM(nTherm,CO_CoolingOut_1);                       
               break;
               
          case VALVE_AUXCOOLING:
               valueOut = (v->protectionValveReq & mask) ? 1 : *pntVAC ;
               objPnt = &(_OBJV1(AuxiliaryCoolingOut,nTherm));
               coNr = CO_INX_THERM(nTherm,CO_AuxiliaryCoolingOut_1);                        
               break;
               
          default:
              objPnt = 0;
              break;
          }
      }
      else
      {
          valueOut = 0;
          switch (mask)
          {  
          case VALVE_HEATING:
               objPnt = &(_OBJV1(HeatingOut,nTherm));
               coNr = CO_INX_THERM(nTherm,CO_HeatingOut_1);
               _V1(prevHeatingOut,nTherm) = 0;
               break;
               
          case VALVE_AUXHEATING:
               objPnt = &(_OBJV1(AuxiliaryHeatingOut,nTherm));
               coNr = CO_INX_THERM(nTherm,CO_AuxiliaryHeatingOut_1);               
               break;

          case VALVE_COOLING:      
               objPnt = &(_OBJV1(CoolingOut,nTherm));
               coNr = CO_INX_THERM(nTherm,CO_CoolingOut_1); 
               _V1(prevCoolingOut,nTherm) = 0;
               break;
               
          case VALVE_AUXCOOLING:
               objPnt = &(_OBJV1(AuxiliaryCoolingOut,nTherm));
               coNr = CO_INX_THERM(nTherm,CO_AuxiliaryCoolingOut_1); 
               break;
               
          default:
              objPnt = 0;
              break;
          }     
      }
      v->valveOutput |= (valueOut == 1) << cnt;
      if ( ((objPnt != 0)&&(valueOut != *objPnt))||(v->valveFirstRun == NO) )
      {
          *objPnt = valueOut;
          if (( oksend == 1)||(_vp(nTherm).valveFirstRun == NO))
              SetTransmitRequest(coNr);
      }
  } 
  v->valveFirstRun = YES;
}

//-----------------------------------------------------------------------------
// ValveStatus
//-----------------------------------------------------------------------------  
void ValveStatus(BYTE nTherm)
{
    TYPE_VALVE_PROTECTION *v = &_vp(nTherm);
    
    v->valveHeatingStatus = 0;
    v->valveCoolingStatus = 0;
    v->valveHeatingAlarm = 0;
    v->valveCoolingAlarm = 0;    
    
    if  (_therm(nTherm).heatingCoolingMode == HEATING) 
    {
        if (_PARTHERM(nTherm).HeatingControlType == _2POINTS )
        {
            v->valveHeatingStatus = ((v->valveOutput & VALVE_HEATING) != OFF);
        }
        else
        {
            if ( _V1(temperature,nTherm) < _V1(actualSetpoint,nTherm) )
            {
              v->valveHeatingStatus = ON;
              if (_V1(ftc,nTherm).heatingTempLimitActivated == YES)
              {
                  v->valveHeatingAlarm = ON;
              }
            }
        }
    }

    if  (_therm(nTherm).heatingCoolingMode == COOLING) 
    {
        if (_V1(coolingControlType,nTherm) == _2POINTS )
        {
            if ( v->valveCoupled & VALVE_COUPLED_PRIMARY )
            {
                v->valveCoolingStatus = ((v->valveOutput & VALVE_HEATING) != OFF);
            }
            else
            {
                v->valveCoolingStatus = ((v->valveOutput & VALVE_COOLING) != OFF);
            }
        }
        else
        {
            if ( _V1(temperature,nTherm) > _V1(actualSetpoint,nTherm) )
            {
                v->valveCoolingStatus = ON;
                if (_ac(nTherm).passiveAnticondActivated == YES)
                {
                    v->valveCoolingAlarm = ON;
                }
            }
        }
      
    }
    
}

