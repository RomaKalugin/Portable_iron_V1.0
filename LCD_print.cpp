#include "LCD_print.h"
#include "Read_Temper.h"
#include "Set_Temp.h"
#include "Timer.h"
#include "Switch_Menu.h"
#include "Arduino.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const float iron_version = 1.00;

static unsigned long timer_scroll;
static unsigned long previous_timer_scroll = 0;

static unsigned long current_time;
static unsigned long previous_time = 0;
const uint16_t interval_blinking = 500;
static uint8_t state_standby = 0; 

static const unsigned char PROGMEM unpress_logo[] =
{ 
B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000011, B11100000, B01111100, B00000000, B00000000, B00000000, B00000000, B00000000,
B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000011, B11100000, B01111100, B00000000, B00000000, B00000000, B00000000, B00000000,
B00000000, B00000000, B00000000, B00000000, B00000000, B00110000, B00000000, B00001100, B01111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B10000000,
B00000000, B00000000, B00000000, B00000000, B00000000, B00110000, B00000000, B00001100, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11000000,
B00000000, B00000000, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B11000000,
B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B11111111, 
B00111111, B11111111, B10000000, B00000000, B00000000, B00110000, B00000000, B00001100, B11000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B11111111, 
B00001111, B11111111, B10000000, B00000000, B00000000, B00110000, B00000000, B00001100, B11000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B11111111,
B00000011, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B11000000,
B00000000, B00000000, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B11000000,
B00000000, B00000000, B00000000, B00000000, B00000000, B00110000, B00000000, B00001100, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11000000,
B00000000, B00000000, B00000000, B00000000, B00000000, B00110000, B00000000, B00001100, B01111111, B11111111, B11111111, B11111111, B11111111, B11111111, B11111111, B10000000,
B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000,
B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000, B00000000
};

static const unsigned char PROGMEM heating[]=
{
  B00000010,B00000000,
  B00000111,B00000000,
  B00001111,B10000000,
  B00011101,B11000000,
  B00111000,B11100000,
  B01110000,B01110000,
  B11100000,B00111000
};

static const unsigned char PROGMEM cooling[]=
{
  B11100000,B00111000,
  B01110000,B01110000,
  B00111000,B11100000,
  B00011101,B11000000,
  B00001111,B10000000,
  B00000111,B00000000,
  B00000010,B00000000,
};

static const unsigned char PROGMEM heat[]=
{
  B00100000,B01000000,
  B01110000,B11100000,
  B11011001,B10110000,
  B10001111,B00010000,
  B10000110,B00010000
};

static const unsigned char PROGMEM iron[]=
{
  B00000010,
  B00000110,
  B00001110,
  B00011110,
  B00110110,
  B01100110,
  B01100110,
  B01100110,
  B01100110,
  B01100110,
  B11111111,
  B11111111,
  B11000011,
  B11000011,
  B11000011,
  B11000011,
  B11000011,
  B11000011,
  B11000011,
  B11000011,
  B11000011,
  B11000011,
  B11000011,
  B11000011,
  B11111111,
  B11111111,
};

static const unsigned char PROGMEM clear_rectangle[]=
{
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000,
  B00000000
};

uint8_t i = 10;
uint8_t l = 114;
uint8_t n = 0;


/*  
void testdrawline() {
  int16_t i;
 
  display.clearDisplay(); // Clear display buffer

  for(i=0; i<display.width(); i+=4) {
    display.drawLine(0, 0, i, display.height()-1, SSD1306_WHITE);
    display.display(); // Update screen with each newly-drawn line
    delay(1);
  }
  for(i=0; i<display.height(); i+=4) {
    display.drawLine(0, 0, display.width()-1, i, SSD1306_WHITE);
    display.display();
    delay(1);
  }
  delay(250);

  display.clearDisplay();

  for(i=0; i<display.width(); i+=4) {
    display.drawLine(0, display.height()-1, i, 0, SSD1306_WHITE);
    display.display();
    delay(1);
  }
  for(i=display.height()-1; i>=0; i-=4) {
    display.drawLine(0, display.height()-1, display.width()-1, i, SSD1306_WHITE);
    display.display();
    delay(1);
  }
  delay(250);

  display.clearDisplay();

  for(i=display.width()-1; i>=0; i-=4) {
    display.drawLine(display.width()-1, display.height()-1, i, 0, SSD1306_WHITE);
    display.display();
    delay(1);
  }
  for(i=display.height()-1; i>=0; i-=4) {
    display.drawLine(display.width()-1, display.height()-1, 0, i, SSD1306_WHITE);
    display.display();
    delay(1);
  }
  delay(250);

  display.clearDisplay();

  for(i=0; i<display.height(); i+=4) {
    display.drawLine(display.width()-1, 0, 0, i, SSD1306_WHITE);
    display.display();
    delay(1);
  }
  for(i=0; i<display.width(); i+=4) {
    display.drawLine(display.width()-1, 0, i, display.height()-1, SSD1306_WHITE);
    display.display();
    delay(1);
  }

  delay(2000); // Pause for 2 seconds
}

void testdrawrect(void) {
  display.clearDisplay();

  for(int16_t i=0; i<display.height()/2; i+=2) {
    display.drawRect(i, i, display.width()-2*i, display.height()-2*i, SSD1306_WHITE);
    display.display(); // Update screen with each newly-drawn rectangle
    delay(1);
  }

  delay(2000);
}

void testfillrect(void) {
  display.clearDisplay();

  for(int16_t i=0; i<display.height()/2; i+=3) {
    // The INVERSE color is used so rectangles alternate white/black
    display.fillRect(i, i, display.width()-i*2, display.height()-i*2, SSD1306_INVERSE);
    display.display(); // Update screen with each newly-drawn rectangle
    delay(1);
  }

  delay(2000);
}

void testdrawcircle(void) {
  display.clearDisplay();

  for(int16_t i=0; i<max(display.width(),display.height())/2; i+=2) {
    display.drawCircle(display.width()/2, display.height()/2, i, SSD1306_WHITE);
    display.display();
    delay(1);
  }

  delay(2000);
}

void testfillcircle(void) {
  display.clearDisplay();

  for(int16_t i=max(display.width(),display.height())/2; i>0; i-=3) {
    // The INVERSE color is used so circles alternate white/black
    display.fillCircle(display.width() / 2, display.height() / 2, i, SSD1306_INVERSE);
    display.display(); // Update screen with each newly-drawn circle
    delay(1);
  }

  delay(2000);
}

void testdrawroundrect(void) {
  display.clearDisplay();

  for(int16_t i=0; i<display.height()/2-2; i+=2) {
    display.drawRoundRect(i, i, display.width()-2*i, display.height()-2*i,
      display.height()/4, SSD1306_WHITE);
    display.display();
    delay(1);
  }

  delay(2000);
}

void testfillroundrect(void) {
  display.clearDisplay();

  for(int16_t i=0; i<display.height()/2-2; i+=2) {
    // The INVERSE color is used so round-rects alternate white/black
    display.fillRoundRect(i, i, display.width()-2*i, display.height()-2*i,
      display.height()/4, SSD1306_INVERSE);
    display.display();
    delay(1);
  }

  delay(2000);
}

void testdrawtriangle(void) {
  display.clearDisplay();

  for(int16_t i=0; i<max(display.width(),display.height())/2; i+=5) {
    display.drawTriangle(
      display.width()/2  , display.height()/2-i,
      display.width()/2-i, display.height()/2+i,
      display.width()/2+i, display.height()/2+i, SSD1306_WHITE);
    display.display();
    delay(1);
  }

  delay(2000);
}

void testfilltriangle(void) {
  display.clearDisplay();

  for(int16_t i=max(display.width(),display.height())/2; i>0; i-=5) {
    // The INVERSE color is used so triangles alternate white/black
    display.fillTriangle(
      display.width()/2  , display.height()/2-i,
      display.width()/2-i, display.height()/2+i,
      display.width()/2+i, display.height()/2+i, SSD1306_INVERSE);
    display.display();
    delay(1);
  }

  delay(2000);
}

void testdrawchar(void) {
  display.clearDisplay();

  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0, 0);     // Start at top-left corner
  display.cp437(true);         // Use full 256 char 'Code Page 437' font

  // Not all the characters will fit on the display. This is normal.
  // Library will draw what it can and the rest will be clipped.
  for(int16_t i=0; i<256; i++) {
    if(i == '\n') display.write(' ');
    else          display.write(i);
  }

  display.display();
  delay(2000);
}

void testdrawstyles(void) {
  display.clearDisplay();

  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0,0);             // Start at top-left corner
  display.println(F("Hello, world!"));

  display.setTextColor(SSD1306_BLACK, SSD1306_WHITE); // Draw 'inverse' text
  display.println(3.141592);

  display.setTextSize(2);             // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.print(F("0x")); display.println(0xDEADBEEF, HEX);

  display.display();
  delay(2000);
}


void testdrawchar(void) {
  display.clearDisplay();

  display.setTextSize(2);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0, 0);     // Start at top-left corner
  display.cp437(true);         // Use full 256 char 'Code Page 437' font

  display.write(248); 
  // Not all the characters will fit on the display. This is normal.
  // Library will draw what it can and the rest will be clipped.
  //for(int16_t i=0; i<256; i++) {
//    if(i == '\n') display.write(' ');
//    else          display.write(i);
//  }

  display.display();
  delay(2000);
}*/

/*Function: Starting OLED display and print version of portable iron*/
void Print_Version_Iron(){
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
  Serial.println(F("SSD1306 allocation failed"));
  for(;;); // Don't proceed, loop forever
  }
  display.setRotation(0);
  display.clearDisplay();
  display.setTextSize(1); // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(5, 10);  // ___(x, y);
  display.print(F("Version "));
  display.setCursor(55, 4);
  display.setTextSize(2); // Draw 2X-scale text
  display.println(iron_version);
  display.display();      // Show initial text
}
/**********************************************************************/

/*Function: Clear display*/
void Clear_Display(){
  display.clearDisplay();
}
/************************/

/*Function: Print char "degree" , uses "Code Page 437",
  Print: temperature*/
void Print_Temp(){
  display.clearDisplay();
  display.setCursor(65, 4);
  display.setTextSize(2); // Draw 2X-scale text
  display.cp437(true);         // Use full 256 char 'Code Page 437' font
  display.write(248);
  display.setTextSize(3); // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 4);
  display.print(Change_Temper(Get_Temp()), 1);
  display.drawBitmap(105, 4, iron, 8, 26, 1);
  display.setCursor(80, 4);
  display.print(F("C"));
  display.display();      // Show initial text
}
/*************************************************************************/

/*Function: Print char "degree" , uses "Code Page 437",
  Input parameter: print_val,
  Print parameter: print_val*/
void Print_Request_Temp(int print_val){
  display.clearDisplay();
  display.setTextSize(2); // Draw 2X-scale text
  display.setCursor(55, 0);
  display.cp437(true);         // Use full 256 char 'Code Page 437' font
  display.write(248);
  display.setTextSize(3); // Draw 3X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print(print_val, 1);
  display.display();      // Show initial text
}
/************************************************************************************************************/

/*Function: Print reading temperature and scroll up arrows(imitation heating)*/
void Print_Heating(){
  i--;
  if(i == 0){i = 10;}
  int k = i + 6;
  int j = k + 6;
  display.setTextColor(SSD1306_WHITE);
  display.drawBitmap(115, 0, clear_rectangle, 8, 32, 1);
  display.display();      // Show initial text
  display.drawBitmap(115, i, heating, 16, 7, 1);
  display.drawBitmap(115, k, heating, 16, 7, 1);
  display.drawBitmap(115, j, heating, 16, 7, 1);
  display.setTextColor(SSD1306_WHITE);
  display.display();      // Show initial text
}
/**************************************************************************/

/*Function: Print minimal value of temperature
  Parameter: min_val*/
void Print_Min(int min_val){
  display.clearDisplay();
  display.setTextSize(2); // Draw 2X-scale text
  display.setCursor(55, 0);
  display.cp437(true);         // Use full 256 char 'Code Page 437' font
  display.write(248);
  display.setTextSize(3); // Draw 3X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print(min_val, 1);
  display.setTextSize(2);
  display.setCursor(65, 4);
  display.print(F("min"));
  display.display();      // Show initial text
}
/**********************************************/

/*Function: Print maximal value of temperature,
  Parameter: max_val*/
void Print_Max(int max_val){
  display.clearDisplay();
  display.setTextSize(2); // Draw 2X-scale text
  display.setCursor(55, 0);
  display.cp437(true);         // Use full 256 char 'Code Page 437' font
  display.write(248);
  display.setTextSize(3); // Draw 3X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print(max_val, 1);
  display.setTextSize(2);
  display.setCursor(65, 4);
  display.print(F("max"));
  display.display();      // Show initial text
}
/**********************************************/

/*Function: Print reading temperature and scroll down arrows(imitation cooling)*/
void Print_Cooling(){
  n++;
  if(n == 10){n = 0;}
  int k = n + 6;
  int j = n + 6;
  display.clearDisplay();
  display.setCursor(65, 4);
  display.setTextSize(2); // Draw 2X-scale text
  display.cp437(true);         // Use full 256 char 'Code Page 437' font
  display.write(248);
  display.setTextSize(3); // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 4);
  display.print(Change_Temper(Get_Temp()), 1);
  display.setCursor(80, 4);
  display.print(F("C"));
  display.drawBitmap(105, 4, iron, 8, 26, 1);
  display.drawBitmap(115, n, cooling, 16, 7, 1);
  display.drawBitmap(115, k, cooling, 16, 7, 1);
  display.drawBitmap(115, j, cooling, 16, 7, 1);
  display.setTextColor(SSD1306_WHITE);
  display.display();      // Show initial text
}
/*********************************************************************************/

/*Function: Print read temperature, when temperature stabilized*/
void Print_Heat(){
  l++;
  if(l == 128){l = 114;}
  display.clearDisplay();
  display.setCursor(65, 4);
  display.setTextSize(2); // Draw 2X-scale text
  display.cp437(true);         // Use full 256 char 'Code Page 437' font
  display.write(248);
  display.setTextSize(3); // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 4);
  display.print(Change_Temper(Get_Temp()), 1);
  display.setCursor(80, 4);
  display.print(F("C"));
  display.drawBitmap(l, 0, heat, 16, 5, 1);
  display.drawBitmap(l, 6, heat, 16, 5, 1);
  display.drawBitmap(l, 12, heat, 16, 5, 1);
  display.drawBitmap(105, 4, iron, 8, 26, 1);
  display.setTextColor(SSD1306_WHITE);
  display.display();      // Show initial text
}
/*****************************************************************/

/*
void testscrolltext(void) {
  display.clearDisplay();

  display.setTextSize(2); // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(10, 0);
  display.println(F("scroll"));
  display.display();      // Show initial text
  delay(100);

  // Scroll in various directions, pausing in-between:
  display.startscrollright(0x00, 0x0F);
  delay(2000);
  display.stopscroll();
  delay(1000);
  display.startscrollleft(0x00, 0x0F);
  delay(2000);
  display.stopscroll();
  delay(1000);
  display.startscrolldiagright(0x00, 0x07);
  delay(2000);
  display.startscrolldiagleft(0x00, 0x07);
  delay(2000);
  display.stopscroll();
  delay(1000);
}

void testdrawbitmap(void) {
  display.clearDisplay();

  display.drawBitmap(
    (display.width()  - LOGO_WIDTH ) / 2,
    (display.height() - LOGO_HEIGHT) / 2,
    no_press_logo, LOGO_WIDTH, LOGO_HEIGHT, 1);
  display.display();
  delay(1000);
}
*/


/*Function: Blinking "press" image*/
void Standby_LCD() {  
  current_time = GetTimer();
  if (current_time - previous_time >= interval_blinking){
    previous_time = current_time;
    if (state_standby == 0){
      state_standby = 1;
    }
    else {
      state_standby = 0;
    }
    switch(state_standby){
      case 0: Standby_unpress_lcd(); break;
      case 1: Standby_press_lcd(); break;
    }
  }

/*  // Show the display buffer on the screen. You MUST call display() after
  // drawing commands to make them visible on screen!
  display.display();
  delay(2000);
  display.display() is NOT necessary after every single drawing command,
  // unless that's what you want...rather, you can batch up a bunch of
  // drawing operations and then update the screen all at once by calling
  display.display(). These examples demonstrate both approaches...
  testdrawline();      // Draw many lines
  testdrawrect();      // Draw rectangles (outlines)
  testfillrect();      // Draw rectangles (filled)
  testdrawcircle();    // Draw circles (outlines)
  testfillcircle();    // Draw circles (filled)
  testdrawroundrect(); // Draw rounded rectangles (outlines)
  testfillroundrect(); // Draw rounded rectangles (filled)
  testdrawtriangle();  // Draw triangles (outlines)
  testfilltriangle();  // Draw triangles (filled)
  testdrawchar();      // Draw characters of the default font
  testdrawstyles();    // Draw 'stylized' characters
  testscrolltext();    // Draw scrolling text
  testdrawbitmap();    // Draw a small bitmap image

  // Invert and restore display, pausing in-between
  display.invertDisplay(true);
  delay(1000);
//  display.invertDisplay(false);
//  delay(1000);*/
  
/*
  for(int16_t i=0; i<max(display.width(),display.height())/2; i+=5) {
    display.drawTriangle(
      display.width()/2  , display.height()/2-i,
      display.width()/2-i, display.height()/2+i,
      display.width()/2+i, display.height()/2+i, SSD1306_WHITE);
    display.display();
    delay(1);
  }
*/
}


/*Function: Print bitmap logo, press*/
void Standby_press_lcd(){
  display.clearDisplay();
  display.drawBitmap(0, 0, unpress_logo, 128, 32, 1);
  display.display(); // Show the display buffer on the screen
  display.setTextSize(1); // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(5, 5);
  display.print(F("Press"));
  display.display();      // Show initial text
}
/***********************************/

/*Function: Print bitmap logo, unpress*/
void Standby_unpress_lcd(){
  display.clearDisplay();
  display.drawBitmap(0, 0, unpress_logo, 128, 32, 1);
  display.display(); // Show the display buffer on the screen
  display.setTextSize(1); // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  //display.setCursor(5, 5);
  //display.print(F("Press"));
  display.display();      // Show initial text
}
/**************************************/
