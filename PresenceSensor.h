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
#ifndef PRESENCESENSOR_H
#define PRESENCESENSOR_H

#define NOT_OCCUPIED                            0
#define OCCUPIED                                1
#define COMFORT_EXTENSION                       0
#define COMFORT_LIMITATION                      1
#define COMFORT_EXTENSION_LIMITATION            2
#define COMFORT_STANDBY                         0
#define COMFORT_ECONOMY                         1

typedef struct  {
  BYTE          absenceStatus;
  BYTE          prevAbsenceStatus;
  BYTE          fase;
  BYTE          contactStatus         :1; ///< stato ( or dei possibili ingressi ) del sensore presenza 
  BYTE          functionActivated     :1; ///< stato , attivo o non attivo , della funzione di risparmio energetico o comfort 
  BYTE          extension             :1; ///< stato , attivo o non attivo , estensione comfort
  BYTE          functionStop          :1;
  BYTE          reduction_enabled     :1;  
  BYTE          reduction_activated   :1;      
  BYTE          extension_enabled     :1;
  BYTE          extension_activated   :1;  
} TYPE_PRESENCE_SENSOR ;

extern TYPE_PRESENCE_SENSOR _V1(presence,MAX_THERM);

/**
 * @brief	
 * @return	nd
 */
void            PresenceEnableFunction(BYTE nTherm);

/**
 * @brief	
 * @return	nd
 */
void            PresenceRestartTimer(BYTE nTherm);

/**
 * @brief	
 * @return	nd
 */
BYTE            PresenceStatus(BYTE hvacMode, BYTE nTherm); 



#endif // PRESENCESENSOR_H