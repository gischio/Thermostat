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
#ifndef HUMIDEHUMISETPOINTS_H
#define HUMIDEHUMISETPOINTS_H

#define MAX_SP_HUMI                             8000
#define MIN_SP_HUMI                             2000
#define MAX_SP_DEHUMI                           8000
#define MIN_SP_DEHUMI                           2000

/**
 * @brief	
 * @return	nd
 */
void            HumiDehumiSetpointsInit(void); 

/**
 * @brief	
 * @return	nd
 */
void            HumiDehumiSetpoints(BYTE nTherm);


#endif // HUMIDEHUMISETPOINTS_H