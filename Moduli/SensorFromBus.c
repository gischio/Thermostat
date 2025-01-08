/**@addtogroup SBS_Code_EK-EQ2-TP
@{
commento.....
*/
//------------------------------------------------------------------

#include "project.h"
#include "eib_stk.h"
#include "app.h"
#include EIB_DEVICE_MODEL_APP_H_FILE
#include <string.h>
#include "ApplicationTimers.h"
#include "ApplicationUtility.h"
#include "ListValues.h"

WORD aiParam[NR_MISURE];


const T_SENSORS_FROMBUS sensorsFromBusList[] = {
  //    CO nr                       ,Timer              ,mask                   ,timeout     ,Timer CR              ,Time CR                
  { CO_PotenzaIstantaneaFbkW_1   ,TimerCOTimeout     ,FROMBUS_POT_MISURA1    ,&aiParam[0] ,TimerCOReadCyclic     ,&PARAMETER.CMisura[0].u__000C },
  { CO_PotenzaIstantaneaFbkW_2   ,TimerCOTimeout+1   ,FROMBUS_POT_MISURA2    ,&aiParam[1] ,TimerCOReadCyclic+1   ,&PARAMETER.CMisura[1].u__000C },
  { CO_PotenzaIstantaneaFbkW_3   ,TimerCOTimeout+2   ,FROMBUS_POT_MISURA3    ,&aiParam[2] ,TimerCOReadCyclic+2   ,&PARAMETER.CMisura[2].u__000C },
  { CO_PotenzaIstantaneaFbkW_4   ,TimerCOTimeout+3   ,FROMBUS_POT_MISURA4    ,&aiParam[3] ,TimerCOReadCyclic+3   ,&PARAMETER.CMisura[3].u__000C },
  { END_TAG                      ,0                  ,FROMBUS_END_INX        ,0           ,0                     ,0                             },
};

//------------------------------------------------------------------
// SensorFromBusInit
//------------------------------------------------------------------
void  SensorFromBusInit( void )
{
  WORD cnt=0;
  WORD param;
  unsigned long roiTime = delayAfterBusOn*10L;  
  
  sensorsFromBusEnabled  = 0;
  sensorsFromBusTimeout  = 0;
  sensorsFromBusError    = 0;
  sensorsFromBusUpdated  = 0;
  sensorsFromBusROI      = 0;  
  
  for ( BYTE chn = 0; chn < NR_MISURE; chn++ )
  {
     if ( PARAMETER.CMisura[chn].CanaleMisura == CHN_ESTERNO ) 
     {
          __SetSensorFromBusEnabled(chn);
          if ( PARAMETER.CMisura[chn].ValorePotenzaFbROI ) 
              __SetSensorFromBusROI(chn);
          aiParam[chn] = GetIntegerConst(&PARAMETER.CMisura[chn].ValorePotenzaFbTimeOut);
     }
  }
  
  while (sensorsFromBusList[cnt].CO != END_TAG )
  {
      if ( __IsSensorFromBusEnabled(sensorsFromBusList[cnt].mask) )
      {
          if (*sensorsFromBusList[cnt].time) 
               EZ_StartTimer( sensorsFromBusList[cnt].timer, *sensorsFromBusList[cnt].time, TM_MODE_1S );
          param = GetCyclicReadingInterval((*sensorsFromBusList[cnt].timeReadCyclic)&0x0F);
          if ( (param > 0)||(__IsSensorFromBusROI(sensorsFromBusList[cnt].mask)) )
          {
              EZ_StartTimer( sensorsFromBusList[cnt].timerReadCyclic, roiTime, TM_MODE_1MS ); 
              roiTime += 250;
          }
      }
      cnt++;
  }
}

//------------------------------------------------------------------
// SensorFromBusTimeoutAndError  
//------------------------------------------------------------------
void  SensorFromBusTimeoutAndError(void )
{
  WORD cnt=0;
  WORD param;
  void *pCO;
  WORD coNr;
  T_SENSORS_FROMBUS_INX mask;
  WORD timer;
  WORD paramReadCyclic;
  WORD timerReadCyclic;  

  while (sensorsFromBusList[cnt].CO != END_TAG )
  {
      param = *sensorsFromBusList[cnt].time;
      coNr  = sensorsFromBusList[cnt].CO;
      mask  = sensorsFromBusList[cnt].mask;  
      timer = sensorsFromBusList[cnt].timer;
      paramReadCyclic = GetCyclicReadingInterval(*sensorsFromBusList[cnt].timeReadCyclic & 0x0F);
      timerReadCyclic = sensorsFromBusList[cnt].timerReadCyclic;      
      pCO = GetObjectValuePtr(coNr);      
      if ( __IsSensorFromBusEnabled(mask) )
      {
          if ( (paramReadCyclic > 0)||(__IsSensorFromBusROI(mask)) )
          {
              if ( EZ_GetState( timerReadCyclic ) )
              {
                  SetReceiveRequest(coNr);
                  if (paramReadCyclic > 0)
                      EZ_StartTimer( timerReadCyclic, paramReadCyclic, TM_MODE_1S );
                  __ResetSensorFromBusROI(mask);
              }
          }        
          if ( EZ_GetState(timer) && (param != 0) )
          {                      
              __SetSensorFromBusTimeout(mask);
              if ( pCO != 0 )
                if (IS_FORMBUS_ANALOG(mask))
#ifdef AN_CO__SHORTFLOAT                  
                  *((WORD*)pCO) = SENSOR_INVALID_DATA; // analogico shortfloat
#else 
                  *((WORD*)pCO) = 0;
#endif              
                else
                  *((BYTE*)pCO) = 0; // digitale
          }
          else
          {
              if ( CheckUpdateFlag(coNr) )
              {
                  if ( paramReadCyclic > 0)
                  {
                      EZ_StartTimer( timerReadCyclic, paramReadCyclic, TM_MODE_1S ); 
                  }                
#ifdef AN_CO__SHORTFLOAT                    
                  if ( (pCO != 0)&&(IS_FORMBUS_ANALOG(mask)) )
                  {   /* è una porcata ma dovrebbe funzionare ;) i CO digitali hanno
                         valore 0 o 1 quindi non verranno mai considerati in errore ... */
                      if ( *((WORD*)pCO) == SENSOR_INVALID_DATA )
                          __SetSensorFromBusError(mask);
                      else
                          __ResetSensorFromBusError(mask);
                  }
#endif                  
                  EZ_StartTimer( timer, param, TM_MODE_1S );
                  __ResetSensorFromBusTimeout(mask);
                  __SetSensorFromBusUdated(mask);
              }
          }
      }
      cnt++;
  } 
}


                                                   
