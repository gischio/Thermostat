//! TLC594x_CFG.h
//!
//! Progetto EK-EQ2_TP
//! Funzioni interfaccia HW MSP430(F6724) per LED Driver TLC594x
//! v0.1 2014-10-01 GCC

//! Parts from the std Arduino TLC5940 GPL Library by Alex Leone

#ifndef TLC594X_CFG_H
#define TLC594X_CFG_H


/** \file
    Configuration constants for the Tlc594x library
*/

/** Number of TLCs daisy-chained.  To daisy-chain, attach the SOUT (TLC pin 17)
    of the first TLC to the SIN (TLC pin 26) of the next.  The rest of the pins
    are attached normally.
    \note Each TLC needs it's own IREF resistor */
#define NUM_TLCS           1

/** Bit-bang using any two i/o pins */
#define TLC_BITBANG        1
/** Use the much faster hardware SPI module */
#define TLC_SPI            2

/** Determines how data should be transfered to the TLCs.  Bit-banging can use
    any two i/o pins, but the hardware SPI is faster.
    - Bit-Bang = TLC_BITBANG
    - Hardware SPI = TLC_SPI (default) */
#define DATA_TRANSFER_MODE    TLC_SPI
//#define DATA_TRANSFER_MODE    TLC_BITBANG

/** Determines how long each PWM period should be, in clocks.
    \f$\displaystyle f_{PWM} = \frac{f_{osc}}{2 * TLC\_PWM\_PERIOD} Hz \f$
    \f$\displaystyle TLC\_PWM\_PERIOD = \frac{f_{osc}}{2 * f_{PWM}} \f$
    This is related to TLC_TLC_GSCK_PERIOD:
    \f$\displaystyle TLC\_PWM\_PERIOD =
       \frac{(TLC\_GSCLK\_PERIOD + 1) * 4096}{2} \f$
    \note The default of 8192 means the PWM frequency is 976.5625Hz */
#define TLC_PWM_PERIOD    8192

/** Determines how long each period GSCLK is.
    This is related to TLC_PWM_PERIOD:
    \f$\displaystyle TLC\_GSCLK\_PERIOD =
       \frac{2 * TLC\_PWM\_PERIOD}{4096} - 1 \f$
    \note Default is 3 */
#define TLC_TLC_GSCK_PERIOD    3

/** Enables/disables XERR (TLC pin 16) functionality to check for shorted/broken
    LEDs
    - 0 XERR is not connected (default)
    - 1 XERR is connected
    \note XERR is active low */
#define TLC_XERR_ENABLED    0

// In following definition, the bit number is used, not the mask
// (e.g. for bit 3, use '3' not '0x08').

#if DATA_TRANSFER_MODE == TLC_SPI
// Uses USCI_A #2
// Pins are:
// MOSI > P2.3
// MISO > P2.2  (unused)
// SCK  > P2.5
#endif

#define TLC_MOSI_PIN      3
#define TLC_MOSI_IN       P2IN
#define TLC_MOSI_OUT      P2OUT //(TLC_MOSI_IN+0x02)
#define TLC_MOSI_DIR      P2DIR //(TLC_MOSI_IN+0x04)
#define TLC_MOSI_REN      P2REN //(TLC_MOSI_IN+0x06)
#define TLC_MOSI_DS       P2DS  //(TLC_MOSI_IN+0x08)
#define TLC_MOSI_SEL      P2SEL //(TLC_MOSI_IN+0x10)

#define TLC_MISO_PIN      2
#define TLC_MISO_IN       P2IN
#define TLC_MISO_OUT      P2OUT //(TLC_MISO_IN+0x02)
#define TLC_MISO_DIR      P2DIR //(TLC_MISO_IN+0x04)
#define TLC_MISO_REN      P2REN //(TLC_MISO_IN+0x06)
#define TLC_MISO_DS       P2DS  //(TLC_MISO_IN+0x08)
#define TLC_MISO_SEL      P2SEL //(TLC_MISO_IN+0x10)

#define TLC_SCK_PIN       5
#define TLC_SCK_IN        P2IN
#define TLC_SCK_OUT       P2OUT //(TLC_SCK_IN+0x02)
#define TLC_SCK_DIR       P2DIR //(TLC_SCK_IN+0x04)
#define TLC_SCK_REN       P2REN //(TLC_SCK_IN+0x06)
#define TLC_SCK_DS        P2DS  //(TLC_SCK_IN+0x08)
#define TLC_SCK_SEL       P2SEL //(TLC_SCK_IN+0x10)

#if !(DATA_TRANSFER_MODE == TLC_BITBANG \
 || DATA_TRANSFER_MODE == TLC_SPI)
#error "Invalid DATA_TRANSFER_MODE specified, see DATA_TRANSFER_MODE"
#endif

#define TLC_SS_PIN        1
#define TLC_SS_IN         P1IN
#define TLC_SS_OUT        P1OUT //(TLC_SS_IN+0x02)
#define TLC_SS_DIR        P1DIR //(TLC_SS_IN+0x04)
#define TLC_SS_REN        P1REN //(TLC_SS_IN+0x06)
#define TLC_SS_DS         P1DS  //(TLC_SS_IN+0x08)
#define TLC_SS_SEL        P1SEL //(TLC_SS_IN+0x10)

#define TLC_XLAT_PIN      4
#define TLC_XLAT_IN       P2IN
#define TLC_XLAT_OUT      P2OUT //(TLC_XLAT_IN+0x02)
#define TLC_XLAT_DIR      P2DIR //(TLC_XLAT_IN+0x04)
#define TLC_XLAT_REN      P2REN //(TLC_XLAT_IN+0x06)
#define TLC_XLAT_DS       P2DS  //(TLC_XLAT_IN+0x08)
#define TLC_XLAT_SEL      P2SEL //((&TLC_XLAT_IN)+0x10)

#define TLC_MODE_PIN      6
#define TLC_MODE_IN       P2IN
#define TLC_MODE_OUT      P2OUT //(TLC_MODE_IN+0x02)
#define TLC_MODE_DIR      P2DIR //(TLC_MODE_IN+0x04)
#define TLC_MODE_REN      P2REN //(TLC_MODE_IN+0x06)
#define TLC_MODE_DS       P2DS  //(TLC_MODE_IN+0x08)
#define TLC_MODE_SEL      P2SEL //(TLC_MODE_IN+0x10)

#if     TLC_XERR_ENABLED
#define TLC_XERR_PIN      x
#define TLC_XERR_IN       PxIN
#define TLC_XERR_OUT      PxOUT //(TLC_XERR_IN+0x02)
#define TLC_XERR_DIR      PxDIR //(TLC_XERR_IN+0x04)
#define TLC_XERR_REN      PxREN //(TLC_XERR_IN+0x06)
#define TLC_XERR_DS       PxS  //(TLC_XERR_IN+0x08)
#define TLC_XERR_SEL      PxSEL //(TLC_XERR_IN+0x10)
#endif
// These are forced by the use of timers
// Listed for reference only
// TLC_BLANK_PIN     P3.7 (PM_TA1.1) - used as actual IO pin just for init
// TLC_GSCK_PIN      P3.3 (remapped to PM_TA3.1)
#define TLC_BLANK_PIN      7
#define TLC_BLANK_IN       P3IN
#define TLC_BLANK_OUT      P3OUT //(TLC_MODE_IN+0x02)
#define TLC_BLANK_DIR      P3DIR //(TLC_MODE_IN+0x04)
#define TLC_BLANK_REN      P3REN //(TLC_MODE_IN+0x06)
#define TLC_BLANK_DS       P3DS  //(TLC_MODE_IN+0x08)
#define TLC_BLANK_SEL      P3SEL //(TLC_MODE_IN+0x10)


/* Various Macros */

/** Arranges 2 grayscale values (0 - 4095) in the packed array format (3 bytes).
    This is for array initializers only: the output is three comma seperated
    8-bit values. */
#define GS_DUO(a, b)    ((a) >> 4), ((a) << 4) | ((b) >> 8), (b)

/** Arranges 4 dot correction values (0 - 63) in the packed array format.
    \see setDCtoProgmemArray */
#define DC_QUARTET(a, b, c, d)   ((a) << 2) | ((b) >> 4), \
                                 ((b) << 4) | ((c) >> 2), \
                                 ((c) << 6) | (d)

#endif  // TLC594X_CFG.h
