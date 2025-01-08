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
#ifndef CO2FUNCTIONS_H
#define CO2FUNCTIONS_H

#define CONT_VENT_MIN               0
#define CONT_VENT_MAX               1
#define CONT_VENT_REG               2
#define CONT_VENT_NU                3

#define DONOT_SEND_CO               0
#define SEND_CO_ON_RT               1
#define SEND_CO_ON_LOCK_CHANGE      2
#define SEND_CO_ON_COV              3
#define SEND_CO_ON_DISABLE_CHANGE   4

extern BYTE CO2ThrLevel;

/**
 * @brief	
 * @return	nd
 */
void            CO2FunctionsInit(void); 

/**
 * @brief	
 * @return	nd
 */
BYTE            CO2Functions(void);

#endif // CO2FUNCTIONS_H