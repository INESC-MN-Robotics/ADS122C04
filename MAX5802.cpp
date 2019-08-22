////////////////////////////////////////////////////////////////
// ADS122.cpp
// Arduino library to program and configure the ADS122C04 chip from TEXAS INSTRUMENTS
// Developed by: Pedro Ribeiro
//
// INESC-MN, 2018
////////////////////////////////////////////////////////////////

#include "Arduino.h"
#include <Wire.h>
#include "MAX5802.h"

void MAX5802::init(byte address){
  this->address = address;

  Wire.beginTransmission(this->address); //At start-up reset DAC configuration and setpoints
  Wire.write(SOFT_RESET);
  Wire.write(0x00);
  Wire.write(0x00);
  Wire.endTransmission();

  delay(1000); //Wait 1 sec for stabilization

  //Wire.beginTransmission(this->address);
  //Wire.write(LATCH_TRANSPARENT); //Make latch transparent - change voltage value when register is updated
  //Wire.write(CONFIG_DAC_B|CONFIG_DAC_A); //For both DACs
  //Wire.write(0x00);
  //Wire.endTransmission();
  //delay(10);

  Wire.beginTransmission(this->address);
  Wire.write(byte(REF_PD_ON) | byte(REF_MODE_2V0)); //Set internal, 2.0 V reference on (even during powerdown)
  //Wire.write(0x76);
  Wire.write(0x00);
  Wire.write(0x00);
  Wire.endTransmission();
  delay(10);

  this->writeDAC(0 ,ALL_DACS);
}

void MAX5802::writeDAC(unsigned int code, byte devices){

    if(devices == DAC_A){
      this->dac_a.code = code << 4;

      Wire.beginTransmission(this->address);  
      Wire.write(CODE_N_LOAD_N | devices);
      Wire.write(this->dac_a.bytes[1]);
      Wire.write(this->dac_b.bytes[0]);
      Wire.endTransmission();
    }
    else if(devices == DAC_B){
      this->dac_b.code = code << 4;

      Wire.beginTransmission(this->address);
      Wire.write(CODE_N_LOAD_N | devices);
      Wire.write(this->dac_b.bytes[1]);
      Wire.write(this->dac_b.bytes[0]);
      Wire.endTransmission();
    }
    else if(devices == ALL_DACS){
      this->dac_a.code = code << 4;
      this->dac_b = dac_a;

      Wire.beginTransmission(this->address);
      Wire.write(CODE_N_LOAD_N | devices);
      Wire.write(dac_b.bytes[1]);
      Wire.write(dac_b.bytes[0]);
      Wire.endTransmission();
    }
  
}
