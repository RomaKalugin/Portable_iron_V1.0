#ifndef _SETUP_PIN_H_
#define _SETUP_PIN_H_

bool GetBtn_Left();
bool GetBtn_Right();
int GetTimer_Right_Btn();
int GetTimer_Left_Btn();
void Pin_Setup();
void Setup_PWM_Frequency(int freq_type);
void Pin_PWM(int state_PWM);
void Pin_INH(bool state_INH);
void Read_Btn_Left();
void Read_Btn_Right();
void Read_Btn();
void Check_Btn();
void Pin_Buzz(bool state_buzz);

#endif
