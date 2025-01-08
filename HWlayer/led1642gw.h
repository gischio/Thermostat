

/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <struppi@struppi.name> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.
 * (c) 2014 Stefan Rupp
 * ----------------------------------------------------------------------------
 */


#ifndef LED1642GW_H_
#define LED1642GW_H_

typedef unsigned long DWORD;
typedef unsigned short WORD;
typedef unsigned char BYTE;
typedef unsigned char BITS;
typedef unsigned char BOOL;

#define FADE_DISABLED     0
#define FADE_ENABLED      1
#define FADE_STEP         25

#define FLUSH_DISABLED    0
#define FLUSH_ENABLED     1

#include "led1642gw_config.h"

#define SINGLE_LED_BRIGHTNESS

//#ifdef SINGLE_LED_BRIGHTNESS
//#warning =======================================
//#warning >>> led1642GW: SINGLE_LED_BRIGHTNESS
//#warning =======================================
//#endif

extern WORD ledswitch[NUM_LED1642GW_ICs];
extern WORD ledblinking[NUM_LED1642GW_ICs];
#ifdef SINGLE_LED_BRIGHTNESS
extern WORD ledMaxBrightness[NUM_LED_CHANNELS*NUM_LED1642GW_ICs];
#else
extern WORD ledMaxBrightness[((NUM_LED_CHANNELS-1)/LEDS_X_GROUP)+1];
#endif

void led1642gw_init(void);
void led1642gw_turn_all_on(void);
void led1642gw_turn_all_off(void);
void led1642gw_flush(void);
void led1642gw_set_channel(BYTE channel, WORD value);
void led1642gw_clear(void);
void led1642gw_flush_config(void);
void led1642gw_set_gain(BYTE gain);
void led1642gw_set_current_mode(BYTE mode);

void led1642gw_set_PWM_counter(BYTE mode);
void led1642gw_flush_ledswitch(void);
void led1642gw_led_switch(BYTE channel, BYTE mode);
BYTE led1642gw_ledbrightness_update(WORD * buf, BYTE flush,BYTE fade,WORD *blinking);
BYTE led1642gw_ledswitch_update(WORD * buf, BYTE flush,BYTE fade);

#endif // LED1642GW_H_
