

/*
 * ----------------------------------------------------------------------------
 * "THE BEER-WARE LICENSE" (Revision 42):
 * <struppi@struppi.name> wrote this file. As long as you retain this notice you
 * can do whatever you want with this stuff. If we meet some day, and you think
 * this stuff is worth it, you can buy me a beer in return.
 * (c) 2014 Stefan Rupp
 * ----------------------------------------------------------------------------
 */

#include <string.h>
#include "msp430f6724.h"
#include "led1642gw.h"
#include "led1642gw_config.h"

#ifdef SINGLE_LED_BRIGHTNESS
#warning ====================================================
#warning >>> led1642GW: SINGLE_LED_BRIGHTNESS option active
#warning ====================================================
#endif

/* The buffer to hold the LED values.
 * The data in this buffer can be manipulated with
 * e.g. led1642gw_set().
 * calling led1642gw_flush() sends the data in this buffer
 * the data registers of the LED1642 ICs.
 */
//static
static WORD config_reg[NUM_LED1642GW_ICs];
static WORD ledswitch[NUM_LED1642GW_ICs];
static WORD ledbuffer[NUM_LED_CHANNELS];
static int fade_step = FADE_STEP;
#pragma required = ledblinking
WORD ledblinking[NUM_LED1642GW_ICs];

#ifdef SINGLE_LED_BRIGHTNESS
WORD ledMaxBrightness[NUM_LED_CHANNELS*NUM_LED1642GW_ICs] =
{
0x0FF,0x0FF,0x0FF,0x0FF,0x0FF,0x0FF,0x0FF,0x0FF,
0x0FF,0x0FF,0x0FF,0x0FF,0x0FF,0x0FF,0x0FF,0x0FF,
};
#else
WORD ledMaxBrightness[((NUM_LED_CHANNELS-1)/LEDS_X_GROUP)+1] =
{0x0FF,0x0FF,};
#endif




struct IC_PORT_MAP const icMap[3] = {
{ // IC0
  (unsigned char*)&P2DIR, (unsigned char*)&P2OUT, 5,(1<<5), // CLK
  (unsigned char*)&P4DIR, (unsigned char*)&P4OUT, 0,(1<<0), // SDI
  (unsigned char*)&P4DIR, (unsigned char*)&P4OUT, 3,(1<<3), // LE
},
{ // IC1
  (unsigned char*)&P2DIR, (unsigned char*)&P2OUT, 5,(1<<5), // CLK
  (unsigned char*)&P4DIR, (unsigned char*)&P4OUT, 1,(1<<1), // SDI
  (unsigned char*)&P4DIR, (unsigned char*)&P4OUT, 4,(1<<4), // LE
},
{ // IC2
  (unsigned char*)&P2DIR, (unsigned char*)&P2OUT, 5,(1<<5), // CLK
  (unsigned char*)&P4DIR, (unsigned char*)&P4OUT, 2,(1<<2), // SDI
  (unsigned char*)&P4DIR,( unsigned char*)&P4OUT, 5,(1<<5), // LE
}};


/*
 * Write 16 bits of \data, with LE set high
 * for the number of clock cycles specified in \le_clocks.
 * MSB comes first, LSB is last.
 */
static void write_data(BYTE n_ic, WORD data, BYTE le_clocks)
{
  WORD mask = 0x8000;
  signed char bit;

  __disable_interrupt();
  SET_LE_L(n_ic);
  for (bit=15; bit>=le_clocks; bit--) {
	SET_CLK_L(n_ic);
    if(data&mask) {
        SET_SDI_H(n_ic);
    }
    else {
        SET_SDI_L(n_ic);
    }
    SET_CLK_H(n_ic);
    mask >>= 1;
  }

  SET_LE_H(n_ic);

  for (/*noting to initialize*/; bit>=0; bit--) {
	SET_CLK_L(n_ic);
    if(data&mask) {
        SET_SDI_H(n_ic);
    }
    else {
        SET_SDI_L(n_ic);
    }
    SET_CLK_H(n_ic);
    mask >>= 1;
 }

  // set all pins to low after transmission
  SET_CLK_L(n_ic);
  SET_LE_L(n_ic);
  SET_SDI_L(n_ic);
   __enable_interrupt();
}


/*
 * Write data to BRIGHTNESS DATA LATCH register.
 * that means setting LE high for 3 or 4 clock cycles
 */
static void write_data_latch(BYTE n_ic,WORD data)
{
  write_data(n_ic,data, 4);
}


/*
 * Write data to BRIGHTNESS GLOBAL LATCH register.
 * that means setting LE high for 5 or 6 clock cycles
 */
static void write_global_latch(BYTE n_ic,WORD data)
{
  write_data(n_ic,data, 6);
}


#ifdef DAISY_CHAIN
/*
 * This function shifts data through the 16bit shift
 * register of the LED1642GW, without writing the data
 * to any internal register of the IC.
 * This way, we can daisy chain an bunch of LED1642GW ICs,
 * and still get data through to any of those.
 */
static void write_no_command(BYTE n_ic,WORD data)
{
  write_data(n_ic,data, 0);
}
#endif

/*
 * Write data to CONFIG register.
 * that means setting LE high for 7 clock cycles
 */
void led1642gw_flush_config()
{
  BYTE ic;


#ifdef DAISY_CHAIN
  for (ic=0; ic<(NUM_LED1642GW_ICs-1); ic++) {
    write_no_command(ic,config_reg[ic]);
  }
  write_data(ic,config_reg[ic], 7);
#else
  for (ic=0; ic<(NUM_LED1642GW_ICs); ic++) {
      write_data(ic,config_reg[ic], 7);
  }

#endif
}


/*
 * Turn all channels on, so the data in the DATA LATCH
 * register affects the LEDs attached to the IC.
 */
void led1642gw_turn_all_on(void)
{
  BYTE ic;

#ifdef DAISY_CHAIN
  for (ic=0; ic<(NUM_LED1642GW_ICs-1); ic++) {
    write_no_command(ic,0xffff);
  }
  write_data(ic,0xffff, 2);
#else
  for (ic=0; ic<(NUM_LED1642GW_ICs); ic++) {
    write_data(ic,0xffff, 2);
  }
#endif
}


/*
 * Turn all channels off,
 */
void led1642gw_turn_all_off(void)
{
  BYTE ic;

#ifdef DAISY_CHAIN
  for (ic=0; ic<(NUM_LED1642GW_ICs-1); ic++) {
    write_no_command(ic,0x0000);
  }
  write_data(ic,0x0000, 2);
#else
  for (ic=0; ic<(NUM_LED1642GW_ICs); ic++) {
    write_data(ic,0x0000, 2);
  }
#endif
}

/*****************************************************
*****************************************************/
void led1642gw_set_gain(BYTE gain)
{

  if (gain > 0x3f) {
    gain = 0x3f;
  }

  //for (BYTE ic=0; ic<(NUM_LED1642GW_ICs-1); ic++) {
  for (BYTE ic=0; ic<(NUM_LED1642GW_ICs); ic++) {
    config_reg[ic] &= ~(0x003f);
    config_reg[ic] |=  gain;
  }

}
/*****************************************************
*****************************************************/
BYTE led1642gw_ledswitch_update(WORD * buf, BYTE flush,BYTE fade)
{

  if ( memcmp(ledswitch,buf,sizeof(ledswitch)) != 0 )
  {
      if ( fade == FADE_ENABLED )
      {
          for ( BYTE j = 0; j < NUM_LED_CHANNELS; j ++ )
          {
              if ( (ledswitch[j/16]^buf[j/16]) & 0x0001 << (j%16) )
              {
                  //ledbuffer[j] = 0;
              }
          }
      }
      memcpy(ledswitch,buf,sizeof(ledswitch));
      if ( flush ) led1642gw_flush_ledswitch();
      return 1;
  }
  else
  {
      return 0;
  }
}
/*****************************************************
*****************************************************/

BYTE led1642gw_ledbrightness_update(WORD * buf, BYTE flush,BYTE fade,WORD *blinking)
{
  int direction;
  WORD brightness;

  if ( memcmp(ledbuffer,buf,sizeof(ledbuffer)) != 0 )
  {
      if ( fade == FADE_ENABLED )
      {
          for ( BYTE j = 0; j < NUM_LED_CHANNELS; j ++ )
          {
              if ( ledbuffer[j] != buf[j] )
              {
                  direction = ledbuffer[j];
#ifdef SINGLE_LED_BRIGHTNESS
                  brightness = ledMaxBrightness[j];
#else
                  brightness = ledMaxBrightness[j/LEDS_X_GROUP];
#endif
                  //direction +=  ledbuffer[j] > buf[j] ?  -fade_step : fade_step;
                  BYTE app;
                  app = (ledbuffer[j] > buf[j]);
                  direction +=  ledbuffer[j] > buf[j] ?  -(brightness/fade_step) : (brightness/fade_step);
                  if (  (direction > 0)&&(direction < brightness )&&
                        !(blinking[j/16] & (0x0001 << j%16)) )
                  {
                      ledbuffer[j] = direction;
                      if ( app )
                      {
                        if ( ledbuffer[j] < buf[j] )
                          ledbuffer[j] = buf[j];
                      }
                      else
                      {
                        if ( ledbuffer[j] > buf[j] )
                          ledbuffer[j] = buf[j];
                      }

                  }
                  else
                  {
                      ledbuffer[j] = buf[j] ;
                  }
              }
          }
      }
      else
      {
          memcpy(ledbuffer,buf,sizeof(ledbuffer));
      }
      if ( flush == FLUSH_ENABLED) led1642gw_flush();

      return 1;
  }
  else
  {
      return 0;
  }
}
/*****************************************************
*****************************************************/
void led1642gw_led_switch(BYTE channel, BYTE mode)
{
  BYTE ic;

  if (channel < NUM_LED_CHANNELS) {
    ic = (16*(channel/16));
    channel = (channel%16);
    if ( mode )
        ledswitch[ic] |= 1<<channel;
    else
        ledswitch[ic] |= ~(1<<channel);
  }
}
/*****************************************************
*****************************************************/
void led1642gw_set_PWM_counter(BYTE mode)
{
  for (BYTE ic=0; ic<=(NUM_LED1642GW_ICs-1); ic++) {
    if (mode) { config_reg[ic] |= (1U<<15); }
    else { config_reg[ic] &= ~(1U<<15); }
  }
}
/*****************************************************
*****************************************************/
void led1642gw_set_current_mode(BYTE mode)
{
  //for (BYTE ic=0; ic<(NUM_LED1642GW_ICs-1); ic++) {
  for (BYTE ic=0; ic<=(NUM_LED1642GW_ICs); ic++) {
    if (mode) { config_reg[ic] |= (1<<6); }
    else { config_reg[ic] &= ~(1<<6); }
  }
}

/*
 * Initialize the pins of the ATMega processor
 * to drive the data signals to the ICs
 * and initialize the LED buffer to zero.
 */
void led1642gw_init(void)
{

  for ( BYTE ic = 0; ic < 3; ic++ )
  {
      SET_CLK_L(ic);
      SET_SDI_L(ic);
      SET_LE_L(ic);
      DDR_CLK(ic) |= (1<<PIN_CLK(ic));
      DDR_SDI(ic) |= (1<<PIN_SDI(ic));
      DDR_LE(ic)  |= (1<<PIN_LE(ic));
  }
  //---- leds pwm ----
  __disable_interrupt();
  PMAPKEYID = 0x2D52;
  PMAPCTL |= PMAPRECFG;
  P3MAP3 = PM_TA3_1;
  PMAPKEYID = 0x0F0F0;
  __enable_interrupt();
  P3DIR |= BIT3;
  P3SEL |= BIT3;
  TA3CCR0 = 16;                         // PWM Period/2
  TA3CCTL1 = OUTMOD_6;                  // CCR1 toggle/set
  TA3CCR1 = 8;                          // CCR1 PWM duty cycle
  TA3CTL = TASSEL_1 | MC_3 | TACLR;     // SMCLK, up-down mode, clear TAR

  memset(ledbuffer, 0x00, sizeof(ledbuffer));
  memset(config_reg, 0x00, sizeof(config_reg));
  memset(ledswitch, 0x00, sizeof(ledswitch));

  led1642gw_set_PWM_counter(1);
  led1642gw_set_gain(0x0C); //0x10);
  led1642gw_flush_config();
  led1642gw_turn_all_off();
  led1642gw_flush();
}
/*****************************************************
*****************************************************/
void led1642gw_flush_ledswitch(void)
{
  //BYTE channel;
  BYTE ic;

#ifdef DAISY_CHAIN

#else
  for (ic=0; ic<(NUM_LED1642GW_ICs); ic++) {
      write_data(ic,ledswitch[ic], 2);
  }
#endif

}

/*
 * Transmit data from the ledbuffer to the BRIGHTNESS latches of
 * the LED driver ICs.
 * Let's assume, we have n LED1642GW ICs daisy chained. Then
 * we write n-1 times with write_no_command, to shift all
 * data through the 16bit shift registers of each of the ICs.
 * Then we once write with write_data_latch to store the data
 * in the BRIGHTNESS DATA registers of the respective ICs.
 * We do this for all but the last set of brightness data,
 * where we don't write to the DATA LATCH, but to the GLOBAL DATA LATCH.
 */
void led1642gw_flush(void)
{
  BYTE channel;
  BYTE ic;

#ifdef DAISY_CHAIN

  // for each of the first 15 channels, do the following:
  for (channel=0; channel<NUM_LED1642GW_CHANNELS-1; channel++) {
	  // shift data throught the first n-1 ICs with write_no_command
	  for (ic=0; ic<(NUM_LED1642GW_ICs-1); ic++) {
		  write_no_command(ic,ledbuffer[channel+(NUM_LED1642GW_CHANNELS*ic)]);
	  }
	  // then, when the brightness data has propagated through the
	  // shift registers, write all data into the DATA LATCH of
	  // all of the ICs.
	  write_data_latch(ic,ledbuffer[channel+(ic*NUM_LED1642GW_CHANNELS)]);
  }
  // for the 16th channel, we don't write to the DATA LATCH, but
  // to the CLOBAL data latch.
  // once more, we do the trick with write_no_command, to
  // shift data through all the ICs
  for (ic=1; ic<NUM_LED1642GW_ICs; ic++) {
	  write_no_command(ic,ledbuffer[(ic*NUM_LED1642GW_CHANNELS)-1]);
  }
  // than, at last, write data to the global latch, to force
  // the ICs to update their brightness data from the DATA LATCHES.
  write_global_latch(ic,ledbuffer[(ic*NUM_LED1642GW_CHANNELS)-1]);

#else

#ifdef CORR_ERROR_LEDS_CS
  BYTE ch;
  for (ic=0; ic<NUM_LED1642GW_ICs; ic++) {
    for (channel=NUM_LED1642GW_CHANNELS-1; channel > 0; channel--) {
        ch = channel;
        if ((ic==0)&&( channel == 2 )) ch = 10;
        if ((ic==0)&&( channel == 10 )) ch = 2;
        write_data_latch(ic,ledbuffer[ch+(NUM_LED1642GW_CHANNELS*ic)]);
    }
    write_global_latch(ic,ledbuffer[(NUM_LED1642GW_CHANNELS*ic)]);
  }

#else
  for (ic=0; ic<NUM_LED1642GW_ICs; ic++) {
    for (channel=NUM_LED1642GW_CHANNELS-1; channel > 0; channel--) {
        write_data_latch(ic,ledbuffer[channel+(NUM_LED1642GW_CHANNELS*ic)]);
    }
    write_global_latch(ic,ledbuffer[channel+(NUM_LED1642GW_CHANNELS*ic)]);
  }
#endif

#endif

}

/*****************************************************
*****************************************************/
void led1642gw_set_channel(BYTE channel, WORD value)
{
/*
  BYTE appo; // sistema l'ordine dei canali !
  if (channel < NUM_LED_CHANNELS) {
    appo = (16*(channel/16));
    channel= appo + (15-(channel%16));
    ledbuffer[channel] = value;
  }
*/
  ledbuffer[channel] = value;

}

/*****************************************************
*****************************************************/
void led1642gw_clear(void)
{
  memset(ledbuffer, 0x00, sizeof(ledbuffer));
}

/*****************************************************
*****************************************************/
void led1642gw_set_blinking(BYTE led,BYTE flag)
{
  if ( flag )
      ledblinking[led/16] |=  (0x0001 << (led % 16));
  else
      ledblinking[led/16] &= ~(0x0001 << (led % 16));
}



