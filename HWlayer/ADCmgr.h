//! ADCmgr.h
//!
//! Progetto EK-EQ2_TP
//! Funzioni interfaccia HW MSP430(F6724) per ADC SAR + SD
//! v0.1 2014-10-01 GCC

#ifndef ADCMGR_H
#define ADCMGR_H

#include "msp430f6724.h"
//#include "Msp430_const.h"
#include "application.h"


#define SD24_16BIT

//#ifdef SD24_16BIT
//#warning ========================================================
//#warning >>> ADCmgr: SD24_16BIT
//#warning ========================================================
//#endif

// nADC = 0..4
// 0, 1, 2  are SAR 10-bit ADC10 channels #0, #1 and #2
// 3, 4     are S/D #0 and #1

// In many functions below, the ADC channel must be specified.
// For most of them, this information is only used to select which of the ADC
// units is used: if any ADC10 channel is selected, the functions ADCOnOff() -
// ADCReady() - ADCgetValue() have the same effect regardless of the channel.
// ADCstart() actually selects a particular channel.
// One could perform e.g. following:
//      ADCOnOff(0, TRUE);
//      ADCstart(2);        // << only this matters
//      while(!ADCready(0));
//      ADCgetValue(1);
//      ADCOnOff(1, FALSE);
// (as long as only ADC10 channels are used) and the result would be a read
// from channel 2.
// A similar behaviour applies to channels 3 and 4, which have independent
// pipelines (the conversion runs in parallel) but common commands.
//
// This is a summary of independent/common operations for ADCs of the two groups:
//
//                   Ch. 0,1,2      Ch. 3,4
//                    (ADC10)        (SD24)
// ADCinit()           -------common-------
// ADCOnOff(.)        common         common
// ADCready(.)        common       individual
// ADCstart(.)      individual       common
// ADCgetValue(.)     common       individual

void ADCinit(void);

void ADCOnOff(BYTE nADC, BYTE status);

BYTE ADCready(BYTE nADC);

BYTE ADCstart(BYTE nADC);

WORD ADCgetValue(BYTE nADC);

#endif  // ADCmgr.h
