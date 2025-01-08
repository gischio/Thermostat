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
#ifndef PSYCHROMETRICVALUES_H
#define PSYCHROMETRICVALUES_H


extern long     _V1(dewPoint,MAX_THERM);

/**
 * @brief	
 *
 * @note
 */
BYTE            PsychrometricValueRT(BYTE paramRT,WORD CO_nr,WORD timer_nr,BYTE nTherm);

/**
 * @brief	
 *
 * @note
 */
void            PsychrometricValue (BYTE nTherm);


/**
 * @}
 */



#endif // PSYCHROMETRICVALUES_H