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
#ifndef HUMIDIFICATION_H
#define HUMIDIFICATION_H

#define HUMIDIFICATION_ENABLE(x)      ( ((_PARTHERM(x).HumidityFromBus == ENABLED)|| \
                                         (PARAMETER.RhEnableSensor == ENABLED))&& \
                                        (_PARTHERM(x).Humidification != DISABLED) )

/**
 * @brief	
 * @return	nd
 */
void            HumidificationInit(void); 

/**
 * @brief	
 * @return	nd
 */
BYTE            Humidification(BYTE hvacMode,BYTE nTherm);

#endif // HUMIDIFICATION_H