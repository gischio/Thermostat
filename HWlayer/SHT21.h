
#ifndef SHT21_H
#define SHT21_H

#define xxxSHT21_READ_T
#define xxxSGP30_BASELINE

#ifdef SHT21_READ_T
#warning ========================================================
#warning SHT21_READ_T << Lettura temperatura abilitata ! >>
#warning ========================================================
#endif
#ifdef SGP30_BASELINE
#warning ========================================================
#warning SGP30_BASELINE << Gestione baseline abilitata ! >>
#warning ========================================================
#endif


// sensor command
typedef enum{
  TRIG_T_MEASUREMENT_HM    = 0xE3, // command trig. temp meas. hold master
  TRIG_RH_MEASUREMENT_HM   = 0xE5, // command trig. humidity meas. hold master
  TRIG_T_MEASUREMENT_POLL  = 0xF3, // command trig. temp meas. no hold master
  TRIG_RH_MEASUREMENT_POLL = 0xF5, // command trig. humidity meas. no hold master
  USER_REG_W               = 0xE6, // command writing user register
  USER_REG_R               = 0xE7, // command reading user register
  SOFT_RESET               = 0xFE  // command soft reset
}etSHT2xCommand;

typedef enum {
  SHT2x_RES_12_14BIT       = 0x00, // RH=12bit, T=14bit
  SHT2x_RES_8_12BIT        = 0x01, // RH= 8bit, T=12bit
  SHT2x_RES_10_13BIT       = 0x80, // RH=10bit, T=13bit
  SHT2x_RES_11_11BIT       = 0x81, // RH=11bit, T=11bit
  SHT2x_RES_MASK           = 0x81  // Mask for res. bits (7,0) in user reg.
} etSHT2xResolution;

typedef enum {
  SHT2x_EOB_ON             = 0x40, // end of battery
  SHT2x_EOB_MASK           = 0x40, // Mask for EOB bit(6) in user reg.
} etSHT2xEob;

typedef enum {
  SHT2x_HEATER_ON          = 0x04, // heater on
  SHT2x_HEATER_OFF         = 0x00, // heater off
  SHT2x_HEATER_MASK        = 0x04, // Mask for Heater bit(2) in user reg.
} etSHT2xHeater;

// measurement signal selection
typedef enum{
  HUMIDITY,
  TEMP
}etSHT2xMeasureType;

/*
typedef enum{
  I2C_ADR_W                = 128,   // sensor I2C address + write bit
  I2C_ADR_R                = 129    // sensor I2C address + read bit
}etI2cHeader;
*/

typedef enum{
  ACK_ERROR                = 0x02,
  TIME_OUT_ERROR           = 0x04,
  CHECKSUM_ERROR           = 0x08,
  UNIT_ERROR               = 0x10
}etError;

typedef enum{
  SGP_INIT_AIR_QUALITY            = 0x00,
  SGP_GET_FEATURE_SET_VERSION_RQ  = 0x01,
  SGP_GET_FEATURE_SET_VERSION_RX  = 0x02,  
  SGP_GET_BASE_LINE_RQ            = 0x03,  
  SGP_GET_BASE_LINE_RX            = 0x04,  
  SGP_SET_BASE_LINE               = 0x05,  
  SGP_MEASURE_AIR_QUALITY_RQ      = 0x06,  
  SGP_MEASURE_AIR_QUALITY_RX      = 0x07,  
  SGP_MEASURE_WAIT_NEXT           = 0x50,
}etSGPStatus;

typedef enum{
  SHT_SOFT_RESET           = 0x00,
  SHT_TRIG_RH              = 0x01,
  SHT_READ_RH              = 0x02,  
  SHT_CONV_RH              = 0x03,  
  SHT_TRIG_T               = 0x04,
  SHT_READ_T               = 0x05,  
  SHT_CONV_T               = 0x06,  
}etSHTStatus;

typedef enum{
  I2C_SHT                  = 0x00,
  I2C_SGP                  = 0x01,
}etI2CStatus;
  
//#define I2C_TRANSMIT_MODE   UCTR
//#define I2C_RECEIVE_MODE    0x00

#define SGP30_ADDR          0x58
#define SHT21_ADDR          0x40

#define I2C_R_MODE          0
#define I2C_W_MODE          UCTR
#define I2C_NAK             2
#define TXLENGTH            10
#define RXLENGTH            10

#define SGP30_CRC8_INIT     0xFF
#define SHT21_CRC8_INIT     0x00




#define I2C_WAIT_BEFORE_STOP    0x01

#define ClearRxBuffer()       memset(RxData,0,sizeof(RxData))

extern long rShtTemp;
#ifdef SHT21_SEND_RAW_VALUE
extern unsigned int rRhRawValue;
#endif
#ifdef SGP30_SEND_RAW_VALUE            
extern unsigned int rCO2RawValue;
#endif

extern unsigned int TVOC;

unsigned char I2C_poll( unsigned char reset );
unsigned char SHT21_poll( unsigned char reset );
unsigned long SHT21_read( void );
//extern void ReadRH( void );
extern unsigned char SHT2x_CheckCrc(unsigned char data[], unsigned char nbrOfBytes, unsigned char checksum);
#endif  // SHT21_H
