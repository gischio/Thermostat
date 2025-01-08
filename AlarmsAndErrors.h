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
#ifndef ALARMSANDERRORS_H
#define ALARMSANDERRORS_H

//  ALLARMI MAPPATI (A1-A8) ************************************************************
#define A_ROOM_CNTRL              1L<<0      // Room controller alarm
#define A_THERM_LOCK              1L<<1      // Thermal generator lock 
#define A_TEMP_SENSOR             1L<<2      // internal temperature sensor
#define A_TEMP_LIMIT              1L<<3
//  ERROR MAPPATI (E1-E8) **************************************************************
#define E_TEMP_SENSOR_FB          1L<<8      // temperature sensor failure
#define E_REL_HUMIDITY_FB         1L<<9      // umidity sensor failure
#define E_TEMP_ANTISTRAT_FB       1L<<10     // temperature sensor failure
#define E_TEMP_OUTDOOR_FB         1L<<11     // temperature sensor failure
#define E_TEMP_COIL_TEMP_FB       1L<<12     // temperature sensor failure
#define E_TEMP_FLOOR_TEMP_FB      1L<<13     // temperature sensor failure
#define E_TEMP_FLOW_TEMP_FB       1L<<14     // temperature sensor failure
//  ANALOG TIMEOUT MAPPATI (W1-W8) **************************************************************
#define TO_TEMP_SENSOR_FB         1L<<16     // timeout temperature sensor 
#define TO_REL_HUMIDITY_FB        1L<<17     // timeout umidity sensor 
#define TO_TEMP_ANTISTRAT_FB      1L<<18     // timeout temperature sensor 
#define TO_TEMP_OUTDOOR_FB        1L<<19     // timeout temperature sensor 
#define TO_TEMP_COIL_TEMP_FB      1L<<20     // timeout temperature sensor 
#define TO_TEMP_FLOOR_TEMP_FB     1L<<21     // timeout temperature sensor 
#define TO_TEMP_FLOW_TEMP_FB      1L<<22     // timeout temperature sensor 
//  DIGITAL TIMEOUT MAPPATI (W9-W16) **************************************************************
#define TO_DIG_ANTICOND_FB        1L<<24     // timeout digital anticondensation
#define TO_DIG_WCONT1_FB          1L<<25     // timeout digital windows contact 1
#define TO_DIG_WCONT2_FB          1L<<26     // timeout digital windows contact 2
#define TO_DIG_PRESENCE1_FB       1L<<27     // timeout digital presence 1
#define TO_DIG_PRESENCE2_FB       1L<<28     // timeout digital presence 2
#define TO_DIG_CARDHOLDER_FB      1L<<29     // timeout digital card holder

extern unsigned long _V1(errorSended,MAX_THERM); 
extern unsigned long _V1(errors,MAX_THERM);

#define ALARM_A_OFFSET            0
#define ALARM_E_OFFSET            8
#define ALARM_W_OFFSET            16

/**
 * @brief	
 * @return	nd
 */
void            AlarmsAndErrorsInit(void); 

/**
 * @brief	
 * @return	nd
 */
BYTE            AlarmsAndErrors(BYTE nTherm);

#endif // ALARMSANDERRORS_H