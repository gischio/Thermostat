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
// $HeadURL: https://tapko-app.tapko.local:1443/svn/KAIstack/trunk/appl_example/demo_dimmer/src/app.h $
//
// Project:           EIB-Communication-Stack
//
// $Author: Adler_K $
//
// $Date: 2013-08-26 10:22:57 +0200 (Mo, 26 Aug 2013) $
// $Revision: 608 $
//------------------------------------------------------------------

//------------------------------------------------------------------
#ifndef APP_H
#define APP_H

//#define SINGLE_THERMOSTAT     // > application.h
#define APPLICATION_EEPROM      1 // enable data stored on EEPROM

//------------------------------------------------------------------
// application / HW IDs
//------------------------------------------------------------------

///=================================================================
/// SWITCH DI COMPILAZIONE - TIPO DISPOSITIVO
/// Definire NELLA CONFIGURAZIONE DEL PROGETTO le seguenti costanti
/// (una sola o più per gruppo a seconda di come indicato):

/// Gruppo OEM (UNA SOLA fra le opzioni):
//#define EKINEX    // Tutti i modelli

/// Gruppo Tipo device (UNA SOLA fra le opzioni):
//#define EK_ET2_TP

/// Gruppo opzioni debug/funzioni (tutte le necessarie):
// (>>> vedi file application.h)

/// =================================================================

#define EK_ET2                  0x4554

/// --------------------------------------------------------
///  FW RELEASE NUMBER
/// --------------------------------------------------------

#define APP_APPL_VERSION    6       // Release version nr.

/// --------------------------------------------------------
///  Internal minor version number
#define FW_MIN_VERSION      0       // Minor version nr.
/// --------------------------------------------------------

#ifdef EKINEX
#define APP_MANUF_ID       0x00B6

#ifdef EK_ET2_TP
/// ========================================================
/// EKINEX EK_ET2_TP
/// ========================================================
// ATTENZIONE - NON TOCCARE KNX_HW_TYPE PERCHE' DEVE CORRISPONDERE AL VALORE INSERITO NELL'APPLICATIVO ETS!
#define KNX_HW_TYPE         'E','T',0x02,0x01,0x00,0x00  // ET2 1=TP
#define BUILD_TYPE          EK_ET2
#define BUILD_APP           EK_ET2      // Usato per determinare la compilazione
#define APP_APPL_ID         EK_ET2      // Usato per l'identificazione da ETS
// Per convenzione i due ultimi byte di APP_ORDER_NR sono la 'major version (release)' e la 'minor version' del FW
#define APP_ORDER_NR        'E','K','E','T','2',0x00,0x00,0x00,(APP_APPL_VERSION),(FW_MIN_VERSION)
//#define FW_VER            "EK-ET2 00x"
#define FW_VER              'E','K','-','E','T','2',' ','0','0'+(APP_APPL_VERSION)/10,'0'+(APP_APPL_VERSION)%10,0x00
#define KNX_FIRMWARE_VER     KNX_VERSION(0,0,APP_APPL_VERSION)
#endif  //EK_ET2_TP

#endif  //EKINEX

// Definizione SERIAL_NUMBER:   (2019-04)
// 0x00 0xB6 = Mfg code (es. Ekinex)
// 0x39      = Codice (ekinex) prodotto (es. LM2=0x39)
// 0x01      = Codice (ekinex) terzista (01=EuroElettronica, FE=Ekinex)
// 0x00 0x00 = Seriale progressivo (da incrementare in fase di programmazione)

// La parte variabile del seriale è quella costituita dagli ultimi 4 byte.
// I RELATIVI DATI VANNO IMPOSTATI NEL FILE DI CONFIGURAZIONE DEL JOB DEL PROGRAMMATORE!
// Questi byte sono qui impostati a 0xFF (valore 'blank' perché in programmazione vengono scritti i soli bit a 0)

// NOTE:
// =====
// - Per poter emettere un unico file .hex per tutti i terzisti, anche il codice terzista deve stare nella parte variabile;
//   per mantenere l'allineamento a indirizzo pari, lo stesso vale per il codice prodotto.
// - I dati KNX_SERIAL_NR_INIT saranno messi nella variabile 'DM_deviceConst'.
// - PER LA POSIZIONE DEFINITIVA IN MEMORIA DI DM_deviceConst CONTROLLARE IL FILE .MAP GENERATO (finché non sarà inserito come
//   assoluto nel file cfg del linker).
// - L'indirizzo di inizio effettivo della parte di seriale da scrivere (indirizzo da inserire nella cfg del programmatore)
//   è (&DM_deviceConst (inizio KNX_SERIAL_NR_INIT) + 2 (inizio parte variabile))

#undef  KNX_SERIAL_NR_INIT
#define KNX_SERIAL_NR_INIT      _HIBYTE(APP_MANUF_ID),_LOBYTE(APP_MANUF_ID),0xFF,0xFF,0xFF,0xFF

#ifndef FW_VER
#error type not defined
#endif

//------------------------------------------------------------------
//------------------------------------------------------------------
// size of systemtables
//------------------------------------------------------------------
//------------------------------------------------------------------
/**
Size of the address table (excluding physical address)
With this constant the amount of memory for the address table is reserved.
The maximum allowed size depends on the device model.
*/

#ifdef EK_ET2_TP
#define APP_addrTabSize      254
#endif
//------------------------------------------------------------------
/**
Size of the association table
With this constant the amount of memory for the association table is reserved.
The maximum size depends on the device model.
*/
#ifdef EK_ET2_TP
#define APP_assocTabSize     254
#endif
//------------------------------------------------------------------
/**
Number of group communication objects.
The maximum size depends on the device model.
*/
#ifdef EK_ET2_TP
#define APP_objectTabSize    138
#endif

//#define EIB_EASY_PARAMETER_SIZE   2    // size of easy parameter

//------------------------------------------------------------------
//------------------------------------------------------------------
//configuration of timer
//------------------------------------------------------------------
//------------------------------------------------------------------

//------------------------------------------------------------------
/**
Specifies the number of timers, which are available.
3 Timer are used by the system. By default the timer numbers 0 to 2 are used.
*/
#define TM_TIMER_NR          3 // number of timer (3 used by system)
#define NI_TIMER_NR          3

#define TimerSend            0


/* EZ_TIMER ... */
typedef enum  {
  nosTimer                          = 0,
  TimerDelayAfterBusRecovery        = nosTimer                            + INPUTS,
  BusMonitorVoltageMonitorTimer     ,
  TimerLed                          ,
  TimerLFDelay                      ,
  TimerLFRT                         = TimerLFDelay                        + MAX_LOGIC_FUNCTIONS,
  TimerFanOutRT                     = TimerLFRT                           + MAX_LOGIC_FUNCTIONS,
  TimerCO2SensorRT                  = TimerFanOutRT                       + MAX_THERM,
  TimerTemperatureSensorRT          = TimerCO2SensorRT                    + MAX_THERM,
  TimerTemperatureThresholdRT       = TimerTemperatureSensorRT            + MAX_THERM,
  TimerRhSensorRT                   = TimerTemperatureThresholdRT         + MAX_THERM,
  TimerRhThresholdsRT               = TimerRhSensorRT                     + MAX_THERM,
  TimerWeightedTemperatureRT        = TimerRhThresholdsRT                 + MAX_THERM,
  TimerHCSwitchover                 = TimerWeightedTemperatureRT          + MAX_THERM,
  TimerSetpointRT                   = TimerHCSwitchover                   + MAX_THERM,
  TimerValveProtection              = TimerSetpointRT                     + MAX_THERM,
  TimerHeatingOutRT                 = TimerValveProtection                + MAX_THERM, //= TimerValveProtection + 4,
  TimerAuxHeatingOutRT              = TimerHeatingOutRT                   + MAX_THERM,
  TimerCoolingOutRT                 = TimerAuxHeatingOutRT                + MAX_THERM,
  TimerAuxCoolingOutRT              = TimerCoolingOutRT                   + MAX_THERM,
  TimerHeatingCoolingRT             = TimerAuxCoolingOutRT                + MAX_THERM,
  TimerSceneActivationDelay         = TimerHeatingCoolingRT               + MAX_THERM,
  TimerDewPointRT                   = TimerSceneActivationDelay           + (MAX_THERM*8),
  TimerWindowsContact               = TimerDewPointRT                     + MAX_THERM,
  TimerAbsenceTime                  = TimerWindowsContact                 + MAX_THERM*2,
  TimerCardHolderInsetion           = TimerAbsenceTime                    + MAX_THERM*2,
  TimerCardHolderRemoval            = TimerCardHolderInsetion             + MAX_THERM,
  TimerFanOnDelay                   = TimerCardHolderRemoval              + MAX_THERM,
  TimerFanOffDelay                  = TimerFanOnDelay                     + MAX_THERM,
  TimerHumCommandRT                 = TimerFanOffDelay                    + MAX_THERM,
  TimerDehCommandRT                 = TimerHumCommandRT                   + MAX_THERM,
  TimerDelayDeh                     = TimerDehCommandRT                   + MAX_THERM,
  TimerEndOfManual                  = TimerDelayDeh                       + MAX_THERM,
  TimerEndOfComfortExt              = TimerEndOfManual                    + MAX_THERM,
  TimerDelayHVACModeChange          = TimerEndOfComfortExt                + MAX_THERM,
  TimerDelayHCModeChange            = TimerDelayHVACModeChange            + MAX_THERM,
  TimerFanSpeedChange               = TimerDelayHCModeChange              + MAX_THERM,
  TimerPassiveAnticond              = TimerFanSpeedChange                 + MAX_THERM,
  TimerActiveAnticond               = TimerPassiveAnticond                + MAX_THERM,
  TimerCOTimeout                    = TimerActiveAnticond                 + MAX_THERM,
  TimerCOReadCyclic                 = TimerCOTimeout                      + N_COFROMBUS*MAX_THERM,
  TimerFilterAI1                    = TimerCOReadCyclic                   + N_COFROMBUS*MAX_THERM,
  TimerFilterAI2                    ,
  TimerFilterAI3                    ,
  TimerCO2VentilationRT             ,
  TimerCO2Threshold1RT              ,
  TimerCO2Threshold2RT              ,
  TimerCO2Threshold3RT              ,
  TimerLast                         ,
} T_NRTIMER ;

#define EZ_TIMER_NR          TimerLast + 1

//------------------------------------------------------------------
// definition of communication objects
//-----------------------------------------------------------------
enum CommObjects {
#define KAI_DEF_CO_ENUM
#include "def_CommObj.h"
#include "coTAb.h"
};

//------------------------------------------------------------------
// definition des BCU-RAM
//------------------------------------------------------------------
typedef struct {
    BYTE            ramFlags[APP_objectTabSize];                // RAM-flags
    BYTE            TechnicalAlarm;
    BYTE            LedsIntensityFB;
    WORD            TempValue;
    BYTE            TempThreshold1Switch;
    BYTE            TempThreshold1Lock;
    WORD            TempThreshold1Value;
    BYTE            TempThreshold2Switch;
    BYTE            TempThreshold2Lock;
    WORD            TempThreshold2Value;
    WORD            CO2Value;
    WORD            RhValue_2bytes;
    BYTE            RhValue_1byte;
    BYTE            RhThreshold1Switch;
    BYTE            RhThreshold1Lock;
    WORD            RhThreshold1_2bytesValue;
    BYTE            RhThreshold1_1byteValue;
    BYTE            RhThreshold2Switch;
    BYTE            RhThreshold2Lock;
    WORD            RhThreshold2_2bytesValue;
    BYTE            RhThreshold2_1byteValue;
    WORD            RoomTempFB;
    WORD            Humidity2bytesFB;
    BYTE            Humidity1byteFB;
    WORD            AntistratificationTempFB;
    WORD            OutdoorTempFB;
    WORD            CoilTempFB;
    WORD            FloorTempFB;
    WORD            FlowTempFB;
    BYTE            WindowsContactSensor1FB;
    BYTE            WindowsContactSensor2FB;
    BYTE            PresenceSensor1FB;
    BYTE            PresenceSensor2FB;
    BYTE            ContactHolderFB;
    BYTE            AnticondensationFB;
    WORD            WeightedTemp;
    BYTE            HeatingCoolingStatusOut;
    BYTE            HeatingCoolingStatusIn;
    BYTE            HVACModeIn;
    BYTE            HVACForcedModeIn;
    BYTE            HVACModeOut;
    BYTE            HVACManualMode;
    BYTE            ChronoModeStatus;
    WORD            ActualSetpoint;
    WORD            ManualSetpoint;
    BYTE            ManualSetpointStatus;
    WORD            SetpointComfort[2];
    WORD            SetpointStandby[2];
    WORD            SetpointEconomy[2];
    WORD            SetpointBuildProtection[2];
    BYTE            DisableTempController;
    BYTE            RoomTempControllerStatus;
    BYTE            HeatingOut;
    BYTE            CoolingOut;
    BYTE            AuxiliaryHeatingOut;
    BYTE            AuxiliaryCoolingOut;
    BYTE            AuxiliaryHeatingDisable;
    BYTE            AuxiliaryCoolingDisable;
    BYTE            FanContinuousSpeed;
    BYTE            FanSpeed[3];
    BYTE            FanControlDisable;
    BYTE            FanStepSpeed;
    BYTE            FanManualSpeedStep;
    BYTE            FanManualSpeedPercentage;
    BYTE            FanManualOperation;
    BYTE            FanManualOffStatus;
    BYTE            TempControllerAlarm         ;
    WORD            DewPointTemp                ;
    WORD            HumiditySetpointForDehum    ;
    WORD            HumiditySetpointForHum      ;
    BYTE            DehumCommand                ;
    BYTE            DehumWaterBatteryCommand    ;
    BYTE            DehumIntegrationControl     ;
    BYTE            DehumControlDisable         ;
    BYTE            HumidCommand                ;
    BYTE            HumidControlDisable         ;
    BYTE            AnticondensationAlarm       ;
    BYTE            ThermalGeneratorLock        ;
    BYTE            HVACProtectionModeStatus    ;
    BYTE            HVACScene;
    BYTE            AlarmText[15];
    BYTE            CO2Treshold1Percentage;
    BYTE            CO2Threshold1Lock;
    BYTE            CO2Treshold2Percentage;
    BYTE            CO2Threshold2Lock;
    BYTE            CO2Treshold3Percentage;
    BYTE            CO2Threshold3Lock;
    BYTE            CO2Treshold4Percentage;
    BYTE            CO2Threshold4Lock;
    BYTE            CO2VentilationPercentage;
#define             CO2VentilationCounter         CO2VentilationPercentage
#define             CO2VentilationBit1            CO2VentilationPercentage
    BYTE            CO2VentilationBit2;
    BYTE            CO2VentilationBit3;
    BYTE            CO2VentilationBit4;
    BYTE            CO2VentilationLock;
    BYTE            CO2VentilationDisableFB;
//    BYTE            CO2VentilationSceneNumberControl;
    BYTE            LogicFunctionIn             [MAX_LOGIC_FUNCTIONS][MAX_LOGIC_OBJECTS];
    BYTE            LogicFunctionOut            [MAX_LOGIC_FUNCTIONS];
} APP_Ram;


#ifdef SINGLE_THERMOSTAT
#define _OBJV1(o,t)     OBJ_VALUE->o
#define _OBJV2(o,t,i)   OBJ_VALUE->o[i]
#else
#define _OBJV1(o,t)     OBJ_VALUE->o[t]
#define _OBJV1(o,t,i)   OBJ_VALUE->o[t][i]
#endif


//------------------------------------------------------------------
/**
@brief definition of EEPROM-Parameter for ETS
The parameters are defined in the structure APP_Parameter in app.h.
The parameters are defined as a part of the structure DM_EEPROM,
which contains the whole EEPROM data.
At least one parameter has to be defined.
This structure can contain other structures or arrays.
The parameters are initialised in the file app_data.c.
*/
//------------------------------------------------------------------
// definition of EEPROM-Parameter for ETS
//------------------------------------------------------------------

/************************* Funzioni logiche v2 *******************************/
typedef struct {
      union {
                    BYTE      data_offset_0170;
                    struct {
                    BITS      LogicFunctionOutRT            : 4;
                    BITS      r__0170_3                     : 1;
                    BITS      LogicOperationType            : 2;
                    BITS      LogicFunctionEnabled          : 1;
                    };
      };
      union {
                    BYTE      data_offset_0171;
                    struct {
                    BITS      r__0171_3                     : 5;
                    BITS      LogicFunctionUpdateOut        : 1;
                    BITS      LogicFunctionOutLimit         : 2;
                    };
      };
      WORD         LogicFunctionsReadDelay;
      union {
                    BYTE      data_offset_0174;
                    struct {
                    BITS      r__0174_4                     : 4;
                    BITS      LogicObj4Enabled              : 1;
                    BITS      LogicObj3Enabled              : 1;
                    BITS      LogicObj2Enabled              : 1;
                    BITS      LogicObj1Enabled              : 1;
                    };
      };
      union {
                    BYTE      data_offset_0175;
                    struct {
                    BITS      LogicObj4ReadAtStartup        : 1;
                    BITS      LogicObj3ReadAtStartup        : 1;
                    BITS      LogicObj2ReadAtStartup        : 1;
                    BITS      LogicObj1ReadAtStartup        : 1;
                    BITS      LogicObj4Negated              : 1;
                    BITS      LogicObj3Negated              : 1;
                    BITS      LogicObj2Negated              : 1;
                    BITS      LogicObj1Negated              : 1;
                    };
      };
      union {
                    BYTE      data_offset_0176;
                    struct {
                    BITS      LogicObject4DefaultValue      : 2;
                    BITS      LogicObject3DefaultValue      : 2;
                    BITS      LogicObject2DefaultValue      : 2;
                    BITS      LogicObject1DefaultValue      : 2;
                    };
      };
      BYTE          r__0177;

  } T_LOGIC_F;
#define lfEnableMask          data_offset_0174
#define lfReadAtStartupMask   data_offset_0175
#define lfObjNegated          data_offset_0175
#define lfDefValueMask        data_offset_0176

/************************* fine Funzioni logiche v2 **************************/


typedef struct
{
    union {                                                     //
        BYTE           data_offset_0000;                        //
        struct{                                                 //
        BITS           LogicFunctions       : 1;                //
        BITS           TechnicalAlarm       : 1;                //
        BITS           u__0000_0            : 6;                //
        };                                                      //
    };                                                          //
    union {
        BYTE           data_offset_0001;
        struct{
        BITS           LedsIntensity        : 4;
        BITS           u__0001_4            : 3;
        BITS           LedsIntensityFromBus : 1;
        };
    };
    BYTE            r_0002;
    BYTE            r_0003;
    WORD            DelayAfterBusVoltageRecovery;
    BYTE            r_0006;
    BYTE            r_0007;
    BYTE            r_0008;
    BYTE            r_0009;
    BYTE            r_000A;
    BYTE            r_000B;
    WORD            CO2MeasureCOV;
    union {
        BYTE           data_offset_000E;
        struct{
        BITS           CO2EnableSensor      : 1;
        BITS           u__000E_6            : 1;
        BITS           CO2MeasureFilterType : 2;
        BITS           CO2MeasureRT         : 4;
        };
    };
    BYTE            r_000F;
    BYTE            r_0010;
    BYTE            r_0011;
    BYTE            r_0012;
    BYTE            r_0013;
    BYTE            r_0014;
    BYTE            CO2MeasureOffset;
    BYTE            r_0016;
    BYTE            r_0017;
    WORD            TempMeasureCOV;
    union {
        BYTE           data_offset_001A;
        struct{
        BITS           TempEnableSensor      : 1;
        BITS           u__001A_6             : 1;
        BITS           TempMeasureFilterType : 2;
        BITS           TempMeasureRT         : 4;
        };
    };
    union {
        BYTE           data_offset_001B;
        struct{
        BITS           TempThreshold2        : 2;
        BITS           TempThreshold1        : 2;
        BITS           TempThresholdstRT     : 4;
        };
    };
    WORD            TempThreshold1Value;
    WORD            TempThreshold2Value;
    BYTE            TempThresholdsHyst;
    BYTE            TempMeasureOffset;
    union {
        BYTE           data_offset_0022;
        struct{
        BITS           TempThreshold1LockBehaviourAfterBusOn  : 2;
        BITS           u__0022_4                              : 2;
        BITS           TempThreshold1LockBehaviour            : 2;
        BITS           TempThreshold1EnableLock               : 1;
        BITS           TempThreshold1EnableValueFB            : 1;
        };
    };
    union {
        BYTE           data_offset_0023;
        struct{
        BITS           TempThreshold2LockBehaviourAfterBusOn  : 2;
        BITS           u__0023_4                              : 2;
        BITS           TempThreshold2LockBehaviour            : 2;
        BITS           TempThreshold2EnableLock               : 1;
        BITS           TempThreshold2EnableValueFB            : 1;
        };
    };
    WORD            RhMeasureCOV;
    union {
        BYTE           data_offset_0026;
        struct{
        BITS           RhEnableSensor      : 1;
        BITS           u__0026_6           : 1;
        BITS           RhMeasureFilterType : 2;
        BITS           RhMeasureRT         : 4;
        };
    };
    union {
        BYTE           data_offset_0027;
        struct{
        BITS           RhThreshold2        : 2;
        BITS           RhThreshold1        : 2;
        BITS           RhThresholdsRT      : 4;
        };
    };
    BYTE            RhThreshold1Value;
    BYTE            r_0029;
    BYTE            RhThreshold2Value;
    BYTE            r_002B;
    union {
        BYTE           data_offset_002C;
        struct{
        BITS           u__002C_4           : 4;
        BITS           RhThresholdsHyst    : 4;
        };
    };
    BYTE            RhMeasureOffset;
    union {
        BYTE           data_offset_002E;
        struct{
        BITS           RhThreshold1LockBehaviourAfterBusOn  : 2;
        BITS           u__002E_4                            : 2;
        BITS           RhThreshold1LockBehaviour            : 2;
        BITS           RhThreshold1EnableLock               : 1;
        BITS           RhThreshold1EnableValueFB            : 1;
        };
    };
    union {
        BYTE           data_offset_002F;
        struct{
        BITS           RhThreshold2LockBehaviourAfterBusOn  : 2;
        BITS           u__002F_4                            : 2;
        BITS           RhThreshold2LockBehaviour            : 2;
        BITS           RhThreshold2EnableLock               : 1;
        BITS           RhThreshold2EnableValueFB            : 1;
        };
    };
    BYTE            r_0030;
    BYTE            r_0031;
    BYTE            r_0032;
    BYTE            r_0033;
    union {
        BYTE           data_offset_0034;
        struct{
        BITS           u__0034_3              : 5;
        BITS           LedCO2Enable           : 1;
        BITS           LedRhEnable            : 1;
        BITS           LedTermEnable          : 1;
        };
    };
    BYTE            r_0035;
    BYTE            r_0036;
    BYTE            r_0037;
    /* TERMOREGOLAZIONE */
    struct THERM {
        union {
            BYTE          RCyclicHumRoomT;
            struct{
            BITS          Humidity1ByteFromBusReadCyclic          : 4;
            BITS          RoomTempFromBusReadCyclic               : 4;
            };
        };
        union {
            BYTE          RCyclicAntistratHum;
            struct{
            BITS          AntistrtificationTempFromBusReadCyclic  : 4;
            BITS          Humidity2ByteFromBusReadCyclic          : 4;
            };
        };
        union {
            BYTE          RCyclicOutdoorT;
            struct{
            BITS          r__033A_4                               : 4;
            BITS          OutdoorTempFromBusReadCyclic            : 4;
            };
        };
        union {
            BYTE          RCyclicFloorTCoilT;
            struct{
            BITS          FloorTempFromBusReadCyclic              : 4;
            BITS          CoilTempFromBusReadCyclic               : 4;
            };
        };
        union {
            BYTE          RCyclicFlowT;
            struct{
            BITS          r__003C_4                               : 4;
            BITS          FlowTempFromBusReadCyclic               : 4;
            };
        };
        BYTE              r__003D;
        BYTE              r__003E;
        union {
            BYTE          RCyclicWCont1Anticond;
            struct{
            BITS          WindowContact1FromBusReadCyclic     : 4;
            BITS          AnticondensationFromBusReadCyclic   : 4;
            };
        };
        union {
            BYTE          RCyclicPres1WCont1;
            struct{
            BITS          PresenceSensor1FromBusReadCyclic    : 4;
            BITS          WindowContact2FromBusReadCyclic     : 4;
            };
        };
        union {
            BYTE          RCyclicCardHPres11;
            struct{
            BITS          CardHolderContactFromBusReadCyclic  : 4;
            BITS          PresenceSensor2FromBusReadCyclic    : 4;
            };
        };
        BYTE              r__0042;
        BYTE              r__0043;
        BYTE              r__0044;
        union {
            BYTE          u__0045;
            struct {
            BITS          r__0045_4                           : 4;
            BITS          DisableTempContrSignal              : 1;
            BITS          DisableTempContrFB                  : 1;
            BITS          SetpointManagement                  : 2;
            };
        };
        WORD              DigitalSensorsFromBusTimeout;
        union {
            BYTE          AIFromBus;
            struct {
            BITS          FlowTempFromBus         : 1;
            BITS          FloorTempFromBus        : 1;
            BITS          CoilTempFromBus         : 1;
            BITS          LightSensorFromBus      : 1;
            BITS          OutdoorTempFromBus      : 1;
            BITS          AntistratTempFromBus    : 1;
            BITS          HumidityFromBus         : 1;
            BITS          RoomTempFromBus         : 1;
            };
        };
        union {
            BYTE          DIFromBus;
            struct {
            BITS          CardHolderContactInv        : 1;
            BITS          AnticondensationContactInc  : 1;
            BITS          AnticondensationFromBus     : 1;
            BITS          CardHolderContactFromBus    : 1;
            BITS          PresenceSensor2FromBus      : 1;
            BITS          PresenceSensor1FromBus      : 1;
            BITS          WindowContact2FromBus       : 1;
            BITS          WindowContact1FromBus       : 1;
            };
        };
        union {
            BYTE          u__004A;
            struct {
            BITS          PresenceSensor2FromBusInv  : 1;
            BITS          PresenceSensor1FromBusInv  : 1;
            BITS          WindowContact2FromBusInv   : 1;
            BITS          WindowContact1FromBusInv   : 1;
            BITS          HumidityFromBusCODimension : 4;
            };
        };
        BYTE              r__004B;
        BYTE              r__004C;
        union {
            BYTE          u__004D;
            struct {
            BITS          RelativeWeight             : 4;
            BITS          r__004D_4                  : 4;
            };
        };
        WORD              ChangeOfValueWeightedTemp;
        union {
            BYTE          u__0050;
            struct {
            BITS          r__0050_4                  : 4;
            BITS          RepetitionTimeWeightedTemp : 4;
            };
        };
        BYTE              r__0051;
        union {
            BYTE          u__0052;
            struct {
            BITS          HeatingCoolingRT        : 4;
            BITS          HeatingCoolingSwitchOver: 2;
            BITS          ThermostatFunction      : 2;
            };
        };
        union {
            BYTE          u__0053;
            struct {
            BITS          HvacModeAfterDownload   : 2;
            BITS          r__0053_5               : 2;
            BITS          SetpointRT              : 4;
            };
        };
        BYTE              r__0054;
        union {
            BYTE          r__0355;
            struct {
            BITS          r__0055_5               : 4;
            BITS          EndManualSetpoint       : 4;
            };
        };
        WORD              AnalogSensorsFromBusTimeout;
        WORD              HeatingComfortTemperature;
        WORD              HeatingStandbyTemperature;
        WORD              HeatingEconomyTemperature;
        WORD              HeatingBuildingProtectionTemperature;
        union {
            BYTE          u__0060;
            struct {
            BITS          HeatingHystPosition     : 1;
            BITS          r__0060_6               : 1;
            BITS          CommandCO               : 1;
            BITS          HeatingType             : 3;
            BITS          HeatingControlType      : 2;
            };
        };
        union {
            BYTE          HCntlRT;
            struct {
            BITS          r__0061_4               : 4;
            BITS          HeatingControlRT        : 4;
            };
        };
        BYTE              HeatingHysteresisRange;
        BYTE              HeatingControlCOV;
        WORD              HeatingPWMCycleTime;
        BYTE              HeatingProportionalBand;
        BYTE              HeatingIntegralTime;
        union {
            BYTE          u__0068;
            struct {
            BITS          r__0068_7               : 1;
            BITS          HeatingFloorTempLimit   : 1;
            BITS          ValveProtectionTime     : 3;
            BITS          ValveProtectionFrequency: 2;
            BITS          ValveProtection         : 1;
            };
        };
        union {
            BYTE          u__0069;
            struct {
            BITS          r__0069_4               : 4;
            BITS          AuxHeatingVentilation   : 1;
            BITS          AuxHeatingDiabledFromKNX: 1;
            BITS          AuxHeatingCommandCO     : 1;
            BITS          AuxiliaryHeating        : 1;
            };
        };
        WORD              HeatingTempLimitValue;
        BYTE              AuxHeatingOffset;
        BYTE              AuxHeatingHysteresisRange;
        union {
            BYTE          HAuxCntlRT;
            struct {
            BITS          r__006E_4               : 4;
            BITS          AuxHeatingControlRT     : 4;
            };
        };
        BYTE              TempLimitHysteresisRange;
        BYTE              HeatingMinControlValue;
        BYTE              HeatingMaxControlValue;
        WORD              CoolingComfortTemperature;
        WORD              CoolingStandbyTemperature;
        WORD              CoolingEconomyTemperature;
        WORD              CoolingBuildingProtectionTemperature;
        union {
            BYTE          u__007A;
            struct {
            BITS          CoolingHystPosition     : 1;
            BITS          r__007A_5               : 2;
            BITS          CoolingType             : 3;
            BITS          CoolingControlType      : 2;
            };
        };
        union {
            BYTE          CCntlRT;
            struct {
            BITS          r__007B_4               : 4;
            BITS          CoolingControlRT        : 4;
            };
        };
        BYTE              CoolingHysteresisRange;
        BYTE              CoolingControlCOV;
        WORD              CoolingPWMCycleTime;
        BYTE              CoolingProportionalBand;
        BYTE              CoolingIntegralTime;
        BYTE              u__0082;
        union {
            BYTE          u__0083;
            struct {
            BITS          r__0083_7                : 1;
            BITS          ActiveAnticondensation   : 2;
            BITS          AnticondensationWithProbe: 1;
            BITS          AuxCoolingVentilation    : 1;
            BITS          AuxCoolingDiabledFromKNX : 1;
            BITS          r_0383_1                 : 1;
            BITS          AuxiliaryCooling         : 1;
            };
        };
        union {
            BYTE          u__0084;
            struct {
            BITS          r__0084_4               : 4;
            BITS          AnticondAlarmADelay     : 4;
            };
        };
        BYTE              r__0085;
        BYTE              AuxCoolingOffset;
        BYTE              AuxCoolingHysteresisRange;
        BYTE              ActiveAnticondensationHystRange;
        union {
            BYTE          CAuxCntlRT;
            struct {
            BITS          DesignFlowTemperature   : 4;
            BITS          AuxCoolingControlRT     : 4;
            };
        };
        BYTE              CoolingMinControlValue;
        BYTE              CoolingMaxControlValue;
        union {
            BYTE          u__008C;
            struct {
            BITS          r__008C_7               : 1;
            BITS          FanDehumidification     : 1;
            BITS          FanHotStart             : 1;
            BITS          FanManualOperation      : 1;
            BITS          FanControlType          : 2;
            BITS          r__008C_0               : 2;
            };
        };
        BYTE              FanFirstThreshold;
        BYTE              FanSecondThreshold;
        BYTE              FanThirdThreshold;
        BYTE              FanSpeedControlHysteresis;
        BYTE              FanSpeedProportionalBand;
        BYTE              FanSpeedCOV;
        BYTE              FanMinTemperatureToStart;
        BYTE              FanMinTemperatureForDehumidification;
        BYTE              u__0095;
        union {
            BYTE          u__0096;
            struct {
            BITS          FanAntistratificationDifferential: 4;
            BITS          r__0096_0                        : 4;
            };
        };
        BYTE              u__0097;
        BYTE              u__0098;
        BYTE              u__0099;
        union {
            BYTE          u__009A;
            struct {
            BITS          u__009A_4                            : 3;
            BITS          FanDisableVentilationFromBusInverted : 1;
            BITS          FanDisableVentilationFromBus         : 1;
            BITS          FanAntistratification                : 1;
            BITS          u__009A_0                            : 2;
            };
        };
        BYTE              AntistratificationHysteresis;
        union {
            BYTE          u__009C;
            struct {
              BYTE        u__009C_4                       : 4;
              BYTE        FanStartDelay                   : 4;
            };
        };
        union {
            BYTE          u__009D;
            struct {
              BYTE        u__009D_4                       : 4;
              BYTE        FanStopDelay                    : 4;
            };
        };
        WORD              DelayAfterModeChange;
        union {
            BYTE          u__00A0;
            struct {
              BYTE        u__00A0_4                       : 4;
              BYTE        FanControlRT                    : 4;
            };
        };
        BYTE             r__00A1;
        BYTE             r__00A2;
        BYTE             r__00A3;
        union {
            BYTE          u__00A4;
            struct {
            BITS         r__00A4_1               : 7;
            BITS         DewPointTemperature     : 1;
            };
        };
        union {
            BYTE          u__00A5;
            struct {
            BITS         DewPointTemperatureCOV  : 4;
            BITS         DewPointTemperatureRT   : 4;
            };
        };
        WORD            TempValueToSend;
        BYTE             r__00A8;
        BYTE             r__00A9;
        BYTE             r__00AA;
        BYTE             r__00AB;
        BYTE             r__00AC;
        BYTE             r__00AD;
        BYTE             r__00AE;
        union {
            BYTE         u__00AF;
            struct {
            BITS         r__00AF_5                    : 3;
            BITS         SetpointTemperatureLevels    : 1;
            BITS         PresenceSensorsUse           : 2;
            BITS         PresenceSensorFunctionality  : 1;
            BITS         WindowContactFunctionality   : 1;
            };
        };
        WORD             WindContTimeBeforeActivation;
        WORD             PresenceSensTimeBeforeActivation;
        BYTE             r__00B4;
        BYTE             r__00B5;
        BYTE             r__00B6;
        union {
            BYTE          u__00B7;
            struct {
            BITS         r__00B7_6                : 2;
            BITS         CardHolderRemoval        : 2;
            BITS         CardHolderInsertion      : 2;
            BITS         r__00B7_1                : 1;
            BITS         CardHolderFunction       : 1;
            };
        };
        WORD             CardHolderInsertionDelay;
        WORD             CardHolderRemovalDelay;
        BYTE             r__00BC;
        BYTE             r__00BD;
        BYTE             r__00BE;
        BYTE             r__00BF;
        union {
            BYTE         u__00C0;
            struct {
            BITS         DehumidificationIntegration          : 1;
            BITS         DehumidificationWithTemperature      : 1;
            BITS         DehumidificationHysteresis           : 4;
            BITS         Dehumidification                     : 2;
            };
        };
        BYTE             DehumidificationSetPoint;
        WORD             DehumidificationDelay;
        BYTE             DifferenceForIntegration;
        BYTE             IntegrationHysteresis;
        union {
            BYTE          u__00C6;
            struct {
            BITS         r__00C6_6                            : 2;
            BITS         DehumidificationInvertDisableSignal  : 1;
            BITS         DehumidificationDisableFromBus       : 1;
            BITS         DehumidificationControlRT            : 4;
            };
        };
        BYTE             r__00C7;
        BYTE             r__00C8;
        BYTE             r__00C9;
        union {
            BYTE          u__00CA;
            struct {
            BITS         r__00CA_6                 : 2;
            BITS         HumidificationHysteresis  : 4;
            BITS         Humidification            : 2;
            };
        };
        BYTE             HumidificationSetPoint;
        union {
            BYTE          u__00CC;
            struct {
            BITS         r__00CC_6                          : 2;
            BITS         HumidificationInvertDisableSignal  : 1;
            BITS         HumidificationDisableFromBus       : 1;
            BITS         HumidificationControlRT            : 4;
            };
        };
        BYTE             r__00CD;
        BYTE             r__00CE;
        BYTE             r__00CF;
        BYTE             r__00D0;

// scene -----------------------------------

       union {
            BYTE          u__00D1;
            struct {
            BITS         r__00D1_2               : 6;
            BITS         DownloadOverwrite       : 1;
            BITS         r__00D1_0               : 1;
            };
        };
        union {
            BYTE         HVACSceneEnabled;
            struct {
            BITS         Scene8                  : 1;
            BITS         Scene7                  : 1;
            BITS         Scene6                  : 1;
            BITS         Scene5                  : 1;
            BITS         Scene4                  : 1;
            BITS         Scene3                  : 1;
            BITS         Scene2                  : 1;
            BITS         Scene1                  : 1;
            };
        };
        union {
            BYTE         HVACSceneLearningMode;
            struct {
            BITS         LearningMode8           : 1;
            BITS         LearningMode7           : 1;
            BITS         LearningMode6           : 1;
            BITS         LearningMode5           : 1;
            BITS         LearningMode4           : 1;
            BITS         LearningMode3           : 1;
            BITS         LearningMode2           : 1;
            BITS         LearningMode1           : 1;
            };
        };
        BYTE             HVACSceneBehavior[HVAC_MAX_SCENE];
        BYTE             HVACSceneNumber[HVAC_MAX_SCENE];
        WORD             HVACSceneActivationDelay[HVAC_MAX_SCENE];

// fine scene -------------------------------------------
#ifdef SINGLE_THERMOSTAT
    } Therm;
#else
    } Therm[MAX_THERM];
#endif
    BYTE            r__00F4;
    BYTE            r__00F5;
    BYTE            r__00F6;
    BYTE            r__00F7;
    BYTE            r__00F8;
    union {
        BYTE        u__00F9;
        struct {
        BITS        r__00F9_1                : 7;
        BITS        CO2FunctEnable           : 1;
        };
    };
    struct CO2THRESHOLDS {
        WORD            CO2FunctThreshold1Value;
        BYTE            CO2FunctThreshold1Hyst;
        union {
            BYTE        u__00FD;
            struct {
            BITS        CO2FunctThreshold1COType : 3;
            BITS        r__00FD_2                : 3;
            BITS        CO2FunctThreshold1Type   : 2;
            };
        };
        BYTE            CO2FunctThreshold1AboveValueToSend;
        BYTE            CO2FunctThreshold1BelowValueToSend;
        union {
            BYTE        u__0100;
            struct {
            BITS        r__0100_2                : 4;
            BITS        CO2FunctThreshold1RT     : 4;
            };
        };
        union {
            BYTE        u__0101;
            struct {
            BITS        r__0101_7                                 : 1;
            BITS        CO2FunctThreshold1LockBehaviourAfterBusOn : 2;
            BITS        r__0101_3                                 : 2;
            BITS        CO2FunctThreshold1LockBehaviour           : 2;
            BITS        CO2FunctThreshold1EnableLock              : 1;
            };
        };
        BYTE            CO2FunctThreshold1LockValue;
        BYTE            r__0103; //CO2FunctThreshold1UnlockValue;
        BYTE            r__0104;
        BYTE            r__0105;
    } CO2Thresholds[3];

    union {
        BYTE        u__011E;
        struct {
        BITS        r__011E_3                   : 5;
        BITS        CO2VentilationCOType        : 2;
        BITS        CO2VentilationEnableFunct   : 1;
        };
    };
//    BYTE            CO2VentilationValueToSend1;
//    BYTE            CO2VentilationValueToSend2;
//    BYTE            CO2VentilationValueToSend3;
//    BYTE            CO2VentilationValueToSend4;
    BYTE            CO2VentilationValueToSend[4];
    union {
        BYTE        u__0123;
        struct {
        BITS        r__0123_4                   : 4;
        BITS        CO2VentilationOutRT         : 4;
        };
    };
    union {
        BYTE        u__0124;
        struct {
        BITS        r__0124_7                         : 1;
        BITS        CO2VentilationBehaviourAfterBusOn : 2;
        BITS        CO2VentilationDisableInverted     : 1;
        BITS        CO2VentilationDisableFB           : 1;
        BITS        CO2VentilationLockBehaviour       : 2;
        BITS        CO2VentilationEnableLock          : 1;
        };
    };
    BYTE            CO2VentilationLockValue;
    BYTE            CO2VentilationHyst;
    BYTE            CO2VentilationCOV;
    BYTE            CO2VentilationP1Y;
    BYTE            CO2VentilationP2Y;
    WORD            CO2VentilationP1X;
    WORD            CO2VentilationP2X;
    /* FUNZIONI LOGICHE */
    T_LOGIC_F       logicFunctions[MAX_LOGIC_FUNCTIONS];
    BYTE            r_0336;
    BYTE            r_0337;
} xAPP_Parameter;

typedef struct {
  union {
    BYTE  dummy1APP_Parameter[sizeof(xAPP_Parameter)];
    xAPP_Parameter;
  };
#ifndef ONLY_EEPROM
  /* !!! controllo che xAPP_Parameter non sia già allineato , in tal
     caso non bisogna inserire fillerEepromAlignment !!! */
#if BUILD_APP == EK_CG2
  BYTE    fillerEepromAlignment[0x5600-0x5530];
#else
  //BYTE    fillerEepromAlignment[0x4C00-0x4A58];
  BYTE    fillerEepromAlignment[0x4C00-0x4B98];
#endif
#endif
} APP_Parameter;

//------------------------------------------------------------------
// definition of EEPROM-Parameter not in ETS
//-----------------------------------------------------------------
struct APP_Flash {
    WORD            param_crc16;
    WORD            marker;
    TYPE_SAVED_DATA;
    BYTE            download;
};

typedef struct
{
    struct APP_Flash    page1 ;
    BYTE                fillerEepromSector1[512-sizeof(struct APP_Flash)]; /* allineamento a 512 byte!!! */
    struct APP_Flash    page2;
    BYTE                fillerEepromSector2[512-sizeof(struct APP_Flash)]; /* allineamento a 512 byte!!! */
} APP_Eeprom;


#define PARAMETER_DOWNLOADED()        (partialDownload == 0x04071968L)
#define SET_PARAMETER_DOWNLOADED()    (partialDownload = 0x04071968L)
#define CLR_PARAMETER_DOWNLOADED()    (partialDownload = 0x0)

#define PARAMETER_ACCPTED             0x68

#define pLogicF(a)                    ((T_LOGIC_F *)&PARAMETER.logicFunctions[a])

#ifdef SINGLE_THERMOSTAT
#define   _PARTHERM(x)                 PARAMETER.Therm
#define   CO_INX_THERM(t,c)            (c)
#else
#define   _PARTHERM(x)                 PARAMETER.Therm[x]
#define   CO_INX_THERM(t,c)            ((t*68)+c)
#endif

#endif // APP_H
