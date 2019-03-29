#include "Arduino.h"

#include"stdio.h"
#include"stdlib.h"

void arduino_printf(char *fmt ,...)
{
    va_list v_arg;
    char string[128];
    va_start(v_arg,fmt);
    vsprintf(string,fmt,v_arg);
    Serial.print(string);
}
