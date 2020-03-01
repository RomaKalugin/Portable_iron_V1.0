#include <Wire.h>
#include "Read_Battery.h"
#include "READ_TEMPER.h"
#include "SETUP_PIN.h"
#include "Timer.h"
#include "BUZZING.h"
#include "PID.h"
#include "LCD_print.h"
#include "Set_Temp.h"
#include "Main_Menu.h"
#include "Switch_Menu.h"

//const uint16_t interval_PWM_switch = 10000; //10000; // 10s



void setup() {
  Serial.begin(115200);    
  Print_Version_Iron();
  delay(1000);
  Pin_Setup();
  Setup_PWM_Frequency(1); //0 - 15kHz(default), 1 - 1.9kHz(default), 2 - 60Hz
  Buzzing(); 
  Pin_INH(0);  //0 - off, 1 - on
  Pin_PWM(0);  //0 - off(0%), 1023 - on(100%)
}

void loop() {
  for(;;){
    Read_Input_Volt();
    Timer();
    Read_Btn();
    Main_Menu();
  }
}
