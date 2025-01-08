/**@addtogroup SBS_Code_EK-EQ2-TP
@{
commento.....

Funzioni logiche v2 

*/
//------------------------------------------------------------------
#include "project.h"
#include "eib_stk.h"
#include "app.h"
#include EIB_DEVICE_MODEL_APP_H_FILE      
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "Application.h"
#include "ApplicationUtility.h"
#include "ApplicationTimers.h"
#include "ListValues.h"
#include "LogicFunctions.h"


LF_STATUS   lfStatus[MAX_LOGIC_FUNCTIONS];
BYTE        lfObjReadStatus[MAX_LOGIC_FUNCTIONS];
BYTE        lfObjUpdateStatus[MAX_LOGIC_FUNCTIONS];
BYTE        lfPrevOut[MAX_LOGIC_FUNCTIONS];
BYTE        lfRTEnabled[MAX_LOGIC_FUNCTIONS];


#define DEFAULT_LF_VALUE_NONE   0
#define DEFAULT_LF_VALUE_OFF    1
#define DEFAULT_LF_VALUE_ON     2

const BYTE maskDefaultValue[] = {
  6,4,2,0
};

//-------------------------------------------------------------------- 
// FiltersInit
//-------------------------------------------------------------------- 
void LogicFunctionsInit(void)
{
  T_LOGIC_F       *pLf;
  
  for ( BYTE  lf = 0; lf < MAX_LOGIC_FUNCTIONS; lf++)
  {
      pLf = pLogicF(lf);
      if (( pLf->LogicFunctionEnabled != YES )||( pLf->lfEnableMask == 0 ))
      {
          lfStatus[lf] = LF_DISABLED;
          continue;
      }
      lfStatus[lf] = LF_INIT_BUS_ON_DELAY;
      lfObjUpdateStatus[lf] = ~pLf->lfEnableMask | 0x0F;
      lfObjReadStatus[lf]   = ~pLf->lfEnableMask | 0x0F;
      lfPrevOut[lf] = NAN;
  }
}

//-------------------------------------------------------------------- 
// FilterFunction
//-------------------------------------------------------------------- 
void LogicFunctions(void)
{
    T_LOGIC_F       *pLf;
    BYTE            skipReadObj = NO;
    BYTE            mask;
    unsigned short  param;
    BYTE            firstObj;
    BYTE            lfOut;
    BYTE            lfIn;
    BYTE            defaultValue;
    BYTE            flagTx = 0;
    
    
    for ( BYTE  lf = 0; lf < MAX_LOGIC_FUNCTIONS; lf++)
    {
        pLf = pLogicF(lf);
        if (( pLf->LogicFunctionEnabled != YES )||( pLf->lfEnableMask == 0 ))
          continue;
        
        mask = 0x80;
        switch ( lfStatus[lf] )
        {
        case LF_INIT_BUS_ON_DELAY:
            OBJ_VALUE->LogicFunctionOut[lf] = 0;
            param = GetIntegerConst(&pLf->LogicFunctionsReadDelay); 
            if ( param != 0 )
            {
                lfStatus[lf] = LF_WAIT_BUS_ON_DELAY;
                EZ_StartTimer(TimerLFDelay + lf, param*10L, TM_MODE_1MS);
            }
            else
            {
                lfStatus[lf] = LF_INIT;
            }          
            break;
        case LF_WAIT_BUS_ON_DELAY:
            if ( EZ_GetState(TimerLFDelay + lf ) )
                lfStatus[lf] = LF_INIT;
            break;
            
        case LF_INIT: // richiesta di lettura dei CO
            for ( BYTE  lobj = 0; lobj < MAX_LOGIC_OBJECTS; lobj++,mask>>=1)
            {
                defaultValue = (pLf->lfDefValueMask >> maskDefaultValue[lobj] ) & 0x03;              
                
                if ( (pLf->lfReadAtStartupMask << 4 )& mask ) // read at start up ?
                {
                    if (( pLf->lfEnableMask & mask )&& !(lfObjReadStatus[lf] & mask )&&(!skipReadObj ))
                    {
                        lfObjReadStatus[lf] |= mask;
                        SetReceiveRequest( LOBJ_IN_idx(lf,lobj) );
                        skipReadObj = YES;
                        if ( defaultValue != DEFAULT_LF_VALUE_NONE  )
                        {
                            lfObjUpdateStatus[lf] |= mask;
                            OBJ_VALUE->LogicFunctionIn[lf][lobj] = defaultValue == DEFAULT_LF_VALUE_OFF ? 0:1;
                        }                        
                    }
                }
                else
                {
                    lfObjReadStatus[lf] |= mask;
                    if ( defaultValue != DEFAULT_LF_VALUE_NONE  )
                    {
                        lfObjUpdateStatus[lf] |= mask;
                        OBJ_VALUE->LogicFunctionIn[lf][lobj] = defaultValue == DEFAULT_LF_VALUE_OFF ? 0:1;
                    }
                }
            }
            if ( lfObjReadStatus[lf] == 0xFF ) lfStatus[lf] = LF_WAIT_UPDATE;         
            break;
            
        case LF_WAIT_UPDATE: // attesa lettura/ricezione spontanea dei CO
            for ( BYTE  lobj = 0; lobj < MAX_LOGIC_OBJECTS; lobj++,mask>>=1)
            {
                if ( CheckUpdateFlag(LOBJ_IN_idx(lf,lobj)) )
                {
                    lfObjUpdateStatus[lf] |= mask;
                }
            }                  
            if ( lfObjUpdateStatus[lf] == 0xFF ) 
            {
                lfStatus[lf] = LF_PREREADY;                 
            }
            else
            {
                break;    
            }
        case LF_PREREADY:
            param = GetCyclicSendingInterval(pLf->LogicFunctionOutRT); 
            if ( param != 0 )
            {
                lfRTEnabled[lf] = YES;
                EZ_StartTimer(TimerLFRT + lf, param, TM_MODE_1S);
            }
            lfStatus[lf] = LF_READY;   
            //break;
            
        case LF_READY:
            firstObj = YES;
            lfOut = 0;
            mask = 0x80;
            for ( BYTE  lobj = 0; lobj < MAX_LOGIC_OBJECTS; lobj++,mask>>=1)
            {
                if (pLf->lfEnableMask & mask)
                {
                    if ( CheckUpdateFlag(LOBJ_IN_idx(lf,lobj)) && (pLf->LogicFunctionUpdateOut == 1) )
                    {
                        flagTx = 1;
                    }
                    lfIn = pLf->lfObjNegated & mask ? !OBJ_VALUE->LogicFunctionIn[lf][lobj] : 
                                                       OBJ_VALUE->LogicFunctionIn[lf][lobj]; 
                    if ( firstObj == YES )
                    {
                        lfOut = lfIn;
                        firstObj = NO;
                    }
                    else
                    {
                        switch (pLf->LogicOperationType)
                        {
                        case LOGIC_OPERATION_TYPE_OR:
                            lfOut |= lfIn;
                            break;
            
                        case LOGIC_OPERATION_TYPE_AND:
                            lfOut &= lfIn;
                            break;
            
                        case LOGIC_OPERATION_TYPE_XOR:
                            lfOut ^= lfIn;
                            break;                
                        }              
                    }
                }
            }
            if ( ( lfOut != lfPrevOut[lf] ) ||( flagTx ) )
            {
                flagTx = 0;
                OBJ_VALUE->LogicFunctionOut[lf] = lfOut;
                lfPrevOut[lf] = lfOut;
                if ( ((lfOut == 0)&&(pLf->LogicFunctionOutLimit == LF_OUTONLY_0))||
                     ((lfOut == 1)&&(pLf->LogicFunctionOutLimit == LF_OUTONLY_1))||
                     (pLf->LogicFunctionOutLimit == LF_OUT_ALL) )
                    SetTransmitRequest(LOBJ_OUT_idx(lf));
            }
            else
            {
                if ((lfRTEnabled[lf] == YES)&&(EZ_GetState(TimerLFRT + lf )))
                {
                    param = GetCyclicSendingInterval(pLf->LogicFunctionOutRT); 
                    EZ_StartTimer(TimerLFRT + lf, param, TM_MODE_1S);
                    if ( ((lfOut == 0)&&(pLf->LogicFunctionOutLimit == LF_OUTONLY_0))||
                         ((lfOut == 1)&&(pLf->LogicFunctionOutLimit == LF_OUTONLY_1))||
                         (pLf->LogicFunctionOutLimit == LF_OUT_ALL) )
                        SetTransmitRequest(LOBJ_OUT_idx(lf));
                }
            }
            break;
            
        case LF_DISABLED:
            break;
        }
    }
}
