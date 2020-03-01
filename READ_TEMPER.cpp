#include "READ_TEMPER.h"
#include "Arduino.h"

const float a = 0.0000007;
const float b = -0.001;
const float c = 1.04;
const float d = 6.602;
static uint16_t i = 0;
static uint16_t i_prev = 0;


/*******Temperature variable**********
static float steinhart;
#define THERMISTORNOMINAL 470//3900
#define TEMPERATURENOMINAL 20
#define NUMSAMPLES 100
#define BCOEFFICIENT 3453
#define SERIESRESISTOR 9780   // 10000
int samples[NUMSAMPLES];
uint8_t i;
static float average;
uint16_t temperature_ntc;
**************************************/

static uint16_t temperat_K;

const uint8_t ADC0 = A3;
static uint32_t val_ADC0 = 0;

static float temp;

int GetTemperature(){
  return temperat_K;
}

double Get_Temp(){
  return temp;
}

void Read_Temperature_Termocouple(){
  for(int i = 0; i <=10; i++){
  for(uint8_t i = 0; i < 20; i++){
  val_ADC0 += analogRead(ADC0);
  }
  temperat_K = (val_ADC0 / 20);
  val_ADC0 = 0;
  temp += a * (pow(temperat_K,3)) + b * (pow(temperat_K,2)) + c * temperat_K + d;
  }
  temp = temp / 10;
}

int Change_Reading_Temper(int temp_request){
  while (i < 800)
  {
     if(i_prev < temp_request && temp_request < i)
     {
        return i;
        i = 800;
     }
     i_prev = i;
     i = i + 5 ;    
  }
  i = 0;
}

/*void Read_Temperature_NTC(){
  temperature_ntc = analogRead(A1);
  for (i = 0; i < NUMSAMPLES; i++) {
  samples[i] = temperature_ntc;
  delay(10);
  }
  average = 0;
  for (i = 0; i < NUMSAMPLES; i++) {
  average += samples[i];
  }
  average /= NUMSAMPLES;
  average = 1023 / average - 1;
  average = SERIESRESISTOR / average;
  steinhart = average / THERMISTORNOMINAL; // (R/Ro)
  steinhart = log(steinhart); // ln(R/Ro)
  steinhart /= BCOEFFICIENT; // 1/B * ln(R/Ro)
  steinhart += 1.0 / (TEMPERATURENOMINAL + 273.15); // + (1/To)
  steinhart = 1.0 / steinhart;
  steinhart -= 273.15;
  //Serial.println(steinhart);
}*/
