#include "Buzzing.h"
#include "Timer.h"
#include "SETUP_PIN.h"
#include "Arduino.h"

static unsigned long time;
static const uint8_t interval_Buzz = 10;

void Buzzing(){
    time = GetTimer();
    if(time < interval_Buzz){
      Pin_Buzz(1);
    }
    else 
    {
      Pin_Buzz(0);  
    }
}
