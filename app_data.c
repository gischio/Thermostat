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
// $HeadURL: https://tapko-app.tapko.local:1443/svn/KAIstack/trunk/appl_example/echo/src/app_data.c $
//
// Project:           EIB-Communication-Stack
//
// $Author: Zivkovic_z $
//
// $Date: 2014-01-22 12:58:13 +0100 (Mi, 22 Jan 2014) $
// $Revision: 846 $
//------------------------------------------------------------------
#include "project.h"
#include "eib_stk.h"
#include "app.h"
#include EIB_DEVICE_MODEL_APP_H_FILE

//------------------------------------------------------------------
// start of EEPROM data
#include "mem_eeprom.h"
#ifdef BCU_EEPROM_INIT
//------------------------------------------------------------------
// initialization of EEPROM-constants
//------------------------------------------------------------------
#ifndef ONLY_EEPROM
BCU_DEFINE_EEPROM_INIT = {
   0x00,
   0xFF,0xFE
   //EIB_ADDR(0xFFFE)     // se systemB
};
#endif

#ifndef ONLY_EEPROM
const DM_SEEPROM cpyDM_eeprom = {
#else
BCU_DEFINE_EEPROM_INIT = {
    SYS_EEPROM1
#endif
// ------------------------------------   address table
#define KAI_DEF_ADDR_TAB
#include "def_CommObj.h"
#include "cotab.h"
// ------------------------------------   association table
#define KAI_DEF_ASSOC_TAB
#include "def_CommObj.h"
#include "cotab.h"
// ------------------------------------   object table
#define KAI_DEF_CO_TAB
#include "def_CommObj.h"
#include "cotab.h"
// ------------------------------------   easy parameter
#ifdef EIB_EASY_PARAMETER_SIZE
   {0},
#endif
// ------------------------------------   parameter (APP_Parameter)
//   {
#include "C_initAPP_Parameters.h"
//   },
// ------------------------------------   applikation EEPROM (APP_Eeprom)
#ifdef APPLICATION_EEPROM

   {
      0,                    // param_crc16
      0xFF55,               // marker
      0,                    // dummy1
      0,                    // dummy2
      0,0,0,0,              // baselineCO2
      700,                  // tempThreshold1
      4500,                 // tempThreshold2
      0,                    // tempLock
      0,                    // tempLock
      6500,                 // rhThreshold1
      6500,                 // rhThreshold2
      0,                    // rhLock
      0,                    // rhLock
      0,0,0,                // CO2Lock[3]
      0,                    // CO2VentLock
      0,                    // CO2VentilationValueAtBusOff
      0,                    // CO2VentilationDisableFB
      127,                  // ledIntensityFromBus
#include "Thermostat_defEEPROM.h"
      0,                    // download
   },

#endif
   SYS_EEPROM2
    };

MEMSEG_EEPROM char DM_easyChannelInfo[8] = {0,0,0,0,0,0,0,0};
#endif

#ifdef BCU_EEPROM_NO_INIT
BCU_DEFINE_EEPROM_NO_INIT;
MEMSEG_EEPROM char DM_easyChannelInfo[8];
#endif

//------------------------------------------------------------------
// end of EEPROM data
#include "mem_default.h"
//------------------------------------------------------------------

#define KAI_DEF_CO_PTR
#include "def_CommObj.h"
#include "cotab.h"


//------------------------------------------------------------------
// start of dummy segment
#include "mem_dummy.h"
//------------------------------------------------------------------

//------------------------------------------------------------------
// Declaration of the ETS Parameter
//------------------------------------------------------------------
//PAR_<type>_<length>_<offset>_<name>
//ETS_PARAMETER (delayTime, UI_16_0);

//------------------------------------------------------------------
// end of dummy segments
#include "mem_default.h"
//------------------------------------------------------------------


#ifdef EIB_INTERFACE_OBJECTS
#define OBJECT_TYPE_KAI_DEMO       0xff01

INTERFACE_OBJECT_START(Prop_KAIdemo, OBJECT_TYPE_KAI_DEMO)
  //DEF_PROPERTY (200, propRW, IOC_PTR_DATA,             PDT_INT,   &PARAMETER.delayTime),
INTERFACE_OBJECT_END()

const BCU_ObjectTable MN_userObjects =
{
  (const BCU_ObjectDesc *) &Prop_KAIdemo,
  0
};
#endif // EIB_INTERFACE_OBJECTS

