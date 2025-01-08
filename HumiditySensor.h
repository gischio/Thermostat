/**
 * @brief ....
 *
 * @note
 * Copyright(C) SBS, 2014
 * All rights reserved.
 *
 * @par
 * @addtogroup
 * @{
 */
#ifndef HUMIDITYSENSOR_H
#define HUMIDITYSENSOR_H

extern long     _V1(humidity,MAX_THERM);

/**
 * @brief	
 * @param	
 * @return	
 * @note        
 *   
 */
void            HumiditySensorInit(void) ;

/**
 * @brief	
 * @param	
 * @return	
 * @note        
 *   
 */
void            HumiditySensor(BYTE nTherm);

/**
 * @}
 */
#endif // HUMIDITYSENSOR_H