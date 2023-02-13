
#include "header.h"

System sys;

void setup()
{
    Serial.begin(9600);
}

void loop()
{
    if (Serial.available())
    {
        sys.getSerialInput();
    }
}
