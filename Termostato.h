
#ifndef APPLICATION_H
#define APPLICATION_H

typedef unsigned long   DWORD;
typedef unsigned short  WORD;
typedef unsigned char   BYTE;
typedef unsigned char   BITS;
typedef unsigned char   BOOL;


#define STARATURA_SONDA_INTERNA   -250

#define HVAC_PROTECTION_STATUS

#ifdef HVAC_PROTECTION_STATUS
#warning ========================================================
#warning HVAC_PROTECTION_STATUS
#warning ========================================================
#endif


//------------------------------------------------------------------
#define MIN_CYCLIC_TIME                         10  //sec

#define STAT_BUS_HALT   0xFE
#define STAT_BUS_DOWN   0xFF
#define STAT_BUS_OFF    0x00
#define STAT_BUS_ON     0x01

/* */
#define TRUE                                    (1 == 1)
#define FALSE                                   (1 == 0)
#define NAN                                     0x55
/** Device operation as */
#define STAND_ALONE                             0
#define SLAVE                                   1
#define STAND_ALONE_CHRONO                      2
/* Temperature displayed unit */
#define CELSIUS                                 0
#define FAHRENHEIT                              1
/* Default displayed information  */
#define ACTUAL_TEMPERATURE                      0
#define TEMPERATURE_SETPOINT                    1
/* Leds correlation */
#define CORRELA_HIGH_INVERSE                    0
#define CORRELA_MEDIUM_INVERSE                  1
#define CORRELA_SMALL_INVERSE                   2
#define CORRELA_NONE                            3
#define CORRELA_SMALL_DIRECT                    4
#define CORRELA_MEDIUM_DIRECT                   5
#define CORRELA_HIGH_DIRECT                     6
/* Button function level  */
#define END_USER_ONLY                           0
#define ADVANCED_USER                           1
#define ADVANCED_USER_AND_SYSTEM                2
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
/* external input type */
#define EXTIN_DISABLED                          0
#define EXTIN_GENERIC_DI                        1
#define EXTIN_WINDOWS_CONTACT                   2
#define EXTIN_CARD_HOLDER                       3
#define EXTIN_ANTICONDENSATION                  4
#define EXTIN_COIL_BATTERY                      5
#define EXTIN_ROOM_TEM                          6
#define EXTIN_ANTISTRATIFICATION                7
#define EXTIN_FLOOR_TEMP                        8
#define EXTIN_OUTDOOR_TEMP                      9
#define EXTIN_GENERIC_NTC                       10
#define IS_EXTIN_ANALOG(x)                      ((x>=EXTIN_COIL_BATTERY)&&(x<=EXTIN_GENERIC_NTC))
/* Contact type */
#define CONTACT_NO                              0
#define CONTACT_NC                              1
/* */
#define NOT_INVERTED                            0
#define INVERTED                                1
/* */
#define SENSOR_INTERNAL                         0
#define SENSOR_INPUT                            1
#define SENSOR_FROM_BUS                         2
/* Visualization type */
#define FULL                                    0
#define PARTIAL                                 1
#define ONLY_TEMPERATURE                        2
/* Behaviour on button press */
#define BACKLIGHT_ONLY                          0
#define BACKLIGHT_AND_FUNCTION                  1
/* Leds Behaviour */
#define FIXED                                   0
#define WHEN_CONTACT_CLOSE                      1
#define STATUS_FROM_BUS                         2
/* Thermostat function */
#define HEATING_FUNCTION                        0
#define COOLING_FUNCTION                        2
#define HEATING_COOLING_FUNCTION                1
/* Command communication object */
#define UNIQUE                                  0
#define SEPARATED                               1
/* Thermostat Switch over */
#define MANUAL                                  0
#define FROM_BUS                                1
#define AUTOMATIC                               2
/* Heating/Cooling type */
#define RADIATOR                                0
#define ELECTRIC                                1
#define FANCOIL                                 2
#define FLOOR_RADIANT                           3
#define CEILING_RADIANT                         4
/* Control type */
#define CONTINUOUS                              0
#define PWM                                     1
#define _2POINTS                                2
/* Ventilation control type */
#define FANS_1SPEED                             0
#define FANS_2SPEED                             1
#define FANS_3SPEED                             2
#define FANS_CONTINUOUS                         3
/* Ventilation manual operation */
#define FANS_NO_TEMP_DEPENDING                  0
#define FANS_TEMP_DEPENDING                     1
/* Dehumidificatio/Humidification function */
#define HD_DISABLE                              0
#define HD_COOLING_ONLY                         1
#define HD_HEATING_ONLY                         2
#define HD_COOLING_AND_HEATING                  3
/* Dehumidificatio function */
#define INDIPENDENT_FROM_TEMPERATURE            0
#define DIPENDENT_ON_TEMPERATURE                1
/* Presence sensors use */
#define COMFORT_EXTENSION                       0
#define COMFORT_LIMITATION                      1
#define COMFORT_EXTENSION_LIMITATION            2
#define COMFORT_STANDBY                         0
#define COMFORT_ECONOMY                         1
/* Card holder switch to */
#define CARD_HOLDER_IN_SWITCH_NONE              0
#define CARD_HOLDER_IN_SWITCH_COMFORT           1
#define CARD_HOLDER_IN_SWITCH_STANDBY           2
#define CARD_HOLDER_IN_SWITCH_ECONOMY           3
//#define CARD_HOLDER_IN_SWITCH_PROTECTION        4
#define CARD_HOLDER_OUT_SWITCH_NONE             0
#define CARD_HOLDER_OUT_SWITCH_PROTECTION       1
//#define CARD_HOLDER_OUT_SWITCH_COMFORT          1
#define CARD_HOLDER_OUT_SWITCH_STANDBY          2
#define CARD_HOLDER_OUT_SWITCH_ECONOMY          3
/* active anticondensation */
#define ENABLE_TPROJ                            1
#define ENABLE_PROBE                            2
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
/* behaviour at locking/unlocking */
#define NONE                                    0
#define AS_CLOSE_SHORT_PRESS                    1
#define AS_OPEN_LONG_PRESS                      2
/* send cyclically  */
//#define NONE                                    0
#define OFF_VALUE_1                             1
#define ON_VALUE_2                              2
#define BOTH                                    3
/* send cyclically  */
//#define NONE                                    0
//#define OFF_VALUE_1                             1
//#define ON_VALUE_2                              2
#define TOGLE                                   3
/* Value send after bus on  */
//#define NONE                                    0
//#define OFF_VALUE_1                             1
//#define ON_VALUE_2                              2
#define PREVIOUS                                  3
/* Dimmer reaction to long press  */
#define DARKER                                  0
#define BRIGHTER                                1
#define TOGLE_DARKER_BRIGHTER                   2
#define PREVIOUS                                3
/* */
#define SIZE_1BIT                               0
#define SIZE_2BIT                               1
#define SIZE_BYTE                               2
#define SIZE_SIGNED_CHAR                        3
#define SIZE_WORD                               4
#define SIZE_SIGNED_SHORT                       5
#define SIZE_FLOAT                              6
#define SIZE_PERCENT                            7
/*  */
#define OFF                                     0
#define ON                                      1
#define COOLING                                 0
#define HEATING                                 1
#define NR_CO_TIMEOUT                           10
#define END_TAG                                 0xFFFF
#define ROCKERS_AS_THERMOSTAT                   0

#define ROCKERS_AS_BUTTON                       1
#define HVAC_AUTO                               0
#define HVAC_COMFORT                            1
#define HVAC_STANDBY                            2
#define HVAC_ECONOMY                            3
#define HVAC_PROTECTION                         4

#define NOT_OCCUPIED                            0
#define OCCUPIED                                1
#define CLOSED                                  0
#define OPEN                                    1

#define SCENE_LEARNING_BIT                      (1<<7)

#define SENSOR_INVALID_DATA                     0xFF7F     // oggetto di comunicazione
#define SENSOR_ERROR                            67076096   // variabile ( conversione CO)
//------------------------------------------------------------------
// functions 
//------------------------------------------------------------------
#define FUNCT_THERMOSTAT                        (1L<<0)
#define FUNCT_FANCOIL                           (1L<<1)
#define FUNCT_VALVE_PROTECTION                  (1L<<2)
#define FUNCT_HUMIDITY_READ                     (1L<<3)
#define FUNCT_HUMIDIFICATION                    (1L<<4)
#define FUNCT_DEHUMIDIFICATION                  (1L<<5)
#define FUNCT_WINDOWS                           (1L<<6)
#define FUNCT_PRESENCE                          (1L<<7)
#define FUNCT_COIL_BATTERY                      (1L<<8)
#define FUNCT_PASS_ANTICOND                     (1L<<9)
#define FUNCT_ACTIVE_ANTICOND                   (1L<<10)
#define FUNCT_ACTIVE_ANTICOND_NOPROB            (1L<<11)
#define FUNCT_CARDHOLDER                        (1L<<12)
#define FUNCT_ANTISTRAT                         (1L<<13)
#define FUNCT_FLOOR_TEMP                        (1L<<14)
#define FUNCT_OUTDOOR_TEMP                      (1L<<15)
#define FUNCT_GENERIC_TEMP                      (1L<<16)
#define FUNCT_PSYCHROMETRIC_VALUE               (1L<<17)
#define FUNCT_WEIGHTED_TEMP                     (1L<<18)
#define FUNCT_MANUAL                            (1L<<19)
#define FUNCT_SETPOINT                          (1L<<20)
//------------------------------------------------------------------
// fancoilFunct 
//------------------------------------------------------------------
#define FUNCT_FANCOIL_HEATING                   (1<<0)
#define FUNCT_FANCOIL_COOLING                   (1<<1)
#define FUNCT_FANCOIL_AUXHEATING                (1<<2)
#define FUNCT_FANCOIL_AUXCOOLING                (1<<3)
//------------------------------------------------------------------
// thermFunct 
//------------------------------------------------------------------
#define FUNCT_THERM_HEATING                     (1<<0)
#define FUNCT_THERM_COOLING                     (1<<1)
#define FUNCT_THERM_AUXHEATING                  (1<<2)
#define FUNCT_THERM_AUXCOOLING                  (1<<3)
//------------------------------------------------------------------
#define VALVE_NR                                4
#define VALVE_HEATING                           (1<<0)
#define VALVE_COOLING                           (1<<1)
#define VALVE_AUXHEATING                        (1<<2)
#define VALVE_AUXCOOLING                        (1<<3)
//------------------------------------------------------------------
#define VALVE_COUPLED_PRIMARY                   (1<<0)
#define VALVE_COUPLED_AUX                       (1<<1)

#define EXT_IN1                                 0
#define EXT_IN2                                 1

#define MAX_SP_HUMI                             8000
#define MIN_SP_HUMI                             2000
#define MAX_SP_DEHUMI                           8000
#define MIN_SP_DEHUMI                           2000


#define IS_HEATING_AND_COOLING_ENABLED          ((thermFunc & FUNCT_THERM_HEATING)&& \
                                                 (thermFunc & FUNCT_THERM_COOLING))

#define IS_HUMIDITY_ENABLED       ((PARAMETER.Humidification == HD_COOLING_AND_HEATING)|| \
                                   ( (heatingCoolingMode == COOLING)&&(PARAMETER.Humidification == HD_COOLING_ONLY) )|| \
                                   ( (heatingCoolingMode == HEATING)&&(PARAMETER.Humidification == HD_HEATING_ONLY)) ) 

#define IS_DEHUMIDITY_ENABLED      ( ( PARAMETER.Dehumidification == HD_COOLING_AND_HEATING )|| \
                                     ( (heatingCoolingMode == COOLING)&&(PARAMETER.Dehumidification == HD_COOLING_ONLY) )|| \
                                     ( (heatingCoolingMode == HEATING)&&(PARAMETER.Dehumidification == HD_HEATING_ONLY) ) )

/* CO ricevuti dal bus ! */
typedef enum {
       FROMBUS_ROOM_TEMP         = 0,
       FROMBUS_REL_HUMIDITY1        ,
       FROMBUS_REL_HUMIDITY2        ,   
       FROMBUS_ANTISTRAT_TEMP       ,
       FROMBUS_OUTDOOR_TEMP         ,
       FROMBUS_LIGHT_SENSOR         ,
       FROMBUS_COIL_TEMP            ,
       FROMBUS_FLOOR_TEMP           ,
       FROMBUS_FLOW_TEMP            ,
       FROMBUS_ANTICONDESATION      ,
       FROMBUS_WINDOWS_CONTACT1     ,
       FROMBUS_WINDOWS_CONTACT2     ,
       FROMBUS_PRESENCE1            ,
       FROMBUS_PRESENCE2            ,
       FROMBUS_CARD_HOLDER          ,
       FROMBUS_END_INX              ,
} T_SENSORS_FROMBUS_INX;

#define IS_FORMBUS_ANALOG(x)               (x<FROMBUS_ANTICONDESATION)

//------------------------------------------------------------------
// macros
/* CO ricevuti dal bus ! */
#define __SetSensorFromBusTimeout(x)            (sensorsFromBusTimeout |= (1L<<x))
#define __ResetSensorFromBusTimeout(x)          (sensorsFromBusTimeout &= ~(1L<<x)) 
#define __IsSensorFromBusTimeout(x)             (sensorsFromBusTimeout & (1L<<x))

#define __SetSensorFromBusError(x)              (sensorsFromBusError |= (1L<<x))
#define __ResetSensorFromBusError(x)            (sensorsFromBusError &= ~(1L<<x)) 
#define __IsSensorFromBusError(x)               (sensorsFromBusError & (1L<<x))

#define __SetSensorFromBusUdated(x)             (sensorsFromBusUpdated |= (1L<<x))
#define __ReseSensorFromBusUdated(x)            (sensorsFromBusUpdated &= ~(1L<<x)) 
#define __IsSensorFromBusUpdated(x)             (sensorsFromBusUpdated & (1L<<x))

#define __SetSensorFromBusEnabled(x)            (sensorsFromBusEnabled |= (1L<<x))
#define __ResetSensorFromBusEnabled(x)          (sensorsFromBusEnabled &= ~(1L<<x)) 
#define __IsSensorFromBusEnabled(x)             (sensorsFromBusEnabled & (1L<<x))

#define __IsSensorFromBusFail(x)                (__IsSensorFromBusError(x) || \
                                                 __IsSensorFromBusTimeout(x) ) 
//------------------------------------------------------------------   
#ifdef SENSOR_FROM_BUS_READ_CYCLIC
typedef struct {
  WORD                  CO;
  WORD                  timer;
  T_SENSORS_FROMBUS_INX mask;
  WORD                  *time;
  WORD                  timerReadCyclic;
  const BYTE            *timeReadCyclic;
  BYTE                  lshitf;  
} T_SENSORS_FROMBUS;
#else
typedef struct {
  WORD                  CO;
  WORD                  timer;
  T_SENSORS_FROMBUS_INX mask;
  WORD                  *time;
} T_SENSORS_FROMBUS;
#endif

extern BYTE           UserChangeHvacMode;

extern WORD           sensorsFromBusEnabled;
extern WORD           sensorsFromBusTimeout;
extern WORD           sensorsFromBusError;
extern WORD           sensorsFromBusUpdated;

extern BYTE           validBusRecovery;
extern const T_SENSORS_FROMBUS SensorsFromBusList[];

extern long           intTemperature;                 ///< temperatura sensore interno

extern DWORD          functions;
extern BYTE           fancoilFunc;
extern BYTE           thermFunc;

extern BYTE           valLedBrightness;     ///< intensità luminosa (0-255) leds (valore usato dalla routine HW)

extern BYTE           chInput;

void      CheckEnabledSensors(void);

/** @defgroup GLOBAL_VAR
 *  @{
 */
/**      
* variabili utilizzate dall' interfaccia utente     
* e memorizzare in fase di powerdown    
*/

typedef struct {
  long          comfort[2];          
  long          standby[2];         
  long          economy[2];         
  long          protection[2];        
} TYPE_SETPOINTS;

typedef struct  {
  BYTE          hvacAuto;                     ///< HVAC auto enable
  BYTE          hvacModeFromBus;              ///< modo HVAC impostato da bus
  BYTE          hvacMode;                     ///< modo HVAC impostato 
  BYTE          hvacModeForced;               ///< modo forzato HVAC ; 0 -> nessuno
  BYTE          setpointHumidification;       ///< setpoint umidificazione impostato (0-255?) 
  BYTE          setpointDehumidification;     ///< setpoint deumidificazione impostato (0-255?)
  
  long          setpointComfort[2];           ///< setpoint (utente) comfort impostato 1/100°C ; 0->cooling, 1->heating 
  long          setpointStandby[2];           ///< setpoint (utente) standby impostato 1/100°C ; 0->cooling, 1->heating 
  long          setpointEconomy[2];           ///< setpoint (utente) economy impostato 1/100°C ; 0->cooling, 1->heating 
  long          setpointProtection[2];        ///< setpoint (utente) protection impostato 1/100°C ; 0->cooling, 1->heating 
  long          defSetpointComfort[2];        ///< setpoint (ets) comfort impostato 1/100°C ; 0->cooling, 1->heating 
  long          defSetpointStandby[2];        ///< setpoint (ets) standby impostato 1/100°C ; 0->cooling, 1->heating 
  long          defSetpointEconomy[2];        ///< setpoint (ets) economy impostato 1/100°C ; 0->cooling, 1->heating 
  long          defSetpointProtection[2];     ///< setpoint (ets) protection impostato 1/100°C ; 0->cooling, 1->heating 
  int           neutralZone;                  ///< zona neutra , calcolata quando viene scaricato l'applicativo ETS
  BYTE          fanSpeed;                     ///< vel. ventilatore impostata ; 0->off, 1 .. 5->manuale, 6->auto
  BYTE          manualFanSpeed;               ///< vel. ventilatore impostata ; 0->off, 1 .. 5->manuale
  BYTE          heatingCoolingMode;           ///< modo di conduzione impianto 0-> cooling , 1->heating 
  BYTE          fansDisabled;                 ///< ventilazione disabilitata da bus
  BYTE          setpointChangeLocked;         ///< variazione del setpoint ; 0->abilitata , 1->disabilitata 
  BYTE          manualChangeLocked;           ///< variazione manuale ; 0->abilitata , 1->disabilitata 
  BYTE          rockersLocked;                ///< utilizzo dei pulsanti ; 0->abilitata , 1->disabilitata 
  BYTE          humidificationDisable;        ///< umidificazione disabilitata da bus ?
  BYTE          dehumidificationDisable;      ///< deumidificazione disabilitata da bus ?
  BYTE          auxiliaryHeatingDisabled;
  BYTE          auxiliaryCoolingDisabled;  
  BYTE          backLightValue;               ///< valore massimo retroilluminazione  
  BYTE          manualActive;                 ///< funzione setpoint manuale abilitata
  BYTE          manualFuncFBEnabled;
  long          setpointBaseManuale;
  long          setpointManuale;
  long          setpointOffset;
} TYPE_SAVED_DATA ;


/**      
* variabili contenenti i valori delle grandezze fisiche misurate,
* acquisite da bus o calcolate ...
*/
typedef struct  {
  long          temperature;                  ///< temperatura in uso "pesata in base alle impostazioni" (fixed-point 1/100°C)
  long          humidity;                     ///< umidità in uso (fixed-point 1/10%) 
  long          light;                        ///< luminosità 1 lx 
  long          dewPoint;                     ///< temperatura di dew-point (fixed-point 1/100°C) 
  long          perceivedTemperature;         ///< temperatura percepita (fixed-point 1/100°C)
  long          relHumidity;                  ///< umidità relativa (fixed-point 1/100%)
  float         absHumidity;                  ///< umidità assoluta 
  long          outdoorTemperature;           ///< temperatura esterna (fixed-point 1/100°C)
  long          actualSetpoint;               ///< setpoint utilizzato per le funzioni di termoregolazione
  long          airQuality;
  //long          setpointManuale;
  //long          setpointOffset;
  //long          setpointBaseManuale;
  long          flowTemp;
  BYTE          statoRegolazioneHeating;
  BYTE          statoRegolazioneCooling;
} TYPE_VALUES ;


extern BYTE   thermLock;
extern const WORD fw_version;


/**      
* valori uscite riscaldamento e raffrescamento
*/
typedef struct  {
  BYTE          heatingOut;                   ///< valore uscita riscaldamento
  BYTE          auxiliaryHeatingOut;          ///< valore uscita riscaldamento ausiliario
  BYTE          coolingOut;                   ///< valore uscita raffreddamento
  BYTE          auxiliaryCoolingOut;          ///< valore uscita reffreddamento ausiliario
} TYPE_THERM_OUT;


extern long   intLight;                       ///< luminosità sensore interno
extern long   intHumidity;                    ///< umidità sensore interno
extern long   intTemperature;                 ///< temperatura sensore interno
extern long   mainTemperature;                ///< temperatura principale 
extern long   addTemperature;                 ///< temperatura aggiuntiva

extern BYTE   hvacModeUsed;                   ///< modo HVAC in uso  


extern unsigned char saveDelay;
extern unsigned char filler;
extern struct APP_Flash *actualEep;
extern struct APP_Flash *emptyEep;

extern struct {
TYPE_VALUES;
TYPE_THERM_OUT;
TYPE_SAVED_DATA;  
};

/**      
* Valori ingressi esterni ... 
*/
typedef union {
  long  ai;
  BYTE  di;
} TYPE_EXTIN;

extern TYPE_EXTIN ExtInput[];
extern TYPE_EXTIN prevExtInput[];

extern BYTE coolingControlType;

extern __no_init DWORD a_marker;                     
extern __no_init DWORD z_marker;   
extern BYTE  bus_status;                     // 1        0xFF=power down, 0=bus_off, 1=bus_on

extern WORD delayAfterBusOn;

/***********************************************/

/**
 * @}
 */

void FunctionsSet(void);
void SensorFromBusEnableSet(void);


#define LUSetTransmitRequest(o)   if ( AL_IsObjLinked(o) ) \
                                  SetTransmitRequest(o);    


#endif // APPLICATION_H