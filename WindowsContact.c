/**@addtogroup SBS_Code_EK-EQ2-TP
@{
commento.....
*/
//------------------------------------------------------------------

#include "project.h"
#include "eib_stk.h"
#include "app.h"
#include EIB_DEVICE_MODEL_APP_H_FILE      
#include "ApplicationTimers.h"
#include "SensorFromBus.h"
#include "Thermostat.h"
#include "WindowsContact.h"

TYPE_WINDOW_CONTACT _V1(winc,MAX_THERM);

//------------------------------------------------------------------
// WindowsContactInit
//
//------------------------------------------------------------------
BYTE WindowsContactInit( void ) 
{
  return 1;
}

//------------------------------------------------------------------
// WindowsContactRead
//
// 0 -> closed
// 1 -> open
//
//------------------------------------------------------------------
BYTE WindowsContactRead(BYTE nTherm) 
{
  BYTE status = 0;
  BYTE val = 0;

#ifdef EXT_IN_PRESENT  
  /* external input */
  for ( BYTE inx = 0; inx < EXTIN_MAX_INPUTS; inx++)
  {
      if ( EXT_INPUT_TYPE(inx) == EXTIN_WINDOWS_CONTACT )
          status |= ExtInput[inx].di&0x1;
  }
#endif  
  /* communication objects */
  if ( _PARTHERM(nTherm).WindowContact1FromBus == ENABLE )
  {
      if ( (!__IsSensorFromBusTimeout( nTherm,FROMBUS_WINDOWS_CONTACT1))&& 
             __IsSensorFromBusUpdated( nTherm,FROMBUS_WINDOWS_CONTACT1) )
      {
          val = _OBJV1(WindowsContactSensor1FB,nTherm)&0x01;
          status |= (_PARTHERM(nTherm).WindowContact1FromBusInv == YES) ? (val^0x01) : val;
      }
  }
  if ( _PARTHERM(nTherm).WindowContact2FromBus == ENABLE )
  {
      if ( (!__IsSensorFromBusTimeout( nTherm,FROMBUS_WINDOWS_CONTACT2))&& 
             __IsSensorFromBusUpdated( nTherm,FROMBUS_WINDOWS_CONTACT2) )
      {
          val = _OBJV1(WindowsContactSensor2FB,nTherm)&0x01; 
          status |= (_PARTHERM(nTherm).WindowContact2FromBusInv == YES) ? (val^0x01) : val;
      }
  }
    
  return status;
}
//------------------------------------------------------------------
// WindowsContactStatus
//------------------------------------------------------------------
BYTE WindowsContactStatus( BYTE hvacMode,BYTE nTherm ) 
{
  /* leggi sempre anche se non è attiva la funzione , il valore viene solo visualizzato ! */
  _V1(winc,nTherm).windowsContactStatus = WindowsContactRead(nTherm);
  
  if ( _PARTHERM(nTherm).WindowContactFunctionality == DISABLE ) return hvacMode;
  
  if ( _V1(winc,nTherm).windowsContactStatus != _V1(winc,nTherm).prevWindowsContactStatus )
  {
      if ( _V1(winc,nTherm).windowsContactStatus == OFF )
      {
          _V1(winc,nTherm).windowsProtectionActive = NO;
          EZ_StopTimer( TimerWindowsContact+nTherm );  
      }
      else
      {
          EZ_StartTimer( TimerWindowsContact+nTherm, GetIntegerConst(&_PARTHERM(nTherm).WindContTimeBeforeActivation), TM_MODE_1S );
      }
  }
  
  if ( EZ_GetState(TimerWindowsContact+nTherm) ) 
      _V1(winc,nTherm).windowsProtectionActive = YES;

  _V1(winc,nTherm).prevWindowsContactStatus = _V1(winc,nTherm).windowsContactStatus;
  hvacMode = (_V1(winc,nTherm).windowsProtectionActive == YES) ? HVAC_PROTECTION : hvacMode;
  
  return hvacMode;
}
