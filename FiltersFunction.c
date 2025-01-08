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
#include <string.h>
#include "ApplicationUtility.h"
#include "ApplicationTimers.h"
#include "ListValues.h"
#include "FiltersFunction.h"


const BYTE filterType[3] = {
  1,
  3,
  6,
};

long* const pValue[NR_FILTER] = {
  &rCO2Value,
  &rTemperature[0],  
  &rRhValue,
};

long* const pDstValue[NR_FILTER] = {
#ifdef FIR
  &fAppCO2Value,
#else  
  &fCO2Value,
#endif  
  &fTemperature[0],  
  &fRhValue,
};

BYTE filter[NR_FILTER];
int parOffset[NR_FILTER];
long accVal[NR_FILTER];
BYTE accCnt[NR_FILTER];

//-------------------------------------------------------------------- 
// FiltersInit
//-------------------------------------------------------------------- 
void FiltersInit(void)
{
  
  memset(accVal,0,NR_FILTER*4);
  memset(accCnt,0,NR_FILTER);  
  
  if ( PARAMETER.CO2EnableSensor )
  {
      parOffset[0] = GetCO2Offset(PARAMETER.CO2MeasureOffset)*100; ;
      EZ_StartTimer( TimerFilterAI1+0, 1, TM_MODE_1MS ); 
      filter[0] = filterType[PARAMETER.CO2MeasureFilterType]*10;                
  }
  if ( PARAMETER.TempEnableSensor )
  {
      parOffset[1] = GetTemperatureOffset(PARAMETER.TempMeasureOffset)*10; 
      EZ_StartTimer( TimerFilterAI1+1, 1, TM_MODE_1MS ); 
      filter[1] = filterType[PARAMETER.TempMeasureFilterType];                
  }
  if ( PARAMETER.RhEnableSensor )
  {
      parOffset[2] = GetRhOffset(PARAMETER.RhMeasureOffset)*100; 
      EZ_StartTimer( TimerFilterAI1+2, 1, TM_MODE_1MS ); 
      filter[2] = filterType[PARAMETER.RhMeasureFilterType];                
  }      
}

//-------------------------------------------------------------------- 
// FilterFunction
//-------------------------------------------------------------------- 
void FilterFunction(void)
{
    for ( BYTE sensorN=0; sensorN < NR_FILTER; sensorN++)
    { 
#ifdef CO2_VALUE_FROM_BUS
        if ( sensorN == 0 ) continue;
#endif      
#ifdef TEMP_VALUE_FROM_BUS
        fTemperature[0] = rTemperature[0]*10;
        if ( sensorN == 1 ) continue;
#endif      
#ifdef RH_VALUE_FROM_BUS
        if ( sensorN == 2 ) continue;
#endif      
        
        if ( EZ_GetState(TimerFilterAI1+sensorN) )
        {  
            if ( *(long*)pValue[sensorN] <= -9998 )
            {
                *(long*)pDstValue[sensorN] = SENSOR_ERROR;
                accVal[sensorN] = 0;
                accCnt[sensorN] = 0;  
            }
            else
            {
                accVal[sensorN] += (*(long*)pValue[sensorN] * 10);
                accVal[sensorN] += parOffset[sensorN];
                if ( accCnt[sensorN] < MAX_ACC_CNT )
                {
                    accCnt[sensorN]++;
                }
                else
                {
                    accVal[sensorN] -= *(long*)pDstValue[sensorN];
                }
                *(long*)pDstValue[sensorN] = accVal[sensorN] / accCnt[sensorN]; 
            }
            if ( *(long*)pDstValue[sensorN] == SENSOR_ERROR )
            {  
                EZ_StartTimer( TimerFilterAI1+sensorN, 100, TM_MODE_1MS );
            }
            else
            {
                EZ_StartTimer( TimerFilterAI1+sensorN, filter[sensorN], TM_MODE_1S );
            }
        }
    }
}
