
// ADS122C04 single measurement channel
// by Pedro Ribeiro, INESC-MN, 2018

// Demonstrates how to program, and acquire data
// with the ADS122C04 adc with I2C communication

// Created 10 April 2018

// This example code is in the public domain.

//Required libraries
#include <Wire.h> //Controls and mediates I2C communication
#include "ADS122.h" //Allows ADS122C04 programming


#define ADC_L00_ADDRESS 0x40 //ADS122C04 I2C address for L-00 PCB
#define ADC_R11_ADDRESS 0x45 //ADS122C04 I2C address for R-11 PCB


ADS122 adc_L00,adc_R11 ; //Define an instance of the ADS122 class as a global variable.

int gain = 4; // 1, 2, 4, 8, 16, 32, 64, 128 //
bool turbo = true;
bool pga = false;
int datarate = 1000;  // 20, 45, 90, 175, 330, 600, 1000 //

// ADC_CLASS.set_up(adress, channel(0-3), gain, pga, datarate, turbo);

long real_rate;
bool real_rate_display = false;
unsigned long minutes = 300000;
int time_r = minutes;
int timer = time_r / 1000;


void setup() {
  Serial.begin(2000000); //Initialize UART communication
  Wire.begin(); //Initialize I2C communication
  #if ADS122_DEBUG==1 
  Serial.println("Arduino up from powerdown state");
  #endif
  adc_L00.set_up(ADC_L00_ADDRESS, 0, gain, pga, datarate, turbo);
  real_rate = 0;
  Serial.println("START");
}


// byte val = 0;
// byte response = 3;
Byte3 result;


void loop() {


  if(millis() < minutes){
    Byte3 result;
    int delay_l = 0;
  
  
          adc_L00.measure(false, delay_l);
          real_rate++;
          
          //Order the ADC to transmit the read value.
          //This function returns a Byte3 union (defined in ADS122.h)
          result = adc_L00.read();
        
          //The result is stored in the Byte3 union. To be recognized as an integer,
          //request the Byte3.code element. This element can then be printed to the 
          //serial port. 
          Serial.println(result.code*2.048*1000/8388608,6); // mV with 6 decimal places
          //Serial.println(millis());
  }
  else{
    if(real_rate_display == false){
          Serial.print("Sample-Rate> ");
          Serial.println(real_rate/timer);
          real_rate_display = true;
    }
  }

}



