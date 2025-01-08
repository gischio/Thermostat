
#ifndef LED1642GW_CONFIG_H_
#define LED1642GW_CONFIG_H_

#ifdef CS_RA
#define NUM_LED1642GW_ICs (3)
#define LEDS_X_GROUP      (16)
#else
#define NUM_LED1642GW_ICs (1)
#define LEDS_X_GROUP      (8)
#endif

#define NUM_LED1642GW_CHANNELS (16) // number of LED channels per IC
//total numer of channels. needed to calculate the buffer size.
#define NUM_LED_CHANNELS (NUM_LED1642GW_CHANNELS*NUM_LED1642GW_ICs)




struct IC_PORT_MAP{
    unsigned char  *ddr_clk;
    unsigned char  *port_clk;
    unsigned char   pin_clk;
    unsigned char   pin_mask_clk;
    unsigned char  *ddr_sdi;
    unsigned char  *port_sdi;
    unsigned char   pin_sdi;
    unsigned char   pin_mask_sdi;    
    unsigned char  *ddr_le;
    unsigned char  *port_le;
    unsigned char   pin_le;
    unsigned char   pin_mask_le;    
} ;

#define DDR_CLK(x)      (*icMap[x].ddr_clk)
#define PORT_CLK(x)     (*icMap[x].port_clk)
#define PIN_CLK(x)      (icMap[x].pin_clk)
#define PIN_MASK_CLK(x) (icMap[x].pin_mask_clk)
#define SET_CLK_H(x)    ((PORT_CLK(x)) |=  PIN_MASK_CLK(x))
#define SET_CLK_L(x)    ((PORT_CLK(x)) &= ~PIN_MASK_CLK(x))

#define DDR_SDI(x)      (*icMap[x].ddr_sdi)
#define PORT_SDI(x)     (*icMap[x].port_sdi)
#define PIN_SDI(x)      (icMap[x].pin_sdi)
#define PIN_MASK_SDI(x) (icMap[x].pin_mask_sdi)
#define SET_SDI_H(x)    ((PORT_SDI(x)) |=  PIN_MASK_SDI(x))
#define SET_SDI_L(x)    ((PORT_SDI(x)) &= ~PIN_MASK_SDI(x))

#define DDR_LE(x)       (*icMap[x].ddr_le)
#define PORT_LE(x)      (*icMap[x].port_le)
#define PIN_LE(x)       (icMap[x].pin_le)
#define PIN_MASK_LE(x)  (icMap[x].pin_mask_le)
#define SET_LE_H(x)     ((PORT_LE(x)) |=  PIN_MASK_LE(x))
#define SET_LE_L(x)     ((PORT_LE(x)) &= ~PIN_MASK_LE(x))


#endif // LED1642GW_CONFIG_H_

