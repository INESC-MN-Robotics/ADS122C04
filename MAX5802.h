////////////////////////////////////////////////////////////////
// ADS122.cpp
// Arduino library to program and configure the ADS122C04 chip from TEXAS INSTRUMENTS
// Developed by: Pedro Ribeiro
//
// INESC-MN, 2018
////////////////////////////////////////////////////////////////

#ifndef MAX5802_h
#define MAX5802_h

#include "Arduino.h"
#include <Wire.h>

//DEBUGGING
#define MAX5802_DEBUG 0//Define this macro as 1 to enable verbose output

//DAC SELECTOR
#define DAC_A 0x0
#define DAC_B 0x1
#define ALL_DACS 0x4

//DAC COMMANDS
#define CODE_N 0x0
#define LOAD_N 0x10
#define CODE_N_LOAD_ALL 0x20
#define CODE_N_LOAD_N 0x30

//CONFIG COMMANDS
#define POWER_ON 0x40
#define POWER_PD_1K 0x41
#define POWER_PD_100K 0x42
#define POWER_PD_HZ 0x43
#define SOFT_CLEAR 0x50
#define SOFT_RESET 0x51
#define LATCH_ON 0x60
#define LATCH_TRANSPARENT 0x61
#define REF_PD_OFF 0x70
#define REF_PD_ON 0x74
#define REF_MODE_EXT 0x00
#define REF_MODE_2V5 0x01
#define REF_MODE_2V0 0x02
#define REF_MODE_4V1 0x03

//CONFIG DAC SELECTOR
#define CONFIG_DAC_A 0x01
#define CONFIG_DAC_B 0x10

union Byte2 {
  long int code;
  byte bytes[2];
}; //Union - converts 2 bytes into an unsigned int

class MAX5802 {
  private: 
    Byte2 dac_a;
    Byte2 dac_b;

    double ref_voltage;
    bool powerdown;
    byte address;

  public:
    void init(byte address);
    void writeDAC(unsigned int code, byte devices);
};

#endif
