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

union Address{
  unsigned short value;
  byte code[2];
};

Address voltage;

unsigned int option;

ADS122 adc; //Define an instance of the ADS122 class as a global variable.

void setup() {
  Serial.begin(115200); //Initialize UART communication
  pinMode(8,OUTPUT);
  digitalWrite(8,HIGH);
  Wire.begin(); //Initialize I2C communication
  voltage.value = 0;
  #if ADS122_DEBUG==1 
  Serial.println("Arduino up from powerdown state");
  #endif
  adc.init(byte(ADC_ADDRESS)); //Initialize the ADS122 instance, using the I2C address as input argument
  delay(1000);
  //Configure register 0. This configuration configures the ADC to measure the voltage from channel 3 as
  //a single ended input, with gain 1 and disables the internal programmable gain amplifier (PGA)
  adc.set(byte(ADS122_REG0),byte(ADS122_MUX_IN0_IN1|ADS122_GAIN_16|ADS122_PGA_ENABLED)); 
  delay(100);
  //Configure register 1. This sets the acquisition speed to 2000 SPS (2x1000 SPS, because TURBO mode is on), the
  //measurement mode to single, disables the internal temperature sensor and sets the ADC voltage reference to the internal reference (2.048 V)
  adc.set(byte(ADS122_REG1),byte(ADS122_DR_20|ADS122_MODE_NORMAL|ADS122_CM_SINGLE|ADS122_VREF_EXTERNAL|ADS122_TS_DISABLED));
  delay(1000);
}

byte val = 0;
byte response = 3;
Byte3 result;

void loop() {

  Byte3 result;

  while(Serial.available() > 0){
    option = Serial.parseInt();
    switch(option){
      case 0:
        int averages;
        unsigned int tempo;
        averages = Serial.parseInt();
        if(averages != 0){
          tempo = millis();
          for(int i = 0; i < averages; i++){
            adc.measure(true,7);
            result = adc.read();
            if(bitRead(result.bytes[2],7)==1){
              result.code = result.code | 0xFF000000;
            }
            Serial.println(result.code,DEC);
          }
          Serial.println(millis() - tempo);
        }
        else{
          while(averages == 0){
            adc.measure(true,7);
            result = adc.read();
            if(bitRead(result.bytes[2],7)==1){
              result.code = result.code | 0xFF000000;
            }
            Serial.println(result.code,DEC);
            while(Serial.available() > 0){
              averages = Serial.parseInt();
            }
          }
        }
        break;
      case 1:
        int temp;
        temp = voltage.value;
        voltage.value = Serial.parseInt();
        if(voltage.value != 0){
          Wire.beginTransmission(0x4C); 
          Wire.write(byte(0x30));            // sends instruction byte  
          Wire.write(voltage.code[1]);
          Wire.write(voltage.code[0]);             // sends potentiometer value byte  
          Wire.endTransmission();     // stop transmitting
      
          Serial.print("Voltage: ");
          Serial.println((double)voltage.value/65535*1.25);
          delay(1000);
          for(int i = 0; i < 10; i++){
            adc.measure(true,7);
            result = adc.read();
            if(bitRead(result.bytes[2],7)==1){
              result.code = result.code | 0xFF000000;
            }
            Serial.println(result.code,DEC);
          }
        }
        else{
          voltage.value = temp;
        }
        break;
      default:
        Serial.println("Unrecognized");
        break;
    }
  }
}


