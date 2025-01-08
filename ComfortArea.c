/**@addtogroup SBS_Code_EK-EQ2-TP
@{
commento.....
*/
//------------------------------------------------------------------

#include "project.h"
#include "eib_stk.h"
#include "app.h"
#include EIB_DEVICE_MODEL_APP_H_FILE
#include "ApplicationUtility.h"
#include "ListValues.h"
#include "ApplicationTimers.h"
#include <stdlib.h>
#include <math.h>

BYTE comfort;

//-----------------------------------------------------------------------------
// ComfortAreaInit
//
// 
//-----------------------------------------------------------------------------
void ComfortAreaInit (void)
{
  WORD param;
  
  param = GetCyclicSendingInterval(PARAMETER.ComfortRT);
  if ( param )
  {
      EZ_StartTimer( TimerComfortAreaRT, param, TM_MODE_1S );
  }
  EZ_StartTimer( TimerComfortAreaCO,10, TM_MODE_1S );
  comfort = 99;
}
               
       
//-----------------------------------------------------------------------------
// ComfortArea
//
// 
//-----------------------------------------------------------------------------
void ComfortArea (void)
{
  BYTE appComfort;
  BYTE txFlag = 0;
  
  if (( temperature > ShortF2LongI(PARAMETER.MaxTemp) ) ||
      ( temperature < ShortF2LongI(PARAMETER.MinTemp) ) ||
      ( humidity    > PARAMETER.MaxRelHumidity*100 ) ||  
      ( humidity    < PARAMETER.MinRelHumidity*100 ) ||   
      ( (absHumidity*10) > PARAMETER.MaxAbsHumidity ))    
  {
      appComfort = 0;
  }
  else
  {
      appComfort = 1;
  }   
  
  if (EZ_GetState(TimerComfortAreaCO)) 
  {
      if ( comfort != appComfort )
      {
          comfort = appComfort;
          OBJ_VALUE->ComfortState = comfort;
          txFlag = 1;
      }
      EZ_StartTimer( TimerComfortAreaCO,10, TM_MODE_1S );
  }
  
  if (EZ_GetState(TimerComfortAreaRT)) 
  {
    txFlag = 1;
    EZ_StartTimer( TimerComfortAreaRT, 
                   GetCyclicSendingInterval(PARAMETER.ComfortRT), TM_MODE_1S );
   
  }    
  
  if ( txFlag == 1)  SetTransmitRequest( CO_ComfortState );
}