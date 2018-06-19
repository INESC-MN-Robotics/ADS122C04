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

void setup() {
  Serial.begin(115200); //Initialize UART communication
  Wire.begin(); //Initialize I2C communication
  #if ADS122_DEBUG==1 
  Serial.println("Arduino up from powerdown state");
  #endif
  //--------------------------------------------------------------------ADC-R11--------------------------------------------------------------------------
  adc_R11.init(byte(ADC_R11_ADDRESS)); //Initialize the ADS122 instance, using the I2C address as input argument
  delay(1000);
  
  //Configure register 0. This configuration configures the ADC to measure the voltage from a channel as
  //a single ended input, with gain 1 and disables the internal programmable gain amplifier (PGA)
  adc_R11.set(byte(ADS122_REG0),byte(ADS122_MUX_IN0|ADS122_GAIN_1|ADS122_PGA_DISABLED)); 
  delay(100);
  
  //Configure register 1. This sets the acquisition speed to 40 SPS (2x20 SPS, because TURBO mode is on), the
  //measurement mode to single, disables the internal temperature sensor and sets the ADC voltage reference to the internal reference (2.048 V)
  adc_R11.set(byte(ADS122_REG1),byte(ADS122_DR_90|ADS122_MODE_NORMAL|ADS122_CM_SINGLE|ADS122_VREF_INTERNAL|ADS122_TS_DISABLED));
  delay(1000);
  
  //Configure register 2. This sets the current to 100 µA
  adc_R11.set(byte(ADS122_REG2),byte(ADS122_DRDY_MEASURING|ADS122_DCNT_DISABLED|ADS122_CRC_DISABLED|ADS122_BURNOUT_DISABLED|ADS122_IDAC_100));
  delay(1000);
 
  //Configure register 3. This chooses the current source to use.
  adc_R11.set(byte(ADS122_REG3),byte(ADS122_I1MUX_AIN0|ADS122_I2MUX_DISABLED));
  delay(1000);
  
  //--------------------------------------------------------------------ADC-L00--------------------------------------------------------------------------
  adc_L00.init(byte(ADC_L00_ADDRESS)); //Initialize the ADS122 instance, using the I2C address as input argument
  delay(1000);
  
  //Configure register 0. This configuration configures the ADC to measure the voltage from a channel as
  //a single ended input, with gain 1 and disables the internal programmable gain amplifier (PGA)
  adc_L00.set(byte(ADS122_REG0),byte(ADS122_MUX_IN0|ADS122_GAIN_1|ADS122_PGA_DISABLED)); 
  delay(100);
  
  //Configure register 1. This sets the acquisition speed to 40 SPS (2x20 SPS, because TURBO mode is on), the
  //measurement mode to single, disables the internal temperature sensor and sets the ADC voltage reference to the internal reference (2.048 V)
  adc_L00.set(byte(ADS122_REG1),byte(ADS122_DR_90|ADS122_MODE_NORMAL|ADS122_CM_SINGLE|ADS122_VREF_INTERNAL|ADS122_TS_DISABLED));
  delay(1000);
  
  //Configure register 2. This sets the current to 100 µA
  adc_L00.set(byte(ADS122_REG2),byte(ADS122_DRDY_MEASURING|ADS122_DCNT_DISABLED|ADS122_CRC_DISABLED|ADS122_BURNOUT_DISABLED|ADS122_IDAC_100));
  delay(1000);
 
  //Configure register 3. This chooses the current source to use.
  adc_L00.set(byte(ADS122_REG3),byte(ADS122_I1MUX_AIN0|ADS122_I2MUX_DISABLED));
  delay(1000);



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
        Serial.println(result.code*3.3*1000/16777216,6); // mV with 6 decimal places
      
      //---------------------------------------L_00-CH1---------------------------------------
        adc_L00.init(byte(ADC_L00_ADDRESS));
        delay(10);
        adc_L00.set(byte(ADS122_REG0),byte(ADS122_MUX_IN1|ADS122_GAIN_1|ADS122_PGA_DISABLED)); 
        delay(10);
        adc_L00.set(byte(ADS122_REG1),byte(ADS122_DR_90|ADS122_MODE_NORMAL|ADS122_CM_SINGLE|ADS122_VREF_INTERNAL|ADS122_TS_DISABLED));
        delay(10);
        adc_L00.set(byte(ADS122_REG2),byte(ADS122_DRDY_MEASURING|ADS122_DCNT_DISABLED|ADS122_CRC_DISABLED|ADS122_BURNOUT_DISABLED|ADS122_IDAC_100));
        delay(10);
        adc_L00.set(byte(ADS122_REG3),byte(ADS122_I1MUX_AIN1|ADS122_I2MUX_DISABLED));
        delay(10);  
       
        adc_L00.measure(false, delay_l);
        
        result = adc_L00.read();

        Serial.print("Left CH1> ");
        Serial.println(result.code*3.3*1000/16777216,6); // mV with 6 decimal places
        
      //---------------------------------------L_00-CH2---------------------------------------
        adc_L00.init(byte(ADC_L00_ADDRESS));
        delay(10);
        adc_L00.set(byte(ADS122_REG0),byte(ADS122_MUX_IN2|ADS122_GAIN_1|ADS122_PGA_DISABLED)); 
        delay(10);
        adc_L00.set(byte(ADS122_REG1),byte(ADS122_DR_90|ADS122_MODE_NORMAL|ADS122_CM_SINGLE|ADS122_VREF_INTERNAL|ADS122_TS_DISABLED));
        delay(10);
        adc_L00.set(byte(ADS122_REG2),byte(ADS122_DRDY_MEASURING|ADS122_DCNT_DISABLED|ADS122_CRC_DISABLED|ADS122_BURNOUT_DISABLED|ADS122_IDAC_100));
        delay(10);
        adc_L00.set(byte(ADS122_REG3),byte(ADS122_I1MUX_AIN2|ADS122_I2MUX_DISABLED));
        delay(10);  
       
        adc_L00.measure(false, delay_l);
        
        result = adc_L00.read();

        Serial.print("Left CH2> ");
        Serial.println(result.code*3.3*1000/16777216,6); // mV with 6 decimal places
      
      
      //---------------------------------------L_00-CH3---------------------------------------
        adc_L00.init(byte(ADC_L00_ADDRESS));
        delay(10);
        adc_L00.set(byte(ADS122_REG0),byte(ADS122_MUX_IN3|ADS122_GAIN_1|ADS122_PGA_DISABLED)); 
        delay(10);
        adc_L00.set(byte(ADS122_REG1),byte(ADS122_DR_90|ADS122_MODE_NORMAL|ADS122_CM_SINGLE|ADS122_VREF_INTERNAL|ADS122_TS_DISABLED));
        delay(10);
        adc_L00.set(byte(ADS122_REG2),byte(ADS122_DRDY_MEASURING|ADS122_DCNT_DISABLED|ADS122_CRC_DISABLED|ADS122_BURNOUT_DISABLED|ADS122_IDAC_100));
        delay(10);
        adc_L00.set(byte(ADS122_REG3),byte(ADS122_I1MUX_AIN3|ADS122_I2MUX_DISABLED));
        delay(10);  
       
        adc_L00.measure(false, delay_l);
        
        result = adc_L00.read();

        Serial.print("Left CH3> ");
        Serial.println(result.code*3.3*1000/16777216,6); // mV with 6 decimal places
      
      //---------------------------------------L_00-CH0 (resart the loop)---------------------------------------
        
        adc_L00.init(byte(ADC_L00_ADDRESS));
        delay(10);
        adc_L00.set(byte(ADS122_REG0),byte(ADS122_MUX_IN0|ADS122_GAIN_1|ADS122_PGA_DISABLED)); 
        delay(10);
        adc_L00.set(byte(ADS122_REG1),byte(ADS122_DR_90|ADS122_MODE_NORMAL|ADS122_CM_SINGLE|ADS122_VREF_INTERNAL|ADS122_TS_DISABLED));
        delay(10);
        adc_L00.set(byte(ADS122_REG2),byte(ADS122_DRDY_MEASURING|ADS122_DCNT_DISABLED|ADS122_CRC_DISABLED|ADS122_BURNOUT_DISABLED|ADS122_IDAC_100));
        delay(10);
        adc_L00.set(byte(ADS122_REG3),byte(ADS122_I1MUX_AIN0|ADS122_I2MUX_DISABLED));
        delay(10);

      //-----------------------------------------------------------------------------------
      //-----------------------------------------R_11--------------------------------------
      //-----------------------------------------------------------------------------------
         
        adc_R11.measure(false, delay_r);
        
        result = adc_R11.read();

        Serial.print("Right CH0> ");
        Serial.println(result.code*3.3*1000/16777216,6); // mV with 6 decimal places
      
      
      //---------------------------------------R_00-CH1---------------------------------------
        adc_R11.init(byte(ADC_R11_ADDRESS));
        delay(10);
        adc_R11.set(byte(ADS122_REG0),byte(ADS122_MUX_IN1|ADS122_GAIN_1|ADS122_PGA_DISABLED)); 
        delay(10);
        adc_R11.set(byte(ADS122_REG1),byte(ADS122_DR_90|ADS122_MODE_NORMAL|ADS122_CM_SINGLE|ADS122_VREF_INTERNAL|ADS122_TS_DISABLED));
        delay(10);
        adc_R11.set(byte(ADS122_REG2),byte(ADS122_DRDY_MEASURING|ADS122_DCNT_DISABLED|ADS122_CRC_DISABLED|ADS122_BURNOUT_DISABLED|ADS122_IDAC_100));
        delay(10);
        adc_R11.set(byte(ADS122_REG3),byte(ADS122_I1MUX_AIN1|ADS122_I2MUX_DISABLED));
        delay(10);  
       
        adc_R11.measure(false, delay_r);
        
        result = adc_R11.read();

        Serial.print("Right CH1> ");
        Serial.println(result.code*3.3*1000/16777216,6); // mV with 6 decimal places
        
        //---------------------------------------R_00-CH2---------------------------------------
        adc_R11.init(byte(ADC_R11_ADDRESS));
        delay(10);
        adc_R11.set(byte(ADS122_REG0),byte(ADS122_MUX_IN2|ADS122_GAIN_1|ADS122_PGA_DISABLED)); 
        delay(10);
        adc_R11.set(byte(ADS122_REG1),byte(ADS122_DR_90|ADS122_MODE_NORMAL|ADS122_CM_SINGLE|ADS122_VREF_INTERNAL|ADS122_TS_DISABLED));
        delay(10);
        adc_R11.set(byte(ADS122_REG2),byte(ADS122_DRDY_MEASURING|ADS122_DCNT_DISABLED|ADS122_CRC_DISABLED|ADS122_BURNOUT_DISABLED|ADS122_IDAC_100));
        delay(10);
        adc_R11.set(byte(ADS122_REG3),byte(ADS122_I1MUX_AIN2|ADS122_I2MUX_DISABLED));
        delay(10);  
       
        adc_R11.measure(false, delay_r);
        
        result = adc_R11.read();

        Serial.print("Right CH2> ");
        Serial.println(result.code*3.3*1000/16777216,6); // mV with 6 decimal places
      
          //---------------------------------------R_00-CH3---------------------------------------
        adc_R11.init(byte(ADC_R11_ADDRESS));
        delay(10);
        adc_R11.set(byte(ADS122_REG0),byte(ADS122_MUX_IN3|ADS122_GAIN_1|ADS122_PGA_DISABLED)); 
        delay(10);
        adc_R11.set(byte(ADS122_REG1),byte(ADS122_DR_90|ADS122_MODE_NORMAL|ADS122_CM_SINGLE|ADS122_VREF_INTERNAL|ADS122_TS_DISABLED));
        delay(10);
        adc_R11.set(byte(ADS122_REG2),byte(ADS122_DRDY_MEASURING|ADS122_DCNT_DISABLED|ADS122_CRC_DISABLED|ADS122_BURNOUT_DISABLED|ADS122_IDAC_100));
        delay(10);
        adc_R11.set(byte(ADS122_REG3),byte(ADS122_I1MUX_AIN3|ADS122_I2MUX_DISABLED));
        delay(10);  
       
        adc_R11.measure(false, delay_r);
        
        result = adc_R11.read();

        Serial.print("Right CH3> ");
        Serial.println(result.code*3.3*1000/16777216,6); // mV with 6 decimal places
      
        
      //---------------------------------------R_11-CH0 (restart the loop)---------------------------------------
        
        adc_R11.init(byte(ADC_R11_ADDRESS));
        delay(10);
        adc_R11.set(byte(ADS122_REG0),byte(ADS122_MUX_IN0|ADS122_GAIN_1|ADS122_PGA_DISABLED)); 
        delay(10);
        adc_R11.set(byte(ADS122_REG1),byte(ADS122_DR_90|ADS122_MODE_NORMAL|ADS122_CM_SINGLE|ADS122_VREF_INTERNAL|ADS122_TS_DISABLED));
        delay(10);
        adc_R11.set(byte(ADS122_REG2),byte(ADS122_DRDY_MEASURING|ADS122_DCNT_DISABLED|ADS122_CRC_DISABLED|ADS122_BURNOUT_DISABLED|ADS122_IDAC_100));
        delay(10);
        adc_R11.set(byte(ADS122_REG3),byte(ADS122_I1MUX_AIN0|ADS122_I2MUX_DISABLED));
        delay(10);
}



