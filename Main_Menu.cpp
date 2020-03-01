#include "Main_Menu.h"
#include "Switch_Menu.h"
#include "Setup_Pin.h"
#include "Timer.h"
#include "PID.h"
#include "LCD_Print.h"
#include "Arduino.h"

/*Function: Switch menu item*/
void Main_Menu(){
  switch(GetState_Menu()){
    case 0: Standby_LCD(); Switch_Main_Menu(); Pin_INH(0); Pin_PWM(0); break;  // State menu 0, iron wait, when user pressed button
    case 1: State_Heating(); /*Switch_Back_to_State_0();*/ break;              // State menu 1, iron heating, and print reading temperature. Hold set temperature(use PID)
    case 2: Switch_Heating_Set_Temp(); Switch_Back_to_State_0(); break;        // State menu 2, changing heat temperature
    case 3: Switch_Set_Preset_Temp(); Switch_Back_to_State_0(); break;         // State menu 3, changing preset temperature
  }
}
/****************************/
