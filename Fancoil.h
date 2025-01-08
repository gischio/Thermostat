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
#ifndef FANCOIL_H
#define FANCOIL_H

#define FUNCT_FANCOIL_HEATING                   (1<<0)
#define FUNCT_FANCOIL_COOLING                   (1<<1)
#define FUNCT_FANCOIL_AUXHEATING                (1<<2)
#define FUNCT_FANCOIL_AUXCOOLING                (1<<3)
#define FUNCT_ANTISTRAT                         (1<<4)
#define FUNCT_COIL_BATTERY                      (1<<5)

#define FAN_SPEED_OFF                           0
#define FAN_SPEED_AUTO                          6

/* Ventilation control type */
#define FANS_1SPEED                             0
#define FANS_2SPEED                             1
#define FANS_3SPEED                             2
#define FANS_CONTINUOUS                         3
/* Ventilation manual operation */
#define FANS_NO_TEMP_DEPENDING                  0
#define FANS_TEMP_DEPENDING                     1

#define FANCOIL_SET_FIRSTRUN(x)                 (firstRunFancoil |= (1<<x))
#define FANCOIL_CHK_FIRSTRUN(x)                 (firstRunFancoil & (1<<x))
#define FANCOIL_RST_FIRSTRUN(x)                 (firstRunFancoil &= (~(1<<x)))

#define FANCOIL_SET_ANTISTRAT(x)                (antistrat |= (1<<x))
#define FANCOIL_CHK_ANTISTRAT(x)                (antistrat & (1<<x))
#define FANCOIL_RST_ANTISTRAT(x)                (antistrat &= (~(1<<x)))

typedef struct  {
  BYTE          visFanLevel;
  BYTE          actualFanOut;
  long          coilBatteryTemperature;       ///< valore di temperatura scambiatore fancoil , in caso di errore = SENSOR_ERROR
  BYTE          fanManualOperation          :1;
  BYTE          fanOnDelay                  :1; ///< stato , attivo (1) o non attivo (0) , della funzione di ritardo accensione ventilatore
  BYTE          fanOffDelay                 :1; ///< stato , attivo (1) o non attivo (0), della funzione di ritardo spegnimento ventilatore
  BYTE          hotStartFunctionActivated   :1; ///< stato , attivo (1) o non attivo (0), della funzione di hotstart  
  BYTE          antistratificationActivated :1; ///< stato , attivo (1) o non attivo (0), della funzione antistratificazione
  BYTE          fanControlDisabled          :1; ///< stato , disabilitato (1) o abilitato (0) , del controllore fancoil
  BYTE          fan_allowed                 :1;
  BYTE          prevFan_allowed             :1;
  BYTE          fanCoilEnabled              :1; ///< stato , presente (1) o non presente (0), della funzione fancoil
  BYTE          fanCoilFirstRun             :1; ///< prima esecuzione del controllore fancoil dopo il reset
} TYPE_VENTILATION ;

#ifdef SINGLE_THERMOSTAT 
#define _fc(x)                fc
#define _fancoilFunctions(x)  fancoilFunctions
#else
#define _fc(x)                fc[x]
#define _fancoilFunctions(x)  fancoilFunctions[x]
#endif

extern TYPE_VENTILATION _fc(MAX_THERM);
extern BYTE             _fancoilFunctions(MAX_THERM);


/**
 * @brief	
 * @return	nd
 */
void            FancoilInit( void );

/**
 * @brief	
 * @return	nd
 */
void            FancoilFunction(BYTE stopAll,BYTE nTherm);

/**
 * @brief	
 * @return	nd
 */
BYTE            AutoFansSpeed(BYTE fansSpeed,long sp,BYTE nTherm);

/**
 * @brief	
 * @return	nd
 */
BYTE            ManualFansSpeed(struct THERM *therm, BYTE fansSpeed);

/**
 * @brief	
 * @return	nd
 */
void            SetFancoilFunctions(void);

#endif // FANCOIL_H