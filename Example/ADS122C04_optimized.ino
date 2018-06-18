
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

int gain = 1; // 1, 2, 4, 8, 16, 32, 64, 128 //
bool turbo = false;
bool pga = false;
int datarate = 90;  // 20, 45, 90, 175, 330, 600, 1000 //

// ADC_CLASS.set_up(adress, channel(0-3), gain, pga, datarate, turbo);

void setup() {
  Serial.begin(115200); //Initialize UART communication
  Wire.begin(); //Initialize I2C communication
  #if ADS122_DEBUG==1 
  Serial.println("Arduino up from powerdown state");
  #endif
  //--------------------------------------------------------------------ADC-R11--------------------------------------------------------------------------
  adc_R11.set_up(ADC_R11_ADDRESS, 0, gain, pga, datarate, turbo);
  delay(4000);
  
  //--------------------------------------------------------------------ADC-L00--------------------------------------------------------------------------
  adc_L00.set_up(ADC_L00_ADDRESS, 0, gain, pga, datarate, turbo);
  delay(4000);


}

// byte val = 0;
// byte response = 3;
Byte3 result;


void loop() {

  Byte3 result;

  int delay_l = 50;
  int delay_r = 50;
  

        //Order the ADC to measure.
        //ADS122::measure accepts a bool and an unsigned interger as arguments. 
        //If the first argument is false, the function will wait for the amount of miliseconds in the second argument
        //for the measurement to finish
        //If the first argument is true, the function will wait for the pin in the second argument to go LOW
        //to acknowledge the measurement is finished
        adc_L00.measure(false, delay_l);
        
        //Order the ADC to transmit the read value.
        //This function returns a Byte3 union (defined in ADS122.h)
        result = adc_L00.read();
      
        //The result is stored in the Byte3 union. To be recognized as an integer,
        //request the Byte3.code element. This element can then be printed to the 
        //serial port. 

        Serial.print("Left CH0> ");
        Serial.println(result.code*2.048*1000/8388608,6); // mV with 6 decimal places
      
      //---------------------------------------L_00-CH1---------------------------------------
        adc_L00.set_up(ADC_L00_ADDRESS, 1, gain, pga, datarate, turbo);
       
        adc_L00.measure(false, delay_l);
        
        result = adc_L00.read();

        Serial.print("Left CH1> ");
        Serial.println(result.code*2.048*1000/8388608,6); // mV with 6 decimal places
        
      //---------------------------------------L_00-CH2---------------------------------------
        adc_L00.set_up(ADC_L00_ADDRESS, 2, gain, pga, datarate, turbo);
       
        adc_L00.measure(false, delay_l);
        
        result = adc_L00.read();

        Serial.print("Left CH2> ");
        Serial.println(result.code*2.048*1000/8388608,6); // mV with 6 decimal places
      
      
      //---------------------------------------L_00-CH3---------------------------------------
        adc_L00.set_up(ADC_L00_ADDRESS, 3, gain, pga, datarate, turbo); 
       
        adc_L00.measure(false, delay_l);
        
        result = adc_L00.read();

        Serial.print("Left CH3> ");
        Serial.println(result.code*2.048*1000/8388608,6); // mV with 6 decimal places
      
      //---------------------------------------L_00-CH0 (resart the loop)---------------------------------------
        
        adc_L00.set_up(ADC_L00_ADDRESS, 0, gain, pga, datarate, turbo);

      //-----------------------------------------------------------------------------------
      //-----------------------------------------R_11--------------------------------------
      //-----------------------------------------------------------------------------------

        adc_R11.measure(false, delay_r);
        
        result = adc_R11.read();

        Serial.print("Right CH0> ");
        Serial.println(result.code*2.048*1000/8388608,6); // mV with 6 decimal places
      
      
      //---------------------------------------R_00-CH1---------------------------------------
        adc_R11.set_up(ADC_R11_ADDRESS, 1, gain, pga, datarate, turbo);
        adc_R11.measure(false, delay_r);
        
        result = adc_R11.read();

        Serial.print("Right CH1> ");
        Serial.println(result.code*2.048*1000/8388608,6); // mV with 6 decimal places
        
        //---------------------------------------R_00-CH2---------------------------------------
        adc_R11.set_up(ADC_R11_ADDRESS, 2, gain, pga, datarate, turbo);
       
        adc_R11.measure(false, delay_r);
        
        result = adc_R11.read();

        Serial.print("Right CH2> ");
        Serial.println(result.code*2.048*1000/8388608,6); // mV with 6 decimal places
      
          //---------------------------------------R_00-CH3---------------------------------------
        adc_R11.set_up(ADC_R11_ADDRESS, 3, gain, pga, datarate, turbo);
       
        adc_R11.measure(false, delay_r);
        
        result = adc_R11.read();

        Serial.print("Right CH3> ");
        Serial.println(result.code*2.048*1000/8388608,6); // mV with 6 decimal places
      
        
      //---------------------------------------R_11-CH0 (restart the loop)---------------------------------------
        
        adc_R11.set_up(ADC_R11_ADDRESS, 0, gain, pga, datarate, turbo);
}



