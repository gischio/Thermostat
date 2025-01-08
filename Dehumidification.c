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
#include "HumiditySensor.h"
#include "ValveProtection.h"
#include "Dehumidification.h"


//BYTE    dehumDisable[MAX_THERM];
//BYTE    setpointDehum[MAX_THERM];
BYTE    reqDeh[MAX_THERM];
BYTE    dehDelay[MAX_THERM];
BYTE    fistTimeDehumi = NO;

//-----------------------------------------------------------------------------
// DehReadCOEnable
//----------------------------------------------------------------------------- 
BYTE DehReadCOEnable(BYTE dehumDisable,BYTE nTherm)
{
  
    if ( CheckUpdateFlag(CO_INX_THERM(nTherm,CO_DehumControlDisable_1)) )
        dehumDisable = ContactFromBus(_OBJV1(DehumControlDisable,nTherm) , 
                                                 _PARTHERM(nTherm).DehumidificationInvertDisableSignal);
        
    return dehumDisable;    
}
//-----------------------------------------------------------------------------
// DehIntegration
//-----------------------------------------------------------------------------  
BYTE DehIntegration(BYTE intOut,BYTE dehOut,BYTE nTherm)
{
    struct THERM *p = (struct THERM*)&_PARTHERM(nTherm);
    
    if ( (p->DehumidificationIntegration == YES )&&
         (_V1(temperature,nTherm) != SENSOR_ERROR)&&(dehOut == 1) )
    {
        if ( _V1(temperature,nTherm) > ( _V1(actualSetpoint,nTherm)+GetTemperatureIntegration(p->DifferenceForIntegration) ))
        {
            intOut = 1;
        }
        else if ( _V1(temperature,nTherm) < ( _V1(actualSetpoint,nTherm)+GetTemperatureIntegration(p->DifferenceForIntegration) -
                                  GetHysteresis(p->IntegrationHysteresis)*10) )
        {
            intOut = 0;
        }
    }
    else
    {
        intOut = 0;
    }
    
    return intOut;
}
//-----------------------------------------------------------------------------
// humValueErr
//-----------------------------------------------------------------------------   
BYTE DehValueErr(BYTE nTherm)
{
    if ( _V1(humidity,nTherm) == SENSOR_ERROR )
        return 1;

    return 0;
}
//-----------------------------------------------------------------------------
// DehumidificationReady
//
// deumidificazione abilitata per il modo di funzionamento corrente , sensore
// di umidità ok ?
//-----------------------------------------------------------------------------   
BYTE DehumidificationReady(BYTE hvacMode,BYTE nTherm)
{
  struct THERM *p = (struct THERM*)&_PARTHERM(nTherm);
   
  if (hvacMode != HVAC_PROTECTION) 
  {
      if ( ( p->Dehumidification == HD_COOLING_AND_HEATING )||
         ( (_therm(nTherm).heatingCoolingMode == COOLING)&&(p->Dehumidification == HD_COOLING_ONLY) )||
         ( (_therm(nTherm).heatingCoolingMode == HEATING)&&(p->Dehumidification == HD_HEATING_ONLY) ) )
      {
          if ( ( p->DehumidificationWithTemperature == YES )&&      
               ( _V1(temperature,nTherm) == SENSOR_ERROR ) )
              return 0;
          if ( ! DehValueErr(nTherm) )  
              return 1;  
      }
  }
  
  return 0;  
}
//-----------------------------------------------------------------------------
// DehumidificationInit
//-----------------------------------------------------------------------------                                                   
void DehumidificationInit(void)
{
  WORD param;

  fistTimeDehumi = 0;
  for ( BYTE nTherm = 0; nTherm < MAX_THERM; nTherm++ )
  {
      reqDeh[nTherm] = 0;
      dehDelay[nTherm] = 0;    
      if (!DEHUMIDIFICATION_ENABLE(nTherm) ) continue;
    
      fistTimeDehumi |= (1<<nTherm);
      if ( param = GetCyclicSendingInterval(_PARTHERM(nTherm).DehumidificationControlRT) )
      {
          EZ_StartTimer( TimerDehCommandRT+nTherm, param, TM_MODE_1S );
      }
  }
  
}

//-----------------------------------------------------------------------------
// Dehumidification
//-----------------------------------------------------------------------------                                                   
BYTE Dehumidification(BYTE hvacMode,BYTE nTherm) 
{
  BYTE flagTxDeh = 0;
  BYTE flagTxInt = 0;
  BYTE flagTxAcq = 0;
  BYTE dehOut = _OBJV1(DehumCommand,nTherm);  
  BYTE intOut = _OBJV1(DehumIntegrationControl,nTherm);  
  BYTE acqOut = _OBJV1(DehumWaterBatteryCommand,nTherm);  
  WORD param;
  struct THERM *p = (struct THERM*)&_PARTHERM(nTherm);

  
  if (!DEHUMIDIFICATION_ENABLE(nTherm)) return 0;
  
  if ( p->DehumidificationDisableFromBus == YES )
      _therm(nTherm).dehumidificationDisable = DehReadCOEnable( _therm(nTherm).dehumidificationDisable,nTherm );
  else
      _therm(nTherm).dehumidificationDisable = 0;
        
  if ( DehumidificationReady(hvacMode,nTherm) && ( _therm(nTherm).dehumidificationDisable  != 1) )
  {
      //--------------------------------      
      if ( _V1(humidity,nTherm) > ((_therm(nTherm).setpointDehumidification*100) + GetHumidityHysteresis(p->DehumidificationHysteresis)*10) )
      {
          reqDeh[nTherm] = 1;
      }
      else if ( _V1(humidity,nTherm) < _therm(nTherm).setpointDehumidification*100 )
      {
          reqDeh[nTherm] = 0;
      }
      //--------------------------------
      
      if ( (_therm(nTherm).heatingCoolingMode == COOLING) && (_V1(coolingControlType,nTherm) == _2POINTS) && 
           (p->Dehumidification == HD_COOLING_ONLY) )
      {
          if (p->DehumidificationWithTemperature == YES)
          {
              if ( (reqDeh[nTherm] == 1)&&(_V1(coolingOut,nTherm) == 0))
              {
                  reqDeh[nTherm] = 0;
              }
              acqOut = reqDeh[nTherm];
              dehOut = reqDeh[nTherm];
              intOut = DehIntegration(intOut,acqOut,nTherm);
          }
          else
          {  
              dehOut = reqDeh[nTherm]; 
              acqOut = reqDeh[nTherm];  // attivo subito batteria acqua !
              intOut = DehIntegration(intOut,acqOut,nTherm);  
              param = GetIntegerConst( &p->DehumidificationDelay);
              if ( param != 0 )
              {
                  if ( (dehDelay[nTherm] == 0)&&(reqDeh[nTherm] == 1) )
                  {
                      EZ_StartTimer( TimerDelayDeh+nTherm, param, TM_MODE_1S ); 
                      intOut = 0;
                      dehOut = 0;
                      dehDelay[nTherm] = 1;
                  }
                  else
                  {
                      if ( !EZ_GetState(TimerDelayDeh+nTherm)&&(dehDelay[nTherm] != 2) ) 
                      {
                          intOut = 0;
                          dehOut = 0;
                      }
                      else
                      {
                          dehDelay[nTherm] = 2;
                      }
                  }
              }
          }
      }
      else
      {
          dehOut = reqDeh[nTherm]; 
          acqOut = 0;
          intOut = 0;
          dehDelay[nTherm] = 0;
          EZ_StopTimer( TimerDelayDeh+nTherm ); 
      }
  }
  else
  { 
      reqDeh[nTherm] = 0;
  }
  
  if ( reqDeh[nTherm] == 0 )
  {
      intOut = 0;
      acqOut = 0;
      dehOut = 0;
      dehDelay[nTherm] = 0;
      EZ_StopTimer( TimerDelayDeh+nTherm );   
  }
  
  // RT
  if ( EZ_GetState(TimerDehCommandRT+nTherm) ) 
  {
      EZ_StartTimer( TimerDehCommandRT+nTherm, 
                     GetCyclicSendingInterval(p->DehumidificationControlRT), TM_MODE_1S );
      flagTxInt = flagTxAcq = flagTxDeh = 1;
  }
  
  if (( _OBJV1(DehumIntegrationControl,nTherm) != intOut )||(fistTimeDehumi&(1<<nTherm)) )
  {
      flagTxInt = 1;
      _OBJV1(DehumIntegrationControl,nTherm) = intOut;
  }      
  if (( _OBJV1(DehumCommand,nTherm) != dehOut )||(fistTimeDehumi&(1<<nTherm)) )
  {
      flagTxDeh = 1;
      _OBJV1(DehumCommand,nTherm) = dehOut;
  }  
  if (( _OBJV1(DehumWaterBatteryCommand,nTherm) != acqOut )||(fistTimeDehumi&(1<<nTherm)) )
  {
      flagTxAcq = 1;
      _OBJV1(DehumWaterBatteryCommand,nTherm) = acqOut;
  }    
  
  if ( flagTxDeh ) SetTransmitRequest( CO_INX_THERM(nTherm,CO_DehumCommand_1) );
  if ( flagTxInt ) SetTransmitRequest( CO_INX_THERM(nTherm,CO_DehumIntegrationControl_1) );            
  if ( flagTxAcq ) SetTransmitRequest( CO_INX_THERM(nTherm,CO_DehumWaterBatteryCommand_1) ); 

  fistTimeDehumi &= ~(1<<nTherm);

  return 1;
}