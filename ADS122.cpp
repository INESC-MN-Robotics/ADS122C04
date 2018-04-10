#include "Arduino.h"
#include <Wire.h>
#include "ADS122.h"

byte ADS122::readreg(byte f_address){
  //Reads a requested ADC register
  //INPUT PARAMETERS
  //byte f_address -> Register address to be written
  //RETURN
  //Read register value
  byte reg;
  
  #if ADS122_DEGUG==1
  Serial.print("Reading register 0x");
  Serial.println(f_address >> 2,HEX);
  #endif
  Wire.beginTransmission(this->address);
  Wire.write(byte(ADS122_READREG)|f_address);
  Wire.endTransmission();
  Wire.requestFrom(byte(f_address),1);
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
  #if ADS122_DEBUG==1
  Serial.print("Writing register 0x");
  Serial.println(f_address >> 2, HEX);
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
  //Serial.println("Initializing...");
  #if ADS122_DEBUG == 1
  Serial.println("Initializing...");
  #endif
  Serial.println("Passou1");
  this->address = f_address;
  Serial.println("Passou2");
  Wire.beginTransmission(f_address);
  Wire.write(byte(ADS122_RESET));
  Wire.endTransmission();
  Serial.println("Passou3");
  this->reg[3] = this->readreg(ADS122_REG3);
  this->reg[2] = this->readreg(ADS122_REG2);
  this->reg[2] = this->readreg(ADS122_REG1);
  this->reg[1] = this->readreg(ADS122_REG0);
  Serial.println("Passou4");
}

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




