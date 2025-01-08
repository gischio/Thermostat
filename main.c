//------------------------------------------------------------------
//
// Copyright (c) 1999 - 2009 Tapko Technologies GmbH
//
// PROPRIETARY RIGHTS of Tapko Technologies GmbH are involved
// in the subject matter of this material. All manufacturing,
// reproduction, use, and sales rights pertaining to this subject matter
// are governed by the license agreement. The recipient of this software
// implicitly accepts the terms of the license.
//
// This file is part of the Libraries of Tapko Technologies GmbH
// (e.g. TAPKO EIB/KNX Communication Stack)
//
//------------------------------------------------------------------
// $HeadURL: https://tapko-app.tapko.local:1443/svn/KAIstack/trunk/appl_example/empty_msp430_sysB/src/main.c $
//
// Project:           EIB-Communication-Stack
//
// $Author: Adler_K $
//
// $Date: 2013-07-29 13:31:58 +0200 (Mo, 29 Jul 2013) $
// $Revision: 569 $
//------------------------------------------------------------------
//
// 14/01/2015
// r1.0.1
//
// DCOCLK = 16MHz
//

#include <string.h>
#include "project.h"
#include "eib_stk.h"
#include "app.h"
#include EIB_DEVICE_MODEL_APP_H_FILE
#include "ApplicationUtility.h"
#include "print_cfg.h"

//------------------------------------------------------------------
// main
//------------------------------------------------------------------
void main(void)
{

  SFRRPCR |= SYSNMI;

  hwinit () ;                          // peripheral settings
  // PGM key and LED
  P1REN |= (BIT6);                    // Key: PullUp/Dn enabled
  P1OUT |= (BIT6);                    // Key: Pull: Up
  WDG_STOP();
  mDelay(500);

  WDG_START();

  if ( GetProgramButton() || (DM_eeprom.ADT_physAddr[0] == 0xFF && DM_eeprom.ADT_physAddr[1] == 0xFE) )
  {
      InitProgramLED()
      ResetEEPROM();
      while( GetProgramButton() )
      {
          SetProgramLED(0);           // LED-off
          mDelay(50);
          SetProgramLED(1);           // LED-on
          mDelay(100);
      }
  }

  SC_StackInit();
  WDG_START();

  while (_TRUE)
  {
    WDG_RESET();

#ifdef DEBUG_LOOP_TIME
     P3OUT ^= BIT3;
#endif

    switch(SC_CyclicCalls())
    {
/*      case SC_SLEEP:
        KAI_LpmSleep();
        break;

      case SC_SLEEP:
        KAI_LpmDeepSleep();
        break;
*/
    }
  }
}



