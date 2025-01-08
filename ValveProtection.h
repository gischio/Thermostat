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
#ifndef VALVEPROTECTION_H
#define VALVEPROTECTION_H

#define VALVE_NR                                4
#define VALVE_HEATING                           (1<<0)
#define VALVE_COOLING                           (1<<1)
#define VALVE_AUXHEATING                        (1<<2)
#define VALVE_AUXCOOLING                        (1<<3)
#define VALVE_COUPLED_PRIMARY                   (1<<0)
#define VALVE_COUPLED_AUX                       (1<<1)

typedef struct {
  WORD                  CO;
  WORD                  timer;
  const BYTE            *rt;
} T_VALVE_OUT_RT;

/**      
* variabili utilizzate per la gestione protezione valvole
*/
typedef struct  {
  BYTE          valveCoupled;
  BYTE          valveEnabled; 
  BYTE          thermostatValveReq;                 ///< richiesta attivazione valvole proveniente dalla routine termostati (condizionata da accoppiamento valvole)   
  BYTE          cpyThermostatValveReq;            ///< richiesta attivazione valvole proveniente dalla routine termostati      
  BYTE          protectionValveReq;               ///< richiesta attivazione valvole proveniente dalla routine protezione valvole
  BYTE          protectionValveFase[VALVE_NR];    ///< fase protezione valvole
  WORD          protectionValveCounter[VALVE_NR]; ///< conto alla rovescia per attivazione protezione valvole  
  BYTE          valveOutput;  
  BYTE          valveLed                  :1;
  BYTE          valveHeatingStatus        :1;
  BYTE          valveCoolingStatus        :1;  
  BYTE          valveHeatingAlarm         :1;
  BYTE          valveCoolingAlarm         :1;  
  BYTE          valveFirstRun             :1;    
} TYPE_VALVE_PROTECTION ;


//extern BYTE coolingOut[MAX_THERM];
//extern BYTE auxiliaryCoolingOut[MAX_THERM];
//extern BYTE heatingOut[MAX_THERM];
//extern BYTE auxiliaryHeatingOut[MAX_THERM];

#ifdef SINGLE_THERMOSTAT 
#define _vp(x)                vp
#else
#define _vp(x)                vp[x]
#endif

extern TYPE_VALVE_PROTECTION _vp(MAX_THERM);

/**
 * @brief	
 *   
 */
void          ValveProtectionInit(void);

/**
 * @brief	
 *   
 */
void          ValveInit(void);

/**
 * @brief	
 *   
 */
void            ValveRT(BYTE nTherm);

/**
 * @brief	in base al tipo di controllo restituisce il valore di attivazione
 *              valvola
 * @param	controlType : tipo di controllo (uscita)
 * @return	valore attivazione valvola 
 * @note        tipo di controllo : continuo->valore = 255, PWM o 2 punti->valore = 1
 *   
 */
BYTE            ProtectionOutputValue(BYTE controlType);

/**
 * @brief	Gestisce l' invio degli oggetti di comunicazione per l' attivazione
 *              delle valvole .\n
 *               
 
 * @param	-
 * @return	-
 *   
 */
void            ValveOutput(BYTE nTherm);

/**
 * @brief	-
 * @param	-
 * @return	-
 *   
 */
void            ValveOutStatusAdj(BYTE nTherm);

/**
 * @brief	Modulo gestione protezione valvola . \n
 *              Il modulo viene eseguito se la funzione è abilitata , 
 *              vedi functions & FUNCT_VALVE_PROTECTION . \n
 *              4 stati possibili : \n
 *              0-> conto alla rovescia , la valvola non è attiva ! \n
 *                  alla scadenza passa allo stato 1 . \n 
 *              1-> avvio attivazione forzata della valvola \n
 *                  passa allo stato 2 . \n 
 *              2-> attesa scadenza tempo di attivazione forzata  \n
 *                  alla scadenza passa allo stato 3 . \n 
 *              3-> riavvia il conto alla rovescia , la valvola è attiva ! \n 
 *                  passa allo stato 0 . \n \n
 *              qualora la funzione termostato rischieda l'attivazione di una valvola
 *              il ciclo riprende dallo stato 3
 * @param	-
 * @return	-
 *   
 */
void            ValveProtection(BYTE nTherm);

/**
 * @brief	-
 * @param	-
 * @return	-
 *   
 */
void            ValveStatus(BYTE nTherm);

/**
 * @}
 */
#endif // VALVEPROTECTION_H