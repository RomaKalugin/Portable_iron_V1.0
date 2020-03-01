#include "Set_Temp.h"
#include "Setup_Pin.h"
#include "Arduino.h"

static double temp;
static uint16_t temper_set;  // temperature in LSB 359 = 300 degree Celsius
static uint16_t request_temper;
static uint16_t i = 0;
static uint16_t i_prev = 0;
const float a = 0.0000007;
const float b = -0.001;
const float c = 1.04;
const float d = 24.602;

int Get_Request_Temp(){
  return request_temper;
}
int Get_Temper_Set(){
  return temper_set;
}

void Set_Temp(int val_request){
  temp = a * (pow(val_request, 3)) + b * (pow(val_request, 2)) + c * val_request + d;
  temper_set = (int)temp;
}

void Request_Temper(){
  request_temper = Change_Temper(Get_Temper_Set());
}

int Read_Request_Temp(){
  Read_Btn();
  if (GetBtn_Left() == 1){
    temper_set--;
  }
  if (GetBtn_Right() == 1){
    temper_set++;
  }
  else ;
  return temper_set;
}

int Change_Temper(int temp_request){
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


/*
 * int i = 5;
 *   while(i--)
 *   if(A[i])
*/
