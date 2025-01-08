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
#ifndef SCENESHVAC_H
#define SCENESHVAC_H

#define HVAC_MAX_SCENE                          8
#define SCENE_LEARNING_BIT                      (1<<7)

extern BYTE  HVACSceneBehavior[MAX_THERM][HVAC_MAX_SCENE];

/**
 * @brief	
 *
 *
 * @note
 */
void ScenesHVACInit(void);
/**
 * @brief	
 *
 *
 * @note
 */
BYTE ScenesHVAC(BYTE modo,BYTE nTherm);

/**
 * @}
 */
#endif // SCENESHVAC_H