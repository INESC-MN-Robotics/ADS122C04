// I2C Digital Potentiometer
// by Nicholas Zambetti <http://www.zambetti.com>
// and Shawn Bonkowski <http://people.interaction-ivrea.it/s.bonkowski/>

// Demonstrates use of the Wire library
// Controls AD5171 digital potentiometer via I2C/TWI

// Created 31 March 2006

// This example code is in the public domain.

// This example code is in the public domain.


#include <Wire.h>

#define ADC_ADDRESS 0x40

union Byte3 {
  unsigned long int i;
  byte bytes[3];
};

void setup() {
  byte response = 15;
  Wire.begin(); // join i2c bus (address optional for master)
  Serial.begin(9600);
  delay(1000);
  Wire.beginTransmission(byte(ADC_ADDRESS));
  Wire.write(byte(0x06));
  Wire.endTransmission();
  Wire.beginTransmission(byte(ADC_ADDRESS));
  Wire.write(byte(0x40));
  Wire.write(byte(0xB1));
  Wire.endTransmission();
  Serial.println("Writing 0xB1 on register 0x00");
  delay(100);
  Wire.beginTransmission(byte(ADC_ADDRESS));
  Wire.write(byte(0x20));
  Wire.endTransmission();
  Wire.requestFrom(byte(ADC_ADDRESS),1);
  while(Wire.available()){
    response = Wire.read();
    Serial.print("Response: 0x");
    Serial.println(response,HEX);
  }
  delay(100);
  Wire.beginTransmission(byte(ADC_ADDRESS));
  Wire.write(byte(0x44));
  Wire.write(byte(0x00));
  Wire.endTransmission();
  Serial.println("Writing 0x04 on register 0x01");
  delay(100);
  Wire.beginTransmission(byte(ADC_ADDRESS));
  Wire.write(byte(0x24));
  Wire.endTransmission();
  Wire.requestFrom(byte(ADC_ADDRESS),1);
  while(Wire.available()){
    response = Wire.read();
    Serial.print("Response: 0x");
    Serial.println(response,HEX);
  }
}

byte val = 0;
byte response = 3;
Byte3 result;

void loop() {

  int exp_bytes = 3;
  double voltage = 0;
  Byte3 result;
  result.i = 0;
  
  Wire.beginTransmission(byte(ADC_ADDRESS));
  Wire.write(byte(0x08));
  Wire.endTransmission();
  delay(50);

  Wire.beginTransmission(byte(ADC_ADDRESS));
  Wire.write(byte(0x10));
  Wire.endTransmission();
  Wire.requestFrom(byte(ADC_ADDRESS),exp_bytes);
  exp_bytes = exp_bytes -1;
  while(Wire.available()){
    result.bytes[exp_bytes] = Wire.read();
    exp_bytes--;
  }
  
  Serial.println(result.i,DEC);
  voltage = double(result.i)/double(pow(2,24))*2.5-0.67;
  //Serial.println(voltage,DEC);
}

