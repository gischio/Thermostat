/*
 * Copyright (c) 2017, Sensirion AG
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * * Neither the name of Sensirion AG nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include "msp430f6724.h"
#include "i2c.h"


const sgp_command command[CMD_MEASURE_SIGNAL+1] = {
  {0x20, 0x03},
  {0x20, 0x08},
  {0x20, 0x15},
  {0x20, 0x1E},
  {0x20, 0x32},
  {0x20, 0x2F},  
  {0x20, 0x50},    
};

#define STATUS_SUCCESS 1
#define STATUS_FAIL    0

#define EUSCI_B_I2C_NAK_INTERRUPT                                      UCNACKIE
#define EUSCI_B_I2C_ARBITRATIONLOST_INTERRUPT                            UCALIE
#define EUSCI_B_I2C_STOP_INTERRUPT                                      UCSTPIE
#define EUSCI_B_I2C_START_INTERRUPT                                     UCSTTIE
#define EUSCI_B_I2C_TRANSMIT_INTERRUPT0                                 UCTXIE0
#define EUSCI_B_I2C_TRANSMIT_INTERRUPT1                                 UCTXIE1
#define EUSCI_B_I2C_TRANSMIT_INTERRUPT2                                 UCTXIE2
#define EUSCI_B_I2C_TRANSMIT_INTERRUPT3                                 UCTXIE3
#define EUSCI_B_I2C_RECEIVE_INTERRUPT0                                  UCRXIE0
#define EUSCI_B_I2C_RECEIVE_INTERRUPT1                                  UCRXIE1
#define EUSCI_B_I2C_RECEIVE_INTERRUPT2                                  UCRXIE2
#define EUSCI_B_I2C_RECEIVE_INTERRUPT3                                  UCRXIE3
#define EUSCI_B_I2C_BIT9_POSITION_INTERRUPT                            UCBIT9IE
#define EUSCI_B_I2C_CLOCK_LOW_TIMEOUT_INTERRUPT                        UCCLTOIE
#define EUSCI_B_I2C_BYTE_COUNTER_INTERRUPT                             UCBCNTIE

#define EUSCI_B_I2C_TRANSMIT_MODE                                          UCTR
#define EUSCI_B_I2C_RECEIVE_MODE                                           0x00

#define EUSCI_B_I2C_STOP_SEND_COMPLETE                                     0x00
#define EUSCI_B_I2C_SENDING_STOP                                        UCTXSTP

void I2C_masterReceiveMultiByteStop (void);
unsigned char I2C_masterReceiveMultiByteFinishWithTimeout (unsigned char *txData,
                                                           unsigned long timeout);
void I2C_clearInterrupt (unsigned int mask);
unsigned int I2C_getInterruptStatus (unsigned int mask);
void I2C_masterReceiveStart (void);
void I2C_setMode (unsigned char mode);
unsigned char I2C_getMode (void);
void I2C_enable (void);
void I2C_disable (void);
unsigned int I2C_masterIsStopSent (void);
unsigned int I2C_masterIsStartSent (void);
void I2C_enableInterrupt (unsigned int mask);
void I2C_masterSendStart (void);
void I2C_setSlaveAddress (unsigned char slaveAddress);
void I2C_masterSendMultiByteNext (unsigned char txData);
unsigned char I2C_masterSendMultiByteStopWithTimeout (unsigned long timeout);
void I2C_masterSendMultiByteStop (void);
unsigned char I2C_masterReceiveSingle (void);


uint8_t *pRXData;
uint8_t rXcount;
uint8_t *pTXData;
uint8_t tXcount;

/*
 * INSTRUCTIONS
 * ============
 *
 * Implement all functions where they are marked as IMPLEMENT.
 * Follow the function specification in the comments.
 */

/**
 * Initialize all hard- and software components that are needed for the I2C
 * communication.
 */
void sensirion_i2c_init()
{

    P2SEL |= 0x03;   
    P2OUT &= ~(BIT1|BIT0); 
    P2DIR |= (BIT1|BIT0);                               // Direction: output
    UCB0CTLW0 |= UCSWRST;                               // Enable SW reset
    UCB0CTLW0 |= UCMST | UCMODE_3 | UCSSEL_2 | UCSYNC;  // I2C Master, use SMCLK
    UCB0BRW = 80;                                       // 100 KHz
    UCB0I2CSA = 0x58;                                   // Slave Address 
    UCB0CTLW0 &= ~UCSWRST;                              // Clear SW reset, resume operation
    UCB0IE |= UCTXIE0;                                  // Enable TX interrupt         
}

/**
 * Execute one read transaction on the I2C bus, reading a given number of bytes.
 * If the device does not acknowledge the read command, an error shall be
 * returned.
 *
 * @param address 7-bit I2C address to read from
 * @param data    pointer to the buffer where the data is to be stored
 * @param count   number of bytes to read from I2C and store in the buffer
 * @returns 0 on success, error code otherwise
 */
int8_t sensirion_i2c_read(uint8_t address, uint8_t* data, uint16_t count)
{
    
    I2C_setSlaveAddress(0x58);
    I2C_setMode(EUSCI_B_I2C_RECEIVE_MODE);
    I2C_enable();
    I2C_clearInterrupt(	EUSCI_B_I2C_RECEIVE_INTERRUPT0 +
			EUSCI_B_I2C_BYTE_COUNTER_INTERRUPT +
			EUSCI_B_I2C_NAK_INTERRUPT
		      );
    I2C_enableInterrupt( EUSCI_B_I2C_RECEIVE_INTERRUPT0 +
                         EUSCI_B_I2C_BYTE_COUNTER_INTERRUPT +
                         EUSCI_B_I2C_NAK_INTERRUPT
                       );    
    while (EUSCI_B_I2C_SENDING_STOP == I2C_masterIsStopSent());
    
    pRXData = data;
    rXcount = count;
    I2C_masterReceiveStart();  
    
    while (rXcount);
    
    return 0;
}

/**
 * Execute one write transaction on the I2C bus, sending a given number of bytes.
 * The bytes in the supplied buffer must be sent to the given address. If the
 * slave device does not acknowledge any of the bytes, an error shall be
 * returned.
 *
 * @param address 7-bit I2C address to write to
 * @param data    pointer to the buffer containing the data to write
 * @param count   number of bytes to read from the buffer and send over I2C
 * @returns 0 on success, error code otherwise
 */
int8_t sensirion_i2c_write(uint8_t address, const uint8_t* data, uint16_t count)
{
    I2C_setSlaveAddress(0x58);
    I2C_setMode(EUSCI_B_I2C_RECEIVE_MODE);
    I2C_enable();
    I2C_clearInterrupt(	EUSCI_B_I2C_RECEIVE_INTERRUPT0 +
			EUSCI_B_I2C_BYTE_COUNTER_INTERRUPT +
			EUSCI_B_I2C_NAK_INTERRUPT
		      );
    I2C_enableInterrupt( EUSCI_B_I2C_RECEIVE_INTERRUPT0 +
                         EUSCI_B_I2C_BYTE_COUNTER_INTERRUPT +
                         EUSCI_B_I2C_NAK_INTERRUPT
                       );    
    
    pTXData = (uint8_t*)data;
    tXcount = count;
    
    I2C_masterSendStart();
    
    while (tXcount);
    
    return 0;
}

/**
 * Sleep for a given number of microseconds. The function should delay the
 * execution for at least the given time, but may also sleep longer.
 *
 * @param useconds the sleep time in microseconds
 */
void sensirion_sleep_usec(uint32_t useconds) {

    for (unsigned long i=0;i<useconds;i++);
}

/**
 */
#pragma vector = USCI_B0_VECTOR
__interrupt void USCI_B0_TXRXISR ( void )
{
  switch(__even_in_range(UCB0IV,0x1E))
  {
        case 0x00: break;                   // Vector 0: No interrupts break;
        case 0x02: break;
        case 0x04:
          I2C_masterSendStart();
          break;                            // Vector 4: NACKIFG break;
        case 0x06: break;                   // Vector 6: STTIFG break;
        case 0x08: break;                   // Vector 8: STPIFG break;
        case 0x0a: break;                   // Vector 10: RXIFG3 break;
        case 0x0c: break;                   // Vector 14: TXIFG3 break;
        case 0x0e: break;                   // Vector 16: RXIFG2 break;
        case 0x10: break;                   // Vector 18: TXIFG2 break;
        case 0x12: break;                   // Vector 20: RXIFG1 break;
        case 0x14: break;                   // Vector 22: TXIFG1 break;
        case 0x16: 
            *pRXData = I2C_masterReceiveSingle();   // Get RX data
            pRXData++;
    	    if (--rXcount) 
            {
	      rXcount = 0;
            }
        break;                              // Vector 24: RXIFG0 break;            
        case 0x18:
              if (rXcount--)                // Check TX byte counter
              {
                  I2C_masterSendMultiByteNext( *pTXData);
                  pTXData++;
              }
              else
              {
                  I2C_masterSendMultiByteStop();
              }
        break;                              // Vector 26: TXIFG0 break;
        default: break;
  }
}
/**
 */
void I2C_masterSendStart (void)
{
    UCB0CTLW0 |= UCTXSTP;
}

/**
 */
void I2C_setSlaveAddress (unsigned char slaveAddress)
{
    UCB0I2CSA = slaveAddress;
}
/**
 */
unsigned char I2C_masterSendMultiByteStartWithTimeout (
    unsigned char txData,
    unsigned long timeout
    )
{
    unsigned int txieStatus = UCB0IE & UCTXIE;

    UCB0IE &= ~(UCTXIE);

    UCB0CTLW0 |= UCTR +  UCTXSTT;

    while ((!(UCB0IFG & UCTXIFG)) && --timeout);

    if (timeout == 0){
        return (STATUS_FAIL);
    }

    UCB0TXBUF = txData;

    UCB0IE |= txieStatus;

    return(STATUS_SUCCESS);
}
/**
 */
void I2C_setMode (unsigned char mode)
{
    UCB0CTLW0 &= ~EUSCI_B_I2C_TRANSMIT_MODE;
    UCB0CTLW0 |= mode;
}
/**
 */
unsigned char I2C_getMode (void)
{
    return (UCB0CTLW0 & UCTR);
}
/**
 */
void I2C_enable (void)
{
    UCB0CTLW0 &= ~(UCSWRST);
}
/**
 */
void I2C_disable (void)
{
    UCB0CTLW0 |= UCSWRST;
}
/**
 */
unsigned int I2C_masterIsStopSent (void)
{
    return (UCB0CTLW0 & UCTXSTP);
}
/**
 */
unsigned int I2C_masterIsStartSent (void)
{
    return (UCB0CTLW0 & UCTXSTT);
}
/**
 */
void I2C_enableInterrupt (unsigned int mask)
{
    UCB0IE |= mask;
}
/**
 */
void I2C_disableInterrupt (unsigned int mask)
{
    UCB0IE &= ~(mask);
}
/**
 */
void I2C_clearInterrupt (unsigned int mask)
{
    UCB0IFG &= ~(mask);
}
/**
 */
unsigned int I2C_getInterruptStatus (unsigned int mask)
{
    return (UCB0IFG & mask);
}
/**
 */
void I2C_masterReceiveStart (void)
{
    UCB0CTLW0 &= ~UCTR;
    UCB0CTLW0 |= UCTXSTT;
}
/**
 */
unsigned char I2C_masterReceiveMultiByteFinishWithTimeout (unsigned char *txData,
                                                           unsigned long timeout)
{
    uint32_t timeout2 = timeout;

    UCB0CTLW0 |= UCTXSTP;

    while ((UCB0CTLW0 & UCTXSTP) && --timeout);

    if (timeout == 0){
        return (STATUS_FAIL);
    }

    while ((!(UCB0IFG & UCRXIFG)) && --timeout2);

    if (timeout2 == 0){
        return (STATUS_FAIL);
    }

    *txData = (UCB0RXBUF);

    return (STATUS_SUCCESS);
}
/**
 */
void I2C_masterReceiveMultiByteStop (void)
{
    UCB0CTLW0 |= UCTXSTP;
}
/**
 */
void I2C_masterSendMultiByteNext (unsigned char txData)
{
    if (!(UCB0IE & UCTXIE)){
        while (!(UCB0IFG & UCTXIFG)) ;
    }

    UCB0TXBUF = txData;
}
/**
 */
unsigned char I2C_masterSendMultiByteStopWithTimeout (unsigned long timeout)
{
    if (!(UCB0IE & UCTXIE)){
        while ((!(UCB0IFG & UCTXIFG)) && --timeout) ;

        if (timeout == 0){
            return (STATUS_FAIL);
        }
    }

    UCB0CTLW0 |= UCTXSTP;

    return (STATUS_SUCCESS);
}
/**
 */
void I2C_masterSendMultiByteStop (void)
{
    if (!(UCB0IE & UCTXIE)){
        while (!(UCB0IFG & UCTXIFG)) ;
    }

    UCB0CTLW0 |= UCTXSTP;
}
/**
 */
unsigned char I2C_masterReceiveSingle (void)
{
    if(!(UCB0IE & UCRXIE0)) {
        while(!(UCB0IFG & UCRXIFG0));
    }

    return (UCB0RXBUF);
}