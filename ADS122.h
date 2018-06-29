////////////////////////////////////////////////////////////////
// ADS122.cpp
// Arduino library to program and configure the ADS122C04 chip from TEXAS INSTRUMENTS
// Developed by: Pedro Ribeiro
//
// INESC-MN, 2018
////////////////////////////////////////////////////////////////

#ifndef ADS122_h
#define ADS122_h

#include "Arduino.h"
#include <Wire.h>

//DEBUGGING
#define ADS122_DEBUG 0 //Define this macro as 1 to enable verbose output

//REGISTER ADDRESSES
#define ADS122_REG0 0x0 
#define ADS122_REG1 0x4 
#define ADS122_REG2 0x8 
#define ADS122_REG3 0xC 

//COMMANDS
#define ADS122_RESET 0x06
#define ADS122_START 0x08
#define ADS122_POWERDOWN 0x04
#define ADS122_RDATA 0x10
#define ADS122_READREG 0x20
#define ADS122_WRITEREG 0x40

//REGISTER 0 OPTIONS
//MUX
#define ADS122_MUX_IN0-IN1 0x00
#define ADS122_MUX_IN0-IN2 0x10
#define ADS122_MUX_IN0-IN3 0x20
#define ADS122_MUX_IN1-IN0 0x30
#define ADS122_MUX_IN1-IN2 0x40
#define ADS122_MUX_IN1-IN3 0x50
#define ADS122_MUX_IN2-IN3 0x60
#define ADS122_MUX_IN3-IN2 0x70
#define ADS122_MUX_IN0 0x80
#define ADS122_MUX_IN1 0x90
#define ADS122_MUX_IN2 0xA0
#define ADS122_MUX_IN3 0xB0
#define ADS122_MUX_VREFP-VREFN 0xC0
#define ADS122_MUX_AVDD-AVSS 0xD0
#define ADS122_MUX_SHORTED 0xE0
#define ADS122_MUX_RESERVED 0xF0
//GAIN
#define ADS122_GAIN_1 0x00
#define ADS122_GAIN_2 0x02
#define ADS122_GAIN_4 0x04
#define ADS122_GAIN_8 0x06
#define ADS122_GAIN_16 0x08
#define ADS122_GAIN_32 0x0A
#define ADS122_GAIN_64 0x0C
#define ADS122_GAIN_128 0x0E
//PGA BYPASS
#define ADS122_PGA_ENABLED 0x00
#define ADS122_PGA_DISABLED 0x01

//REGISTER 1 OPTIONS
//DATA RATE - NORMAL VALUES (DOUBLE FOR TURBO)
#define ADS122_DR_20 0x00
#define ADS122_DR_45 0x20
#define ADS122_DR_90 0x40
#define ADS122_DR_175 0x60
#define ADS122_DR_330 0x80
#define ADS122_DR_600 0xA0
#define ADS122_DR_1000 0xC0
#define ADS122_DR_RESERVED 0xE0
//MODE 
#define ADS122_MODE_NORMAL 0x00
#define ADS122_MODE_TURBO 0x10
//CONVERSION MODE
#define ADS122_CM_SINGLE 0x00
#define ADS122_CM_CONTINUOUS 0x08
//VOLTAGE REFERENCE SELECTION
#define ADS122_VREF_INTERNAL 0x00
#define ADS122_VREF_EXTERNAL 0x04
#define ADS122_VREF_SUPPLY 0x06
//TEMPERATURE SENSOR MODE
#define ADS122_TS_DISABLED 0x00
#define ADS122_TS_ENABLED 0x01

//REGISTER 2 OPTIONS
//DATA READY - READ ONLY!
#define ADS122_DRDY_MEASURING 0x00
#define ADS122_DRDY_READY 0x10
//DATA COUNTER MODE
#define ADS122_DCNT_DISABLED 0x00
#define ADS122_DCNT_ENABLED 0x40
//DATA INTEGRITY CHECK MODE
#define ADS122_CRC_DISABLED 0x00
#define ADS122_CRC_INVERTED 0x10
#define ADS122_CRC_CRC16 0x20
#define ADS122_CRC_RESERVED 0x30
//BURNOUT CURRENT SOURCES MODE
#define ADS122_BURNOUT_DISABLED 0x00
#define ADS122_BURNOUT_ENABLED 0x08
//CURRENT SOURCE VALUE - VALUES IN uA
#define ADS122_IDAC_DISABLED 0x00
#define ADS122_IDAC_10 0x01
#define ADS122_IDAC_50 0x02
#define ADS122_IDAC_100 0x03
#define ADS122_IDAC_250 0x04
#define ADS122_IDAC_500 0x05
#define ADS122_IDAC_1000 0x06
#define ADS122_IDAC_1500 0x07

//REGISTER 3 OPTIONS
//CURRENT SOURCE 1 ROUTING
#define ADS122_I1MUX_DISABLED 0x00
#define ADS122_I1MUX_AIN0 0x20
#define ADS122_I1MUX_AIN1 0x40
#define ADS122_I1MUX_AIN2 0x60
#define ADS122_I1MUX_AIN3 0x80
#define ADS122_I1MUX_REFP 0xA0
#define ADS122_I1MUX_REFN 0xC0
#define ADS122_I1MUX_RESERVED 0xE0
//CURRENT SOURCE 2 ROUTING
#define ADS122_I2MUX_DISABLED 0x00
#define ADS122_I2MUX_AIN0 0x04
#define ADS122_I2MUX_AIN1 0x08
#define ADS122_I2MUX_AIN2 0x0C
#define ADS122_I2MUX_AIN3 0x10
#define ADS122_I2MUX_REFP 0x14
#define ADS122_I2MUX_REFN 0x18
#define ADS122_I2MUX_RESERVED 0x1C
//The 2 LSBs of this register are RESERVED (should be always 0) 

 

union Byte3 {
  unsigned long int code;
  byte bytes[3];
}; //Union - converts 3 bytes into an unsigned long int

class ADS122 {
  private:
    //VARIABLES
    byte reg[4]; //Array storing register information
    byte address; //ADC I2C address
    //FUNCTIONS
    byte readreg(byte f_address);
    void writereg(byte f_address, byte message);
    void panic();
  
  public:
    //VARIABLES
    unsigned long int cal_x;
    unsigned long int cal_y;
    //FUNCTIONS
    void init(byte f_address);
    void set(byte f_address, byte message);
    void reset();
    void measure(bool pinwait, int drd);
    Byte3 read();
    void calib();
};


#endif

