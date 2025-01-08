
#ifndef APPLICATION_H
#define APPLICATION_H

#define SINGLE_THERMOSTAT

typedef unsigned long   DWORD;
typedef unsigned short  WORD;
typedef unsigned char   BYTE;
typedef unsigned char   BITS;
typedef unsigned char   BOOL;

#define ENTER_CRITICAL_SECTION()  __bic_SR_register(__SR_GIE);
#define EXIT_CRITICAL_SECTION()   __bis_SR_register(__SR_GIE);

#define STARATURA_RH            +1000 // 1/100 %

#define float2  unsigned short

#define SAVE_TIME_DELAY                         2 // secondi

#define xxxTEST_EEPROM_WRITE
#define xxxTEST_CE
#define xxxSHT21_SEND_RAW_VALUE
#define xxxSGP30_SEND_RAW_VALUE
#define xxxCO2_VALUE_FROM_BUS
#define xxxRH_VALUE_FROM_BUS
#define xxxTEMP_VALUE_FROM_BUS
#define SETPONT_STANDBY_ECONOMY_FLOAT
#define xxxGESTIONE_ETS_SETPOINT_VERS1
#define FIR

#ifdef TEST_EEPROM_WRITE
#warning ========================================================
#warning TEST EEPROM (FLASH) WRITE !!!!
#warning ========================================================
#endif

#define MAX_INPUTS                              4
#define INPUTS                                  4
#define MAX_CHANNELS                            (MAX_INPUTS/2)
#define CHANNELS                                (INPUTS/2)
#define STATUS                                  8                   // max send values x channel
#define MAX_LOGIC_FUNCTIONS                     8
#define MAX_LOGIC_OBJECTS                       4
#define MAX_THERM                               1   //4

#define HVAC_MAX_SCENE                          8

#define OUT_CHANNELS                            1
#define OUTPUTS                                 2
#define OUT_NUM_SCENES                          8

//------------------------------------------------------------------
#define MIN_CYCLIC_TIME                         10  //sec
/* */
#define TRUE                                    (1 == 1)
#define FALSE                                   (1 == 0)
#define NAN                                     0x55

#define TRIFASE                                 0
#define MONOFASE                                1
/* Filter type  */
#define LOW                                     0
#define MEDIUM                                  1
#define HIGH                                    2
/* Threshold  */
#define NOT_ACTIVE                              0
#define BELOW                                   1
#define ABOVE                                   2
/* hyst position */
#define HYST_BELOW                              0
#define HYST_ABOVE                              1

#define NOT_INVERTED                            0
#define INVERTED                                1

#define CHN_DISABILITATO                        0
#define CHN_INTERNO                             1
#define CHN_ESTERNO                             2

#define ONLY_READING                            0
#define ONLY_TIME_COV                           1
#define ONLY_COV                                0
#define ALWAYS_CYCLIC                           1
#define NO_TRANSMISSION                         1
#define CYCLIC_AND_COV                          2


/*  */
#define NO                                      0
#define YES                                     1
/* */
#define _1BYTE                                  1
#define _2BYTE                                  2
#define ENABLE                                  1
#define ENABLED                                 1
#define DISABLE                                 0
#define DISABLED                                0
#define OFF                                     0
#define ON                                      1

#define SOTTO                                   0
#define SOPRA                                   1

#define NR_ADCHN                                3
#define CHN_MONOFASE1                           0
#define CHN_MONOFASE2                           1
#define CHN_MONOFASE3                           2
#define CHN_TRIFASE                             3

#define EN_CHN_MONOFASE1                        0x1
#define EN_CHN_MONOFASE2                        0x2
#define EN_CHN_MONOFASE3                        0x4
#define EN_CHN_TRIFASE                          0x8

#define SENSOR_INVALID_DATA                     0xFF7F     // oggetto di comunicazione
#define SENSOR_ERROR                            67076096   // variabile ( conversione CO)
#define F_SENSOR_ERROR                          670760     // variabile ( conversione CO)



#define LED_HEATING                             0x0001
#define LED_COOLING                             0x0002
#define LED_DEHUM                               0x0004
#define LED_HUM                                 0x0008
#define LED_CO2_THRESH0                         0x0080
#define LED_CO2_THRESH1                         0x0040
#define LED_CO2_THRESH2                         0x0020
#define LED_CO2_THRESH3                         0x0010

#define LED_HEATING_MODE                        0x0100
#define LED_COOLING_MODE                        0x0200
#define LED_DEHUM_ENABLE                        0x0400
#define LED_HUM_ENABLE                          0x0800


#define LOCK_NONE               0
#define LOCK_SEND_OFF           1
#define LOCK_SEND_ON            2

#define LOCK_NONE               0
#define LOCK_SEND_VALUE         1
#define PREV_AT_BUS_ON          0
#define LOCK_AT_BUS_ON          1
#define UNLOCK_AT_BUS_ON        2

#define UNLOCK_FB               0
#define LOCK_FB                 1



__no_init extern DWORD           a_marker;
__no_init extern DWORD           z_marker;

#define VALID_RAM_DATA()                        (a_marker == 0x01234567 && z_marker == 0x89ABCDEF)
#define SET_VALID_RAM_DATA()                     a_marker = 0x01234567; z_marker = 0x89ABCDEF;
#define CLEAR_VALID_RAM_DATA()                   a_marker = 0; z_marker = 0;

#define N_COFROMBUS     15

extern BYTE           validBusRecovery;

void      CheckEnabledSensors(void);

/** @defgroup GLOBAL_VAR
 *  @{
 */
/**
* variabili utilizzate dall' interfaccia utente
* e memorizzare in fase di powerdown
*/
typedef struct  {
  BYTE          hvacAuto;                     ///< HVAC auto enable
  BYTE          hvacModeForced;               ///???????< modo forzato HVAC ; 0 -> nessuno
  BYTE          hvacModeFromBus;              ///< modo HVAC impostato da bus
  BYTE          hvacMode;                     ///< modo HVAC impostato
  BYTE          hvacModeManual;               ///< modo manuale HVAC impostato
  BYTE          DisableTempController;
  BYTE          setpointHumidification;       ///< setpoint umidificazione impostato (0-255?)
  BYTE          setpointDehumidification;     ///< setpoint deumidificazione impostato (0-255?)
  long          setpointComfort[2];           ///< setpoint (utente) comfort impostato 1/100°C ; 0->cooling, 1->heating
  long          setpointStandby[2];           ///< setpoint (utente) standby impostato 1/100°C ; 0->cooling, 1->heating
  long          setpointEconomy[2];           ///< setpoint (utente) economy impostato 1/100°C ; 0->cooling, 1->heating
  long          setpointProtection[2];        ///< setpoint (utente) protection impostato 1/100°C ; 0->cooling, 1->heating
  long          setpointBaseManuale;
  long          setpointManuale;
  long          setpointOffset;
  int           neutralZone;                  ///< zona neutra , calcolata quando viene scaricato l'applicativo ETS
  BYTE          fanSpeed;                     ///< vel. ventilatore impostata ; 0->off, 1 .. 5->manuale, 6->auto
  BYTE          manualFanSpeed;               ///< vel. ventilatore impostata ; 0->off, 1 .. 5->manuale
  BYTE          heatingCoolingMode;           ///< modo di conduzione impianto 0-> cooling , 1->heating
  BYTE          fansDisabled;                 ///< ventilazione disabilitata da bus
  BYTE          humidificationDisable;        ///< umidificazione disabilitata da bus ?
  BYTE          dehumidificationDisable;      ///< deumidificazione disabilitata da bus ?
  BYTE          auxiliaryHeatingDisabled;
  BYTE          auxiliaryCoolingDisabled;
  BYTE          manualActive;                 ///< funzione setpoint manuale abilitata
} TYPE_TR_DATA ;

typedef struct  {
  BYTE          dummy1;
  BYTE          dummy2;
  BYTE          baselineCO2[4];
  long          tempThreshold1;
  long          tempThreshold2;
  BYTE          tempThreshold1Lock;
  BYTE          tempThreshold2Lock;
  long          rhThreshold1;
  long          rhThreshold2;
  BYTE          rhThreshold1Lock;
  BYTE          rhThreshold2Lock;
  BYTE          CO2ThresholdLock[3];
  BYTE          CO2VentilationLock;
  BYTE          CO2VentilationValueAtBusOff;
  BYTE          CO2VentilationDisableFB;
  BYTE          ledIntensityFromBus;
 /*--- Thermostat   ----*/
#ifdef SINGLE_THERMOSTAT
  TYPE_TR_DATA  therm;
  #define       _therm(x)   therm
#else
  TYPE_TR_DATA  therm[1];
  #define       _therm(x)   therm[x]
#endif
  /*--- Outputs ------*/
  //TYPE_OUT_DATA;
} TYPE_SAVED_DATA ;


#define SAVE_START    dummy1

extern unsigned char saveDelay;
extern struct APP_Flash *actualEep;
extern struct APP_Flash *emptyEep;
extern __no_init unsigned long partialDownload;
extern struct APP_Flash shadow_eeprom;


extern struct {
TYPE_SAVED_DATA;
};

/**
* Valori ingressi esterni ...
*/
typedef union {
  long  ai;
  BYTE  di;
} TYPE_EXTIN;


extern WORD delayAfterBusOn;

typedef WORD KEYSTATUS;

extern KEYSTATUS keyLongPress;
extern KEYSTATUS keyShortPress;
extern KEYSTATUS keyRelease;
extern KEYSTATUS input_bits;

extern BYTE manualEnabled;

/***********************************************/

/**
 * @}
 */

void FunctionsSet(void);
void SensorFromBusEnableSet(void);


#define LUSetTransmitRequest(o)   if ( AL_IsObjLinked(o) ) \
                                  SetTransmitRequest(o);

#define toPERC(x)                 ((x*100L)/255)
#define toX10PERC(x)              ((x*1000L)/255)

typedef enum
{
  MS_wait_bus_recovery    = 0,
  MS_co_read_on_init      = 1,
  MS_enter_normal_run     = 2,
  MS_normal_run           = 3,
  MS_enter_manual_run     = 4,
  MS_manual_run           = 5,
  MS_wait_power_recovery  = 6,
} MS_State;

extern MS_State mainState;

extern DWORD alarms;
extern DWORD warnings;

#define AD_READY          20

extern unsigned int adValue[];
extern long         rTemperature[INPUTS];
extern long         fTemperature[INPUTS];
extern long         rCO2Value;
#ifdef FIR
extern long         fAppCO2Value;
#endif
extern long         fCO2Value;
extern long         rRhValue;
extern long         fRhValue;


#ifdef SINGLE_THERMOSTAT
#define _V1(x,t)             x
#define _V2(x,t,i)           x[i]
#else
#define _V1(x,t)             x[t]
#define _V2(x,t,i)           x[t][i]
#endif

#define ManualModeStatus    ManualSetpointStatus

#endif // APPLICATION_H
