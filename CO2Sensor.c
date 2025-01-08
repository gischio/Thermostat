/**@addtogroup SBS_Code_EK-EQ2-TP
@{
commento.....
*/
//------------------------------------------------------------------

#include "project.h"
#include "eib_stk.h"
#include "app.h"
#include EIB_DEVICE_MODEL_APP_H_FILE      
#include <math.h>
#include <stdlib.h>
#include "ApplicationUtility.h"
#include "ApplicationTimers.h"
#include "ListValues.h"
#include "CO2Sensor.h"
#include "SHT21.h"
#ifdef FIR
#include "FiltroPrimoOrdine.h"
#endif

long    prevCO2Value;

//-------------------------------------------------------------------- 
// CO2SensorInit
//-------------------------------------------------------------------- 
void CO2SensorInit(void)
{
  WORD param;
  
  if ( param = GetCyclicSendingInterval(PARAMETER.CO2MeasureRT) )
  {
      EZ_StartTimer( TimerCO2SensorRT, param, TM_MODE_1S );
  }  
    
}

//-------------------------------------------------------------------- 
// CO2Sensor
//-------------------------------------------------------------------- 
void CO2Sensor(void)
{
  BYTE txflag = 0;
  long param;
  
  
#ifdef FIR
  if ( tm.elapsed1min )
  {
      SetNewValue(fAppCO2Value);
      // Is following float->long conversion intended??
      fCO2Value = (long)GetFilteredValue();
  }
#endif 
  
#ifndef CO2_VALUE_FROM_BUS
#ifdef SGP30_SEND_RAW_VALUE            
  extern unsigned int rCO2RawValue;
  OBJ_VALUE->CO2Value = rCO2RawValue;
#else
  OBJ_VALUE->CO2Value = LongI2ShortF(fCO2Value);
#endif  
#endif  
 

  param = ShortF2LongI(PARAMETER.CO2MeasureCOV);
  if ( param != 0 )
  {
      if ( labs( prevCO2Value - fCO2Value) > (param*100) )
      {
          prevCO2Value = fCO2Value;
          txflag = 1;
      }
  }
  /* RT */
  if (EZ_GetState(TimerCO2SensorRT)) 
  {
    txflag = 1;
    EZ_StartTimer( TimerCO2SensorRT, 
                   GetCyclicSendingInterval(PARAMETER.CO2MeasureRT), TM_MODE_1S );
  }
  
  if ( txflag )  SetTransmitRequest( CO_INX_THERM(nTherm,CO_CO2Value) );
}