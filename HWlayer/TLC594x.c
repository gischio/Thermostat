//! TLC594x.c
//!
//! Progetto EK-EQ2_TP
//! Funzioni interfaccia HW MSP430(F6724) per LED Driver TLC594x
//! v0.1 2014-10-01 GCC

//! Parts from the std Arduino TLC5940 GPL Library by Alex Leone

#include "helpers.h"
#include "TLC594X_CFG.h"
#include "TLC594X.h"

// #define per compilare il codice per l'uso sui PCB del primo lotto di prototipi.
// NB L'USO DEI PCB PROTOTIPO RICHIEDE ANCHE L'USO DELLA LIBRERIA 
// knxSystem6724_uart.lib MODIFICATA PER AVERE IL SEGNALE SAVEB IN INPUT SU P3.7
// anzichè su P2.7 come standard.
// (Copia della libreria è salvata nel progetto come knxSystem6724_uart_saveP3_7.lib.)
#define PCB_VERS_PROTOTYPE 1

/** Packed grayscale data, 24 bytes (16 * 12 bits) per TLC.

    Format: Lets assume we have 2 TLCs, A and B, daisy-chained with the SOUT of
    A going into the SIN of B.
    - byte 0: upper 8 bits of B.15
    - byte 1: lower 4 bits of B.15 and upper 4 bits of B.14
    - byte 2: lower 8 bits of B.0
    - ...
    - byte 24: upper 8 bits of A.15
    - byte 25: lower 4 bits of A.15 and upper 4 bits of A.14
    - ...
    - byte 47: lower 8 bits of A.0

    \note Normally packing data like this is bad practice.  But in this
          situation, shifting the data out is really fast because the format of
          the array is the same as the format of the TLC's serial interface. */
GSBuffer   tlc_GSData;

/** Packed DC data, 12 bytes (16 * 6 bits) per TLC.
    */
DCBuffer   tlc_DCData;

/** Don't add an extra SCLK pulse after switching from dot-correction mode. */
static   BYTE   firstGSafterDC;

// This will be true (!= 0) if update was just called and the data has not been latched in yet. 
volatile BYTE   tlc_XLAT_pending;

// Driver in blanking state
volatile BYTE   tlc_blanking_on;


// Callback after a successful update for the application .
// volatile void (*tlc_onUpdateFinished)(void);

// Interrupt called after an XLAT pulse to prevent more XLAT pulses.
// See further below for this.
// ISR(TIMER1_OVF_vect) {...};

// ========================================================
//  Basic pin Setup
// ========================================================

void  tlc_pin_init(void) {

    CLRBITN(TLC_XLAT_SEL, TLC_XLAT_PIN); 	// Define pin as I/O
    SETBITN(TLC_XLAT_DIR, TLC_XLAT_PIN);	// Output
    CLRBITN(TLC_XLAT_REN, TLC_XLAT_PIN); 	// Pullup/dn disabled
    CLRBITN(TLC_XLAT_DS,  TLC_XLAT_PIN);    // Reduced drive strength

    CLRBITN(TLC_XLAT_OUT, TLC_XLAT_PIN);    // XLAT=0

    CLRBITN(TLC_MODE_SEL, TLC_MODE_PIN); 	
    SETBITN(TLC_MODE_DIR, TLC_MODE_PIN);	// Output	
    CLRBITN(TLC_MODE_REN, TLC_MODE_PIN); 	
    CLRBITN(TLC_MODE_DS,  TLC_MODE_PIN); 		

    CLRBITN(TLC_MODE_OUT, TLC_MODE_PIN); 	// grayscale mode (MODE low)		

#if TLC_XERR_ENABLED
    //TLC_XERR_DIR &= ~_BV(TLC_XERR_PIN);   // XERR as input
    //TLC_XERR_OUT |= _BV(TLC_XERR_PIN);   // enable pull-up resistor

    CLRBITN(TLC_XERR_SEL, TLC_XERR_PIN); 	// Define pin as I/O
    CLRBITN(TLC_XERR_DIR, TLC_XERR_PIN);		// Input
    SETBITN(TLC_XERR_REN, TLC_XERR_PIN); 	// Pullup/dn enabled
    SETBITN(TLC_XERR_OUT, TLC_XERR_PIN); 	// Pull: up (XERR is open drain)
#endif

    // Temporarily define BLANK as I/O and 
    // leave it high until the timers are inited
    CLRBITN(TLC_BLANK_SEL, TLC_BLANK_PIN); 	
    SETBITN(TLC_BLANK_DIR, TLC_BLANK_PIN);	
    CLRBITN(TLC_BLANK_REN, TLC_BLANK_PIN); 	
    CLRBITN(TLC_BLANK_DS, TLC_BLANK_PIN); 	

    SETBITN(TLC_BLANK_OUT, TLC_BLANK_PIN);	// leave blank high

} // tlc_pin_init()

// ========================================================
//  init for grayscale clk (GSCLK + BLANK)
// ========================================================
// Following value gives the prescale factor of the grayscale PWM with respect 
// to the chosen clock source (default ACLK).
// MUST be 2 or higher (and divisible by 2 for a symmetric GSCLK signal).
#define GSPWMDIV 2
// DEBUG:
//#define GSPWMDIV 20

// Width of the BLANK pulse (in GSCLK pulses)
// Not too very wide (<<4096), otherwise affects PWM closing to 100%
// and accuracy of specified PWM setting 
// (i.e. a value of 50 gives a max PWM of less than 99%)
#define GSPWMLAT 10

void  tlc_gs_init(void) {
    // ====================================
    // GSCLK
    // Timer3 - CCR0 usato internamente, CCR1/OUT1 come uscita. 
    // CFG TA3: Out TA3.1 (rimappato su pin 44), clksrc = ACLK, mode=UP
    //          CCR0: val=1..7 (=CCR1*2-1)
    //          CCR1: outmode=011 Set/Reset, val=1..4
    
    // PIN REMAPPING
    //
    // - TA1.1 va rimappato su pin48 (P3.7) dato che pin40 (P2.7) deve essere riservato 
    //   per il segnale SAVEB da stack KNX Tapko.
    //   NB LA MODIFICA SOPRA E' DA FARE SOLO NEI PCB SUCCESSIVI AL PRIMO LOTTO DI PROTOTIPI.
    // - TA3.1 va rimappato su pin44 (P3.3) al posto di TA0.2; dato che originariamente si voleva 
    //   usare TA0 ma è occupato. 
    
    WORD irqen;
    // DISABLE IRQ
    irqen = SFRIE1;
    SFRIE1 = 0;
    // Key for write access
    PMAPKEYID = 0x02D52;    
    PMAPCTL |= PMAPRECFG;   // allow further accesses
    // Remap pins:
    P3MAP7 = PM_TA1_1;
    P3MAP3 = PM_TA3_1;  // P3MAP3 = 0x01D8+0x03; PM_TA3.1 = 25
    // Write invalid key to close access
    PMAPKEYID = 0x0F0F0;
    // RE-ENABLE IRQ
    SFRIE1 = irqen;

    tlc_blanking_on = 1;
    
    // Activate pin P3.3 as peripheral
    P3DIR |= BIT3;
    P3SEL |= BIT3;
    //P3SEL &= ~BIT3;
    

    //TA3CTL - TA3 setup
    TA3CTL &= ~TASSEL1;     // TA3CTL.TASSEL = 01 ACLK
    TA3CTL |= TASSEL0; 

    TA3CTL &= ~(ID1|ID0);   // TA3CTL.ID = 00 No prescaler

    
    TA3CTL &= ~MC1;         // TA3CTL.MC = 01 
    //TA3CTL |= MC0;        // [defer this, will act as start bit]

    TA3CTL &= ~TAIE;        // TA3CTL.TAIE = 0 No IRQ

    TA3CTL |= TACLR;        // TA3CTL.TACLR = 1 Init timer

    // TA3CCR0 - TA3 CCR0 value
    TA3CCR0 = GSPWMDIV-1;       // = 2*TA3CCR1 (-1); used to restart pulse period in set mode

    // TA3CCTL1 - TA3 CCR1 setup
    TA3CCTL1 |= (OUTMOD2|OUTMOD1|OUTMOD0);
    TA3CCTL1 &= ~OUTMOD2;  // TA3CCTL1.OUTMOD = 011 Set/Reset

    // TA3CCR1 - TA3 CCR0 value
    TA3CCR1 = GSPWMDIV / 2;     // Threshold triggers 0->1 transition


    // ====================================
    // BLANK latch (impulso ogni 4096 GSCLK)

    // Uso Timer1.
    // Al conteggio tra 4096 (CCR1) e 4096+<n> (CCR0), dà un impulso su pin TA1.1 e 
    // (se prenotata) chiama la ISR per gestire XLAT. 
    // Se XLAT è prenotata, la ISR è abilitata e <n> è a un valore molto alto 
    // (per dare in modo di eseguire la ISR con il BLANK alto).
    // CFG TA1: Out TA1.1, clksrc = ext (pin 43 TACLK), mode=UP
    //          CCR0: val = 4096+n
    //          CCR1: outmode=011 Set/Reset, val = 4096 

    // Activate P3.2 pin as TACLK input (manual: p.89)
    P3DIR &= ~BIT2;
    P3SEL |= BIT2;

    // Activate pin P3.7 as peripheral (PM_TA1.1 output)
    P3DIR |= BIT7;
    P3SEL |= BIT7;

    //TA1CTL - TA1 setup
    TA1CTL &= ~(TASSEL1|TASSEL0);       // TA1CTL.TASSEL = 00 TACLK

    TA1CTL &= ~(ID1|ID0);               // TA1CTL.ID = 00 No prescaler

    TA1CTL &= ~(MC1|MC0);               // TA1CTL.MC = 00 Timer stopped for now
    // timer started by tlc_requestXLAT()

    // TA1CTL.TAIE = 0  IRQs will be enabled upon request
    // TA1CCTL.CCIE = 0
    // done by TLCsetBlanking(on)

    TA1CTL |= TACLR;                    // TA1CTL.TACLR = 1 Init timer

    // TA1CCR0 - TA1 CCR0 value (4096 pulses + overhead)
    // also done later by calling tlc_releaseXLATreq()
    TA1CCR0 = 4096 + GSPWMLAT;   

    // TA1CCTL1 - TA1 CCR1 setup
    // TA1CCTL1.OUTMOD = 011 Set/Reset - done by TLCsetBlanking(off)
    
    // TA1CCR1 - TA1 CCR1 value
    TA1CCR1 = 4096;

    TLCsetBlanking(1);    // Application code should call TLCsetBlanking(0); as soon as ready
    //TLCsetBlanking(0);
    
} // tlc_gs_init()

// Set LED blanking state:
// raise BLANK line and stop gsclk
void TLCsetBlanking(BYTE status)
{
    if(status) {
        tlc_blanking_on = 1;

        // Set LED blanking state:
        // raise BLANK line and stop gsclk
        TA1CCTL1 &= ~CCIE;   // IRQ OFF for BLANK pulse
        TA1CCTL1 &= ~CCIFG;  // also generic timer int
        TA1CTL  &= ~TAIE;  
        TA1CTL  &= ~TAIFG;  

        TA1CCTL1 &= ~(OUTMOD2|OUTMOD1|OUTMOD0);  // CCR Outmode 0: Output
        TA1CCTL1 |= OUT;    // BLANK out = 1

        TA1CTL &= ~MC0;     // Stop timer for BLANK
        TA3CTL &= ~MC0;     // Stop also timer for GSCLK
        
    } else {
        // Release LED blanking state:
        // Restore gsclk and BLANK timings
        tlc_blanking_on = 0;

        TA1CCTL1 &= ~OUT;   // BLANK out = 1

        // Unless switching towards mode 0, change mode transitioning through mode 7, 
        // as recommended by the manual
        TA1CCTL1 |= (OUTMOD2|OUTMOD1|OUTMOD0);
        TA1CCTL1 &= ~OUTMOD2;  // TA1CCTL1.OUTMOD = 011 Set/Reset

        TA3CTL |= TACLR;   // Init timer
        TA3CTL |= MC0;     // Start timer for GSCLK
        TA1CTL |= TACLR;   // Init timer
        TA1CTL |= MC0;     // Start timer for BLANK
    }
}

// Flag XLAT issue request at the nearest convenience of BLANK=1
// after having sent GS or DC data
BYTE tlc_requestXLAT(void)
{
    // If blanking is on, just issue pulse
    if(tlc_blanking_on) { 
    //if (1 || tlc_blanking_on) { 
        PULSE_HL(TLC_XLAT_OUT, TLC_XLAT_PIN);
        tlc_XLAT_pending = 0;
    } else {
        if (tlc_XLAT_pending) { return 1; }
        // Set an exorbitant value for BLANK reset in order to allow time for the ISR to be called
        TA1CCR0 = 4096 * 3; 
        // flag request
        tlc_XLAT_pending = 1;
        // Enable BLANK INT that will issue XLAT
        TA1CCTL1 |= CCIE;
    }
    return 0;
}

// Remove pending XLAT issue requests (or init)
BYTE tlc_releaseXLATreq(void)
{
    // if (!tlc_XLAT_pending) { return 1; }
    // Return to default CCR0 value (4096 pulses + overhead)
    TA1CCR0 = 4096 + GSPWMLAT;
    // Disable BLANK INT
    TA1CCTL0 &= ~CCIE; TA1CCTL0 &= ~CCIFG;
    TA1CCTL1 &= ~CCIE; TA1CCTL1 &= ~CCIFG;
    // Clear BLANK INT
    TA1CTL &= ~TAIE;
    TA1CTL &= ~TAIFG;   // also generic timer int
    tlc_XLAT_pending = 0;

    return 0;
}

// ISR TimerA1/CCR1 (BLANK)
// If it gets called, an XLAT was scheduled for issue
#define INTERRUPT_HANDLER   TIMER1_A1
#include <interrupt.h>
//Above #defs result in:
//#pragma vector=TIMER1_A1_VECTOR
//__interrupt void ISR_TIMER1_A1(void)
{
  // (Is the 'switch' correct? What if several interrupts are pending,
  //  and TA1IV_TA1CCR1 has more than one bit set?)
  switch (TA1IV)
  {
      
    case TA1IV_TA1CCR1:               // Capture / Compare 1 (Start of BLANK pulse)
        // Check whether the BLANK signal is still high (otherwise we were called way too late)
        if(!GETBITS(TA1CCTL1, OUT)) {
            //...what now?
        }
        // Stop the timer to gain time for our business
        TA3CTL &= ~MC0;
        // Issue XLAT pulse
        PULSE_HL(TLC_XLAT_OUT, TLC_XLAT_PIN);
        // Add an extra SCLK pulse unless we've just set dot-correction data
        // (see comments in TLCupdate())
        if (firstGSafterDC) {
           firstGSafterDC = 0;
        } else {
           PULSE_HL(TLC_SCK_OUT, TLC_SCK_PIN);
        }
        // Restart the timer
        TA3CTL |= TACLR;
        TA3CTL |= MC0;
        
        // Disable further interrupts - we are done
        tlc_releaseXLATreq(); 
        break;
    
    case TA1IV_TA1IFG:                // Timer overflow
        // Hmmm, this is unintended.
        break;
    default:
        break;
  }
  // Provision for callback for support to extended application functions:
  // if (tlc_onUpdateFinished) {
  //     sei();
  //     tlc_onUpdateFinished();
  // }
}

#if DATA_TRANSFER_MODE == TLC_BITBANG

// ========================================================
//  init for grayscale / DC data shift (MOSI + SCK + XLAT)
// ========================================================

/** Sets all the bit-bang pins to output */
void tlc_shift8_init(void)
{
    CLRBITN(TLC_MOSI_SEL, TLC_MOSI_PIN); 	// Define pin as I/O
    SETBITN(TLC_MOSI_DIR, TLC_MOSI_PIN);	// Output
    CLRBITN(TLC_MOSI_REN, TLC_MOSI_PIN); 	// Pullup/dn disabled
    CLRBITN(TLC_MOSI_DS, TLC_MOSI_PIN); 	// Reduced drive strength

    CLRBITN(TLC_SCK_SEL, TLC_SCK_PIN); 		// Define pin as I/O
    SETBITN(TLC_SCK_DIR, TLC_SCK_PIN);		// Output
    CLRBITN(TLC_SCK_REN, TLC_SCK_PIN); 		// Pullup/dn disabled
    CLRBITN(TLC_SCK_DS, TLC_SCK_PIN); 		// Reduced drive strength

    CLRBITN(TLC_SCK_OUT, TLC_SCK_PIN);
}

#elif DATA_TRANSFER_MODE == TLC_SPI

/** Initializes the SPI module to double speed (f_osc / 2) */
void tlc_shift8_init(void)
{
    // Initiate SPI setup
    UCA2CTLW0 |= UCSWRST;
    
    // Initialize USCIregisters
    UCA2CTLW0 |= UCCKPH;             // Clk phase ( =1 ?) (Man. section 35.3.6.1, see also register description)
    UCA2CTLW0 &= ~UCCKPL;            // Clk polarity

    UCA2CTLW0 |= UCMSB;              // MSB first
    // default: 8 bits
    UCA2CTLW0 |= UCMST;              // Master mode
    UCA2CTLW0 &= ~(UCMODE1|UCMODE0); // 3-pin SPI (default)
    UCA2CTLW0 |= UCSYNC;             // Sync mode (SPI)

    UCA2CTLW0 &= ~UCSSEL1;           // UCSSEL = 01 Clk source: ACLK
    UCA2CTLW0 |= UCSSEL0;
    UCA2BR0 = 0x10;                  // Clk Prescaler=16 yields ~250us for the complete burst
    UCA2BR1 = 0x00;
    //UCA2BR1 = 0x01;          // DEBUG
    //UCA2MCTL = 0x00;                // ?? required?
    
    // Configure port pins
    SETBITN(TLC_MOSI_SEL, TLC_MOSI_PIN); 	// Define pin as peripheral
    SETBITN(TLC_MOSI_DIR, TLC_MOSI_PIN);	// Output
    CLRBITN(TLC_MOSI_REN, TLC_MOSI_PIN); 	// Pullup/dn disabled
    CLRBITN(TLC_MOSI_DS, TLC_MOSI_PIN); 	// Reduced drive strength

    SETBITN(TLC_SCK_SEL, TLC_SCK_PIN); 		// Define pin as peripheral
    SETBITN(TLC_SCK_DIR, TLC_SCK_PIN);		// Output
    CLRBITN(TLC_SCK_REN, TLC_SCK_PIN); 		// Pullup/dn disabled
    CLRBITN(TLC_SCK_DS, TLC_SCK_PIN); 		// Reduced drive strength

    // End SPI setup
    UCA2CTLW0 &= ~UCSWRST;

    // Enable interrupts
    // (not required)
    
    ///SPSR = _BV(SPI2X); // double speed (f_osc / 2)
    ///SPCR = _BV(SPE) | _BV(MSTR);  // enable SPI/master mode
}

#endif

/** Pin i/o and Timer setup.  The grayscale register will be reset to all
    zeros, or whatever initialValue is set to and the Timers will start.
    \param initialValue = 0, optional parameter specifing the inital startup
           value */
void TLCinit(WORD initialValue)
{

    tlc_pin_init();
    tlc_gs_init();
    tlc_shift8_init();
    tlc_releaseXLATreq();
    
    // gsclk is still inactive, so let's load the GS data "manually":
    TLCsetAll(initialValue);
    TLCupdate();
    PULSE_HL(TLC_XLAT_OUT, TLC_XLAT_PIN);

    // Start the GS PWM with newly loaded data
    TLCsetBlanking(0);
}

#if DATA_TRANSFER_MODE == TLC_BITBANG
/** Shifts a byte out, MSB first */
void tlc_shift8(BYTE byte)
{
    for (BYTE bit = 0x80; bit; bit >>= 1) {
        if (bit & byte) {
            SETBITN(TLC_MOSI_OUT, TLC_MOSI_PIN);
        } else {
            CLRBITN(TLC_MOSI_OUT, TLC_MOSI_PIN);
        }
        PULSE_HL(TLC_SCK_OUT, TLC_SCK_PIN);
    }
}

#elif DATA_TRANSFER_MODE == TLC_SPI

/** Shifts out a byte, MSB first */
void tlc_shift8(BYTE byte)
{
    UCA2TXBUF = byte;
    while (GETBITS(UCA2STATW, UCBUSY)); // wait for transmission complete
}

#endif

#if TLC_XERR_ENABLED

/** Checks for shorted/broken LEDs reported by any of the TLCs.
    \returns 1 if a TLC is reporting an error, 0 otherwise. */
BYTE TLCreadXERR(void)
{
    return (GETBIT(TLC_XERR_IN, TLC_XERR_PIN) == 0);
}

#endif

/** Shifts in the data from the grayscale data array, #tlc_GSData.
    If data has already been shifted in this grayscale cycle, another call to
    TLCupdate() will immediately return 1 without shifting in the new data.  To
    ensure that a call to TLCupdate() does shift in new data, use
    \code while(TLCupdate()); \endcode
    or
    \code while(tlc_XLAT_pending); \endcode
    \returns 1 if there is data waiting to be latched, 0 if data was
             successfully shifted in */
BYTE TLCupdateBuf(GSBuffer buf)
{
    if (tlc_XLAT_pending) { return 1; }
    
    ///disable_TLC_XLAT_pulses();
    
    // Add an extra SCLK pulse unless we've just set dot-correction data.
    
    // Important: The datasheet appears to say different ("The first GS data input cycle 
    // after dc requires an additional SCLK after XLAT to complete the GS update cycle").
    // The sentence is misleading!!! It sounds like _only_in_this_case_ 193 pulses are required.
    // Actually, the timing chart correctly shows that normally there are _always_ 193 SCLK pulses
    // (only, the 193rd is after the XLAT; anyway, there is normally a count of 193 pulses between 
    // any 2 XLATs). This is apparent in the general timing chart, though not in the specific 
    // GS timing chart (which is misleading).
    // Why 193 pulses if there are 192 bits? Because the outbound SOUT data are out of phase  
    // by 1 with the SIN data, so it takes 193 pulses to complete both SIN and SOUT shifts.
    // What does the notice in the datasheet mean then? If the last shift was a DC, there is
    // an additional delay in making the SOUT data ready (see the "X" in the SOUT row), 
    // therefore the last SOUT bit is completed with the 193rd pulse; if you look at the 
    // following sequences for comparison, you'll notice that the next SID MSB is normally out 
    // at #192. This is what is meant by "complete the update cycle".
    // In the end, however, the situation is following: between any two XLATS there are normally 
    // 193 SCLK pulses, except after the XLAT of a DC shift - in this case the pulses are 192.
    // We could say that, in this case, the first pulse after the XLAT is missing because it would 
    // be #193 of the previous cycle - if it had been a GS cycle rather than a DC cycle (which does 
    // not need it).

    // THE ADDITIONAL SCK HAS BEEN MOVED IN THE ISR right after the XLAT pulse (TO BE TESTED).
    // In order for XERR transfer to be correct, the 193rd SCK should be done immediately, not 
    // with the next TLCupdate().
    //if (firstGSafterDC) {
    //    firstGSafterDC = 0;
    //} else {
    //    PULSE_HL(TLC_SCK_OUT, TLC_SCK_PIN);
    //}
    
    BYTE *p = (buf ? buf : tlc_GSData);
    BYTE *top = p + NUM_TLCS * 24;
    while (p < top) {
        tlc_shift8(*p++);
        tlc_shift8(*p++);
        tlc_shift8(*p++);
    }
    
    // Trigger XLAT as soon as possible
    // (if blanking is active, it is issued immediately)
    tlc_requestXLAT();
    
    ///tlc_XLAT_pending = 1;
    ///enable_TLC_XLAT_pulses();
    ///set_TLC_XLAT_interrupt();
    return 0;
}

/** Clears the grayscale data array, #tlc_GSData, but does not shift in any
    data.  This call should be followed by TLCupdate() if you are turning off
    all the outputs. */
void TLCclearBuf(GSBuffer buf)
{
    TLCsetAll(0);
}

/** Sets channel to value in the grayscale data array, #tlc_GSData.
    \param channel (0 to #NUM_TLCS * 16 - 1).  OUT0 of the first TLC is
           channel 0, OUT0 of the next TLC is channel 16, etc.
    \param value (0-4095).  The grayscale value, 4095 is maximum.
    \see get */
void TLCsetBuf(GSBuffer buf, BYTE channel, WORD value)
{
    BYTE *p = (buf ? buf : tlc_GSData);
    BYTE index8 = (NUM_TLCS * 16 - 1) - channel;
    BYTE *index12p = p + ((((WORD)index8) * 3) >> 1);
    if (index8 & 1) { // starts in the middle
                      // first 4 bits intact | 4 top bits of value
        *index12p = (*index12p & 0xF0) | (value >> 8);
                      // 8 lower bits of value
        *(++index12p) = value & 0xFF;
    } else { // starts clean
                      // 8 upper bits of value
        *(index12p++) = value >> 4;
                      // 4 lower bits of value | last 4 bits intact
        *index12p = ((BYTE)(value << 4)) | (*index12p & 0xF);
    }
}

/** Gets the current grayscale value for a channel
    \param channel (0 to #NUM_TLCS * 16 - 1).  OUT0 of the first TLC is
           channel 0, OUT0 of the next TLC is channel 16, etc.
    \returns current grayscale value (0 - 4095) for channel
    \see set */
WORD TLCgetBuf(GSBuffer buf, BYTE channel)
{
    BYTE *p = (buf ? buf : tlc_GSData);

    BYTE index8 = (NUM_TLCS * 16 - 1) - channel;
    BYTE *index12p = p + ((((WORD)index8) * 3) >> 1);
    return (index8 & 1)? // starts in the middle
            (((WORD)(*index12p & 15)) << 8) | // upper 4 bits
            *(index12p + 1)                       // lower 8 bits
        : // starts clean
            (((WORD)(*index12p)) << 4) | // upper 8 bits
            ((*(index12p + 1) & 0xF0) >> 4); // lower 4 bits
}

/** Sets all channels to value.
    \param value grayscale value (0 - 4095) */
void TLCsetAllBuf(GSBuffer buf, WORD value)
{
    BYTE *p = (buf ? buf : tlc_GSData);
    BYTE firstByte = value >> 4;
    BYTE secondByte = (value << 4) | (value >> 8);
    BYTE *top = p + NUM_TLCS * 24;
    while (p < top) {
        *p++ = firstByte;
        *p++ = secondByte;
        *p++ = (BYTE)value;
    }
}

/** Sets the dot correction for all channels to value.
    The DC value buffer accessible through TLCsetDC(channel, value) 
    is left untouched.
    No other operation is required afterwards to complete DC setting.
    The dot correction value correspondes to maximum output current by
    \f$\displaystyle I_{OUT_n} = I_{max} \times \frac{DCn}{63} \f$
    where
    - \f$\displaystyle I_{max} = \frac{1.24V}{R_{IREF}} \times 31.5 =
         \frac{39.06}{R_{IREF}} \f$
    - DCn is the dot correction value for channel n
    \param value (0-63) DC value. */
void TLCsendAllDC(BYTE value)
{
    ///tlc_dcModeStart();
    // Switch to dot correction mode and clear any waiting grayscale latches
    tlc_releaseXLATreq();
    SETBITN(TLC_MODE_OUT, TLC_MODE_PIN); 	// Dot Correction mode (MODE high)

    BYTE firstByte = value << 2 | value >> 4;
    BYTE secondByte = value << 4 | value >> 2;
    BYTE thirdByte = value << 6 | value;

    for (BYTE i = 0; i < NUM_TLCS * 12; i += 3) {
        tlc_shift8(firstByte);
        tlc_shift8(secondByte);
        tlc_shift8(thirdByte);
    }
    // For DC, no need to wait for BLANK to be H in order to pulse XLAT
    PULSE_HL(TLC_XLAT_OUT, TLC_XLAT_PIN);

    ///tlc_dcModeStop();
    // Switch back to grayscale mode
    CLRBITN(TLC_MODE_OUT, TLC_MODE_PIN); 	// back to grayscale mode (MODE low)		
    firstGSafterDC = 1;
}


/** Sets the dot correction for specified channel to value.
    Just fills buffer with specified values; 
    requires a call to TLCsetAllDC(void) afterwards for effect.
    \param channel (0 to #NUM_TLCS * 16 - 1).  OUT0 of the first TLC is
           channel 0, OUT0 of the next TLC is channel 16, etc.
    \param value (0-63) */
void TLCsetDCBuf(DCBuffer buf, BYTE channel, BYTE value)
{
    BYTE *p = (buf ? buf : tlc_DCData);
    // index8 is the channel #, but corrected for reversed order and 
    // possibly over multiple TLCs
    BYTE index8 = (NUM_TLCS * 16 - 1) - channel;
    // index6p is the address of the first (or only) byte in the buffer 
    // to be affected by the value
    BYTE *index6p = p + ((((WORD)index8) * 3) >> 2);

    value &= 0x3F;      // make sure <value> is no more than 6 bits wide 
    index8 &= 0x03;
    switch(index8) {
        case 0:
            *index6p = (*index6p & 0x03) | ((value << 2) & 0xFC);
            break;
        case 1:
            *index6p = (*index6p & 0xFC) | (value >> 4);
            index6p++;
            *index6p = (*index6p & 0x0F) | ((value << 4) & 0xF0);
            break;
        case 2:
            *index6p = (*index6p & 0xF0) | (value >> 2);
            index6p++;
            *index6p = (*index6p & 0x3F) | ((value << 6) & 0xC0);
            break;
        case 3:
            *index6p = (*index6p & 0xC0) | value;
            break;
    }
}

/** Sets the dot correction for all channels to the values contained in 
    the DC buffer (which has supposedly just been filled through
    TLCsetDC(channel, value).    
    No other operation is required afterwards to complete DC setting.
    The dot correction value correspondes to maximum output current by
    \f$\displaystyle I_{OUT_n} = I_{max} \times \frac{DCn}{63} \f$
    where
    - \f$\displaystyle I_{max} = \frac{1.24V}{R_{IREF}} \times 31.5 =
         \frac{39.06}{R_{IREF}} \f$
    - DCn is the dot correction value for channel n
    */
void TLCupdateDCBuf(DCBuffer buf)
{
    // Switch to dot correction mode and clear any waiting grayscale latches
    tlc_releaseXLATreq();
    SETBITN(TLC_MODE_OUT, TLC_MODE_PIN); 	// Dot Correction mode (MODE high)

    BYTE *p = (buf ? buf : tlc_DCData);
    BYTE *top = p + NUM_TLCS * 12;
    while (p < top) {
        tlc_shift8(*p++);
    }

    // For DC, no need to wait for BLANK to be H in order to pulse XLAT
    PULSE_HL(TLC_XLAT_OUT, TLC_XLAT_PIN);

    // Switch back to grayscale mode
    CLRBITN(TLC_MODE_OUT, TLC_MODE_PIN); 	// back to grayscale mode (MODE low)		
    firstGSafterDC = 1;
}

// end TLC594X.c
