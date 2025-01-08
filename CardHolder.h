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
#ifndef CARDHOLDER_H
#define CARDHOLDER_H

#define CARD_HOLDER_IN_SWITCH_NONE              0
#define CARD_HOLDER_IN_SWITCH_COMFORT           1
#define CARD_HOLDER_IN_SWITCH_STANDBY           2
#define CARD_HOLDER_IN_SWITCH_ECONOMY           3
#define CARD_HOLDER_OUT_SWITCH_NONE             0
#define CARD_HOLDER_OUT_SWITCH_PROTECTION       1
#define CARD_HOLDER_OUT_SWITCH_STANDBY          2
#define CARD_HOLDER_OUT_SWITCH_ECONOMY          3

#define NOT_OCCUPIED                            0
#define OCCUPIED                                1

#define CARDHOLDER_ENABLE(x)          ( (_PARTHERM(x).CardHolderContactFromBus == ENABLED)&& \
                                        (_PARTHERM(x).CardHolderFunction != DISABLED) )

/**      
* variabili utilizzate per la gestione della tasca porta tessera
*/
typedef struct  { 
  BYTE          cardHolderStatus          :1; ///< stato ( or dei possibili ingressi ) del contatto tasca portatessera 
  BYTE          cardHolderFunctionActive  :1; ///< stato , attivo o non attivo , della funzione tasca porta tessera 
  BYTE          prevCardHolderInputStatus :2; ///< valore precedente di cardHolderStatus (3 = NAN )
} TYPE_CARD_HOLDER ;

extern TYPE_CARD_HOLDER cardHolder[MAX_THERM];

/**
 * @brief	inizializzazione varibili usate dalla finzione CardHolder .
 *
 * @return	nd
 */
void            CardHolderInit(void);

/**
 * @brief	lettura ingresso o oggetto di comunicazione abilitati per la
 *              funzione tasca porta tessera .
 *
 * @return	stato dell' ingresso ; 0 NOT OCCUPIED , 
 *                                     1 OCCUPIED,
 *                                     0x80 error !!!! timeout CO o CO non ancora aggiornato , nel caso l'ingresso
 *                                     CardHolder provenga da bus
 *
 * Viene preso in esame un solo ingresso , nel caso in cui siano stati configurati + ingressi 
 * viene selezionato l'ingresso seguendo la priorità :
 * ingresso esterno 1
 * ingresso esterno 2
 * oggetto di comunicazione
 */
BYTE            CardHolderRead(BYTE nTherm);

/**
 * @brief	logica di funzionamento tasca porta tessera
 *
 * @param       hvacMode : modalità operativa attualmente in uso 
 *                         ( HVAC_COMFORT,HVAC_STANDBY,HVAC_ECONOMY,HVAC_PROTECTION) .
 * @return	nuova modalità operativa condizionata dell' eventuale inserzione/
 *              rimozione delle tessera.
 *
 */                                               
BYTE            CardHolder(BYTE hvacMode,BYTE nTherm);

#endif // CARDHOLDER_H