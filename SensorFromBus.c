/**@addtogroup SBS_Code_EK-EQ2-TP
@{
commento.....
*/
//------------------------------------------------------------------

#include "project.h"
#include "eib_stk.h"
#include <string.h>
#include "app.h"
#include EIB_DEVICE_MODEL_APP_H_FILE
#include "ApplicationUtility.h"
#include "ApplicationTimers.h"
#include "ListValues.h"
#include "SensorFromBus.h"
#include "Thermostat.h"


WORD sensorsFromBusEnabled[MAX_THERM];
WORD sensorsFromBusTimeout[MAX_THERM];
WORD sensorsFromBusError[MAX_THERM];
WORD sensorsFromBusUpdated[MAX_THERM];
WORD sensorsFromBusROI[MAX_THERM];

WORD aiParam[MAX_THERM];
WORD diParam[MAX_THERM];  

const T_SENSORS_FROMBUS sensorsFromBusList[MAX_THERM][FROMBUS_END_INX+1] = {
//  CO nr                         ,Timers , mask                     , time
//                                 offset              
{  //therm 1 
  { CO_RoomTempFB_1               ,0      ,FROMBUS_ROOM_TEMP         ,&aiParam[0]  ,&_PARTHERM(0).RCyclicHumRoomT       ,4  },
  { CO_Humidity1byteFB_1          ,1      ,FROMBUS_REL_HUMIDITY1     ,&aiParam[0]  ,&_PARTHERM(0).RCyclicHumRoomT       ,0  },  
  { CO_Humidity2bytesFB_1         ,2      ,FROMBUS_REL_HUMIDITY2     ,&aiParam[0]  ,&_PARTHERM(0).RCyclicAntistratHum   ,4  },   
  { CO_AntistratificationTempFB_1 ,3      ,FROMBUS_ANTISTRAT_TEMP    ,&aiParam[0]  ,&_PARTHERM(0).RCyclicAntistratHum   ,0  },
  { CO_OutdoorTempFB_1            ,4      ,FROMBUS_OUTDOOR_TEMP      ,&aiParam[0]  ,&_PARTHERM(0).RCyclicOutdoorT       ,4  },
  { CO_CoilTempFB_1               ,5      ,FROMBUS_COIL_TEMP         ,&aiParam[0]  ,&_PARTHERM(0).RCyclicFloorTCoilT    ,0  },  
  { CO_FloorTempFB_1              ,6      ,FROMBUS_FLOOR_TEMP        ,&aiParam[0]  ,&_PARTHERM(0).RCyclicFloorTCoilT    ,4  },
  { CO_FlowTempFB_1               ,7      ,FROMBUS_FLOW_TEMP         ,&aiParam[0]  ,&_PARTHERM(0).RCyclicFlowT          ,0  },
  
  { CO_AnticondensationFB_1       ,8      ,FROMBUS_ANTICONDESATION   ,&diParam[0]  ,&_PARTHERM(0).RCyclicWCont1Anticond ,4  },
  { CO_WindowsContactSensor1FB_1  ,9      ,FROMBUS_WINDOWS_CONTACT1  ,&diParam[0]  ,&_PARTHERM(0).RCyclicWCont1Anticond ,0  },
  { CO_WindowsContactSensor2FB_1  ,10     ,FROMBUS_WINDOWS_CONTACT2  ,&diParam[0]  ,&_PARTHERM(0).RCyclicPres1WCont1    ,4  },
  { CO_PresenceSensor1FB_1        ,11     ,FROMBUS_PRESENCE1         ,&diParam[0]  ,&_PARTHERM(0).RCyclicPres1WCont1    ,0  },
  { CO_PresenceSensor2FB_1        ,12     ,FROMBUS_PRESENCE2         ,&diParam[0]  ,&_PARTHERM(0).RCyclicCardHPres11    ,4  },
  { CO_ContactHolderFB_1          ,13     ,FROMBUS_CARD_HOLDER       ,&diParam[0]  ,&_PARTHERM(0).RCyclicCardHPres11    ,0  },
  { END_TAG                       ,0      ,FROMBUS_END_INX           ,0            ,0                                         ,0 },
},/*
{  //therm 2 
  { CO_RoomTempFB_2               ,14     ,FROMBUS_ROOM_TEMP         ,&aiParam[1]  ,&PARAMETER.Therm[1].u__0368   ,4  },
  { CO_Humidity1byteFB_2          ,15     ,FROMBUS_REL_HUMIDITY1     ,&aiParam[1]  ,&PARAMETER.Therm[1].u__0368   ,0  },  
  { CO_Humidity2bytesFB_2         ,16     ,FROMBUS_REL_HUMIDITY2     ,&aiParam[1]  ,&PARAMETER.Therm[1].u__0369   ,4  },   
  { CO_AntistratificationTempFB_2 ,17     ,FROMBUS_ANTISTRAT_TEMP    ,&aiParam[1]  ,&PARAMETER.Therm[1].u__0369   ,0  },
  { CO_OutdoorTempFB_2            ,18     ,FROMBUS_OUTDOOR_TEMP      ,&aiParam[1]  ,&PARAMETER.Therm[1].u__036A   ,4  },
  { CO_CoilTempFB_2               ,19     ,FROMBUS_COIL_TEMP         ,&aiParam[1]  ,&PARAMETER.Therm[1].u__036B   ,0  },  
  { CO_FloorTempFB_2              ,20     ,FROMBUS_FLOOR_TEMP        ,&aiParam[1]  ,&PARAMETER.Therm[1].u__036B   ,4  },
  { CO_FlowTempFB_2               ,21     ,FROMBUS_FLOW_TEMP         ,&aiParam[1]  ,&PARAMETER.Therm[1].u__036C   ,0  },
  
  { CO_AnticondensationFB_2       ,22     ,FROMBUS_ANTICONDESATION   ,&diParam[1]  ,&PARAMETER.Therm[1].u__036F   ,4  },
  { CO_WindowsContactSensor1FB_2  ,23     ,FROMBUS_WINDOWS_CONTACT1  ,&diParam[1]  ,&PARAMETER.Therm[1].u__036F   ,0  },
  { CO_WindowsContactSensor2FB_2  ,24     ,FROMBUS_WINDOWS_CONTACT2  ,&diParam[1]  ,&PARAMETER.Therm[1].u__0370   ,4  },
  { CO_PresenceSensor1FB_2        ,25     ,FROMBUS_PRESENCE1         ,&diParam[1]  ,&PARAMETER.Therm[1].u__0370   ,0  },
  { CO_PresenceSensor2FB_2        ,26     ,FROMBUS_PRESENCE2         ,&diParam[1]  ,&PARAMETER.Therm[1].u__0371   ,4  },
  { CO_ContactHolderFB_2          ,27     ,FROMBUS_CARD_HOLDER       ,&diParam[1]  ,&PARAMETER.Therm[1].u__0371   ,0  },
  { END_TAG                       ,0      ,FROMBUS_END_INX           ,0            ,0           		  ,0 },
},
{  //therm 3 
  { CO_RoomTempFB_3               ,28     ,FROMBUS_ROOM_TEMP         ,&aiParam[2]  ,&PARAMETER.Therm[2].u__0368   ,4  },
  { CO_Humidity1byteFB_3          ,29     ,FROMBUS_REL_HUMIDITY1     ,&aiParam[2]  ,&PARAMETER.Therm[2].u__0368   ,0  },  
  { CO_Humidity2bytesFB_3         ,30     ,FROMBUS_REL_HUMIDITY2     ,&aiParam[2]  ,&PARAMETER.Therm[2].u__0369   ,4  },   
  { CO_AntistratificationTempFB_3 ,31     ,FROMBUS_ANTISTRAT_TEMP    ,&aiParam[2]  ,&PARAMETER.Therm[2].u__0369   ,0  },
  { CO_OutdoorTempFB_3            ,32     ,FROMBUS_OUTDOOR_TEMP      ,&aiParam[2]  ,&PARAMETER.Therm[2].u__036A   ,4  },
  { CO_CoilTempFB_3               ,33     ,FROMBUS_COIL_TEMP         ,&aiParam[2]  ,&PARAMETER.Therm[2].u__036B   ,0  },  
  { CO_FloorTempFB_3              ,34     ,FROMBUS_FLOOR_TEMP        ,&aiParam[2]  ,&PARAMETER.Therm[2].u__036B   ,4  },
  { CO_FlowTempFB_3               ,35     ,FROMBUS_FLOW_TEMP         ,&aiParam[2]  ,&PARAMETER.Therm[2].u__036C   ,0  },
  
  { CO_AnticondensationFB_3       ,36     ,FROMBUS_ANTICONDESATION   ,&diParam[2]  ,&PARAMETER.Therm[2].u__036F   ,4  },
  { CO_WindowsContactSensor1FB_3  ,37     ,FROMBUS_WINDOWS_CONTACT1  ,&diParam[2]  ,&PARAMETER.Therm[2].u__036F   ,0  },
  { CO_WindowsContactSensor2FB_3  ,38     ,FROMBUS_WINDOWS_CONTACT2  ,&diParam[2]  ,&PARAMETER.Therm[2].u__0370   ,4  },
  { CO_PresenceSensor1FB_3        ,39     ,FROMBUS_PRESENCE1         ,&diParam[2]  ,&PARAMETER.Therm[2].u__0370   ,0  },
  { CO_PresenceSensor2FB_3        ,40     ,FROMBUS_PRESENCE2         ,&diParam[2]  ,&PARAMETER.Therm[2].u__0371   ,4  },
  { CO_ContactHolderFB_3          ,41     ,FROMBUS_CARD_HOLDER       ,&diParam[2]  ,&PARAMETER.Therm[2].u__0371   ,0  },
  { END_TAG                       ,0      ,FROMBUS_END_INX           ,0            ,0                             ,0 },
},
{  //therm 4 
  { CO_RoomTempFB_4               ,40     ,FROMBUS_ROOM_TEMP         ,&aiParam[3]  ,&PARAMETER.Therm[3].u__0368   ,4  },
  { CO_Humidity1byteFB_4          ,41     ,FROMBUS_REL_HUMIDITY1     ,&aiParam[3]  ,&PARAMETER.Therm[3].u__0368   ,0  },  
  { CO_Humidity2bytesFB_4         ,42     ,FROMBUS_REL_HUMIDITY2     ,&aiParam[3]  ,&PARAMETER.Therm[3].u__0369   ,4  },   
  { CO_AntistratificationTempFB_4 ,43     ,FROMBUS_ANTISTRAT_TEMP    ,&aiParam[3]  ,&PARAMETER.Therm[3].u__0369   ,0  },
  { CO_OutdoorTempFB_4            ,44     ,FROMBUS_OUTDOOR_TEMP      ,&aiParam[3]  ,&PARAMETER.Therm[3].u__036A   ,4  },
  { CO_CoilTempFB_4               ,45     ,FROMBUS_COIL_TEMP         ,&aiParam[3]  ,&PARAMETER.Therm[3].u__036B   ,0  },  
  { CO_FloorTempFB_4              ,46     ,FROMBUS_FLOOR_TEMP        ,&aiParam[3]  ,&PARAMETER.Therm[3].u__036B   ,4  },
  { CO_FlowTempFB_4               ,47     ,FROMBUS_FLOW_TEMP         ,&aiParam[3]  ,&PARAMETER.Therm[3].u__036C   ,0  },
  
  { CO_AnticondensationFB_4       ,48     ,FROMBUS_ANTICONDESATION   ,&diParam[3]  ,&PARAMETER.Therm[3].u__036F   ,4  },
  { CO_WindowsContactSensor1FB_4  ,49     ,FROMBUS_WINDOWS_CONTACT1  ,&diParam[3]  ,&PARAMETER.Therm[3].u__036F   ,0  },
  { CO_WindowsContactSensor2FB_4  ,50     ,FROMBUS_WINDOWS_CONTACT2  ,&diParam[3]  ,&PARAMETER.Therm[3].u__0370   ,4  },
  { CO_PresenceSensor1FB_4        ,51     ,FROMBUS_PRESENCE1         ,&diParam[3]  ,&PARAMETER.Therm[3].u__0370   ,0  },
  { CO_PresenceSensor2FB_4        ,52     ,FROMBUS_PRESENCE2         ,&diParam[3]  ,&PARAMETER.Therm[3].u__0371   ,4  },
  { CO_ContactHolderFB_4          ,53     ,FROMBUS_CARD_HOLDER       ,&diParam[3]  ,&PARAMETER.Therm[3].u__0371   ,0  },
  { END_TAG                       ,0      ,FROMBUS_END_INX           ,0            ,0                             ,0 },
},*/
};



//------------------------------------------------------------------
// SensorFromBusInit
//------------------------------------------------------------------
void  SensorFromBusInit( void )
{
  WORD cnt=0;
  WORD param;
  WORD roiTime = delayAfterBusOn*10;
  struct THERM *p; 
  
  memset(sensorsFromBusEnabled  ,0,sizeof(sensorsFromBusEnabled));
  memset(sensorsFromBusTimeout  ,0,sizeof(sensorsFromBusTimeout));
  memset(sensorsFromBusError    ,0,sizeof(sensorsFromBusError));
  memset(sensorsFromBusUpdated  ,0,sizeof(sensorsFromBusUpdated));
  memset(sensorsFromBusROI      ,0,sizeof(sensorsFromBusROI));  
  
  for ( BYTE nTherm = 0; nTherm < MAX_THERM; nTherm++ )
  {
      p = (struct THERM*)&_PARTHERM(nTherm);
      
      if ( !(_V1(thermFunctions,nTherm) & FUNCT_THERMOSTAT) ) continue;
      
      diParam[nTherm] = GetIntegerConst(&p->DigitalSensorsFromBusTimeout);
      aiParam[nTherm] = GetIntegerConst(&p->AnalogSensorsFromBusTimeout);
      
      if (p->FlowTempFromBus == ENABLED) 
          __SetSensorFromBusEnabled(nTherm,FROMBUS_FLOW_TEMP);
      
      if (p->FloorTempFromBus == ENABLED) 
          __SetSensorFromBusEnabled(nTherm,FROMBUS_FLOOR_TEMP);
      
      if (p->CoilTempFromBus == ENABLED) 
          __SetSensorFromBusEnabled(nTherm,FROMBUS_COIL_TEMP);
      
      if (p->OutdoorTempFromBus == ENABLED) 
          __SetSensorFromBusEnabled(nTherm,FROMBUS_OUTDOOR_TEMP);
      
      if (p->AntistratTempFromBus == ENABLED) 
          __SetSensorFromBusEnabled(nTherm,FROMBUS_ANTISTRAT_TEMP);
      
      if (p->HumidityFromBus == ENABLED) 
      {
          if (p->HumidityFromBusCODimension == 1)         
              __SetSensorFromBusEnabled(nTherm,FROMBUS_REL_HUMIDITY1);      
          else
              __SetSensorFromBusEnabled(nTherm,FROMBUS_REL_HUMIDITY2);      
      }
      
      if (p->RoomTempFromBus == ENABLED) 
          __SetSensorFromBusEnabled(nTherm,FROMBUS_ROOM_TEMP);          
      
      if (p->AnticondensationFromBus == ENABLED) 
          __SetSensorFromBusEnabled(nTherm,FROMBUS_ANTICONDESATION); 

      if (p->WindowContact1FromBus == ENABLED ) 
          __SetSensorFromBusEnabled(nTherm,FROMBUS_WINDOWS_CONTACT1);    

      if (p->WindowContact2FromBus == ENABLED ) 
          __SetSensorFromBusEnabled(nTherm,FROMBUS_WINDOWS_CONTACT2);   
    
      if (p->PresenceSensor1FromBus == ENABLED ) 
          __SetSensorFromBusEnabled(nTherm,FROMBUS_PRESENCE1);   
      if (p->PresenceSensor2FromBus == ENABLED ) 
          __SetSensorFromBusEnabled(nTherm,FROMBUS_PRESENCE2);    

      if (p->CardHolderContactFromBus == ENABLED )
          __SetSensorFromBusEnabled(nTherm,FROMBUS_CARD_HOLDER);         
        
      cnt = 0;
      while (sensorsFromBusList[nTherm][cnt].CO != END_TAG )
      {
          if ( __IsSensorFromBusEnabled(nTherm,sensorsFromBusList[nTherm][cnt].mask) )
          {
              if (*sensorsFromBusList[nTherm][cnt].time) EZ_StartTimer( SFB_TIMER_TIMEOUT(sensorsFromBusList[nTherm][cnt].nTimer), 
                                                                       *sensorsFromBusList[nTherm][cnt].time, TM_MODE_1S );
              param = GetCyclicReadingInterval((*sensorsFromBusList[nTherm][cnt].timeReadCyclic>>
                                                 sensorsFromBusList[nTherm][cnt].lshitf)&0x0F);
              if (param > 0)
              {
                  EZ_StartTimer( SFB_TIMER_RCYCLIC(sensorsFromBusList[nTherm][cnt].nTimer), roiTime, TM_MODE_1MS ); 
                  roiTime += 250;
              }
              
              if (*sensorsFromBusList[nTherm][cnt].time) 
              {
                  EZ_StartTimer( SFB_TIMER_TIMEOUT(sensorsFromBusList[nTherm][cnt].nTimer), 
                                 *sensorsFromBusList[nTherm][cnt].time, TM_MODE_1S );
              }
          }
          cnt++;
      } 
  }
  
}

//------------------------------------------------------------------
// SensorFromBusTimeoutAndError
//------------------------------------------------------------------
void  SensorFromBusTimeoutAndError( BYTE channel )
{
  WORD cnt=0;
  WORD param;
  void *pCO;
  WORD coNr;
  T_SENSORS_FROMBUS_INX mask;
  WORD timerTimeout;
  WORD paramReadCyclic;
  WORD timerReadCyclic;  

  while (sensorsFromBusList[channel][cnt].CO != END_TAG )
  {
      param = *sensorsFromBusList[channel][cnt].time;
      coNr  = sensorsFromBusList[channel][cnt].CO;
      mask  = sensorsFromBusList[channel][cnt].mask;  
      timerTimeout = SFB_TIMER_TIMEOUT(sensorsFromBusList[channel][cnt].nTimer);
      paramReadCyclic = GetCyclicReadingInterval((*sensorsFromBusList[channel][cnt].timeReadCyclic>>
                                                   sensorsFromBusList[channel][cnt].lshitf)&0x0F);
      timerReadCyclic = SFB_TIMER_RCYCLIC(sensorsFromBusList[channel][cnt].nTimer); 
      pCO = GetObjectValuePtr(coNr);      
      if ( __IsSensorFromBusEnabled(channel,mask) )
      {
          if ( paramReadCyclic > 0)
          {
              if ( EZ_GetState( timerReadCyclic ) )
              {
                  SetReceiveRequest(coNr);
                  EZ_StartTimer( timerReadCyclic, paramReadCyclic, TM_MODE_1S );
              }
          }        
          if ( EZ_GetState(timerTimeout) && (param != 0) )
          {                      
              __SetSensorFromBusTimeout(channel,mask);
              if ( pCO != 0 )
                if (IS_FORMBUS_ANALOG(mask))
                { // verificare c'è anche un sensore da bus grande solo 1 byte !!!
                    *((WORD*)pCO) = SENSOR_INVALID_DATA; // analogico shortfloat
                }
                else
                {
                    *((BYTE*)pCO) = 0; // digitale
                }
          }
          else
          {
              if ( CheckUpdateFlag(coNr) )
              {
                  if ( paramReadCyclic > 0)
                  {
                      EZ_StartTimer( timerReadCyclic, paramReadCyclic, TM_MODE_1S ); 
                  }                
                  if ( (pCO != 0)&&(IS_FORMBUS_ANALOG(mask)) )
                  {   
                      if ( *((WORD*)pCO) == SENSOR_INVALID_DATA )
                          __SetSensorFromBusError(channel,mask);
                      else
                          __ResetSensorFromBusError(channel,mask);
                  }
                  EZ_StartTimer( timerTimeout, param, TM_MODE_1S );
                  __ResetSensorFromBusTimeout(channel,mask);
                  __SetSensorFromBusUdated(channel,mask);
              }
          }
      }
      cnt++;
  } 
}


                                                   
