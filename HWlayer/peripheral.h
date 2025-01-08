//! peripheral.h
//!
//! Funzioni interfaccia HW basso livello per EK-EQ2-TP
//! v0.1 2014-10-01 GCC

#ifndef PERIPHERAL_H
#define  PERIPHERAL_H

#include "msp430f6724.h"
//#include "Msp430_const.h"
#include "application.h"

//#include "ApplicationTimers.h"
#include "TLC594X_CFG.h"
#include "TLC594X.h"
#include "ADCmgr.h"
#include "SHT21.h"


// ============================================================================
//  Application and housekeeping functions/vars
//
//  Functions/vars prefixed by "hw" are meant for internal housekeeping business, 
//  therefore they should not be used unless you really need them.
//  Functions prefixed by "cyc" should be called on a scheduled basis to keep
//  the status of dynamic vars (eg. blink, debounce) up to date. See below for 
//  details.
// ============================================================================

// ============================================================================
//  Initialization
// ============================================================================

//! Initialize all HW peripherals for the MCU
//! 
//! Calls sub-initialization functions
void  PeripheralInit(void);

// ============================================================================
//  Keys / Digital I/O management
// ============================================================================

#define  NKEYS  4
//! Vector of debounced key flags
extern WORD keyVector;
//! Duration of debounce period (in ticks)
extern WORD debounceTicks;      

//! Debounce keys
//! 
//! This function should be called periodically.
//! The debounced key vector is available in <keyVector>.
//! <ticksAccum> is a counter that is always incremented (typically @1ms)
//! <debounceTicks> is the number of ticks during which that the output is 
//! required to be stable.
//! In order to work correctly, this function must be called at least 
//! one time for each tick period (typically
void    cycKeyDebounce(unsigned long ticksCounter);

//! Get state of keys
//! 
//! Raw vector, not debounced
WORD	hwKeysVector(void);

//! Get state of jumpers
//! 
//! Raw vector, not debounced
BYTE	hwJprVector(void);

// ============================================================================
//  Analog input management
// ============================================================================

// Analog values
// All values are filtered (time average) and conditioned (corrected for
// sensor curves); 
// data type is 32-bit signed integer (in 1/100th units).
// Also includes Hygrometer, though it is not actually from an ADC but from I2C.
typedef struct {
    int   NTCInt;           // Internal NTC, °C reading (x 10)
    int   NTCExt1;          // External NTC, channel 1, °C reading (x 10)
    int   NTCExt2;          // External NTC, channel 2, °C reading (x 10)
    int   Brightness;       // Brightness sensor, Lux
    int   Hygro;            // Hygrometric sensor, %
    BYTE  DigIO1;           // Ext1 read as digital value
    BYTE  DigIO2;           // Ext2 read as digital value
} t_AVal;

// RAW Analog values
// All values are filtered (time average) but NOT conditioned (ie directly 
// as read from the converters)
// Also includes Hygrometer, though it is not actually from an ADC but from I2C 
typedef struct {
    WORD  NTC0r;            // Internal NTC, "cold" side  
    WORD  NTC0;             // Internal NTC, "hot" side  
    WORD  NTC1;             // External NTC, channel 1
    WORD  NTC2;             // External NTC, channel 2
    WORD  Brightness;       // Brightness sensor
    WORD  Hygro;            // Hygrometric sensor
} t_AValRaw;

extern t_AVal       Sensors;
extern t_AValRaw    SensorsRaw;

// Value accumulator for averaging
typedef struct {
    long   accum;
    WORD   count;
    WORD   avgVal;
    //WORD   length;
    // Use log2(length)-1 for efficiency:
    // Nr of averaged samples = 2^(length_l2+1)
    // e.g. 1 => 4 samples, 
    //      2 => 8 samples, 
    //      3 => 16 samples, 
    //      4 => 32 samples, 
    //      7 => 256 samples etc    
    BYTE   length_l2;   
} t_avgAcc;

#define N_SENSOR_AVG    6

extern t_avgAcc     sensorValAcc[N_SENSOR_AVG];    // Short-term average
extern t_avgAcc     sensorAvgAcc[N_SENSOR_AVG];    // Long-term average

//! Apply power to analog sensors
//! 
void	hwSensorPwr(BYTE status);

#define TC_TSTEP      100
#define TC_TABLEN     14
#define TC_OPEN       -9999
#define TC_SHORT      -9998
#define TC_PZERO      -400

//! Keep values and averages from ADC channels up to date
//!
//! Important:
//! This function should be called at regular intervals.
//! At every call, a new ADC channel and its mean value are updated; 
//! it takes 6 calls (ADC0, ADC1, ADC2, ADC3, ADC4, I2C) for all channels 
//! to be updated.
//! Parameter "full" indicates that conditioned values (°C - Lux - ppm) must also
//! be updated. 
void cycAnalogUpdate(BYTE full);

//! Change averaging length for a sensor channel, readjusting accumulator
//! content to fit new length.
//! Channel #0 means all channels.
//! New sample accumulation length is 2^(acclen+1) (see also notes to struct t_avgAcc). 
//!
void SensorChangeAvgLen(BYTE ch, BYTE newlen);

//! Return true if a sensor channel has enough accumulated data
//! for a sensible value
BYTE SensorValReady(BYTE ch);

//! Read raw sensor (I2C) hygrometer value
//! 
WORD hwReadHygro();

//! Convert raw ADC value (10-bit) to brightness (ADC 0)
//! 
int hwConvBrightness(WORD rawBrightness);

//! Convert raw sensor (I2C) value to %. value (for I2C Hygrometer sensor)
//! 
WORD hwConvHygro(WORD rawHygro);

//! Convert raw ADC value to temperature (for ADC 1/2)
//! 
int hwConvNTCIntTempX10(WORD rawVal, WORD rawValCold);

//! Convert raw ADC value to temperature (for ADC 3/4)
//! 
int hwConvNTCExtTempX10(WORD rawVal);

// ============================================================================
//  LED management
//
// Important: these functions currently support a single TLC unit (16 LEDs).
// 
// LED assignment in EK-EP2-TP/EK-EQ2-TP:
// Bit      7   6   5   4   3   2   1   0
// LED     HH1 HH2 HL1 HL2 LH1 LH2 LL1 LL2
// +----------+----------+ 
// |          |       HH1| 
// |          |       HH2| 
// |          |          | 
// |          |       HL1| 
// |          |       HL2| 
// |          +----------+ 
// |          |       LH1| 
// |          |       LH2| 
// |          |          | 
// |          |       LL1| 
// |          |       LL2| 
// +----------+----------+ 
// ============================================================================


// Base (dot correction) brightness values for LEDs of different colors
// Values: 0...63 (0x3F)
#define DC_BRIGHTNESS_MAX   63
#define DC_BRIGHTNESS_R     63
#define DC_BRIGHTNESS_G     16
#define DC_BRIGHTNESS_B     63
#define DC_BRIGHTNESS_W     32

// Max greyscale brightness value for all LEDs ("full on" value)
// Values: 0...0x0FFF (12 bit)
#define GS_BRIGHTNESS_MAX   0x0FFF

// Max LED brightness value ("full on" value) for APPLICATION 
// Values: 0...0xFF (8 bit)
#define LED_BRIGHTNESS_MAX   0xFF

// led on/off and blink status
// In order to blink (ordinary blink), a LED must also be turned On.
// Technical alarm: as long as if any number of bits are set in TA_blinkStatus,
// the corresponding LEDs are made to blink at max brightness (with Ton/Toff = 
// TA_Ton/TA_Toff) and the ordinary On/Off-Blink controls are ignored. 
typedef struct {
    BYTE  brightness;      // Value for current LED brightness (common to all LEDs) - 0 to 0xFF (internally converted to 12 bits)
    WORD  Ton[16];         // Value of ON time for blinking (in ticks)
    WORD  Toff[16];        // Value of OFF time for blinking (in ticks)

    WORD  status;          // Mask of LED status (On/Off)
    WORD  blinkStatus;     // Mask of LED Blink status
    WORD  fadeMask;        // Mask of which LEDs must have a fading variation
    WORD  blinkFadeMask;   // Mask of which LEDs blink with fading
    WORD  TA_blinkStatus;  // Mask of LED Blink status for Tech Alarm - supersedes blinkStatus, no fade 
    WORD  TA_Ton;          // Value of ON time for TA blinking (in ticks)
    WORD  TA_Toff;         // Value of OFF time for TA blinking (in ticks)
} hwLEDset;

extern hwLEDset LEDs;

// Step for fading
// Defined as 1/16th of the current (max) brightness, 1 as minimum
#define LED_FADESTEP    (LEDs.brightness>15 ? LEDs.brightness >> 4 : 1)

// Step time: 1*LED_FADESTEP in <LED_FADESTEPTM> ticks (ms)
//! Example values: step=1/16 of 100% value, fade time 0 to 100% = 0.5s => step every 32 ms.
#define LED_FADESTEPTM  16  

//! Update LED value until target is reached (fading)
//!
//! Important:
//! This function should be called at regular intervals - choose period according
//! to desired fade speed and value assigned to LEDs.fadeStep.
//! Example values: step=1/16 of 100% value, fade time 0 to 100% = 0.5s => call every 32 ms.
//!
//! The supplied ticks are used to handle blinking: they accumulate until a count 
//! defined by <LEDs.Ton>+<LEDs.Toff> is reached, then restart from zero up.
//! When the count is in the lower phase (below Ton), the LED is active; 
//! above Ton and up to Ton+Toff it is off.
//! Please notice that the ticks do not define an exact time quantity: their amount 
//! between subsequent calls in relation with the time between these calls determines
//! the resulting blinking time.
void    cycLEDupdate(unsigned long ticksCounter);

//! Set status of all LEDs according to supplied pattern.
//! 
//! LED intensity is set to the stored value.
//! bit 0 is LED #1
//! pattern:    contains the target status of LEDS (0=off, 1=on)
//! fade:       != 0 for gradual on/off fading
void    LEDSetAll(WORD pattern, BYTE fade);

//! Set blink status of all LEDs according to supplied pattern.
//! 
//! LED intensity is set to the stored value.
//! bit 0 is LED #1
//! pattern:    contains the target status of LEDS (0=off, 1=on)
//! fade:       != 0 for gradual on/off fading
void    LEDSetBlinkAll(WORD pattern, BYTE fade);

//! Switch ON or OFF LEDs corresponding to bits set in supplied pattern
//! 
//! LED intensity is set to the stored value
//! bit 0 is LED #1
//! pattern:    contains the mask of affected LEDs (0=unchanged, 1=on or off)
//! status:     target status to be set (0=off, 1=on)
//! fade:       != 0 for gradual on/off fading
void    LEDOnOff(WORD pattern, BYTE status, BYTE fade);
#define LEDOn(p, f) LEDOnOff(p, 1, f)
#define LEDOff(p, f) LEDOnOff(p, 0, f)

//! Switch single LED blink corresponding to bits set in supplied pattern
//! 
//! bit 0 is LED #1
//! pattern:    contains the mask of affected LEDs (0=unchanged, 1=on or off)
//! status:     blink status to be set (0=off, 1=on)
//! fade:       != 0 for gradual on/off fading
void    LEDBlink(WORD pattern, BYTE status, BYTE fade);
#define LEDBlinkOn(p, f) LEDBlink(p, 1, f)
#define LEDBlinkOff(p, f) LEDBlink(p, 0, f)


//! Set LED blanking 
void    LEDBlanking(BYTE status);

//! Set LED base brightness (to configure for different LED colors) 
//! 
//! Channel is 1 to 4 (currently mounted: 1 and 2 only) 
void    hwLED_DC_Setup(BYTE channel, BYTE value, BYTE send);


#if BUILD_APP != EK_ET2
// ============================================================================
//  LCD backlight management
// ============================================================================

#define LCDPWM_MAX          255
#define LCDPWM_STEPTM       10
#define LCDPWM_STEP         LCDPWM_MAX/64


//! Update LCD PWM value until target is reached (fading)
//!
//! Important:
//! This function should be called at regular intervals - choose period according
//! to desired fade speed and value assigned to PWMfadeStep.
//! Example values: step=1/16 of 100% value, fade time 0 to 100% = 0.5s => call every 32 ms.
//!
void    cycLCDPWMupdate(unsigned long ticksCounter);

//! Set LCD backlight PWM
//! 
void    LCDsetPWM(BYTE level, BYTE fade);

//! Return current LCD backlight PWM value
//! 
BYTE    LCDgetPWM(BYTE actual);

//! Switch off LCD backlight
//! 
void    LCDoffPWM(void);

//! Switch on LCD backlight
//! 
void    LCDonPWM(void);

#endif // BUILD_APP != EK_ET2

// Private housekeeping functions
// Not declared static, should not be publicized either
void	hwDIOInit(void);
void	hwLEDDriverInit(void);
void	hwLCDInit(void);
void	hwLCDPWMInit(void);
void	hwAnalogsInit(void);
void	hwI2CInit(void);

#endif  // peripheral.h
