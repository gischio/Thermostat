//! ADCmgr.c
//!
//! Progetto EK-EQ2_TP
//! Funzioni interfaccia HW MSP430(F6724) per ADC SAR + SD
//! v0.1 2014-10-01 GCC


#include "ADCmgr.h"

#ifdef SD24_16BIT
#warning ========================================================
#warning >>> ADCmgr: SD24_16BIT option active
#warning ========================================================
#endif

void ADCinit(void) {

    // ADC10 configuration

    P1SEL |= 0x07;                          // P1.2 to P1.0 assigned to analog

    // PORT REMAPPING SECTION:
    WORD irqen;
    // DISABLE IRQ
    irqen = SFRIE1;
    SFRIE1 = 0;
    // Key for write access
    PMAPKEYID
    = 0x02D52;
    PMAPCTL |= PMAPRECFG;   // allow further accesses
    // Remap pins:
    P1MAP0 = PM_ANALOG;
    P1MAP1 = PM_ANALOG;
    P1MAP2 = PM_ANALOG;
    // Write invalid key to close access
    PMAPKEYID = 0x0F0F0;
    // RE-ENABLE IRQ
    SFRIE1 = irqen;

    ADC10CTL0 &= ~(ADC10ON|ADC10ENC);       // Start disabled

    ADC10CTL1 &= ~(ADC10SHS1|ADC10SHS0);    // (*default*) Trigger source: ADC10SC bit (ADC10SHS = 00)

    //ADC10CTL1 &= ~(ADC10SSEL1|ADC10SSEL0);  // Clock source: MODCLK (ADC10SSEL = 00)

    ADC10CTL1 |=  (ADC10SSEL0);             // Clock source: ACLK
    ADC10CTL1 &= ~(ADC10SSEL1);             // (ADC10SSEL = 01)
    ADC10CTL1 |=  (ADC10DIV1|ADC10DIV0);    // Divide by 4

    ADC10CTL1 |=  (ADC10SHP);               // S/H pulse mode
    // S/H pulse duration:
    ADC10CTL0 &=  ~(ADC10SHT3|ADC10SHT2|ADC10SHT1|ADC10SHT0); // (*default*) ADC10SHT = 0 -> 4 Clk cycles
    // Increase duration if required:
    //ADC10CTL0 |= (?ADC10SHT3|?ADC10SHT2|?ADC10SHT1|?ADC10SHT0);
    //ADC10CTL0 |= (ADC10SHT1|ADC10SHT0);   // 32 clk cycles
    ADC10CTL0 |= (ADC10SHT0);               // 8 clk cycles

    ADC10CTL2 |= ADC10RES;                  // (*default*) format: 10 bit
    ADC10CTL2 &= ~(ADC10DF);                // (*default*) format: unsigned
    ADC10CTL2 |= ADC10SR;                   // Reduce max S/R to 50ksam/s to limit consumption

    ADC10MCTL0 &= ~(ADC10SREF2|ADC10SREF1|ADC10SREF0);  // (*default*) Vref+/- = AVcc and AVss

    // Use internal 2.5V ref:
    // ADC10MCTL0 |= (ADC10SREF0);
    // REFCTL0 |= (REFON|REFVSEL1);

    // Set threshold values for use as "digital" input
    ADC10HI = 0x02FF;   // Hi above (about) 75%
    ADC10LO = 0x00ff;   // Lo below (about) 25%


    // S/D configuration

    SD24BCTL0 |= SD24REFS;                  // Use internal reference
    SD24BCTL0 &= ~(SD24SSEL1|SD24SSEL0);    // Clock source:
    SD24BCTL0 |= SD24SSEL1;                 //   10 = ACLK

    SD24BINCTL0 &= ~(SD24GAIN2|SD24GAIN1|SD24GAIN0);    // (*default*) Gain = 1
    //SD24BOSR0 = 0x003F;                               // OSR = 64 (no need for performance)
    SD24BCCTL0 &= ~(SD24DF1|SD24DF0);                   // (*default*) Output format = Offset binary (0...FS)
    SD24BCCTL0 |=  (SD24ALGN);                          // Output format = left aligned
    SD24BCCTL0 |=  (SD24SNGL);                          // Single (one-shot) conversion mode
    SD24BCCTL0 &= ~(SD24SCS2|SD24SCS1|SD24SCS0);        // SD24SCS = 0b000 - Manual conversion start (bit SD24BCCTL0.SD24SC)
    //SD24BCCTL0 |=  (SD24SCS2);                          // SD24SCS = 0b100 - Conversion start with group 0 (SD24GRP0SC)
    SD24BCTL0 |= SD24PDIV_5;

    SD24BINCTL1 &= ~(SD24GAIN2|SD24GAIN1|SD24GAIN0);    // (*default*) Gain = 1
    //SD24BOSR1 = 0x003F;                               // OSR = 64 (no need for performance)
    SD24BCCTL1 &= ~(SD24DF1|SD24DF0);                   // (*default*) Output format = Offset binary (0...FS)
    SD24BCCTL1 |=  (SD24ALGN);                          // Output format = left aligned
    SD24BCCTL1 |=  (SD24SNGL);                          // Single (one-shot) conversion mode
    SD24BCCTL1 &= ~(SD24SCS2|SD24SCS1|SD24SCS0);        // SD24SCS = 0b000 - Manual conversion start (bit SD24BCCTL1.SD24SC)
    //SD24BCCTL1 |=  (SD24SCS2);                         // SD24SCS = 0b100 - Conversion start with group 0 (SD24GRP0SC)


}

void ADCOnOff(BYTE nADC, BYTE status) {
    if((nADC == 2)||(nADC == 3)||(nADC == 4)) {
        if(status) {
            ADC10CTL0 |= ADC10ON;
        } else {
            ADC10CTL0 &= ~(ADC10ON);
        }
    } else {
        // SD: no need to switch on/off
    }
}

BYTE ADCready(BYTE nADC) {
    if((nADC == 2)||(nADC == 3)||(nADC == 4)) {
        return (!(ADC10CTL0 & ADC10BUSY)); //&& (ADC10CTL0 & (ADC10ON|ADC10ENC)) == (ADC10ON|ADC10ENC));
    } else if(nADC == 0) {
        return ((SD24BCCTL0 & SD24SC) == 0);
    } else if(nADC == 1) {
        return ((SD24BCCTL1 & SD24SC) == 0);
    }
    return 1;
}

BYTE ADCstart(BYTE nADC) {
    if(!ADCready(nADC)) return 0;
    if((nADC == 2)||(nADC == 3)||(nADC == 4)) {
        ADC10CTL0 &= (~ADC10ENC);
        // Select channel
        ADC10MCTL0 &= 0xF0;
        //ADC10MCTL0 |= (nADC&0x03);
        if (nADC==2) {ADC10MCTL0 |= ADC10INCH_0;}
        if (nADC==3) {ADC10MCTL0 |= ADC10INCH_1;}
        if (nADC==4) {ADC10MCTL0 |= ADC10INCH_2;}
        //if((2-nADC) & 0x10) {ADC10MCTL0 |= ADC10INCH1;}
        ADC10CTL0 |= (ADC10ENC|ADC10SC);    // Enable and start conversion
    } else if(nADC == 0) {
        SD24BCCTL0 |= SD24SC;
    } else if(nADC == 1) {
        SD24BCCTL1 |= SD24SC;
    }
    return 1;
}

WORD ADCgetValue(BYTE nADC) {
    WORD res;
    if((nADC == 2)||(nADC == 3)||(nADC == 4)){
        if(!ADCready(nADC)) return 0xFFFF;
        res = ADC10MEM0;
        ADC10CTL0 &= ~(ADC10ENC);      // Disable conversion
    } else if(nADC == 0) {
        // With the chosen left-aligned format,
        // read the high word only and discard the LSBits
        res = SD24BMEMH0;   // SD0
#ifdef SD24_16BIT
        // Reduce raw value from 16 to 11 bit
        res = (res < 0xFFD0 ? ((res + 0x0018) >> 5) : 0xFFFF);
#else
        // Reduce raw value from 16 to 10 bit
        res = (res < 0xFFD0 ? ((res + 0x0020) >> 6) : 0xFFFF);
#endif
    } else if(nADC == 1) {
        res =  SD24BMEMH1;   // SD1
#ifdef SD24_16BIT
        // Reduce raw value from 16 to 11 bit
        res = (res < 0xFFD0 ? ((res + 0x0018) >> 5) : 0xFFFF);
#else
        // Reduce raw value from 16 to 10 bit
        res = (res < 0xFFD0 ? ((res + 0x0020) >> 6) : 0xFFFF);
#endif
    }
    else {
        res = 0xFFFF;
    }
    return res;
}

// end ADCmgr.c
