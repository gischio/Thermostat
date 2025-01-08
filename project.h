//{{TAPKO_EXPORT_MIC
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
// $HeadURL: https://tapko-app.tapko.local:1443/svn/KAIstack/trunk/appl_example/echo/src/project.h $
//
// Project:           EIB-Communication-Stack
//
// $Author: Adler_K $
//
// $Date: 2014-02-05 09:44:25 +0100 (Mi, 05 Feb 2014) $
// $Revision: 895 $
//------------------------------------------------------------------
//}}TAPKO_EXPORT_MIC
///@addtogroup KAI_project_h
///@{
#include "constant.h"
#include "application.h"

/**
Indicates that project settings are included
*/
#define PROJECT_SETTINGS

// automatic detection of the win32 project
#ifdef WIN32
#define DEBUG_WIN32          // debug version for PC without EEPROM
#endif
 
//------------------------------------------------------------------
//------------------------------------------------------------------
//{{TAPKO_EXPORT_MIC
#define KAI_DEBUG       1

/**
Name of the project
*/
#define PROJ               dimmer3

//------------------------------------------------------------------
/**
This onstant defines the KNX device model
possible definitions are (not complete):
- #define EIB_DEVICE_MODEL   @ref EIB_DEVICE_MODEL_0705
- #define EIB_DEVICE_MODEL   @ref EIB_DEVICE_MODEL_5705            
- #define EIB_DEVICE_MODEL   @ref EIB_DEVICE_MODEL_0912             
- #define EIB_DEVICE_MODEL   @ref EIB_DEVICE_MODEL_091A   
*/
//#define EIB_DEVICE_MODEL   EIB_DEVICE_MODEL_07B0
#define EIB_DEVICE_MODEL   EIB_DEVICE_MODEL_0705
#define AST_FORMAT 1

#define RF_HARDWARE_TYPE1
#define SPI_RF_UCA1   // for UCA0

#define MAX_RFSOURCE_ADDRESS_FILTER 8

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// enables the EIB-easy controller mode
//#define EIB_EASY_CONTROLLER_MODE
//#define EIB_EASY_PARAMETER_SIZE 1

//------------------------------------------------------------------
// this section defines the board specific settings
// e.g. Microcontroller, KNX-IO ports, frequency, Compiler
//------------------------------------------------------------------

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// KNX_TARGET  - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
/**
defining the target platform
this includes:
- microcontroller family
- compiler, including version
- type of communication interface to KNX (KAIlink)
possible targets are (not complete):
- communication interface: TPUART:
  -  #define KNX_TARGET  MSP430_IAR4_TF
  -  #define KNX_TARGET  MSP430_IAR5_TF
  -  #define KNX_TARGET  M16C_IAR_TF
  -  #define KNX_TARGET  STM8_IAR1_TF       (in preparation)
  -  #define KNX_TARGET  AVR_IAR5_TF        (in preparation)
- communication interface: KAIlink BIT, FZE 1066
  -  #define KNX_TARGET  MSP430_IAR4_DF
  -  #define KNX_TARGET  MSP430_IAR5_DF
@see Targets
*/
#define KNX_TARGET  MSP430_IAR5_TF
//#define KNX_TARGET  STM32_GCC_TF
//#define KNX_TARGET  STM8_IAR1_TF

//------------------------------------------------------------------
// DERIVATE  - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
/**
Defines the derivate of the microcontroller family.
This variable is used to adapt the register access and the linking
examples for possible definitions (not complete)
- #define DERIVATE           MSP430F149
- #define DERIVATE           MSP430F2370
- #define DERIVATE           MSP430F249
- #define DERIVATE           MSP430F449
- #define DERIVATE           MSP430F5528
- #define DERIVATE           STM8L152C6
- #define DERIVATE           MSP430F5341 
@see Targets
*/
//#define DERIVATE           			 MSP430F5341 
#define DERIVATE           			 MSP430F6724 
//}}TAPKO_EXPORT_MIC




#define XT1_FREQU		     16000000          
#define MSP_TM_TIMERBLOCK            0
#define TUI_UART                     MSP_USCIA1
#define MSP_ALL_CLK_DCO
#define MSP_DCO_FREQU                16000000


#define InitProgramLED()             {P1OUT &= ~BIT7;P1DIR |= BIT7;}
#define GetProgramButton()           ((P1IN & BIT6) == 0)
#define SetProgramLED(state)         {if (state) P1OUT &= ~BIT7; else P1OUT |= BIT7;}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// enables the save functionality of the stack
#define EIB_SAVE
#define MSP_SAVE_PORT 2
#define MSP_SAVE_BIT  0x080
#define MSP_SAVE_LEVEL 0





//------------------------------------------------------------------
/**
If the constant KAISTACK_EVAL_BOARD_H is defined, 
the board definition files for the KAIstack eval boards are included.
If this constant is not defined,
the application specific board definition file (e.g. KAI_board.h) has to be included.
*/
#define KAISTACK_EVAL_BOARD_H


//------------------------------------------------------------------
//------------------------------------------------------------------
// Kind of Application
//------------------------------------------------------------------
//------------------------------------------------------------------

//------------------------------------------------------------------
/**
Defines the type of the application
\n possible values are:
- SC_APPL_CALL_CYCLE: cyclic called application
- SC_APPL_CALL_EVENT: event driven application
- SC_APPL_NONE: no internal application 
*/
//#define SC_APPLICATION_TYPE SC_APPL_CALL_EVENT
#define SC_APPLICATION_TYPE SC_APPL_CALL_CYCLE

//------------------------------------------------------------------
/**
Defines the mode of the programm mode handler.
\n possible values are:
- PKL_KEY_MODE_CYCLE: program mode (and button) is checked cyclic
- PKL_KEY_MODE_EVENT: program button has to be checked by the application
*/
#define PKL_KEY_MODE        PKL_KEY_MODE_CYCLE


//------------------------------------------------------------------
// this section defines required for the make process
// e.g. build type stack, additional defines
//------------------------------------------------------------------
//{{TAPKO_EXPORT_MIC
// stack size
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
#define STACK_SIZE         200

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// additional defines for the compiler / assembler
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
#define APP_DEFINES

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
// defining type of the build
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
#define  BUILD_APPL_FIX 1
//#define BUILD_SYS_LOADABLE 1 
//#define BUILD_APPL_LOADABLE 1
//#define MAKE_USE_LIB        1
#define MAKE_BUILD_LIB      1

//}}TAPKO_EXPORT_MIC
//------------------------------------------------------------------
//------------------------------------------------------------------

//------------------------------------------------------------------
// this section defines the KAIstack specific settings
// e.g. polling cycles
//------------------------------------------------------------------

//------------------------------------------------------------------
/**
Enables additional check routines. E.g. Consistence check of pointer, sorting of tables. 
An error will lead to an error in the corresponding state machine. 
*/
#define MEMORY_CHECK

//------------------------------------------------------------------
/*
This switch enables the property PID_HARDWARE_TYPE in the device object. 
Also the check during the download is enabled. 
The hardware type itself is defined in APP_HW_TYPE in app.h. 
In some device models (x705h, x7B0h, x920h, ..)  the hardware type is enabled by default. 
*/
//#define KNX_HW_TYPE_ENABLE

//------------------------------------------------------------------
//------------------------------------------------------------------
// settings for the message system
//------------------------------------------------------------------
//------------------------------------------------------------------

//------------------------------------------------------------------
/* 
Specifies the number of message buffers for the message system. 
If the number of buffers is too small, blocking situations may occur. 
Tthis switch is set by default in the device model
Min. value for TP end devices: 4 
*/
//#define MS_noOfMessages            4      // number of messages

//------------------------------------------------------------------
/*
Specifies the max APDU length of the telegram.    
KNX standard frame has a maximum APDU length of 15 byte.    
The maximum supported APDU length is 240 byte.
*/
//#define KNX_MAX_APDU_LENGTH        55   // max. APDU length of telegram


//------------------------------------------------------------------
// settings for the application layer (communication object handling)
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
//------------------------------------------------------------------
/**
Specifies the number of communication objects, which are in the state of transmitting. 
If this constant is 0 the limitation is disabled. 
If this value is changed, the number of message buffers has to be adjusted, otherwise it may happen that a dead lock situation occurred. 
*/
#define AL_CO_TransmitLimit         1   
// limit the polling of the communication object
#define AL_CO_PollLimit             50 
// activates the dynamic polling of communiation objects
#define AL_CO_DYNAMIC_POLLING

#define KNX_OBJECT_TAB_SIZE_NO_CHECK
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
//definition of system timer
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
//#define TM_TIMER_NR 3

#define LL_RcvTimer    0
#define LL_SendTimer   1




//------------------------------------------------------------------

//------------------------------------------------------------------
//------------------------------------------------------------------

#ifdef BUILD_SYS_LOADABLE
#define BCU_EEPROM_LENGTH 0x04000
#endif

 

// debug settings
//------------------------------------------------------------------
#ifdef KAI_DEBUG
  #define DBN_MSG_TRACE    (DB_NL)
  #define DBN_LAYER_TRACE

//  #define DB_LL_MSG_TRACE          1      // enable output for messages
//  #define DB_MSG_TRACE             1      // enable output for messages
//  #define DB_APCI_TRACE            1      
//  #define DB_LL_TRACE              1
//  #define DB_TL_TRACE              1 
//  #define DB_AL_TRACE              1
//  #define DB_I2C_TRACE             1

//  #define DB_TRACE
//  #define DB_TRACE_FILE
#endif

///@}

#ifdef EK_ET2_TP
#define KNX_SERIAL_NR_INIT    0x00,0xB6,0x00,0x00,0x00,0x2D
#endif

#define KAI_CONFIG_FILE_NAME         "KAIconfig.dat"
#define KAI_DEFAULT_CONFIG_FILE_NAME "KAIdefaultConfig.dat"

