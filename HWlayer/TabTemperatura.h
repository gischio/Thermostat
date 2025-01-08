/**********************************************************************
* $Id$      TabTemperatura.h        2015-05-12
*//**
* @file     TabTemperatura.h
* @brief    -----
* @version  1.0
* @date     12-05-2015
* @author   Luca
*
* Copyright(C) 2012, SBS
* All rights reserved.
*
***********************************************************************/
#ifndef TABTEMPERATURA_H
#define TABTEMPERATURA_H


//<< TABELLA C (valori conversione) 
const unsigned int Tc[]={
 29,
 54,
 94,
 155,
 238,
 340,
 454,
 567,
 670,
 756,
 825,
 877,
 916,
 944,
};
//<< TABELLA M (valori pendenza) 
const unsigned int Tm[]={
 400,
 250,
 163,
 120,
 98,
 87,
 88,
 97,
 116,
 144,
 192,
 256,
 357,
 10,
};
#endif /* TABTEMPERATURA_H */
