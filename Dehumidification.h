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
#ifndef DEHUMIDIFICATION_H
#define DEHUMIDIFICATION_H

#define DEHUMIDIFICATION_ENABLE(x)    ( ((_PARTHERM(x).HumidityFromBus == ENABLED)|| \
                                         (PARAMETER.RhEnableSensor == ENABLED))&& \
                                        (_PARTHERM(x).Dehumidification != DISABLED) )


extern BYTE    reqDeh[MAX_THERM];

/**
 * @brief	
 * @return	nd
 */
void            DehumidificationInit(void); 

/**
 * @brief	
 * @return	nd
 */
BYTE            Dehumidification(BYTE hvacMode,BYTE nTherm);

#endif // DEHUMIDIFICATION_H