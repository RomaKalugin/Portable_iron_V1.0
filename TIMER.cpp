#include "TIMER.h"
#include "Arduino.h"

static unsigned long time;
static unsigned long previous_time = 0;
static uint8_t interval = 1;
static unsigned long timer = 0;

unsigned long GetTimer(){
  return timer;
}

void Timer(){
  time = millis();
  if (time - previous_time >= interval){
    previous_time = time;
    timer += 1;
  }
}
