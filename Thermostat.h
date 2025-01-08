/*
 * @brief ....
 *
 * @note
 * Copyright(C) SBS, 2014
 * All rights reserved.
 *
 * @par
 * fdsfsdfsdf
 * @par
 * fdfsdfsdfsd
 */
//------------------------------------------------------------------
#ifndef THERMOSTAT_H
#define THERMOSTAT_H

#define xxxEXTERNAL_SENSOR

#ifdef EXTERNAL_SENSOR
#warning ========================================================
#warning EXTERNAL_SENSOR = Sensore da morsetto
#warning ========================================================
#endif

//------------------------------------------------------------------
// functions 
//------------------------------------------------------------------
/*
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
*/
//------------------------------------------------------------------
// thermFunctions
//------------------------------------------------------------------
#define FUNCT_THERMOSTAT                        (1<<0)
#define FUNCT_FANCOIL                           (1<<1)
#define FUNCT_THERM_HEATING                     (1<<2)
#define FUNCT_THERM_COOLING                     (1<<3)
#define FUNCT_THERM_AUXHEATING                  (1<<4)
#define FUNCT_THERM_AUXCOOLING                  (1<<5)
#define FUNCT_PASS_ANTICOND                     (1<<6)
#define FUNCT_ACTIVE_ANTICOND                   (1<<7)
#define FUNCT_ACTIVE_ANTICOND_NOPROB            (1<<8)
//------------------------------------------------------------------


#define COOLING                                 0
#define HEATING                                 1
#define HEATING_AND_COOLING                     2
/* Dehumidificatio/Humidification function */
#define HD_DISABLE                              0
#define HD_COOLING_ONLY                         1
#define HD_HEATING_ONLY                         2
#define HD_COOLING_AND_HEATING                  3
/* Dehumidificatio function */
#define INDIPENDENT_FROM_TEMPERATURE            0
#define DIPENDENT_ON_TEMPERATURE                1
/* modo HVAC */
#define HVAC_AUTO                               0
#define HVAC_COMFORT                            1
#define HVAC_STANDBY                            2
#define HVAC_ECONOMY                            3
#define HVAC_PROTECTION                         4
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
/* Thermostat Switch over */
#define HC_SWITCH_MANUAL                        0
#define HC_SWITCH_FROM_BUS                      1
#define HC_SWITCH_AUTOMATIC                     2

#define HEATING_FUNCTION                        0
#define HEATING_COOLING_FUNCTION                1
#define COOLING_FUNCTION                        2

#define SETPOINT_SINGLE                         0
#define SETPOINT_ABSOLUTE                       1
#define SETPOINT_RELATIVE                       2

#define CMD_CO_UNIQUE                           0
#define CMD_CO_SEPARATED                        1

#define HVAC_AFTER_DOWNLOAD_NO_CHANGE           0
#define HVAC_AFTER_DOWNLOAD_HEATING             1
#define HVAC_AFTER_DOWNLOAD_COOLING             2

#define INPUT_NTC                               0x02

#define IS_HEATING_AND_COOLING_ENABLED(x)       (_PARTHERM(x).ThermostatFunction == HEATING_COOLING_FUNCTION)

#define IS_ENABLE_HC(x)                         ( ((_therm(x).heatingCoolingMode == HEATING)&&(_V1(thermFunctions,x)&FUNCT_THERM_HEATING))|| \
                                                  ((_therm(x).heatingCoolingMode == COOLING)&&(_V1(thermFunctions,x)&FUNCT_THERM_COOLING)) )

#define THERM_SET_FIRSTRUN(x)                   (thermFirstRun |= (1<<x))
#define THERM_CHK_FIRSTRUN(x)                   (thermFirstRun & (1<<x))
#define THERM_RST_FIRSTRUN(x)                   (thermFirstRun &= (~(1<<x)))
#define THERM_SET_HCFIRSTRUN(x)                 (firstHCStatusOut |= (1<<x))
#define THERM_CHK_HCFIRSTRUN(x)                 (firstHCStatusOut & (1<<x))
#define THERM_RST_HCFIRSTRUN(x)                 (firstHCStatusOut &= (~(1<<x)))


extern long          _V1(prevActualSetpoint,MAX_THERM);
extern BYTE          _V1(prevHvacMode,MAX_THERM);
extern BYTE          _V1(prevHvacAuto,MAX_THERM);
extern long          _V1(temperature,MAX_THERM);
extern long          _V1(actualSetpoint,MAX_THERM);
extern BYTE          _V1(heatingOut,MAX_THERM);                  
extern BYTE          _V1(auxiliaryHeatingOut,MAX_THERM);
extern BYTE          _V1(coolingOut,MAX_THERM);
extern BYTE          _V1(auxiliaryCoolingOut,MAX_THERM);
extern BYTE          _V1(heatingCoolingChanged,MAX_THERM);
extern BYTE          _V1(thermLock,MAX_THERM);
extern WORD          _V1(thermFunctions,MAX_THERM);
extern BYTE          _V1(statoRegolazioneCooling,MAX_THERM);
extern BYTE          _V1(statoRegolazioneHeating,MAX_THERM);
extern BYTE          _V1(coolingControlType,MAX_THERM);


/*
extern BYTE     coolingControlType[MAX_THERM];
extern BYTE     spChanged;
extern long     actualSetpoint[MAX_THERM];
extern BYTE     statoRegolazioneCooling[MAX_THERM];
extern BYTE     statoRegolazioneHeating[MAX_THERM];
extern long     temperature[MAX_THERM];
extern WORD     thermFunctions[MAX_THERM];
extern BYTE     thermLock[MAX_THERM];
*/
/**
 * @brief	
 * @return	nd
 */
void            ThermostatInit(void);

/**
 * @brief	
 * @return	nd
 */
BYTE            Thermostat(BYTE _hvacMode,BYTE nTherm);

/**
 * @brief	
 * @return	nd
 */
void            SetThermFunctions(void);

#endif // THERMOSTAT_H