// I2C Digital Potentiometer
// by Nicholas Zambetti <http://www.zambetti.com>
// and Shawn Bonkowski <http://people.interaction-ivrea.it/s.bonkowski/>

// Demonstrates use of the Wire library
// Controls AD5171 digital potentiometer via I2C/TWI

// Created 31 March 2006

// This example code is in the public domain.

// This example code is in the public domain.


#include <Wire.h>
#include "ADS122.h"

#define ADC_ADDRESS 0x40

ADS122 adc;

void setup() {
  //Wire.begin(); // join i2c bus (address optional for master)
  Serial.begin(2000000);
  Wire.begin();
  Serial.println("WOKE");
  adc.init(byte(ADC_ADDRESS));
  delay(1000);
  //adc.reset();
  //Wire.beginTransmission(byte(ADC_ADDRESS));
  //Wire.write(byte(0x06));
  //Wire.endTransmission();
  adc.set(byte(ADS122_REG0),byte(ADS122_MUX_IN3|ADS122_GAIN_1|ADS122_PGA_DISABLED));
  //Wire.beginTransmission(byte(ADC_ADDRESS));
  //Wire.write(byte(0x40));
  //Wire.write(byte(0xB1));
  //Wire.endTransmission();
  //Serial.println("Writing 0xB1 on register 0x00");
  delay(100);
  //Wire.beginTransmission(byte(ADC_ADDRESS));
  //Wire.write(byte(0x20));
  //Wire.endTransmission();
  //Wire.requestFrom(byte(ADC_ADDRESS),1);
  //while(Wire.available()){
//    response = Wire.read();
//    Serial.print("Response: 0x");
//    Serial.println(response,HEX);
//  }
//  delay(100);
    adc.set(byte(ADS122_REG1),byte(ADS122_DR_1000|ADS122_MODE_TURBO|ADS122_CM_SINGLE|ADS122_VREF_INTERNAL|ADS122_TS_DISABLED));
//  Wire.beginTransmission(byte(ADC_ADDRESS));
//  Wire.write(byte(0x44));
//  Wire.write(byte(0xD0));
//  Wire.endTransmission();
//  Serial.println("Writing 0x04 on register 0x01");
//  delay(100);
//  Wire.beginTransmission(byte(ADC_ADDRESS));
//  Wire.write(byte(0x24));
//  Wire.endTransmission();
//  Wire.requestFrom(byte(ADC_ADDRESS),1);
//  while(Wire.available()){
//    response = Wire.read();
//    Serial.print("Response: 0x");
//    Serial.println(response,HEX);
//  }

  delay(500);

  while(true){
    Serial.println("Done");
    delay(5000);
  }
}

byte val = 0;
byte response = 3;
Byte3 result;

void loop() {

  int exp_bytes = 3;
  double voltage = 0;
  Byte3 result;
  result.code = 0;

  //READ CHANNEL 1
  Wire.beginTransmission(byte(ADC_ADDRESS));
  Wire.write(byte(0x40));
  Wire.write(byte(0xB1));
  Wire.endTransmission();
  //delay(100);
  
  Wire.beginTransmission(byte(ADC_ADDRESS));
  Wire.write(byte(0x08));
  Wire.endTransmission();
  delay(1);

  Wire.beginTransmission(byte(ADC_ADDRESS));
  Wire.write(byte(0x10));
  Wire.endTransmission();
  Wire.requestFrom(byte(ADC_ADDRESS),exp_bytes);
  exp_bytes = exp_bytes -1;
  while(Wire.available()){
    result.bytes[exp_bytes] = Wire.read();
    exp_bytes--;
  }
  exp_bytes = 3;
  Serial.print(result.code,DEC);
  //voltage = double(result.code)/double(pow(2,24))*2.048;
  //Serial.println(voltage,DEC);

  Serial.print("\t");

  //READ CHANNEL 2
  Wire.beginTransmission(byte(ADC_ADDRESS));
  Wire.write(byte(0x40));
  Wire.write(byte(0xA1));
  Wire.endTransmission();
  //delay(100);
  
  Wire.beginTransmission(byte(ADC_ADDRESS));
  Wire.write(byte(0x08));
  Wire.endTransmission();
  delay(1);

  Wire.beginTransmission(byte(ADC_ADDRESS));
  Wire.write(byte(0x10));
  Wire.endTransmission();
  Wire.requestFrom(byte(ADC_ADDRESS),exp_bytes);
  exp_bytes = exp_bytes -1;
  while(Wire.available()){
    result.bytes[exp_bytes] = Wire.read();
    exp_bytes--;
  }

  Serial.println(result.code,DEC);
  delay(50);

}


