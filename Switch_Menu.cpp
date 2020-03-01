#include "Switch_Menu.h"
#include "Setup_Pin.h"
#include "Set_Temp.h"
#include "LCD_print.h"
#include "PID.h"
#include "Read_Temper.h"
#include "Arduino.h"

static uint8_t state = 0;

/* variable for temper_set*/
static uint16_t temper = 300;           // 300 degree(default)
static const uint16_t temper_max = 620; // 450 degree 
static const uint16_t temper_min = 79;  // 100 degree
static const uint16_t temp_step = 1;    // 
/********************************/

/* variable for temper_preset_set*/
static uint16_t temper_preset = 300; // 300 degree(default)
static const uint16_t temper_preset_max = 620;
static const uint16_t temper_preset_min = 79;
/********************************/

/* variables for switch_main_menu */
static unsigned long time_up = 0;
static uint16_t interval_preset_temp = 200000;
static uint16_t interval_standby_mode = 300000;
/**********************************/

/* variables for update_pid */
static unsigned long timer;
static unsigned long previous_time_pid = 0;
static const uint8_t interval_update_pid = 190;
/****************************/

/**/
static unsigned long previous_time_print_temp = 0;
static const uint16_t interval_update_print_temp = 500;
/**/

/**/
static unsigned long previous_time_set_request_temp = 0;
static const uint8_t interval_set_request_temp = 100;
/**/

/**/
static unsigned long previous_time_print_state = 0;
static const uint8_t interval_print_state = 15;
/**/

/* variable for back_to_state_0 */
static uint32_t time_up_two = 0;
/********************************/

/**/
static int16_t Temperature;
static int16_t RequestTemperature;
static int16_t PreviousTemperature = 0;
/**/

/**/
static uint32_t timer_current = 0;
/**/

/*
static unsigned long timer = 0;
static unsigned long prev_time = 0;
*/

void Switch_Main_Menu(){
  if(GetBtn_Right() == HIGH && GetBtn_Left() == LOW){
    time_up++;
    //Serial.print("Time_up   =   ");
    //Serial.println(time_up);
  }
  else if(GetBtn_Right() == LOW){
    //Serial.println(time_up);
    if (time_up < 40000 && time_up > 500){ state = 1;}
    if(interval_preset_temp > time_up && time_up > 50000){ state = 2;}
    if(interval_standby_mode > time_up && time_up > interval_preset_temp){state = 3;}
    time_up = 0;
  }
}

void Switch_Back_to_State_0(){
  if(GetBtn_Right() == HIGH && GetBtn_Left() == HIGH){
    time_up_two++;
    //Serial.print("Time_up_2Button   =   ");
    //Serial.println(time_up_two);                                             // улучшить чтение времени // привязка ко времени
  }
  else if (GetBtn_Right() == LOW && GetBtn_Left() == LOW && state != 0){
    if(time_up_two > 300){state = 0;}
    time_up_two = 0;
  }
}

void Switch_Set_Preset_Temp(){
  Set_Temp(temper);
  if(state == 3 && GetBtn_Right() == HIGH){
    temper = temper + temp_step;
    if(temper > temper_max){temper = temper_max; Set_Temp(temper); Request_Temper(); Print_Max(Get_Request_Temp()); delay(500);}
    Set_Temp(temper); 
  }
  if(state == 3 && GetBtn_Left() == HIGH){
    temper = temper - temp_step;
    if(temper < temper_min){temper = temper_min; Set_Temp(temper); Request_Temper(); Print_Min(Get_Request_Temp()); delay(500);}
    Set_Temp(temper);
  }
  Request_Temper();
  Print_Request_Temp(Get_Request_Temp());
}

void Switch_Heating_Set_Temp(){
  timer_current++;
  //Set_Temp(temper);
  if(timer_current >= 1000){
    state = 1;
    timer_current = 0;
  }
  if (state == 2 && GetBtn_Left() == HIGH && GetBtn_Right() == HIGH){
    state = 0; Pin_INH(0); Pin_PWM(0);
  }
  else if (state == 2 && GetBtn_Right() == HIGH){
    temper = temper + temp_step;
    timer_current = 0;
    if(temper > temper_max){temper = temper_max;
    Set_Temp(temper);}
  }
  else if(state == 2 && GetBtn_Left() == HIGH){
    temper = temper - temp_step;
    if(temper < temper_min){temper = temper_min;}
    Set_Temp(temper);
    timer_current = 0;
  }
  if(state == 2 && GetBtn_Right() == HIGH || GetBtn_Left() == HIGH){
    Set_Temp(temper);
    Request_Temper();
    Print_Request_Temp(Get_Request_Temp());
   }
   //Serial.print("timer_current");
   //Serial.println(timer_current);
}

void State_Heating(){
  timer = millis();
  if(timer - previous_time_pid > interval_update_pid){previous_time_pid = timer; Pin_INH(0); /*delay(10);*/ Update_PID(); Pin_INH(1); }
  Pin_PWM(GetOutput());
  Switch_Back_to_State_0();
  if(timer - previous_time_print_temp > interval_update_print_temp){previous_time_print_temp = timer;
    Set_Temp(temper);
    Request_Temper();
    Temperature = Change_Temper(Get_Temp());
    RequestTemperature = Get_Request_Temp(); 
    Print_Temp();
  }
  if(timer - previous_time_print_state >= interval_print_state){ previous_time_print_state = timer;
    if(Temperature - PreviousTemperature > 3 && Temperature - PreviousTemperature < RequestTemperature - 5){/*Print_Heating();*/}
    if(Temperature >= RequestTemperature){Print_Heat();} 
    if(Temperature - PreviousTemperature < -5 && Temperature - PreviousTemperature < RequestTemperature - 5){/*Print_Cooling();*/}
    PreviousTemperature = Temperature;
  }
  if(timer - previous_time_set_request_temp > interval_set_request_temp){
  previous_time_set_request_temp = timer;
  if(state == 1 && GetBtn_Right() == HIGH || GetBtn_Left() == HIGH){
    state = 2; Pin_INH(0);
  }
  }
}


int GetState_Menu(){
return state;
}

int GetRequestTemp(){
	return temper;
}
