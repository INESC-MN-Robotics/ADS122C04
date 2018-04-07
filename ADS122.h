#ifndef ADS122_h
#define ADS122_h

#include "Arduino.h"
#include <Wire.h>

//DEBUGGING
#define ADS122_DEBUG 1

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
    Byte3 acq; //Stores the read acquisition value         
    //FUNCTIONS
    ADS122(byte address);
};


#endif
