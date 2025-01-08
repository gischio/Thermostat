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
#ifndef MANUALFUNCTION_H
#define MANUALFUNCTION_H

typedef struct  {
  BYTE          manualHvacMode;
  BYTE          manualActive          :1; ///< stato ( or dei possibili ingressi ) del sensore presenza 
  BYTE          manualTimerEnabled    :1; ///< stato , attivo o non attivo , della funzione di risparmio energetico o comfort 
  BYTE          manualFuncEnabled     :1;
  BYTE          manualFuncFBOutOfLimits :1;
} TYPE_MANUAL_FUNC ;

extern long _V1(limitProtectionMinMF,MAX_THERM);
extern long _V1(limitProtectionMaxMF,MAX_THERM);


extern TYPE_MANUAL_FUNC _V1(mf,MAX_THERM);

/**
 * @brief	
 * @return	nd
 */
long            ManualGetSetpoint(BYTE nTherm);

/**
 * @brief	
 * @return	nd
 */
void            ManualFunctionInit(void); 

/**
 * @brief	
 * @return	nd
 */
BYTE            ManualExitFromFunction(BYTE nTherm);

/**
 * @brief	
 * @return	nd
 */
BYTE            ManualFunction(BYTE mode,BYTE nTherm);

/**
 * @brief	
 * @return	nd
 */
BYTE            ManualStartFunction(BYTE nTherm); 

#endif // MANUALFUNCTION_H