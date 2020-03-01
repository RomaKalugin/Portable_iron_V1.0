#include "SETUP_PIN.h"
#include "Timer.h"
#include "Arduino.h"

static const uint8_t INH = 3;
static const uint8_t PWM_OUT = 9;
static const uint8_t LEFT_BTN = 4;
static const uint8_t RIGHT_BTN = 7;
static const uint8_t BUZZ = 5;
static unsigned long current_time;
static unsigned long previous_time = 0;
static uint8_t interval_read_button = 10;

static boolean Left_Btn;
static boolean Right_Btn;

bool GetBtn_Left(){
  return Left_Btn;
}

bool GetBtn_Right(){
  return Right_Btn;
}

void Setup_PWM_Frequency(int freq_type){
  switch(freq_type){
    case 0: 
    DDRB |= _BV(PB1) | _BV(PB2);        /* set pins as outputs */
    TCCR1A = _BV(COM1A1) | _BV(COM1B1)  /* non-inverting PWM */
        | _BV(WGM11);                   /* mode 14: fast PWM, TOP=ICR1 */
    TCCR1B = _BV(WGM13) | _BV(WGM12)
        | _BV(CS10);                    /* no prescaling 15Khz */
    OCR1A = 0;
    ICR1 = 0x03ff;                      /* TOP counter value */

  break;
    case 1: 
  DDRB |= _BV(PB1) | _BV(PB2);        /* set pins as outputs */
  TCCR1A = _BV(COM1A1) | _BV(COM1B1)  /* non-inverting PWM */
        | _BV(WGM11);                   /* mode 14: fast PWM, TOP=ICR1 */
  TCCR1B = _BV(WGM13) | _BV(WGM12)
        | _BV(CS11);                    /* prescaling  1.9kHz*/

  OCR1A = 0;
  ICR1 = 0x03ff;
  break;
    case 2: 
    DDRB |= _BV(PB1) | _BV(PB2);        /* set pins as outputs */
    TCCR1A = _BV(COM1A1) | _BV(COM1B1)  /* non-inverting PWM */
        | _BV(WGM11);                   /* mode 14: fast PWM, TOP=ICR1 */
    TCCR1B = _BV(WGM13) | _BV(WGM12)
        | _BV(CS12);                    /* prescaling 60Hz*/
  OCR1A = 0;
  ICR1 = 0x03ff;
  break;
  }
}

void Pin_Setup(){
  pinMode(INH, OUTPUT);
  pinMode(BUZZ, OUTPUT);
  pinMode(LEFT_BTN, INPUT);
  pinMode(RIGHT_BTN, INPUT);
}

void Pin_PWM(int state_PWM){
    OCR1A = state_PWM; 
}

void Pin_INH(bool state_INH){
  switch(state_INH){
    case 0: digitalWrite(INH, LOW); break;
    case 1: digitalWrite(INH, HIGH); break;
    default: break;
  }
}

void Read_Btn_Left(){
  Left_Btn = digitalRead(LEFT_BTN);
}

void Read_Btn_Right(){
  Right_Btn = digitalRead(RIGHT_BTN);
}

void Read_Btn(){
  current_time = millis();
  if (current_time - previous_time >= interval_read_button){
  previous_time = current_time;
  Read_Btn_Left();
  Read_Btn_Right();
}
}

void Check_Btn(){
  
}

void Pin_Buzz(bool state_buzz){
  switch(state_buzz){
    case 0: analogWrite(BUZZ, 0); break;
    case 1: analogWrite(BUZZ, 10); break;
    default: break;
  }
}
