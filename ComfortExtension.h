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
#ifndef COMFORTEXTENTION_H
#define COMFORTEXTENTION_H

typedef struct  {
  BYTE          comfortExtFunctionEnable  :1; ///< funzine abilitata/disabilitata
  BYTE          comfortExtEnabled         :1; ///< stato , attivo o non attivo , della funzione estensione comfort  
} TYPE_COMFORT_EXT ;

extern TYPE_COMFORT_EXT ce;

/**
 * @brief	
 * @return	nd
 */
void            ComfortExtensionInit(void); 

/**
 * @brief	
 * @return	nd
 */
BYTE            ComfortExtensionExitFromFunction(void);

/**
 * @brief	
 * @return	nd
 */
BYTE            ComfortExtensionStartFunction(void);

/**
 * @brief	
 * @return	nd
 */
BYTE            ComfortExtension(BYTE mode);


#endif // COMFORTEXTENTION_H