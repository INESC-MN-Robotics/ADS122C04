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

void ADS122::measure(bool pinwait, int drd){
  Wire.beginTransmission(this->address);
  Wire.write(byte(ADS122_START));
  Wire.endTransmission();
  #if ADS122_DEBUG == 1
  Serial.println(" ");
  Serial.println("ENTERING MEASURE");
  Serial.print("Intruction sent: 0x");
  Serial.println(byte(ADS122_START));
  Serial.println("Starting measurement");
  #endif

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

void ADS122::calib(){
  int i;
  unsigned long int cal;
  cal = 0;

  this->set(byte(ADS122_REG0),byte(ADS122_MUX_IN2|ADS122_GAIN_1|ADS122_PGA_DISABLED)); 
  
  for(i=0;i<100;i++){
    this->measure(true,7);
    cal = cal + this->read().code;
  }
  this->cal_x = cal/100;
  cal = 0;

  this->set(byte(ADS122_REG0),byte(ADS122_MUX_IN3|ADS122_GAIN_1|ADS122_PGA_DISABLED)); 
  
  for(i=0;i<100;i++){
    this->measure(true,7);
    cal = cal + this->read().code;
  }
  this->cal_y = cal/100;

  return;
}




