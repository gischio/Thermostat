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
#include "ApplicationTimers.h"
#include "ManualFunction.h"
#include "ComfortExtension.h"

#define xxxSPEEDUP_COMFORT_EXT

#ifdef SPEEDUP_COMFORT_EXT
    #warning ===============================================================================
    #warning >>>>> ATTENZIONE !
    #warning >>>>> TEST VELOCE DELLA FUNZIONE COMFORT EXTENSION ATTIVATO 
    #warning ===============================================================================
#endif

TYPE_COMFORT_EXT ce;


//-----------------------------------------------------------------------------
// ComfortExtensionStartFunction
//-----------------------------------------------------------------------------                                                   
BYTE ComfortExtensionStartFunction(void) 
{
  if ( ce.comfortExtEnabled  == NO )
  {
    ce.comfortExtEnabled  = YES;
    ManualExitFromFunction();
  }
  
  return ce.comfortExtEnabled ;
}

//-----------------------------------------------------------------------------
// ComfortExtensionExitFromFunction
//-----------------------------------------------------------------------------                                                   
BYTE ComfortExtensionExitFromFunction(void) 
{
  if ( ce.comfortExtEnabled  == YES )
  {
    ce.comfortExtEnabled  = NO;
    ManualExitFromFunction();
    EZ_StopTimer(TimerEndOfComfortExt); 
  }
  
  return ce.comfortExtEnabled ;
}
//-----------------------------------------------------------------------------
// ComfortExtensionInit
//-----------------------------------------------------------------------------                                                   
void ComfortExtensionInit(void) 
{
   ce.comfortExtEnabled = NO;
   ce.comfortExtFunctionEnable = 
     (GeEndOfComfortExtension(PARAMETER.ComfortExtensionHVACMode) != 0);

}
//-----------------------------------------------------------------------------
// ComfortExtension
//-----------------------------------------------------------------------------                                                   
BYTE ComfortExtension(BYTE mode) 
{
  WORD param;
  
  if ( ce.comfortExtFunctionEnable == NO ) ce.comfortExtEnabled = NO;
  
  if (ce.comfortExtEnabled == YES)
  {
    mode = HVAC_COMFORT;
  }
  
  if ((ce.comfortExtEnabled == YES)&&(ce.comfortExtFunctionEnable == YES))
  {
      param = GeEndOfComfortExtension(PARAMETER.ComfortExtensionHVACMode);
      if ( param )
      {
#ifdef SPEEDUP_COMFORT_EXT 
          EZ_StartNotRunningTimer( TimerEndOfComfortExt, param, TM_MODE_1S );          
#else          
          EZ_StartNotRunningTimer( TimerEndOfComfortExt, param*60L, TM_MODE_1S );         
#endif          
          if ( EZ_GetState(TimerEndOfComfortExt) )
          {
              ComfortExtensionExitFromFunction();
          }
          else
          {
              mode = HVAC_COMFORT;
          }
      }
  }
  else
  {
      EZ_StopTimer(TimerEndOfComfortExt); 
  }

  return mode;
}