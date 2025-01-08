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
#include "ScenesHVAC.h"

BYTE  HVACSceneBehavior[MAX_THERM][HVAC_MAX_SCENE];

//-----------------------------------------------------------------------------
// ScenesHVAC
//-----------------------------------------------------------------------------                                                   
BYTE ScenesHVAC(BYTE modo,BYTE nTherm) {
  BYTE indx;
  BYTE mask = 0x80;
  BYTE nrScene;
  BYTE updateFlag;
  long _param;
  BYTE flgChange = 0;
  struct THERM *p = (struct THERM*)&PARAMETER.Therm[nTherm];
  
  if (p->SceneFunction == ENABLE )
  {
      updateFlag = CheckUpdateFlag(CO_INX_THERM(nTherm,CO_HVACSceneNumberControl_1));
      nrScene = (OBJ_VALUE->HVACSceneNumberControl[nTherm] & 0x3F)+1;
      for ( indx = 0; indx < HVAC_MAX_SCENE; indx++,mask>>=1)
      {
          if (( p->HVACSceneNumber[indx] == nrScene  )&&(updateFlag))
          {
            if ( p->HVACSceneEnabled & mask ) 
            {
              if (( OBJ_VALUE->HVACSceneNumberControl[nTherm] & SCENE_LEARNING_BIT )&&( p->HVACSceneLearningMode & mask ))
              {
                if ( HVACSceneBehavior[nTherm][indx] !=  modo )
                {
                  HVACSceneBehavior[nTherm][indx] =  modo;
                }
              }
              else
              {
                _param = GetIntegerConst(&p->HVACSceneActivationDelay[indx]);
                if ( _param )
                {
                  EZ_StartTimer(TimerSceneActivationDelay+indx,_param*100,TM_MODE_1MS);                  
                }
                else
                {
                  if ( HVACSceneBehavior[nTherm][indx] == 0 )
                  {
                      if ( PARAMETER.StandaloneOrSlave == STAND_ALONE_CHRONO )
                      {
                        therm[nTherm].hvacAuto = TRUE;
                        modo = therm[nTherm].hvacModeFromBus;
                        flgChange = 1;
                      }
                  }
                  else
                  {
                      flgChange = 1;
                      modo = HVACSceneBehavior[nTherm][indx]; 
                      if ( p->StandaloneOrSlave == STAND_ALONE_CHRONO )
                          therm[nTherm].hvacAuto = FALSE;
                  }
                }
              }
            }
          }
          if (EZ_GetState(TimerSceneActivationDelay+indx))
          {
              if ( HVACSceneBehavior[nTherm][indx] == 0 )
              {
                  if ( p->StandaloneOrSlave == STAND_ALONE_CHRONO )
                  {
                    therm[nTherm].hvacAuto = TRUE;
                    modo = therm[nTherm].hvacModeFromBus;
                    flgChange = 1;
                  }
              }
              else
              {
                  flgChange = 1;
                  modo = HVACSceneBehavior[nTherm][indx]; 
                  if ( p->StandaloneOrSlave == STAND_ALONE_CHRONO )
                      therm[nTherm].hvacAuto = FALSE;
              }
          }
      }
  }
  
  if ( flgChange == 1)
  {
      ManualExitFromFunction(nTherm);
      ComfortExtensionExitFromFunction();
  }
  
  return modo;
  
}