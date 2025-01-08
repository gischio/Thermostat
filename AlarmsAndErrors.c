/**@addtogroup SBS_Code_EK-EQ2-TP
@{
commento.....
*/
//------------------------------------------------------------------
#include <string.h>
#include <stdio.h>
#include "project.h"
#include "eib_stk.h"
#include "app.h"
#include EIB_DEVICE_MODEL_APP_H_FILE      
#include "ListValues.h"
#include "ApplicationTimers.h"
#include "AlarmsAndErrors.h"
#include "CommandFromBus.h"
#include "Anticondensation.h"
#include "SensorFromBus.h"
#include "FloorTempLimitation.h"
#include "Thermostat.h"

unsigned long _V1(errorSended,MAX_THERM); 
unsigned long _V1(errors,MAX_THERM);

BYTE extInAlarm;

//-----------------------------------------------------------------------------
// AlarmsAndErrorsInit
//-----------------------------------------------------------------------------                                                   
void AlarmsAndErrorsInit(void) 
{
   
   memset(&_V1(errors,0),0,sizeof(errors));
   memset(&_V1(errorSended,0),0,sizeof(errorSended));
   extInAlarm = 0;
}
//-----------------------------------------------------------------------------
// AlarmsAndErrors
//-----------------------------------------------------------------------------                                                   
BYTE AlarmsAndErrors(BYTE nTherm) 
{
  unsigned long k;
  char *coTextError;
  char strapp[4];
  BYTE errN;


  if ( (_V1(temperature,0) == SENSOR_ERROR )||
       (_V1(thermLock,0) == YES ) )
      extInAlarm = YES;  
  else
      extInAlarm = NO;
  
  if ( _OBJV1(TempControllerAlarm,nTherm)!= extInAlarm )
  {
      _OBJV1(TempControllerAlarm,nTherm)= extInAlarm; 
      LUSetTransmitRequest(CO_INX_THERM(nTherm,CO_TempControllerAlarm_1));
  }

  _V1(errors,nTherm) = 0;
  
  _V1(errors,nTherm) |= ( _V1(ftc,nTherm).heatingTempLimitActivated == YES )          ? A_TEMP_LIMIT          : 0;  
  _V1(errors,nTherm) |= ( fTemperature[nTherm] == SENSOR_ERROR )                      ? A_TEMP_SENSOR         : 0;
  /*  sensor error from bus */
  _V1(errors,nTherm) |= ( __IsSensorFromBusError(nTherm,FROMBUS_ROOM_TEMP))           ? E_TEMP_SENSOR_FB      : 0;
  _V1(errors,nTherm) |= ( __IsSensorFromBusError(nTherm,FROMBUS_REL_HUMIDITY1))       ? E_REL_HUMIDITY_FB     : 0;    
  _V1(errors,nTherm) |= ( __IsSensorFromBusError(nTherm,FROMBUS_REL_HUMIDITY2))       ? E_REL_HUMIDITY_FB     : 0;      
  _V1(errors,nTherm) |= ( __IsSensorFromBusError(nTherm,FROMBUS_ANTISTRAT_TEMP))      ? E_TEMP_ANTISTRAT_FB   : 0;      
  _V1(errors,nTherm) |= ( __IsSensorFromBusError(nTherm,FROMBUS_OUTDOOR_TEMP))        ? E_TEMP_OUTDOOR_FB     : 0;        
  _V1(errors,nTherm) |= ( __IsSensorFromBusError(nTherm,FROMBUS_COIL_TEMP))           ? E_TEMP_COIL_TEMP_FB   : 0;          
  _V1(errors,nTherm) |= ( __IsSensorFromBusError(nTherm,FROMBUS_FLOOR_TEMP))          ? E_TEMP_FLOOR_TEMP_FB  : 0;          
  _V1(errors,nTherm) |= ( __IsSensorFromBusError(nTherm,FROMBUS_FLOW_TEMP))           ? E_TEMP_FLOW_TEMP_FB   : 0;            
  /* analog timeout from bus */  
  _V1(errors,nTherm) |= ( __IsSensorFromBusTimeout(nTherm,FROMBUS_ROOM_TEMP))         ? TO_TEMP_SENSOR_FB     : 0;
  _V1(errors,nTherm) |= ( __IsSensorFromBusTimeout(nTherm,FROMBUS_REL_HUMIDITY1))     ? TO_REL_HUMIDITY_FB    : 0;    
  _V1(errors,nTherm) |= ( __IsSensorFromBusTimeout(nTherm,FROMBUS_REL_HUMIDITY2))     ? TO_REL_HUMIDITY_FB    : 0;      
  _V1(errors,nTherm) |= ( __IsSensorFromBusTimeout(nTherm,FROMBUS_ANTISTRAT_TEMP))    ? TO_TEMP_ANTISTRAT_FB  : 0;      
  _V1(errors,nTherm) |= ( __IsSensorFromBusTimeout(nTherm,FROMBUS_OUTDOOR_TEMP))      ? TO_TEMP_OUTDOOR_FB    : 0;        
  _V1(errors,nTherm) |= ( __IsSensorFromBusTimeout(nTherm,FROMBUS_COIL_TEMP))         ? TO_TEMP_COIL_TEMP_FB  : 0;          
  _V1(errors,nTherm) |= ( __IsSensorFromBusTimeout(nTherm,FROMBUS_FLOOR_TEMP))        ? TO_TEMP_FLOOR_TEMP_FB : 0;          
  _V1(errors,nTherm) |= ( __IsSensorFromBusTimeout(nTherm,FROMBUS_FLOW_TEMP))         ? TO_TEMP_FLOW_TEMP_FB  : 0;   
  /* digital timeout from bus */
  _V1(errors,nTherm) |= ( __IsSensorFromBusTimeout(nTherm,FROMBUS_ANTICONDESATION))   ? TO_DIG_ANTICOND_FB    : 0;      
  _V1(errors,nTherm) |= ( __IsSensorFromBusTimeout(nTherm,FROMBUS_WINDOWS_CONTACT1))  ? TO_DIG_WCONT1_FB      : 0;      
  _V1(errors,nTherm) |= ( __IsSensorFromBusTimeout(nTherm,FROMBUS_WINDOWS_CONTACT2))  ? TO_DIG_WCONT2_FB      : 0;      
  _V1(errors,nTherm) |= ( __IsSensorFromBusTimeout(nTherm,FROMBUS_PRESENCE1))         ? TO_DIG_PRESENCE1_FB   : 0;      
  _V1(errors,nTherm) |= ( __IsSensorFromBusTimeout(nTherm,FROMBUS_PRESENCE2))         ? TO_DIG_PRESENCE2_FB   : 0;      
  _V1(errors,nTherm) |= ( __IsSensorFromBusTimeout(nTherm,FROMBUS_CARD_HOLDER))       ? TO_DIG_CARDHOLDER_FB  : 0;        

  coTextError = (char*)_OBJV1(AlarmText,0);
  CommandFromBus(nTherm);
    
  if ( tm.elapsed1sec == NO) return 1;
  k = 1;
  for ( WORD inx = 0; inx < 32; inx++ , k<<=1)
  {
      if ( _V1(errors,nTherm) & k )
      {
          if ( !(_V1(errorSended,nTherm) & k) )
          {
              memset(coTextError,0,sizeof(_OBJV1(AlarmText,0)));
              strcpy(coTextError,"ALARM  ");
              if ( inx >= ALARM_W_OFFSET )
              {
                  coTextError[6] = 'W';
                  errN = inx - ALARM_W_OFFSET + 1;
              }
              else  if ( inx >= ALARM_E_OFFSET )
              {
                  coTextError[6] = 'E';
                  errN = inx - ALARM_E_OFFSET + 1;
              }
              else
              {
                  coTextError[6] = 'A';
                  errN = inx + 1;
              }
              sprintf(strapp,"%d",errN);
              strcat(coTextError,strapp);
              _V1(errorSended,nTherm) |= k;
              LUSetTransmitRequest(CO_INX_THERM(nTherm,CO_AlarmText));
              break;
          }
      }
      else
      {
          if ( _V1(errorSended,nTherm) & k )
          {
              memset(coTextError,0,sizeof(_OBJV1(AlarmText,0)));
              strcpy(coTextError,"NO ALARM  ");
              if ( inx >= ALARM_W_OFFSET )
              {
                  coTextError[9] = 'W';
                  errN = inx - ALARM_W_OFFSET + 1;
              }
              else  if ( inx >= ALARM_E_OFFSET )
              {
                  coTextError[9] = 'E';
                  errN = inx - ALARM_E_OFFSET + 1;
              }
              else
              {
                  coTextError[9] = 'A';
                  errN = inx + 1;
              }
              sprintf(strapp,"%d",errN);              
              strcat(coTextError,strapp);
              _V1(errorSended,nTherm) &= ~k;
              LUSetTransmitRequest(CO_INX_THERM(nTherm,CO_AlarmText));
              break;
          }
      }
  }

  return 1;
}