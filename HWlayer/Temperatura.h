/**********************************************************************
* $Id$      Temperatura.h        2015-05-12
*//**
* @file     Temperatura.h
* @brief    -----
* @version  1.0
* @date     12-05-2015
* @author   Luca
*
* Copyright(C) 2012, SBS
* All rights reserved.
*
***********************************************************************/
#ifndef TEMPERATURA_H
#define TEMPERATURA_H


#define TSTEP			 100
#define TABLEN			 14
#define SONDA_APERTA		-9999
#define SONDA_CORTO		-9998
#define PZERO			-400

#define STARATURA_TEMPERATURA   -40 // 1/10°C

int getNTCTemp(unsigned int Conversione);

#endif /* TEMPERATURA_H */
