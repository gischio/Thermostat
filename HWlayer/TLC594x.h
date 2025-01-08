//! TLC594x.h
//!
//! Progetto EK-EQ2_TP
//! Funzioni interfaccia HW MSP430(F6724) per LED Driver TLC594x
//! v0.1 2014-10-01 GCC

//! Parts from the std Arduino TLC5940 GPL Library by Alex Leone
    
#ifndef TLC594X_H
#define TLC594X_H

#include "msp430f6724.h"
//#include "Msp430_const.h"
#include "application.h"
#include "TLC594X_CFG.h"

/** \file
    Tlc5940 library header file. */

extern volatile BYTE  tlc_XLAT_pending;
extern volatile BYTE  tlc_blanking_on;

typedef  BYTE GSBuffer[NUM_TLCS * 24] ;		// Buffer for GS data
typedef  BYTE DCBuffer[NUM_TLCS * 12] ;		// Buffer for DC data

extern   GSBuffer  tlc_GSData;
extern   DCBuffer  tlc_DCData;

// Callback function for application code after GS shift valid, if required
//extern volatile void  (*tlc_onUpdateFinished)(void);

//! Initialize TLC driver
void    TLCinit(WORD initialValue);

//! Set LED blanking ON/Off
void    TLCsetBlanking(BYTE status);

//! Clear GS buffer. Requires update() afterwards.
void    TLCclearBuf(GSBuffer buf);
#define TLCclear()  TLCclearBuf(0)

//! Send GS buffer to TLC.
BYTE    TLCupdateBuf(GSBuffer buf);
#define TLCupdate()  TLCupdateBuf(0)

//! Set channel to specified GS value. Requires update() afterwards.
void    TLCsetBuf(GSBuffer buf, BYTE channel, WORD value);
#define TLCset(chan, val)  TLCsetBuf(0, chan, val)

//! Returns current GS value for specified channel value. Requires update() afterwards.
WORD    TLCgetBuf(GSBuffer buf, BYTE channel);
#define TLCget(chan)  TLCgetBuf(0, chan)

//! Set all channels to specified GS value. Requires update() afterwards.
void    TLCsetAllBuf(GSBuffer buf, WORD value);
#define TLCsetAll(val)  TLCsetAllBuf(0, val)

//! Sets the dot correction for all channels to value, doesn't alter buffers
void    TLCsendAllDC(BYTE value);

//! Sets the dot correction for specified channel to value (in buffer).
void    TLCsetDCBuf(DCBuffer buf, BYTE channel, BYTE value);
#define TLCsetDC(chan, val)  TLCsetDCBuf(0, chan, val)

//! Sets the dot correction for all channels to the values contained in the DC buffer 
void    TLCupdateDCBuf(DCBuffer buf);
#define TLCupdateDC()  TLCupdateDCBuf(0)

#if TLC_XERR_ENABLED
BYTE    TLCreadXERR(void);
#endif

// Not public:
// void      tlc_pin_init(void);
// void      tlc_gs_init(void);
// void      tlc_shift8_init(void);
// void      tlc_shift8(BYTE byte);
// BYTE      tlc_requestXLAT(void);
// BYTE      tlc_releaseXLATreq(void);

#endif  // TLC594X.h
