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
#include "ManualFunction.h"
#include "ComfortExtension.h"
#include "PresenceSensor.h"
#include "Thermostat.h"
#include "SensorFromBus.h"
#include "CardHolder.h"

TYPE_CARD_HOLDER cardHolder[MAX_THERM];

//-----------------------------------------------------------------------------
// CardHolderInit
//-----------------------------------------------------------------------------                                                   
void CardHolderInit(void)
{
  for ( BYTE nTherm = 0; nTherm < MAX_THERM; nTherm++ ) 
  {
      cardHolder[nTherm].cardHolderStatus = OCCUPIED;
      cardHolder[nTherm].prevCardHolderInputStatus = 3;
  }
}

//-----------------------------------------------------------------------------
// CardHolderRead
//-----------------------------------------------------------------------------                                                   
BYTE CardHolderRead(BYTE nTherm)
{

#ifdef EXT_IN_PRESENT  
  /* questo va bene ma non è ottimizzato ! */
  for ( BYTE inx = 0; inx < EXTIN_MAX_INPUTS; inx++)
  {
      if ( EXT_INPUT_TYPE(inx) == EXTIN_CARD_HOLDER )
          return (ExtInput[inx].di&0x1);
  }
#endif
  if   ( (!__IsSensorFromBusTimeout(nTherm,FROMBUS_CARD_HOLDER)) &&
         ( __IsSensorFromBusUpdated(nTherm,FROMBUS_CARD_HOLDER)) ) 
  {
      return ( _OBJV1(ContactHolderFB,nTherm) );  
  }

  return 0x80; /* errore lettura contatto card holder */

}
//-----------------------------------------------------------------------------
// CardHolder
//-----------------------------------------------------------------------------                                                   
BYTE CardHolder(BYTE hvacMode,BYTE nTherm) 
{
  BYTE status;
  WORD param;
  
  if ( !CARDHOLDER_ENABLE(nTherm) )  return hvacMode;

  status = CardHolderRead(nTherm);
  
  if (status == 0x80) 
  {
     EZ_StopTimer( TimerCardHolderRemoval+nTherm ); 
     EZ_StopTimer( TimerCardHolderInsetion+nTherm ); 
     return hvacMode;
  }

  if ( cardHolder[nTherm].prevCardHolderInputStatus != status )
  {
      cardHolder[nTherm].prevCardHolderInputStatus = status;
      if ( status == OCCUPIED )
      {
          param = GetIntegerConst(&_PARTHERM(nTherm).CardHolderInsertionDelay);
          EZ_StartTimer( TimerCardHolderInsetion+nTherm, param, TM_MODE_1S );
          EZ_StopTimer( TimerCardHolderRemoval+nTherm ); 
      }
      else
      {
          param = GetIntegerConst(&_PARTHERM(nTherm).CardHolderRemovalDelay);
          EZ_StartTimer( TimerCardHolderRemoval+nTherm, param, TM_MODE_1S );
          EZ_StopTimer( TimerCardHolderInsetion+nTherm );  
      }
  }

  if ( EZ_GetState(TimerCardHolderInsetion+nTherm) )
  {   /* solo una volta allo scadere del timer */
      cardHolder[nTherm].cardHolderStatus = OCCUPIED;
      switch (_PARTHERM(nTherm).CardHolderInsertion )  
      {
      case CARD_HOLDER_IN_SWITCH_NONE:
        break;
      case CARD_HOLDER_IN_SWITCH_COMFORT:
        hvacMode = HVAC_COMFORT;
        break;
      case CARD_HOLDER_IN_SWITCH_STANDBY:
        hvacMode = HVAC_STANDBY;
        break;
      case CARD_HOLDER_IN_SWITCH_ECONOMY:
        hvacMode = HVAC_ECONOMY;
        break;
      }
  }      
  if ( EZ_GetState(TimerCardHolderRemoval+nTherm)||(cardHolder[nTherm].cardHolderStatus == NOT_OCCUPIED) )
  {   
      cardHolder[nTherm].cardHolderStatus = NOT_OCCUPIED;
      switch (_PARTHERM(nTherm).CardHolderRemoval )  
      {
      case CARD_HOLDER_OUT_SWITCH_NONE:
        break;
      case CARD_HOLDER_OUT_SWITCH_PROTECTION:
        hvacMode = HVAC_PROTECTION;
        break;            
      case CARD_HOLDER_OUT_SWITCH_STANDBY:
        hvacMode = HVAC_STANDBY;
        break;
      case CARD_HOLDER_OUT_SWITCH_ECONOMY:
        hvacMode = HVAC_ECONOMY;
        break;
      }
  }

  return hvacMode;
}