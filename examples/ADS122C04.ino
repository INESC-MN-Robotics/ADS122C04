
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


// ADC_CLASS.set_up(adress, channel(0-3), gain, pga, datarate, turbo, current);
//The .set_up() function is used to define a regist in an easy way//
int gain = 1; // 1, 2, 4, 8, 16, 32, 64, 128 //
bool turbo = false;
bool pga = false;
int datarate = 90;  // 20, 45, 90, 175, 330, 600, 1000 //
int current = 100;

bool MATLAB = false; //If true, the software will only print numeric values, so its compatible with MATLAB//
                     //If false, it makes it so the channel indicators are printed aswell //



void setup() {
  Serial.begin(115200); //Initialize UART communication
  Wire.begin(); //Initialize I2C communication
  #if ADS122_DEBUG==1 
  Serial.println("Arduino up from powerdown state");
  #endif
  //--------------------------------------------------------------------ADC-R11--------------------------------------------------------------------------
  adc_R11.set_up(ADC_R11_ADDRESS, 0, gain, pga, datarate, turbo, current);
  delay(4000);
  
  //--------------------------------------------------------------------ADC-L00--------------------------------------------------------------------------
  adc_L00.set_up(ADC_L00_ADDRESS, 0, gain, pga, datarate, turbo, current);
  delay(4000);


}

// byte val = 0;
// byte response = 3;
Byte3 result;


void loop() {

  Byte3 result;

  float overload_check;

  int delay_l = delay_gen();
  int delay_r = delay_gen();
  

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
        if(MATLAB == false){
            Serial.print("LeftCH0> ");
            overload_check = result.code*2.048*1000/8388608,6;
            if(overload_check >= 2040){
              Serial.println("OVL");
            }
            else
            Serial.println(result.code*2.048*1000/8388608,6); // mV with 6 decimal places
        }
        else
            Serial.println(result.code*2.048*1000/8388608,6); // mV with 6 decimal places
      
      //---------------------------------------L_00-CH1---------------------------------------
        adc_L00.set_up(ADC_L00_ADDRESS, 1, gain, pga, datarate, turbo, current);
       
        adc_L00.measure(false, delay_l);
        
        result = adc_L00.read();
        if(MATLAB == false){
            Serial.print("LeftCH1> ");
            overload_check = result.code*2.048*1000/8388608,6;
            if(overload_check >= 2040){
              Serial.println("OVL");
            }
            else
            Serial.println(result.code*2.048*1000/8388608,6); // mV with 6 decimal places
        }
        else
            Serial.println(result.code*2.048*1000/8388608,6); // mV with 6 decimal places
        
      //---------------------------------------L_00-CH2---------------------------------------
        adc_L00.set_up(ADC_L00_ADDRESS, 2, gain, pga, datarate, turbo, current);
       
        adc_L00.measure(false, delay_l);
        
        result = adc_L00.read();
        
        if(MATLAB == false){
          Serial.print("LeftCH2> ");
            overload_check = result.code*2.048*1000/8388608,6;
            if(overload_check >= 2040){
              Serial.println("OVL");
            }
            else
            Serial.println(result.code*2.048*1000/8388608,6); // mV with 6 decimal places
        }
        else
            Serial.println(result.code*2.048*1000/8388608,6); // mV with 6 decimal places
      
      //---------------------------------------L_00-CH3---------------------------------------
        adc_L00.set_up(ADC_L00_ADDRESS, 3, gain, pga, datarate, turbo, current); 
       
        adc_L00.measure(false, delay_l);
        
        result = adc_L00.read();

        if(MATLAB == false){
            Serial.print("LeftCH3> ");
            overload_check = result.code*2.048*1000/8388608,6;
            if(overload_check >= 2040){
              Serial.println("OVL");
            }
            else
            Serial.println(result.code*2.048*1000/8388608,6); // mV with 6 decimal places
        }
        else
            Serial.println(result.code*2.048*1000/8388608,6); // mV with 6 decimal places
      
      //---------------------------------------L_00-CH0 (resart the loop)---------------------------------------
        
        adc_L00.set_up(ADC_L00_ADDRESS, 0, gain, pga, datarate, turbo, current);

      //-----------------------------------------------------------------------------------
      //-----------------------------------------R_11--------------------------------------
      //-----------------------------------------------------------------------------------

        adc_R11.measure(false, delay_r);
        
        result = adc_R11.read();
        if(MATLAB == false){
            Serial.print("RightCH0> ");
            overload_check = result.code*2.048*1000/8388608,6;
            if(overload_check >= 2040){
              Serial.println("OVL");
            }
            else
            Serial.println(result.code*2.048*1000/8388608,6); // mV with 6 decimal places
        }
        else
            Serial.println(result.code*2.048*1000/8388608,6); // mV with 6 decimal places
      
      
      //---------------------------------------R_00-CH1---------------------------------------
        adc_R11.set_up(ADC_R11_ADDRESS, 1, gain, pga, datarate, turbo, current);
        adc_R11.measure(false, delay_r);
        
        result = adc_R11.read();

        if(MATLAB == false){
            Serial.print("RightCH1> ");
            overload_check = result.code*2.048*1000/8388608,6;
            if(overload_check >= 2040){
              Serial.println("OVL");
            }
            else
            Serial.println(result.code*2.048*1000/8388608,6); // mV with 6 decimal places
        }
        else
            Serial.println(result.code*2.048*1000/8388608,6); // mV with 6 decimal places
        
        //---------------------------------------R_00-CH2---------------------------------------
        adc_R11.set_up(ADC_R11_ADDRESS, 2, gain, pga, datarate, turbo, current);
       
        adc_R11.measure(false, delay_r);
        
        result = adc_R11.read();
       
        if(MATLAB == false){
            Serial.print("RightCH2> ");
            overload_check = result.code*2.048*1000/8388608,6;
            if(overload_check >= 2040){
              Serial.println("OVL");
            }
            else
            Serial.println(result.code*2.048*1000/8388608,6); // mV with 6 decimal places
        }
        else
            Serial.println(result.code*2.048*1000/8388608,6); // mV with 6 decimal places
      
          //---------------------------------------R_00-CH3---------------------------------------
        adc_R11.set_up(ADC_R11_ADDRESS, 3, gain, pga, datarate, turbo, current);
       
        adc_R11.measure(false, delay_r);
        
        result = adc_R11.read();
        
        if(MATLAB == false){
            Serial.print("RightCH3> ");
            overload_check = result.code*2.048*1000/8388608,6;
            if(overload_check >= 2040){
              Serial.println("OVL");
            }
            else
            Serial.println(result.code*2.048*1000/8388608,6); // mV with 6 decimal places
        }
        else
            Serial.println(result.code*2.048*1000/8388608,6); // mV with 6 decimal places
      
        
      //---------------------------------------R_11-CH0 (restart the loop)---------------------------------------
        
        adc_R11.set_up(ADC_R11_ADDRESS, 0, gain, pga, datarate, turbo, current);
}

int delay_gen(){
  int delay_value = 0;


   // IF TURBO, 2000, IF NOT, 1000 / Nº SENSORS aka 4 = REAL SPS ( 1/SPS) IF NOT WORK*2)
  if(turbo == true){
    switch(datarate){
         case 1000: delay_value = 1; break;
         case 600: delay_value = 1; break;
         case 330: delay_value = 2; break;
         case 175: delay_value = 3; break;
         case 90: delay_value = 8; break;
         case 45: delay_value = 20; break;
         case 20: delay_value = 50; break;
     }
  }
  else{
    switch(datarate){
    case 1000: delay_value = 2; break;
         case 600: delay_value = 2; break;
         case 330: delay_value = 4; break;
         case 175: delay_value = 8; break;
         case 90: delay_value = 20; break;
         case 45: delay_value = 50; break;
         case 20: delay_value = 100; break;
    }
  }
  return delay_value;
}


