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
#ifndef APPLICATIONUTILITY_H
#define APPLICATIONUTILITY_H

#define SetReceiveRequest(objectNr)     AL_TransmitObject(objectNr, TA_TRUE, TA_FALSE)

#define NELEMS(x)         (sizeof(x) / sizeof(x[0]))

/** @defgroup VARIE
 *  @{
 */


/**
 * @brief	
 * @param	
 * @return	 
 * @note
 */
WORD CalcCRC16(BYTE *src,WORD nbyte);

/**
 * @brief	
 * @param	
 * @return	 
 * @note
 */
BYTE PageErased(BYTE *p);

/**
 * @brief	
 * @param	
 * @return	 
 * @note
 */
BYTE ErasePage(BYTE *p);

/**
 * @brief	
 * @param	
 * @return	 
 * @note
 */
struct APP_Flash * GetActivePage(void);

/**
 * @brief	
 * @param	
 * @param	
 * @return	
 * @note
 */
struct APP_Flash* SaveUserData(struct APP_Flash *shadow_eeprom);

/**
 * @brief	restituisce il valore invertito o no
 * @param	value : valore digitale
 * @param	inverted : invertito o non invertito
 * @return	valore 
 * @note
 */
BYTE ContactFromBus(BYTE value, BYTE inverted );
/**
 * @}
 */
/** @defgroup CONVERSIONI
 *  @{
 */
/**
 * @brief	converte valore long in un short float
 * @param	inval : valore da convertire 
 * @return	valore convertito
 * @note
 * il valore long è espresso in (1/100)
 */
short           LongI2ShortF( long inval);
/**
 * @brief	converte un valore short float in un long
 * @param	inval : valore da convertire 
 * @return	valore convertito
 * @note
 * il valore long è espresso in (1/100)
 */
long            ShortF2LongI( short inval);
/**
 * @}
 */
/** @defgroup CALCOLI_PSICOMETRICI
 *  @{
 */
/**
 * @brief	calcolo temperatura di dewpoint
 * @param	h : umidità relativa
 * @param	t : temperatura
 * @return	valore temperatura di rugiada
 * @note
 * k = (log10(h)-2)/0.4343 + (17.62*t)/(243.12+t);\n
 * dew_point = 243.12*k/(17.62-k); 
 */
float           DewpointTemperature(float h,float t); 

/**
 * @brief	calcolo temperatura percepita
 * @param	h : umidità relativa
 * @param	t : temperatura
 * @return	valore di temperatura percepita
 */
float           PerceivedTemperature(float h,float t);

/**
 * @brief	calcolo pressione di saturazione
 * @param	t : temperatura
 * @return	valore di pressione di saturazione
 */
float           Psat(float t);

/**
 * @brief	calcolo umidità assoluta
 * @param	h : umidità relativa
 * @param	t : temperatura
 * @return	valore di umidità assoluta
 */
float           AbsoluteHumidity(float h,float t);

/**
 * @brief	calcolo entalpia
 * @param	ah : umidità assoluta
 * @param	t : temperatura
 * @return	valore di entalpia
 */
float           Enthalpy(float ah,float t);

/**
 * @brief	Resetta la EEPROM con i valori di default contenuti 
 *              in cpyDM_eeprom.
 * @return	-
 */
void            ResetEEPROM( void );

/**
 * @brief	-
 * @return	-
 */
BYTE            Ext_Input_Type(BYTE a);

/**
 * @brief	-
 * @return	-
 */
unsigned long   ReverseFloat(unsigned long *val);

/**
 * @brief	-
 * @return	-
 */
void            ReverseBits(BYTE *b);

/**
 * @brief	-
 * @return	-
 */
void            mDelay(DWORD _delay);

/**
 * @brief	-
 * @return	-
 */
void            uDelay(WORD _delay);

/**
 * @brief	-
 * @return	-
 */
void copyOutput( unsigned char * sourceOut, unsigned char * destOut, int length );

/**
 * @}
 */
#endif // APPLICATIONUTILITY_H