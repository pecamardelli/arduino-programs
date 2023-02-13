#include <Arduino.h>
#line 1 "/home/pablin/arduino_programs/relayTimer/relayTimer.ino"

#include "header.h"

System sys;

#line 6 "/home/pablin/arduino_programs/relayTimer/relayTimer.ino"
void setup();
#line 12 "/home/pablin/arduino_programs/relayTimer/relayTimer.ino"
void loop();
#line 6 "/home/pablin/arduino_programs/relayTimer/relayTimer.ino"
void setup()
{
    Serial.begin(9600);
    Serial.println("Hello World");
}

void loop()
{
    if (Serial.available())
    {
        // sys.getSerialInput();
    }
}

