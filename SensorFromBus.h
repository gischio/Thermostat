/**
 * @brief ....
 *
 * @note
 * Copyright(C) SBS, 2014
 * All rights reserved.
 *
 * @par
 * @addtogroup VALVE_PROTECTION_MODULE
 * @{
 */
#ifndef SENSOR_FROM_BUS_H
#define SENSOR_FROM_BUS_H

typedef enum {
       /* analog */
       FROMBUS_ROOM_TEMP          = 0,
       FROMBUS_REL_HUMIDITY1         ,
       FROMBUS_REL_HUMIDITY2         ,
       FROMBUS_ANTISTRAT_TEMP        ,
       FROMBUS_OUTDOOR_TEMP          ,
       FROMBUS_COIL_TEMP             ,
       FROMBUS_FLOOR_TEMP            ,
       FROMBUS_FLOW_TEMP             ,
       /* digital */  
       FROMBUS_ANTICONDESATION       ,
       FROMBUS_WINDOWS_CONTACT1      ,
       FROMBUS_WINDOWS_CONTACT2      ,
       FROMBUS_PRESENCE1             ,
       FROMBUS_PRESENCE2             ,
       FROMBUS_CARD_HOLDER           ,
       /* fine */  
       FROMBUS_END_INX               ,
} T_SENSORS_FROMBUS_INX;


#define IS_FORMBUS_ANALOG(x)                    (x<FROMBUS_ANTICONDESATION)

#define END_TAG                                 0xFFFF

//------------------------------------------------------------------
// macros
/* CO ricevuti dal bus ! */
#define __SetSensorFromBusTimeout(c,x)            (sensorsFromBusTimeout[c] |= (1L<<(x)))
#define __ResetSensorFromBusTimeout(c,x)          (sensorsFromBusTimeout[c] &= ~(1L<<(x))) 
#define __IsSensorFromBusTimeout(c,x)             (sensorsFromBusTimeout[c] & (1L<<(x)))

#define __SetSensorFromBusError(c,x)              (sensorsFromBusError[c] |= (1L<<(x)))
#define __ResetSensorFromBusError(c,x)            (sensorsFromBusError[c] &= ~(1L<<(x))) 
#define __IsSensorFromBusError(c,x)               (sensorsFromBusError[c] & (1L<<x))

#define __SetSensorFromBusUdated(c,x)             (sensorsFromBusUpdated[c] |= (1L<<(x)))
#define __ReseSensorFromBusUdated(c,x)            (sensorsFromBusUpdated[c] &= ~(1L<<(x))) 
#define __IsSensorFromBusUpdated(c,x)             (sensorsFromBusUpdated[c] & (1L<<(x)))

#define __SetSensorFromBusEnabled(c,x)            (sensorsFromBusEnabled[c] |= (1L<<(x)))
#define __ResetSensorFromBusEnabled(c,x)          (sensorsFromBusEnabled[c] &= ~(1L<<(x))) 
#define __IsSensorFromBusEnabled(c,x)             (sensorsFromBusEnabled[c] & (1L<<(x)))

#define __IsSensorFromBusFail(c,x)                (__IsSensorFromBusError(c,(x)) || \
                                                 __IsSensorFromBusTimeout(c,(x)) ) 
//------------------------------------------------------------------   


#define SFB_TIMER_TIMEOUT(x)                      (TimerCOTimeout+(x))
#define SFB_TIMER_RCYCLIC(x)                      (TimerCOReadCyclic+(x))

typedef struct {
  WORD                  CO;
  WORD                  nTimer;
  T_SENSORS_FROMBUS_INX mask;
  WORD                  *time;
  const BYTE            *timeReadCyclic;
  BYTE                  lshitf;
} T_SENSORS_FROMBUS;

extern WORD           sensorsFromBusEnabled[MAX_THERM];
extern WORD           sensorsFromBusTimeout[MAX_THERM];
extern WORD           sensorsFromBusError[MAX_THERM];
extern WORD           sensorsFromBusUpdated[MAX_THERM];
extern WORD           sensorsFromBusROI[MAX_THERM];

extern const T_SENSORS_FROMBUS SensorsFromBusList[FROMBUS_END_INX];

/**
 * @brief	
 * @param	
 * @return	
 * @note        
 *   
 */
void  SensorFromBusInit( void ) ;

/**
 * @brief	
 * @param	
 * @return	
 * @note        
 *   
 */
void  SensorFromBusTimeoutAndError( BYTE channel );

/**
 * @}
 */
#endif // SENSOR_FROM_BUS_H