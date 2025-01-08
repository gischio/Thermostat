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
#ifndef SETPOINTS_H
#define SETPOINTS_H

#define MAX_COMFORT_SETPOINT_HEATING    5000
#define MIN_COMFORT_SETPOINT_HEATING    1000
#define MAX_COMFORT_SETPOINT_COOLING    5000
#define MIN_COMFORT_SETPOINT_COOLING    1000
#define MAX_STANDBY_OFFSET_HEATING     -100
#define MIN_STANDBY_OFFSET_HEATING     -500
#define MAX_STANDBY_OFFSET_COOLING      500
#define MIN_STANDBY_OFFSET_COOLING      100
#define MAX_ECONOMY_OFFSET_HEATING     -100
#define MIN_ECONOMY_OFFSET_HEATING     -800
#define MAX_ECONOMY_OFFSET_COOLING      800
#define MIN_ECONOMY_OFFSET_COOLING      100
#define MAX_BUILDPROT_SETPOINT_HEATING  1000
#define MIN_BUILDPROT_SETPOINT_HEATING  200
#define MAX_BUILDPROT_SETPOINT_COOLING  5000
#define MIN_BUILDPROT_SETPOINT_COOLING  3000
#define NEUTRAL_ZONE                    100 /* 1/100 °C */

#define MAX_STANDBY_SETPOINT_HEATING    5000
#define MIN_STANDBY_SETPOINT_HEATING    200
#define MAX_STANDBY_SETPOINT_COOLING    5000
#define MIN_STANDBY_SETPOINT_COOLING    200
#define MAX_ECONOMY_SETPOINT_HEATING    5000
#define MIN_ECONOMY_SETPOINT_HEATING    200
#define MAX_ECONOMY_SETPOINT_COOLING    5000
#define MIN_ECONOMY_SETPOINT_COOLING    200


typedef struct {
  long          comfort[2];          
  long          standby[2];         
  long          economy[2];         
  long          protection[2];        
} TYPE_SETPOINTS;

extern long limit_min;
extern long limit_max;

/**
 * @brief	
 * @return	nd
 */
long            SetpointLimits(BYTE mode,BYTE hc);

/**
 * @brief	
 * @return	nd
 */
long            MinimumSetpoint(long defValue,long lowValue);

/**
 * @brief	
 * @return	nd
 */
long            MaximumSetpoint(long defValue,long lowValue);

/**
 * @brief	
 * @return	nd
 */
void            SetpointsAdjust(TYPE_SETPOINTS *setpoint, long delta );

/**
 * @brief	
 * @return	nd
 */
void            SetpointsComfortAdjust( char stagione, long* valore, long curr_value);

/**
 * @brief	
 * @return	nd
 */
void            SetpointsAdjustAfterDownload(void);

/**
 * @brief	
 * @return	nd
 */
void            SetpointsLoadFromEeprom(BYTE nTherm);

/**
 * @brief	
 * @return	nd
 */
void            SetpointsInit(void); 

/**
 * @brief	
 * @return	nd
 */
void            Setpoints(BYTE nTherm);

/**
 * @brief	
 * @return	nd
 */
BYTE            SetpointsFromBus(BYTE nTherm);

/**
 * @brief	
 * @return	nd
 */
void            SetpointSendOnCOV(BYTE nTherm);

#endif // SETPOINTS_H