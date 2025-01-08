/*
 * @brief Utiliti varie ....
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
#ifndef WEIGHTEDTEMPERATURE_H
#define WEIGHTEDTEMPERATURE_H


#define SENSOR_INTERNAL                         0
#define SENSOR_INPUT                            1
#define SENSOR_FROM_BUS                         2


//#define SetReceiveRequest(objectNr)     AL_TransmitObject(objectNr, TA_TRUE, TA_FALSE)
/**
 * @brief	
 * @return	
 */
long            RoomTemperature(BYTE nTherm);

/**
 * @brief	
 * @return	
 */
long            ReadAddMainRoomTemperature(BYTE tempSrc,BYTE nTherm);

/**
 * @brief	
 * @return	
 */
void            WeightedTemperatureInit(void);

/**
 * @brief	calcolo temperatura pesata
 * @return	valore di temperatura mediata fra due valori
 */
long            CalculationOfWeightedTemperature(BYTE nTherm);

/**
 * @}
 */
#endif // WEIGHTEDTEMPERATURE_H