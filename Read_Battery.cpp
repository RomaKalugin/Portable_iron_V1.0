#include "Read_Battery.h"
#include "Arduino.h"

const uint8_t read_input_volt = 2;
static unsigned long input_voltage;

void Read_Input_Volt(){
  input_voltage = 0;
  for(uint8_t i = 0; i <= 10; i++){
   input_voltage += analogRead(read_input_volt);
   //Serial.println(input_voltage);
   //Serial.println(i);
   //delay(10);
    }
    input_voltage = (input_voltage / 11);
    //Serial.println(input_voltage);
    input_voltage = input_voltage * 4700; 
    //Serial.println(input_voltage);
    input_voltage = (input_voltage / 1023);
    //Serial.println(analogRead(read_input_volt));
    //Serial.print(input_voltage);
    //Serial.println(" mV");
}

void Check_Input_Voltage(){
  
}

int GetInputVolt(){
  return input_voltage;
}
