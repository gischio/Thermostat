//! peripheral.c
//!
//! Funzioni interfaccia HW basso livello per EK-EQ2-TP
//! v0.1 2014-10-01 GCC

#include "project.h"

#include "peripheral.h"

//#include "application.h"
//#include "ApplicationUtility.h"
//#include "ApplicationTimers.h"


// Vector of debounced key flags
WORD keyVector;
// Duration of debounce period (in ticks)
WORD debounceTicks;

// Struct of analog values
t_AVal       Sensors;
t_AValRaw    SensorsRaw;

t_avgAcc sensorValAcc[N_SENSOR_AVG];    // Short-term average
t_avgAcc sensorAvgAcc[N_SENSOR_AVG];    // Long-term average

//long   sensorValAcc[6];
//BYTE   sensorValCnt[6];

// Struct of vectors for LED on/off and blink status
hwLEDset LEDs;

// Buffer for current TARGET LED brightness values
// Values in user units (0..255)
static
BYTE hwLEDval[16]; //[NUM_TLCS * 16];

// Buffer for current ACTUAL LED brightness values (for fading)
// Values in user units (0..255)
static
BYTE hwLEDval_actual[16]; //[NUM_TLCS * 16];

#if BUILD_APP != EK_ET2
// LCD backlight PWM status
static
BYTE LCDPWMfade = 1;
static
BYTE LCDPWMlevel;
static
BYTE LCDcurrPWMlevel;
#endif  

#if BUILD_APP == EK_EQ2
    #define hwPollHygro(n) {SHT21_poll(n);}
#else
    #define hwPollHygro(n) {}
#endif

// ============================================================================
//  Initialization

//! Init HW: Digital I/Os (keys, jumpers, aux)
//!
//! Normally called by PeripheralInit()
//#pragma inline=forced
void  hwDIOInit(void) {
    // Keyboard
    P4SEL &= ~(BIT3|BIT2|BIT1|BIT0);    // Usage: Dig I/O
    P4DIR &= ~(BIT3|BIT2|BIT1|BIT0);    // Direction: input
    P4REN |=  (BIT3|BIT2|BIT1|BIT0);    // PullUp/Dn enabled
    P4OUT |=  (BIT3|BIT2|BIT1|BIT0);    // Pull: Up
    P4DS  &= ~(BIT3|BIT2|BIT1|BIT0);        // Drive strength: low

    // Jumpers and AUX IOs
    P4SEL &= ~(BIT6|BIT5|BIT4);         // Usage: Dig I/O
    P4DIR &= ~(BIT6|BIT5|BIT4);         // Direction: input
    P4REN |=  (BIT6|BIT5|BIT4);         // PullUp/Dn enabled
    P4OUT &= ~(BIT6|BIT5|BIT4);         // Pull: Dn
    P4DS  &= ~(BIT6|BIT5|BIT4);         // Drive strength: low

    P3SEL &= ~(BIT6|BIT5|BIT4);         // Usage: Dig I/O
    P3DIR &= ~(BIT6|BIT5|BIT4);         // Direction: input
    P3REN |=  (BIT6|BIT5|BIT4);         // PullUp/Dn enabled
    P3OUT &= ~(BIT6|BIT5|BIT4);         // Pull: Dn
    P3DS  &= ~(BIT6|BIT5|BIT4);         // Drive strength: low

    // Sensor power supply
    P3SEL &= ~(BIT0);                   // Usage: Dig I/O
    P3DIR |=  (BIT0);                   // Direction: Output
    P3REN &= ~(BIT0);                   // PullUp/Dn disabled
    P3OUT &= ~(BIT0);                   // Out: 0

    // Unused IOs
    P1SEL &= ~(BIT3);                   // Usage: Dig I/O
    P1DIR &= ~(BIT3);                   // Direction: input
    P1REN |=  (BIT3);                   // PullUp/Dn enabled
    P1OUT &= ~(BIT3);                   // Pull: Dn
    P1DS  &= ~(BIT3);                   // Drive strength: low

    // PGM key and LED
    //   Managed by TAPKO KNX stack
    P1REN |= (BIT6);                    // Key: PullUp/Dn enabled
    P1OUT |= (BIT6);                    // Key: Pull: Up

    // SAVEB (shutdown) input
    //   Managed by TAPKO KNX stack

}

//! Init HW: LED driver
//!
//! Normally called by PeripheralInit()
//! Must be called AFTER hwDIOInit() (needs to read jumper setting)
void  hwLEDDriverInit(void) {

    // Uses:
    // TimerA_1 (GSCLK)
    // TimerA_3 (BLANK)
    // (optional) USCI_A2 (SPI)
    TLCinit(0);

    LEDs.status = 0x0000;
    LEDs.blinkStatus    = 0x0000;
    LEDs.TA_blinkStatus = 0x0000;
    LEDs.fadeMask       = 0xFFFF;
    LEDs.blinkFadeMask  = 0x0000;

    // Init blink times to 500ms/500ms
    for(BYTE i=0; i < 16; i++) {
        LEDs.Ton[i] = LEDs.Toff[i] = 500;
    }
    LEDs.TA_Ton = LEDs.TA_Toff = 500;

    // Initialize also own GS buffer
    LEDs.brightness = LED_BRIGHTNESS_MAX;
    WORD msk = 0x0001;
    for(BYTE i=0; i < (sizeof(hwLEDval) / sizeof(hwLEDval[0])); i++, msk<<=1) {
        hwLEDval[i] = ((LEDs.status & msk) ? LEDs.brightness : 0);
        hwLEDval_actual[i] = 0;
    }

    BYTE dcval1; // = DC_BRIGHTNESS_MAX;
    BYTE dcval2; // = DC_BRIGHTNESS_MAX;
    // Read jumpers JP1 and JP2
    switch(hwJprVector() & 0x03) {
        case 0:
            dcval1 = DC_BRIGHTNESS_G;
            dcval2 = DC_BRIGHTNESS_B;
            break;
        case 1:
            dcval1 = DC_BRIGHTNESS_W;
            dcval2 = DC_BRIGHTNESS_R;
            break;
        case 2:
            dcval1 = DC_BRIGHTNESS_W;
            dcval2 = DC_BRIGHTNESS_B;
            break;
        case 3:
            dcval1 = DC_BRIGHTNESS_W;
            dcval2 = DC_BRIGHTNESS_G;
            break;
    }
    hwLED_DC_Setup(1, dcval1, 0);
    hwLED_DC_Setup(2, dcval2, 1);
}

//! Init HW: LCD display
//!
//! Normally called by PeripheralInit()
void  hwLCDInit(void) {
    //LCDinit();
}


//! Init HW: ADCs
//!
//! Normally called by PeripheralInit()
void  hwAnalogsInit(void) {
    ADCinit();
    for(BYTE i=0; i<N_SENSOR_AVG; i++) {
        sensorValAcc[i].accum = 0;
        sensorValAcc[i].count = 0;
        sensorValAcc[i].length_l2 = 4;  // Averaging length for "debounce" filter = 2^4 = 16 samples

        sensorAvgAcc[i].accum = 0;
        sensorAvgAcc[i].count = 0;
        sensorAvgAcc[i].length_l2 = 6;  // Averaging length for mean filter = 2^6 = 64 samples
    }
    hwPollHygro(0);
}

// *** TODO
//! Init HW: I2C (Humidity sensor)
//!
//! Normally called by PeripheralInit()
void  hwI2CInit(void) {

    P2SEL |= 0x03;
    P2OUT &= ~(BIT1|BIT0);
    P2DIR |= (BIT1|BIT0);             // Direction: output
    UCB0CTLW0 |= UCSWRST;                               // Enable SW reset
    UCB0CTLW0 |= UCMST | UCMODE_3 | UCSSEL_2 | UCSYNC;  // I2C Master, use SMCLK
    UCB0BRW = 80;                                      // 100 KHz
    UCB0I2CSA = 0x40;                             // Slave Address
    UCB0CTLW0 &= ~UCSWRST;                              // Clear SW reset, resume operation
    UCB0IE |= UCTXIE0;                                  // Enable TX interrupt

}

//! Initialize all HW peripherals for the MCU
//!
//! Calls sub-initialization functions
void  PeripheralInit(void) {
    hwDIOInit();
    hwLEDDriverInit();
    hwLCDInit();
    hwLCDPWMInit();
    hwAnalogsInit();
    hwI2CInit();
}

// ============================================================================
//  Keys / Digital I/O management
//
//! Get state of keys
//!
//! Raw vector, not debounced (application code must take care of debouncing)
//#pragma inline=forced
WORD hwKeysVector() {
	return (WORD)(~P4IN & 0x0F);
}

//! Get state of jumpers
//!
//! Raw vector, not debounced (application code must take care of debouncing if required)
//! Output byte:  0  AUX3  AUX2  AUX1   0  JP3  JP2  JP1
//#pragma inline=forced
BYTE hwJprVector() {
        WORD i;
        i = (~P3IN & 0x70);
        i |= ((~P4IN >> 4) & 0x07);
	return (BYTE)i;
        // OK but compiler complains about volatile access order:
        //return (BYTE)((P3IN & 0x70)|((P4IN >> 4) & 0x07));
}

//! Keyboard debounce
//!
void cycKeyDebounce(unsigned long ticksCounter) {
    static   WORD dbtimers[NKEYS];
    static   BYTE lastVec;
    static   unsigned long lastTicks;


    register BYTE chgVec;

    if(debounceTicks == 0) {debounceTicks = 1;}
    if (debounceTicks) {
        chgVec = lastVec ^ hwKeysVector();
        lastVec = hwKeysVector();

        for (BYTE i = 0, msk = 0x01; i < NKEYS; i++, msk <<= 1 ) {
            if (chgVec & msk) {
                dbtimers[i] = 0;
            } else {
                if (dbtimers[i] < debounceTicks) {
                    if ((dbtimers[i] += (ticksCounter-lastTicks)) >= debounceTicks) {
                        keyVector = (keyVector & (~msk)) | (lastVec & msk);
                    }
                }
            }
        }
        lastTicks = ticksCounter;
    } else {
        keyVector = hwKeysVector();
    }
}

// ============================================================================
//  Analog input management
//

//! Apply power to analog sensors
//!
void hwSensorPwr(BYTE status) {
    if(status) {
        P3OUT |= (BIT0);
    } else {
        P3OUT &= ~(BIT0);
    }
}

// Return true if an accumulator has enough data for a sensible value
#define avgValReady(acc)    ((acc)->count >= (1<<((acc)->length_l2)))

// Return the averaged value for the accumulator
#define avgAvgVal(acc) (((acc)->accum + (1<<((acc)->length_l2-1)))>>((acc)->length_l2))

//! Update sensor accumulated/average values with supplied new value
//!
void hwSensorUpdate(t_avgAcc *acc, WORD val) {

    acc->accum += val;
    if(avgValReady(acc)) {
        acc->accum -= acc->avgVal;
        acc->avgVal = avgAvgVal(acc);
    } else {
        ++(acc->count);
        acc->avgVal = acc->accum / acc->count;
    }
}

//  >>> for value conversion fns for sensors, see bottom of file

//! Change averaging length for a sensor channel, readjusting accumulator
//! content to fit new length.
//! Channel # is 1...N_SENSOR_AVG; 0 means all channels.
//! New sample accumulation length is 2^(acclen+1) (see also notes to struct t_avgAcc).
//!
void SensorChangeAvgLen(BYTE ch, BYTE newlen) {
  BYTE oldlen;

  for(int i = (ch ? ch-1 : 0); i < (ch ? ch : N_SENSOR_AVG); i++) {
      oldlen = sensorValAcc[i].length_l2;
      if(newlen == oldlen) return;
      if(newlen > oldlen) {
          sensorValAcc[i].accum <<= (newlen-oldlen);
      } else {
          sensorValAcc[i].accum += (1<<(newlen-1)); // perform rounding
          sensorValAcc[i].accum >>= (newlen-oldlen);
      }
      sensorValAcc[i].length_l2 = newlen;
  }
}

//! Return true if a sensor channel has enough accumulated data
//! for a sensible value
//! Channel # is 1...N_SENSOR_AVG; 0 returns true if all channels are up to date.
//!
BYTE SensorValReady(BYTE ch) {
  int i;
  if(ch==0) {
    for(i=0; i<N_SENSOR_AVG && (avgValReady(&(sensorValAcc[ch]))); i++);
    return (i==N_SENSOR_AVG);
  }
  return avgValReady(&sensorValAcc[ch]);
}

//! Trigger hygrometer (I2C) value read
//!


//! Read raw sensor (I2C) hygrometer value
//!
//extern unsigned long rh;

//#if BUILD_APP == EK_EQ2
    #define hwReadHygro() (SHT21_read())
//#else
//    #define hwReadHygro() 0UL
//#endif

#define hwADCtoDigIO(v) (v>0xBFFF ? 1 : ((v<0x4000 ? 0 : 0xFF)))

//! Keep values and averages from ADC channels up to date
//!
//! Important:
//! This function should be called at regular intervals.
//! At every call, a new ADC channel and its mean value are updated;
//! it takes 6 calls (ADC0, ADC1, ADC2, ADC3, ADC4, I2C) for all channels
//! to be updated.
//! Parameter "full" indicates that conditioned values (°C - Lux - ppm) must also
//! be updated.
void cycAnalogUpdate(BYTE full) {
    static BYTE nch;
    static BYTE chg;
    static BYTE firstCall;
    //BYTE prev = (nch == 0 ? 3 : nch-1);
    WORD CurrVal;
    WORD avgVal;
    if(nch <= 4) {
        hwSensorPwr(1);
        ADCOnOff(nch, 1);
        ADCstart(nch);
        CurrVal = 0;
        while(!ADCready(nch) && (++CurrVal < 0x7F));
        if(CurrVal == 0x7F) {
            // TIMEOUT OCCURRED!!!
            CurrVal = 0xFFFF;
        }
        // if(nch == 4) {
        //   CurrVal = ADCgetValue(nch);
        // } else
        {
           CurrVal = ADCgetValue(nch);
         }
        ADCOnOff(nch, 0);  // Optional

        // Per spegnere l'alimentazione sarebbe necessario togliere
        // i condensatori di filtro su TUTTI gli ingressi!
        //    hwSensorPwr(0);

        if(nch == 1) {
            hwPollHygro(0);
        }
    } else {
        // Hygrometer
        CurrVal = hwReadHygro();
    }

    // Update average values
    hwSensorUpdate(&sensorValAcc[nch], CurrVal);
    hwSensorUpdate(&sensorAvgAcc[nch], CurrVal);
    //avgVal = avgAvgVal(&sensorValAcc[nch]);
    avgVal = sensorValAcc[nch].avgVal;

    switch (nch) {
        case 0:
            CurrVal = SensorsRaw.Brightness;
            SensorsRaw.Brightness = avgVal;
            if(full && (SensorsRaw.Brightness != CurrVal)) {
                Sensors.Brightness = hwConvBrightness(SensorsRaw.Brightness);
            }
            break;
        case 1:
            CurrVal = SensorsRaw.NTC0r;
            SensorsRaw.NTC0r = avgVal;
            chg = (SensorsRaw.NTC0r != CurrVal);
            break;
        case 2:
            CurrVal = SensorsRaw.NTC0;
            SensorsRaw.NTC0 = avgVal;
            if(full && (chg || (SensorsRaw.NTC0 != CurrVal))) {
                Sensors.NTCInt = hwConvNTCIntTempX10(SensorsRaw.NTC0, SensorsRaw.NTC0r);
            }
            break;
        case 3:
            CurrVal = SensorsRaw.NTC1;
            SensorsRaw.NTC1 = avgVal;
            if(full && ( firstCall || (SensorsRaw.NTC1 != CurrVal))) {
                Sensors.DigIO1 = hwADCtoDigIO(SensorsRaw.NTC1);
                Sensors.NTCExt1 = hwConvNTCExtTempX10(SensorsRaw.NTC1);
            }
            break;
        case 4:
            CurrVal = SensorsRaw.NTC2;
            SensorsRaw.NTC2 = avgVal;
            if(full && ( firstCall || (SensorsRaw.NTC2 != CurrVal))) {
                Sensors.DigIO2 = hwADCtoDigIO(SensorsRaw.NTC2);
                Sensors.NTCExt2 = hwConvNTCExtTempX10(SensorsRaw.NTC2);
            }
            break;
        case 5:
            CurrVal = SensorsRaw.Hygro;
            SensorsRaw.Hygro = avgVal;
            if(full && ( firstCall || (SensorsRaw.Hygro != CurrVal))) {
                Sensors.Hygro = hwConvHygro(SensorsRaw.Hygro);
            }
            break;
    }
    if(++nch>5) {
        nch = 0;
        firstCall = 1;
    }
}

// ============================================================================
//  LED management

//! Set LED base brightness (to configure for different LED colors)
//!
//! Channel is 1 to 4 (currently mounted 1 and 2 only)
void  hwLED_DC_Setup(BYTE channel, BYTE value, BYTE send) {
    for(BYTE i = 0; i < 4; i++)
        { TLCsetDC(4 * i + (channel-1), value); }
    if(send)
        { TLCupdateDC(); }
}


//! Remap LED index to TLC driver channel
//! Mapping:
//! usr Ch: 0   1   2   3   4   5   6   7
//! TLC Ch: 0   1   4   5   8   9  12  13
#define  hwRemapTLCch(ch) ((ch & 0xFE)<<1 | (ch & 0x01))

//! Correct LED brightness value to achieve better progression
//!
static
WORD hwTrimLEDBrightness(BYTE b) {
    return ((b&0x80) ? (WORD)(b<<3) - 0x200  - 0x80 - 0x20:
             ((b&0x40) ? (WORD)(b<<2) - 0x80 - 0x20:
               ((b&0x20) ? (WORD)(b<<1) - 0x20 : (WORD)(b))
             )
           );
}

//! Update LED value until target is reached
//!
//! This function should be called at regular intervals - choose period according
//! to desired fade speed and value assigned to LEDs.fadeStep.
//! Example values: step=1/16 of 100% value, fade time 0 to 100% = 0.5s => call every 32 ms.
//!
//! Current and target values for LED intensity are compared; if any of them
//! is different, the corresponding current value is updated towards the target
//! and the updated actual value is sent.
//! Values are changed either in steps or one-shot (as was requested for each
//! single LED last time its value was changed; this info is stored in LEDs.fadeMask).
//!
//! The supplied ticks are used to handle blinking: they accumulate until a count
//! defined by LEDs.Ton+LEDs.Toff is reached, then restart from zero up.
//! When the count is in the lower phase (below Ton), the LED is active;
//! above Ton and up to Ton+Toff it is off.
//! Please notice that the ticks do not define an exact time quantity: their amount
//! between subsequent calls in relation with the time between these calls determines
//! the resulting blinking time.
//
void  cycLEDupdate(unsigned long ticksCounter) {
    static BYTE     updatePending = 0;
    static BYTE     TA_blinkPrev = 0;
    static WORD     blinkCount[16];
    static WORD     fadeCarry = 0;
    static unsigned long lastTicks;

    signed long     deltaT;
    register WORD   mask;
    register WORD   Ttot;
    register BYTE   fdstep;
    BYTE            doFade = 0;
    BYTE            chg = 0;
    WORD            bright;
    signed int      d;

    bright = LEDs.brightness;

    deltaT = (ticksCounter-lastTicks);

    if(TA_blinkPrev != (LEDs.TA_blinkStatus!=0)) {
        // TA activation status changed: reset all blink counters
        for(BYTE i=0; i < 16; i++) {
            blinkCount[i] = 0;
        }
    }
    TA_blinkPrev = (LEDs.TA_blinkStatus!=0);

    if(LEDs.TA_blinkStatus) {
        // Blink for technical alarm
        Ttot = LEDs.TA_Ton + LEDs.TA_Toff;
        blinkCount[0] += deltaT;
        while(blinkCount[0] >= Ttot) {
            blinkCount[0] -= Ttot;
        }
    } else {
        // Ordinary blink
        for(BYTE i=0; i < 16; i++) {
            if(!(LEDs.blinkStatus&(1<<i)))  {
                blinkCount[i] = 0;
            } else {
                Ttot = LEDs.Ton[i] + LEDs.Toff[i];
                blinkCount[i] += deltaT;
                while(blinkCount[i] >= Ttot) {
                    blinkCount[i] -= Ttot;
                }
            }
        }
    }

    fadeCarry += deltaT;
    if(fadeCarry > LED_FADESTEPTM) {
        doFade = 1;
        fadeCarry -= LED_FADESTEPTM;
        // if one or more periods skipped, restart from period begin
        if(fadeCarry > LED_FADESTEPTM) fadeCarry = 0;
    } else {
        doFade = 0;
    }

    // use fadeCarry/doFade also to prevent too frequent and unnecessary updates
    if(doFade) {

        mask = 0x01;
        for(BYTE ch=0; ch<16; ch++, mask <<= 1) {

            // ON/OFF and BLINK: update GS target values and according to flags
            if(LEDs.TA_blinkStatus) {
                hwLEDval[ch] = (((LEDs.TA_blinkStatus & mask) && blinkCount[0] <= LEDs.TA_Ton)
                ? LED_BRIGHTNESS_MAX : 0);
            } else {
                hwLEDval[ch] = ((LEDs.status & mask) ?                        // LED command On?
                                  ( !(LEDs.blinkStatus&mask)                  // No blink set, just plain "On"
                                   ||(LEDs.blinkStatus&mask
                                       && (blinkCount[ch] <= LEDs.Ton[ch]))   // Blink active and in the On phase
                                    ? bright : 0)
                                  : 0);
            }

            // FADING: update GS actual values to move towards target values
            if(hwLEDval[ch] != hwLEDval_actual[ch]) {
                if((LEDs.TA_blinkStatus == 0)
                   && (LEDs.blinkStatus&mask ? LEDs.blinkFadeMask&mask : LEDs.fadeMask&mask)) {
                    if(doFade) {
                        d = (hwLEDval[ch] - hwLEDval_actual[ch]);
                        fdstep = LED_FADESTEP; // (for optimization only)
                        hwLEDval_actual[ch] += ((d > 0) ?
                            (d <  fdstep ? d :  fdstep) :
                            (d > -fdstep ? d : -fdstep));
                    }
                } else {
                    hwLEDval_actual[ch] = hwLEDval[ch];
                }
                TLCset(hwRemapTLCch(ch), hwTrimLEDBrightness(hwLEDval_actual[ch]));
                chg = 1;
            }
        }
    }

    lastTicks = ticksCounter;

    if(chg || updatePending) {
        updatePending = TLCupdate();
    }
}

//! Set LED status according to supplied pattern.
//! Current implementation handles 1 TLC only!
//!
//! LED intensity is set to the stored value.
//! bit 0 is LED #1
//! pattern:    contains the target status of LEDS (0=off, 1=on)
//! fade:       != 0 for gradual on/off fading
void  LEDSetAll(WORD pattern, BYTE fade) {
    LEDs.status = pattern;
    LEDs.fadeMask = (fade ? 0xFFFF : 0x0000);
    // No data is sent right now to the TLC; the fading loop will
    // detect a difference between actual and target values and
    // send updates until they are level.
}

//! Set blink status of all LEDs according to supplied pattern.
//! Blinking is active only for LEDs which are On.
//!
//! LED intensity is set to the stored value.
//! bit 0 is LED #1
//! pattern:    contains the target status of LEDS (0=off, 1=on)
//! fade:       != 0 for gradual on/off fading
void  LEDSetBlinkAll(WORD pattern, BYTE fade) {
    LEDs.blinkStatus = pattern;
    LEDs.blinkFadeMask = (fade ? 0xFFFF : 0x0000);
    // No data is sent right now to the TLC; the fading loop will
    // detect a difference between actual and target values and
    // send updates until they are level.
}

//! Switch ON or OFF LEDs corresponding to bits set in supplied pattern
//! Current implementation handles 1 TLC only!
//!
//! LED intensity is set to the stored value
//! bit 0 is LED #1
//! pattern:    contains the target status of LEDS (0=unchanged, 1=on or off)
//! status:     target status for LEDS (0=off, 1=on)
//! fade:       != 0 for gradual on/off fading
void  LEDOnOff(WORD pattern, BYTE status, BYTE fade) {
    if(status) {
        LEDs.status |= pattern;
    } else {
        LEDs.status &= ~pattern;
    }
    if(fade) {
        LEDs.fadeMask |= pattern;
    } else {
        LEDs.fadeMask &= ~pattern;
    }
}

//! Switch single LED blink corresponding to bits set in supplied pattern
//!
//! bit 0 is LED #1
//! pattern:    contains the mask of affected LEDs (0=unchanged, 1=on or off)
//! status:     blink status to be set (0=off, 1=on)
//! fade:       != 0 for gradual on/off fading
void  LEDBlink(WORD pattern, BYTE status, BYTE fade) {
    if(status) {
        LEDs.blinkStatus |= pattern;
    } else {
        LEDs.blinkStatus &= ~pattern;
    }
    if(fade) {
        LEDs.blinkFadeMask |= pattern;
    } else {
        LEDs.blinkFadeMask &= ~pattern;
    }
}

//! Set LED blanking
void  LEDBlanking(BYTE status) {
    TLCsetBlanking(status);
}

// ============================================================================
//  LCD backlight management
//
#if BUILD_APP != EK_ET2

//Timer mode: 010 Toggle/Reset
//#define LCD_TMR_MODE (OUTMOD1)
//Timer mode: 111 Reset/Set
#define LCD_TMR_MODE (OUTMOD2|OUTMOD1|OUTMOD0)

//! Correct LCD backlight brightness value to achieve better progression
//!
static
WORD hwTrimLCDPWMBrightness(BYTE b) {
    return ((b&0x80) ? (WORD)(b<<3) - 0x200  - 0x80 - 0x20:
             ((b&0x40) ? (WORD)(b<<2) - 0x80 - 0x20:
               ((b&0x20) ? (WORD)(b<<1) - 0x20 : (WORD)(b))
             )
           );
}

//! Init HW: LCD backlight PWM
//!
//! Normally called by PeripheralInit()
void  hwLCDPWMInit(void) {

    // Uses TimerA_2
    // Timer2 - CCR0 usato internamente (overflow), CCR1/OUT1 come uscita.
    // CFG TA2: Out TA2.1, clksrc = ACLK, prescaler = 128, mode=UP
    //          CCR0: val=255
    //          CCR1: outmode=111 Reset/Set, val=0..255

    // Freq. PWM = 1kHz, resol. = 200 (<1 byte) -> base = 200 kHz
    // ACLK = 12 MHz; / 200k => prescaler = 60 (oppure il max. 64)

    // Freq. PWM = 1kHz, resol. = 1500 (11 bit) -> base = 1.5 MHz
    // ACLK = 12 MHz; / 1.5MHz => prescaler = 8

    // Activate pin P3.1 as timer output
    P3DIR |= (BIT1);
    P3SEL |= (BIT1);

    TA2CTL &= ~(TASSEL1);       // TA3CTL.TASSEL = 01 ACLK
    TA2CTL |= (TASSEL0);

    TA2CTL |= (ID1|ID0);        // TA3CTL.ID = 11 (Prescaler1 = /8)

    TA2EX0 &= ~(TAIDEX2|TAIDEX1|TAIDEX0); // TAIDEX = 000 (Prescaler2 = /1)

    TA2CTL &= ~(TAIE);              // TA3CTL.TAIE = 0 No IRQ

    TA2CTL |= (TACLR);              // TA3CTL.TACLR = 1 Init timer

    TA2CCR0 = 1500-1;               // CCR0 - Top value for counter

    TA2CCTL1 &= ~(OUTMOD2|OUTMOD1|OUTMOD0);
    TA2CCTL1 |= LCD_TMR_MODE;

    TA2CCR1 = hwTrimLCDPWMBrightness(LCDcurrPWMlevel = 128);

    TA2CTL |= (MC1|MC0);            // TA3CTL.MC = 11
    TA2CTL &= ~(MC1);               // TA3CTL.MC = 01
}

//! Update LCD backlight PWM value until target is reached
//!
void  cycLCDPWMupdate(unsigned long ticksCounter) {
    static unsigned long lastTicks;
    static WORD     PWMCarry = 0;
    signed long     deltaT;
    signed int      d;
    // FADING: update GS actual values to move towards target values
    if(LCDPWMlevel != LCDcurrPWMlevel) {
        if(LCDPWMfade) {
            deltaT = (ticksCounter-lastTicks);
            PWMCarry += deltaT;
            if(PWMCarry > LCDPWM_STEPTM) {
                // do fade step
                d = (LCDPWMlevel - LCDcurrPWMlevel);
                LCDcurrPWMlevel += ((d > 0) ?
                    (d < LCDPWM_STEP ? d :  LCDPWM_STEP) :
                    (d > - LCDPWM_STEP ? d : - LCDPWM_STEP));
                PWMCarry -= LCDPWM_STEPTM;
                // if one or more periods skipped, restart from period begin
                if(PWMCarry > LCDPWM_STEPTM) PWMCarry = 0;
            }
        } else {
            LCDcurrPWMlevel = LCDPWMlevel;
        }
        TA2CCR1 = hwTrimLCDPWMBrightness(LCDcurrPWMlevel);
        lastTicks = ticksCounter;
    }
}

//! Set LCD backlight PWM
//!
//#pragma inline=forced
void  LCDsetPWM(BYTE level, BYTE fade) {
    // PWM level: 0...LCDPWM_MAX
    // Does not switch on if it is off
    //TA2CCR1 = level;            // CCR1 - Start at full PWM
    LCDPWMlevel = (level>LCDPWM_MAX ? LCDPWM_MAX : level);
    LCDPWMfade = fade;
}


//! Return current LCD backlight PWM value
//!
BYTE    LCDgetPWM(BYTE actual){
    return (actual ? LCDcurrPWMlevel : LCDPWMlevel);
}

//! Switch off LCD backlight
//!
//#pragma inline=forced
void  LCDoffPWM(void) {
    TA2CTL &= ~(MC1|MC0);       // Stop timer
    TA2CCTL1 &= ~(OUTMOD2|OUTMOD1|OUTMOD0); // Set fixed output
    TA2CCTL1 &= ~(OUT);     // switch output off
}

//! Switch on LCD backlight
//!
//#pragma inline=forced
void  LCDonPWM(void) {
    // Does not change current PWM value
    TA2CTL |= (MC1|MC0);
    TA2CTL &= ~(MC1);       // TA3CTL.MC = 01
    // Also restore outmode
    TA2CCTL1 &= ~(OUTMOD2|OUTMOD1|OUTMOD0);
    TA2CCTL1 |= LCD_TMR_MODE;
    TA2CTL |= (TACLR);
}

#endif  //BUILD_APP != EK_ET2

// ===========================================
//   Value conversion fns for sensors
// ===========================================


// =========================
//  INTERNAL 100k-B4132 NTC
// =========================

// Temperature value conditioning data

// - FOR TABLE VALUES SEE EXTERNAL DOCS -

// Vhot[@-20])/Vref*1024 =  94

// Resolution (0..50°C) = 0,10593 °C/div
// t(cnt) = t1 + (cnt-cnt1)*(t2-t1)/(cnt2-cnt1)
//        = t1 + a*(cnt-cnt1)

// 10*t	= (10*t1) + (20480*a)*(cnt-cnt1)/2048


// Tabelle valori conversione
#define TC100_TABLEN    11
const int TC100_CNT[]={
    0, 59, 139, 236, 341, 393, 444, 536, 611, 670, 715,
};
const int TC100_10T[]={
    -200, -100, 0, 100, 200, 250, 300, 400, 500, 600,
};
const int TC100_20480A[]={
    3477, 2567, 2112, 1943, 1953, 2025, 2238, 2716, 3466, 4578,
};
#define VH100_BASE     94
// Nominal count value for cold joint input
// (Res. 100k+10k Hi / 100k Lo)
#define CNT100_COLD_NOM  488

int hwConvNTCIntTempX10(WORD rawVal, WORD rawValCold) {
    BYTE i=0;
    int cnt;
    int c;
    long q=0;

    cnt = rawVal;

    // Compute "cold joint" (ie Vref/Vsens) compensation
    // If Dc is the difference between actual and nominal cold joint value
    // (which is approx. at half of full value) then the full-scale value should
    // be corrected by -2*Dc; the correction for a value cnt is then
    // cnt[corr] = cnt - (2*Dc * (cnt/1024)) = cnt - ((2*Dc*cnt)/1024)
    if(rawValCold != 0) {
        c = 2 * (CNT100_COLD_NOM - rawValCold);
        cnt -= (c*(long)rawVal/1024);
    }

    if (cnt < VH100_BASE) {
        return(TC_OPEN); //Errore sonda guasta
    }

    // Remove offset (get value 0 at bottom of range)
    cnt -= VH100_BASE;

    for(i=0; i<(TC100_TABLEN-1) && cnt>=TC100_CNT[i+1]; i++);
    if (i >= (TC100_TABLEN-1)) {
      return(TC_SHORT);  //Errore sonda guasta
    }
    q = TC100_10T[i] + ((cnt-TC100_CNT[i])*(long)TC100_20480A[i] + 1024)/2048;

    return (int)q;
}

// =========================
//  EXTERNAL 10k-B3435 NTC
// =========================

// Temperature value conditioning data

// Vref (V)  = 3,3
// Val (V)   = 3,3
// Vcold (V) = 1,57143
// Vofs(V)   = Vhot[@-30]

// - FOR TABLE VALUES SEE EXTERNAL DOCS -

// Input DAC Values are 16-bit wide, unsigned
// (0x0000 = -Vr/0, 0xFFFF = 0/+Vr)

// Vhot[@-30])/Vref*1024 =  84
// Vcold)/Vref*1024      =  488
// Cnt offset =  ((Vhot[@-30]-Vcold) / Vref )* 1024 + 512 = 108

// Resolution (0..50°C)  = 0,093 °C/div
// Resolution (0..100°C) = 0,129 °C/div

// t(cnt) = t1 + (cnt-cnt1)*(t2-t1)/(cnt2-cnt1)
//        = t1 + a*(cnt-cnt1)

// 10*t	= (10*t1) + (20480*a)*(cnt-cnt1)/2048

// Tabelle valori conversione
#define TC10_TABLEN    28
const int TC10_CNT[]={
    14, 46, 84, 126, 174, 225, 280, 337,
    396, 455, 512, 568, 620, 669, 715, 757,
    794, 828, 858, 885, 909, 929, 948, 964,
    978, 991, 1001,
};
const int TC10_10T[]={
    -250, -200, -150, -100, -50,  0,  50, 100,
    150, 200, 250, 300, 350, 400, 450, 500,
    550, 600, 650, 700, 750, 800, 850, 900,
    950, 1000, 1050,
};
const int TC10_20480A[]={
    3158, 2713, 2418, 2151, 1994, 1858, 1797, 1747,
    1752, 1782, 1843, 1942, 2089, 2235, 2470, 2730,
    3010, 3403, 3835, 4234, 5028, 5506, 6517, 7132,
    7951, 9911,
};

int hwConvNTCExtTempX10(WORD rawVal) {
    BYTE i=0;
    int cnt;
    long q=0;

    cnt = rawVal;
     if (cnt < TC10_CNT[0]) {
        return(TC_OPEN); //Errore sonda guasta
    }

    for(i=0; i<(TC10_TABLEN-1) && cnt>=TC10_CNT[i+1]; i++);
    if (i >= (TC10_TABLEN-1)) {
      return(TC_SHORT);  // Errore sonda guasta
    }
    q = TC10_10T[i] + ((cnt-TC10_CNT[i])*(long)TC10_20480A[i] +1024) / 2048;

    return (int)q;
}

// =========================
//  Brightness (Lux) value
// =========================

// Sensor SFH3710: 500uA/lx ~> (@2k7) 741 lx/V ~> (@1024cnt/3V3) 2.444 lx/cnt
// 2.444 ~ 2 + 1/4 + 1/8 + 1/16 + 1/128
// 1024*2.444 ~ 2048 + 256 + 128 + 64 + 8
// lx = cnt * (2048 + 256 + 128 + 64 + 8) / 1024
//    = (cnt<<11 + cnt<<8 + cnt<<7 + cnt<<6 + cnt<<3)>>10

//! Convert raw ADC value (10-bit) to brightness (ADC 0)
//!
int hwConvBrightness(WORD rawb) {
    WORD lx;
    long ll = rawb;
    lx = (((ll<<11) + (ll<<8) + (ll<<7) + (ll<<6) + (ll<<3)) + 0x100)>>10;
    return lx;
}

// =========================
//  Humidity (1/1000) value
// =========================

//! Convert raw sensor (I2C) value to %. value (for I2C Hygrometer sensor)
//!
WORD hwConvHygro(WORD rawHygro) {
    //TODO
// Luca !!!!!!!
    rawHygro = (( rawHygro * 1250UL ) / 65536UL) - 60; // 1/10 %ur
// Luca fine !!!!!!

    return rawHygro;
}

// end peripheral.c
