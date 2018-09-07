////////////////////////////////////////////////////////////////
// ADS122.cpp
// Arduino library to program and configure the ADS122C04 chip from TEXAS INSTRUMENTS
// Developed by: Pedro Ribeiro
//
// INESC-MN, 2018
////////////////////////////////////////////////////////////////

#include "Arduino.h"
#include <Wire.h>
#include "ADS122.h"

byte ADS122::readreg(byte f_address){
  //Reads a requested ADC register
  //INPUT PARAMETERS
  //byte f_address -> Register address to be written
  //RETURN
  //Read register value
  byte reg = 2;
  
  #if ADS122_DEBUG==1
  Serial.println(" ");
  Serial.println("ENTERING READREG");
  Serial.print("Reading register 0x");
  Serial.println(f_address >> 2,HEX);
  Serial.print("I2C address: ");
  Serial.println(this->address,HEX);
  #endif
  Wire.beginTransmission(this->address);
  Wire.write(byte(ADS122_READREG)|f_address);
  Wire.endTransmission();
  #if ADS122_DEBUG==1
  Serial.print("Instruction sent: ");
  Serial.println(byte(ADS122_READREG|f_address),HEX);
  #endif
  Wire.requestFrom(byte(this->address),1);
  while(Wire.available()){
    reg = Wire.read();
  }
  #if ADS122_DEBUG==1
  Serial.print("Value: 0x");
  Serial.println(reg,HEX);
  #endif
  return(reg);
}

void ADS122::panic(){
  //Stops firmware execution - BLOCKING
  //No input parameters
  //Does not return
    while(1){
      Serial.println("Critical condition reached - the firmware is unable to continue safely!");
      Serial.println("Check your hardware and firmware - then restart your arduino");
      delay(5000);
    }
}

void ADS122::writereg(byte f_address, byte message){
  //Writes a message on an ADC register
  //INPUT PARAMETERS
  // f_address -> Register address to be written
  // message -> Content to be written on the register
  //Does not return 

  byte result;

if(message == reg[f_address >> 2]){
  #if ADS122_DEBUG==1
  Serial.print("Instruction was already found on board");
  #endif
  return;
 }

  #if ADS122_DEBUG==1
  Serial.println(" ");
  Serial.println("ENTERING WRITEREG");
  Serial.print("Writing register 0x");
  Serial.println(f_address >> 2, HEX);
  Serial.print("Instruction: ");
  Serial.println(byte(ADS122_WRITEREG)|f_address);
  #endif
  Wire.beginTransmission(this->address);
  Wire.write(byte(ADS122_WRITEREG)|f_address);
  Wire.write(message);
  Wire.endTransmission();
  #if ADS122_DEBUG==1
  Serial.println("Written!");
  #endif
  
  result = this->readreg(f_address);

  if(result == message){
    this->reg[f_address >> 2] = message;
    #if ADS122_DEBUG==1
    Serial.println("Success!");
    #endif
  }
  else{
    Serial.println("Write unsuccessful");
    Serial.print("To be written: 0x");
    Serial.println(message, HEX);
    Serial.print("Actually written: 0x");
    Serial.println(result,HEX);
    this->panic();
  }
  return;
  
}

void ADS122::init(byte f_address){
  //ADS122 class constructor
  //INPUT PARAMETERS
  //ADC I2C address
  #if ADS122_DEBUG==1
  Serial.println("Initializing...");
  Serial.print("Input address: ");
  Serial.println(f_address,HEX);
  #endif
  this->address = f_address;
  #if ADS122_DEBUG==1
  Serial.print("Class address: ");
  Serial.println(this->address,HEX);
  #endif
  Wire.beginTransmission(f_address);
  Wire.write(byte(ADS122_RESET));
  Wire.endTransmission();
  this->reg[3] = this->readreg(ADS122_REG3);
  this->reg[2] = this->readreg(ADS122_REG2);
  this->reg[2] = this->readreg(ADS122_REG1);
  this->reg[1] = this->readreg(ADS122_REG0);
}


//Set is now private//
 void ADS122::set(byte f_address, byte message){
  this->writereg(f_address, message);
  return;
}

void ADS122::reset(){
  Wire.beginTransmission(this->address);
  Wire.write(byte(ADS122_RESET));
  Wire.endTransmission();
  #if ADS122_DEBUG == 1
  Serial.println("Reset instruction sent");
  #endif
}

void ADS122::powerdown(){
	Wire.beginTransmission(this->address);
	Wire.write(byte(ADS122_POWERDOWN)); //Byte value being 0x02, also works with 0x03, consult the "Commands" section in ADS122.h
	Wire.endTransmission();
	#if ADS122_DEBUG == 1
	Serial.println(" ");
	Serial.println("Powerdone instruction sent");
	Serial.print("Instruction sent: 0x");
	Serial.print(byte(ADS122_POWERDOWN));
	#endif
}

void ADS122::measure(bool pinwait, int drd){
  Wire.beginTransmission(this->address);
  Wire.write(byte(ADS122_START));
  Wire.endTransmission();
  //#if ADS122_DEBUG == 1
  //Serial.println(" ");
  // <<<< delay(30); >>>> IN ORDER TO WORK?
  //Serial.print("Intruction sent: 0x");
  //Serial.println(byte(ADS122_START));
  //Serial.println("Starting measurement");
  //#endif

  //If pinwait is true (use pin as way of knowing if measurement is complete), wait for DRDY pin to go low
  if(pinwait == true)
    while(digitalRead(drd)==true);
  else{ //Else, use drd argument as delay to wait for reading the measurement (in milliseconds)
    delay(drd);
  }
}

Byte3 ADS122::read(){
  Byte3 result;
  result.code = 0;
  int exp_bytes = 3;
  Wire.beginTransmission(this->address);
  Wire.write(byte(ADS122_RDATA));
  Wire.endTransmission();
  #if ADS122_DEBUG == 1
  Serial.println(" ");
  Serial.println("ENTERING READ");
  Serial.print("Intruction sent: 0x");
  Serial.println(byte(ADS122_RDATA));
  Serial.println("Requesting read values");
  #endif
  Wire.requestFrom(this->address,3);
  while(Wire.available()){
    exp_bytes--;
    result.bytes[exp_bytes] = Wire.read();
  }
  return(result);
}

void ADS122::set_up(byte adress, int channel, int ain_channel1, int ain_channel2, int gain, bool PGA, int datarate, bool single, bool turbo, int curr, byte reference){
                              
        this->init(adress);
        delay(10);
        byte ch, ain1, ain2, mode, gn, pga, dr, TURBO, current, ref;
        switch(channel){
          case -1: ch= ADS122_MUX_IN0_IN1; break;
          case -2: ch= ADS122_MUX_IN0_IN2; break;
          case -3: ch= ADS122_MUX_IN0_IN3; break;
          case 0: ch= ADS122_MUX_IN0; break;
          case 1: ch= ADS122_MUX_IN1; break;
          case 2: ch= ADS122_MUX_IN2; break;
          case 3: ch= ADS122_MUX_IN3; break;
          case 10: ch= ADS122_MUX_IN1_IN0; break;
          case 12: ch= ADS122_MUX_IN1_IN2; break;
          case 13: ch= ADS122_MUX_IN1_IN3; break;
          case 23: ch= ADS122_MUX_IN2_IN3; break;
          case 32: ch= ADS122_MUX_IN3_IN2; break;
          default: Serial.print("Error: Channel not found, closing function"); return;
        }
        switch(ain_channel1){
          case -1: ain1= ADS122_I1MUX_DISABLED; break;
          case 0: ain1= ADS122_I1MUX_AIN0; break;
          case 1: ain1= ADS122_I1MUX_AIN1; break;
          case 2: ain1= ADS122_I1MUX_AIN2; break;
          case 3: ain1= ADS122_I1MUX_AIN3; break;
          default: Serial.print("Invalid channel for current"); return;
        }
        switch(ain_channel2){
          case -1: ain2= ADS122_I1MUX_DISABLED; break;
          case 0: ain2= ADS122_I1MUX_AIN0; break;
          case 1: ain2= ADS122_I1MUX_AIN1; break;
          case 2: ain2= ADS122_I1MUX_AIN2; break;
          case 3: ain2= ADS122_I1MUX_AIN3; break;
        }
        switch(curr){
          case 0: current = 0x00; break;
          case 10: current = 0x01; break;
          case 50: current = 0x02; break;
          case 100: current = 0x03; break;
          case 250: current = 0x04; break;
          case 500: current = 0x05; break;
          case 1000: current = 0x06; break;
          case 1500: current = 0x07; break;
          default: Serial.print("Error: Invalid Current, closing function"); return;
        }
        switch(gain){
          case 1: gn= 0x00; break;
          case 2: gn= 0x02 ;break;
          case 4: gn= 0x04; break;
          case 8: gn= 0x06; break;
          case 16: gn = 0x08; break;
          case 32: gn= 0x0A; break;
          case 64: gn= 0x0C; break;
          case 128: gn= 0x0E; break;
          default: Serial.print("Error: Gain value not found, closing function"); return;
        }
        switch(PGA){
          case true: pga = 0x00; break;
          case false: pga = 0x01; break;
          default: Serial.print("Invalid PGA boolean argument, closing function"); return;
        }
        switch(datarate){
          case 20: dr= 0x00; break;
          case 45: dr= 0x20; break;
          case 90: dr= 0x40; break;
          case 175: dr= 0x60; break;
          case 330: dr= 0x80; break;
          case 600: dr= 0xA0; break;
          case 1000: dr= 0xC0; break;
          default: Serial.print("Error: Invalid Data Rate value, closing function"); return;
        }
        switch(turbo){
          case true: TURBO= 0x10; break;
          case false: TURBO= 0x00; break;
          default: Serial.print("Invalid turbo boolean argument, closing function"); return;
        }
        switch(single){
          case true: mode= ADS122_CM_SINGLE; break;
          case false: mode= ADS122_CM_SINGLE; break;
          default: Serial.print("Invalid operation mode");
        }

        this->set(byte(ADS122_REG0),byte(ch|gn|pga)); 
        delay(10);
        this->set(byte(ADS122_REG1),byte(dr|TURBO|single|reference|ADS122_TS_DISABLED));
        delay(10);
        this->set(byte(ADS122_REG2),byte(ADS122_DRDY_MEASURING|ADS122_DCNT_DISABLED|ADS122_CRC_DISABLED|ADS122_BURNOUT_DISABLED|current));
        delay(10);
        this->set(byte(ADS122_REG3),byte(ain1|ain2));
        delay(10);  
  
}



