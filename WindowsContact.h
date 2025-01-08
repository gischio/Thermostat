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
#ifndef WINDOWSCONTACT_H
#define WINDOWSCONTACT_H

/**      
* variabili utilizzate per la gestione dei contatti finestra     
*/
typedef struct  {
  BYTE          windowsContactStatus      :1; ///< stato ( or dei possibili ingressi ) del contatto finestra 
  BYTE          prevWindowsContactStatus  :1; ///< valore precedente di  windowsContactStatus 
  BYTE          windowsProtectionActive   :1; ///< stato , attivo o non attivo , della funzione di risparmio energetico 
} TYPE_WINDOW_CONTACT ;

extern TYPE_WINDOW_CONTACT _V1(winc,MAX_THERM);

/**
 * @brief	
 * @param	
 * @return	
 * @note        
 *   
 */
BYTE            WindowsContactInit( void ) ;

/**
 * @brief	
 * @param	
 * @return	
 * @note        
 *   
 */
BYTE            WindowsContactRead(BYTE nTherm);

/**
 * @brief	
 *               
 * @param	-
 * @return	-
 *   
 */
BYTE            WindowsContactStatus(BYTE hvacMode,BYTE nTherm);


/**
 * @}
 */
#endif // WINDOWSCONTACT_H