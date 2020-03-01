#ifndef _LCD_print_H_
#define _LCD_print_H_

void Print_Version_Iron();
void Print_Temp();
void Standby_LCD();
void Print_Request_Temp(int print_val);
void Clear_Display();
void Standby_press_lcd();
void Standby_unpress_lcd();
void Print_Heating();
void Print_Cooling();
void Print_Heat();
//void testdrawchar();
void Print_Min(int min_val);
void Print_Max(int max_val);

#endif
