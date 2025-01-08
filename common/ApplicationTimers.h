/*
 * @brief gestione timer ....
 *
 * @note
 * Copyright(C) SBS, 2014
 * All rights reserved.
 *
 * @par
 * ------------
 * @par
 * ------------
 */
#ifndef APPLICATIONTIMER_H
#define APPLICATIONTIMER_H

#include "eib_stk.h"

/** @defgroup TIMER
 *  @{
 */

typedef struct {
  unsigned long finalTime;              // start time + delay in ms (can't be more than 0x7FFFFFFF)
} EZ_tmStruct;

extern EZ_tmStruct EZ_tmData[];



typedef struct 
{
  union {
      BYTE  timeElapsed;
      struct {
      BYTE  elapsed100msec   :1;    
      BYTE  elapsed250msec   :1;
      BYTE  elapsed500msec   :1;
      BYTE  elapsed1sec      :1;
      BYTE  elapsed10sec     :1;
      BYTE  elapsed1min      :1;
      BYTE  nu1              :2;
      };
  };
  union {
      BYTE  reverse;
      struct {
      BYTE  reverse100msec   :1;    
      BYTE  reverse250msec   :1;
      BYTE  reverse500msec   :1;
      BYTE  reverse1sec      :1;
      BYTE  nu2              :4;
      };  
  };
} TIME_ELAPSED;

extern TIME_ELAPSED tm;

//-------------------------------------------------------------------
// Similar to timer manager, with different memory allocation
//-------------------------------------------------------------------
extern __no_init EZ_tmStruct NI_tmData[NI_TIMER_NR];

/**
 * @brief	avvio timer ciclico
 *
 * @param	timerNr : numero del timer
 * @param	time : temporizzazione ( sec )
 * @return	niente
 *
 * @note
 * se time = 0 , il timer non viene avviato .\n
 * se ( time < MIN_CYCLIC_TIME ) time = MIN_CYCLIC_TIME
 */
void CyclicStartTimer( unsigned int timerNr, unsigned short time );

/**
 * @brief	avvio timer non inizializzato
 *
 * @param	timerNr : numero del timer
 * @param	time : temporizzazione 
 * @param	mode : modo 
 * @return	niente
 *
 */
void NI_StartTimer(unsigned int timerNr, unsigned long time, unsigned char mode);

/**
 * @brief	ferma timer non inizializzato
 *
 * @param	timerNr : numero del timer
 * @return	niente
 *
 */
void NI_StopTimer(unsigned int timerNr);

/**
 * @brief	richiede lo stato di un timer non inizializzato
 *
 * @param	timerNr : numero del timer
 * @return	stato del timer
 *
 */
TA_BOOL NI_GetState(unsigned int timerNr);

/**
 * @brief	avvio timer con base tempi 1msec
 *
 * @param	timerNr : numero del timer
 * @param	time : temporizzazione 
 * @param	mode : modo 
 * @return	niente
 *
 */
void EZ_StartTimer(unsigned int timerNr, unsigned long time, unsigned char mode);

/**
 * @brief	ferma timer con base tempi 1msec
 *
 * @param	timerNr : numero del timer
 * @return	niente
 *
 */
void EZ_StopTimer(unsigned int timerNr);

/**
 * @brief	richiede lo stato di un timer con base tempi 1msec
 *
 * @param	timerNr : numero del timer
 * @return	stato del timer
 *
 */
TA_BOOL EZ_GetState(unsigned int timerNr);

/**
 * @brief	richiede lo stato di un timer con base tempi 1msec
 *
 * @param	timerNr : numero del timer
 * @return	stato del timer
 *
 */
TA_BOOL EZ_GetInfo(unsigned int timerNr, unsigned long *remainingTime);

/**
 * @brief	avvia il timer se non è già stato avviato
 *
 * @param	timerNr : numero del timer
 * @param	time : temporizzazione 
 * @param	mode : modo 
 * @return	TRUE -> timer avviato e non scaduto , FALSE -> nr timer assento o timer già avviato
 *
 */
TA_BOOL EZ_StartNotRunningTimer( unsigned int timerNr, unsigned long time, unsigned char mode );

/**
 * @brief	verifica se il timer è stato avviato
 *
 * @param	timerNr : numero del timer
 * @return	TRUE -> timer avviato , FALSE -> nr timer assento o timer già avviato
 *
 */
TA_BOOL EZ_GetInfoRunning( unsigned int timerNr);

/**
 * @brief	
 *
 * @param	
 * @return	
 *
 */
BYTE TimeElapsed(void);

/**
 * @brief	
 *
 * @param	
 * @return	
 *
 */
void TimeElapsedFlagsClear(void);

/**
 * @}
 */

#endif