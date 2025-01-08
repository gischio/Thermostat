//! helpers.h
//!
//! Progetto EK-EQ2_TP
//! Macro ausiliarie
//! v0.1 2014-10-01 GCC

// Funzioni SPECIFICHE (dipendenti dal mapping dei segmenti)

#ifndef HELPERS_H
#define HELPERS_H

// ==============================================
//  Generic helper macros

#define _BV(x) (1<<x)
#define SETBITN(_byte, _bitnr)  {_byte|=(1<<_bitnr);} 
#define CLRBITN(_byte, _bitnr)  {_byte&=(~(1<<_bitnr));} 
#define TGLBITN(_byte, _bitnr)  {_byte^=(1<<_bitnr);}
#define SETBITS(_byte, _mask) {_byte|=_mask;}
#define CLRBITS(_byte, _mask) {_byte&=(~_mask);} 
#define TGLBITS(_byte, _mask) {_byte^=_mask;}

#define GETBIT(_byte, _bitnr) ((_byte & (1<<_bitnr)) != 0)
#define GETBITS(_byte, _bitnr) ((_byte & _bitnr) != 0)

// Pulses a pin - high then low. 
// #define PULSE_HL(port, pin)   port |= _BV(pin); port &= ~_BV(pin)
#define PULSE_HL(_port, _pin) {SETBITN(_port, _pin); CLRBITN(_port, _pin);} 
// Pulses a pin - low then high. 
#define PULSE_LH(_port, _pin) {CLRBITN(_port, _pin); SETBITN(_port, _pin);} 
// Pulses a pin - toggle 
#define PULSE_TGL(_port, _pin) {TGLBITN(_port, _pin); TGLBITN(_port, _pin);} 

#endif  // helpers.h