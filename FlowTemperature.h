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
#ifndef FLOWTEMPERATURE_H
#define FLOWTEMPERATURE_H

extern long _V1(flowTemp,MAX_THERM);

/**
 * @brief	
 * @return	nd
 */
long            FlowTemperatureRead(BYTE nTherm);

/**
 * @brief	
 * @return	nd
 */
void            FlowTemperatureInit(void);

/**
 * @brief	
 * @return	nd
 */
BYTE            FlowTemperature(BYTE nTherm);

#endif // FLOWTEMPERATURE_H