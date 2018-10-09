// ADS122C04 single measurement channel
// by Pedro Ribeiro, INESC-MN, 2018

// Demonstrates how to program, and acquire data
// with the ADS122C04 adc with I2C communication

// Created 10 April 2018

// This example code is in the public domain.

//Required libraries
#include <Wire.h> //Controls and mediates I2C communication
#include "ADS122.h" //Allows ADS122C04 programming

#define ADC_ADDRESS 0x40 //ADS122C04 I2C address

ADS122 adc; //Define an instance of the ADS122 class as a global variable.

void setup() {
  pinMode(8,OUTPUT);
  pinMode(7,INPUT);
  digitalWrite(8,HIGH);
  Serial.begin(115200); //Initialize UART communication
  Wire.begin(); //Initialize I2C communication
  #if ADS122_DEBUG==1 
  Serial.println("Arduino up from powerdown state");
  #endif
  adc.init(byte(ADC_ADDRESS)); //Initialize the ADS122 instance, using the I2C address as input argument
  delay(1000);
  //Configure register 0. This configuration configures the ADC to measure the voltage from channel 3 as
  //a single ended input, with gain 1 and disables the internal programmable gain amplifier (PGA)
  adc.set(byte(ADS122_REG0),byte(ADS122_MUX_IN3|ADS122_GAIN_1|ADS122_PGA_DISABLED)); 
  delay(100);
  //Configure register 1. This sets the acquisition speed to 2000 SPS (2x1000 SPS, because TURBO mode is on), the
  //measurement mode to single, disables the internal temperature sensor and sets the ADC voltage reference to the internal reference (2.048 V)
  adc.set(byte(ADS122_REG1),byte(ADS122_DR_1000|ADS122_MODE_TURBO|ADS122_CM_SINGLE|ADS122_VREF_INTERNAL|ADS122_TS_DISABLED));
  delay(1000);
}

byte val = 0;
byte response = 3;
Byte3 result;

void loop() {

  Byte3 result;

  if(Serial.available()){

    while(Serial.read() > 0);

  adc.set(byte(ADS122_REG0),byte(ADS122_MUX_IN2|ADS122_GAIN_1|ADS122_PGA_DISABLED)); 

  //Order the ADC to measure.
  //ADS122::measure accepts a bool and an unsigned interger as arguments. 
  //If the first argument is false, the function will wait for the amount of miliseconds in the second argument
  //for the measurement to finish
  //If the first argument is true, the function will wait for the pin in the second argument to go LOW
  //to acknowledge the measurement is finished
  adc.measure(true, 7);

  //Order the ADC to transmit the read value.
  //This function returns a Byte3 union (defined in ADS122.h)
  result = adc.read();

  //The result is stored in the Byte3 union. To be recognized as an integer,
  //request the Byte3.code element. This element can then be printed to the 
  //serial port. 
  Serial.print((double)result.code/8388608*2.048,DEC);
  Serial.print("\t");

  adc.set(byte(ADS122_REG0),byte(ADS122_MUX_IN3|ADS122_GAIN_1|ADS122_PGA_DISABLED)); 

  adc.measure(true, 7);

  result = adc.read();

  Serial.println((double)result.code/8388608*2.048, DEC);
  }
}


