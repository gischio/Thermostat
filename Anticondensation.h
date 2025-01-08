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
#ifndef ANTICONDENSATION_H
#define ANTICONDENSATION_H

#ifdef SINGLE_THERMOSTAT 
#define _ac(x)                ac
#else
#define _ac(x)                ac[x]
#endif


#define ENABLE_TPROJ                            1
#define ENABLE_PROBE                            2

typedef struct {
  BYTE          passiveAnticondActivated    :1; ///< stato , attivo o non attivo , della funzione anticondensa passiva
  BYTE          activeAnticondActivated     :1; ///< stato , attivo o non attivo , della funzione anticondensa attiva
} TYPE_ANTICONDENSATION;


#define ANTICOND_PASS_ENABLE(x)         (_PARTHERM(x).DewPointTemperature == ENABLED)

extern TYPE_ANTICONDENSATION _ac(MAX_THERM);

/**
 * @brief	
 * @return	nd
 */
void            AnticondensationInit(void);

/**
 * @brief	
 * @return	nd
 */
BYTE            PassiveAnticondensation (BYTE thermFail,BYTE setpointMatched,BYTE nTherm);

/**
 * @brief	
 * @return	nd
 */
BYTE            ActiveAnticondensation (BYTE thermFail,BYTE setpointMatched,BYTE nTherm);

/**
 * @brief	
 * @return	nd
 */
BYTE            ActiveAnticondensationNoProbe (BYTE thermFail,BYTE setpointMatched,BYTE nTherm);

#endif // ANTICONDENSATION_H