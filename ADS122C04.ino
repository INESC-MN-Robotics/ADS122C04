// ADS122C04 single measurement channel
// by Pedro Ribeiro, INESC-MN, 2018

// Demonstrates how to program, and acquire data
// with the ADS122C04 adc with I2C communication

// Created 10 April 2018

// This example code is in the public domain.

//Required libraries
#include <Wire.h> //Controls and mediates I2C communication
#include "ADS122.h" //Allows ADS122C04 programming
#include "MAX5802.h" //Allows MAX582 programming

#define ADC_ADDRESS 0x40 //ADS122C04 I2C address
#define DAC_ADDRESS 0x0F //MAX5802 I2C address

#define DRD_PIN 7

union Address{
  unsigned short value;
  byte code[2];
};

int voltage1, voltage2;

unsigned int option;

ADS122 adc; //Define an instance of the ADS122 class as a global variable.
MAX5802 dac;

void setup() {
  Serial.begin(115200); //Initialize UART communication
  pinMode(8,OUTPUT);
  digitalWrite(8,HIGH);
  pinMode(DRD_PIN, INPUT);
  Wire.begin(); //Initialize I2C communication
  #if ADS122_DEBUG==1 
  Serial.println("Arduino up from powerdown state");
  #endif
  adc.init(byte(ADC_ADDRESS)); //Initialize the ADS122 instance, using the I2C address as input argument
  delay(1000);
  dac.init(byte(DAC_ADDRESS));
  delay(1000);
  //Configure register 0. This configuration configures the ADC to measure the voltage from channel 3 as
  //a single ended input, with gain 1 and disables the internal programmable gain amplifier (PGA)
  adc.set(byte(ADS122_REG0),byte(ADS122_MUX_IN1_IN0|ADS122_GAIN_1|ADS122_PGA_ENABLED)); 
  delay(100);
  //Configure register 1. This sets the acquisition speed to 2000 SPS (2x1000 SPS, because TURBO mode is on), the
  //measurement mode to single, disables the internal temperature sensor and sets the ADC voltage reference to the internal reference (2.048 V)
  adc.set(byte(ADS122_REG1),byte(ADS122_DR_20|ADS122_MODE_NORMAL|ADS122_CM_SINGLE|ADS122_VREF_INTERNAL|ADS122_TS_DISABLED));
  delay(1000);
  adc.powerdown();
  //attachInterrupt(digitalPinToInterrupt(DRD_PIN), show_result,LOW);
}

byte val = 0;
byte response = 3;
bool burst = false;
int measures = 0;
Byte3 result;

void loop() {

  Byte3 result;

  while(Serial.available() > 0){
    option = Serial.parseInt();
    switch(option){
      case 0:
        if(burst)
          break;
        int averages;
        unsigned int tempo;
        averages = Serial.parseInt();
        if(averages != 0){
          tempo = millis();
          for(int i = 0; i < averages; i++){
            adc.measure(true ,7);
            result = adc.read();  
            if(bitRead(result.bytes[2],7)==1){
              result.code = result.code | 0xFF000000;
            }
            Serial.println(result.code,DEC);
          }
          //Serial.println(millis() - tempo);
        }
        else{
          while(averages == 0){
            adc.measure(true ,7);
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
        //if(burst)
        //  break;
        int temp;
        voltage2 = Serial.parseInt();
        voltage1 = Serial.parseInt();
        
        Serial.println(voltage2, DEC);
        Serial.print("\t");
        Serial.print(voltage1, DEC);
        //delay(1000);

        if(voltage1 > 0 && voltage2 == 0){
          dac.writeDAC(voltage1,ALL_DACS);
        }
        else if(voltage1 > 0 && voltage2 > 0){
          dac.writeDAC(voltage1,DAC_A);
          dac.writeDAC(voltage2,DAC_B);
        }
        else if(voltage1 < 0 && voltage2 > 0){
          dac.writeDAC(voltage2,DAC_B);
        }
        else if(voltage1 > 0 && voltage2 < 0){
          dac.writeDAC(voltage1,DAC_A);
        }
        else{
          Serial.println("Unknown DAC configuration - ignored");
          delay(1000);
        }
        break;
      case 2:
        if(!burst){
          adc.set(byte(ADS122_REG1),byte(ADS122_DR_20|ADS122_MODE_TURBO|ADS122_CM_CONTINUOUS|ADS122_VREF_EXTERNAL|ADS122_TS_DISABLED));
          burst = true;
          adc.measure(false, 1);
          while(Serial.available() > 0)
            Serial.read();
        }
        else{
          adc.powerdown();
          //adc.set(byte(ADS122_REG1),byte(ADS122_DR_1000|ADS122_MODE_NORMAL|ADS122_CM_CONTINUOUS|ADS122_VREF_INTERNAL|ADS122_TS_DISABLED));
          //adc.measure(true ,DRD_PIN);
          while(Serial.available() > 0)
            Serial.read();
          burst = false;
        }
        break;
      default:
        Serial.println("Unrecognized");
        break;
    }  
  }
  if(burst)
    readBurst(byte(ADS122_MUX_IN1_IN0|ADS122_GAIN_1|ADS122_PGA_ENABLED), byte(ADS122_MUX_IN2_IN3|ADS122_GAIN_1|ADS122_PGA_ENABLED), DRD_PIN);
}

void readBurst(byte first, byte second, int drdpin){
  Byte3 result_x, result_y;
  
  adc.set(ADS122_REG0, first);
  while(digitalRead(drdpin) == true);
  result_x = adc.read();
  adc.set(ADS122_REG0, second);
  while(digitalRead(drdpin) == true);  
  result_y = adc.read();
  
  if(bitRead(result_x.bytes[2],7)==1){
    result_x.code = result_x.code | 0xFF000000;
  }
  Serial.print(result_x.code,DEC);
  Serial.print("\t");
  if(bitRead(result_y.bytes[2],7)==1){
    result_y.code = result_y.code | 0xFF000000;
  }
  Serial.println(result_y.code, DEC);
}

void readBurst(int drdpin){
  Byte3 result;
  
  while(digitalRead(drdpin) == true);
  result = adc.read();
  if(bitRead(result.bytes[2],7)==1){
    result.code = result.code | 0xFF000000;
  }
  Serial.println(result.code,DEC);
}
