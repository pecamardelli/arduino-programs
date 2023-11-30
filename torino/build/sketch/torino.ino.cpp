#include <Arduino.h>
#line 1 "/home/pablin/arduino-programs/torino/torino.ino"
#include "header.h"

Commander commander;
Flowmeter flowmeter;

#line 6 "/home/pablin/arduino-programs/torino/torino.ino"
void setup();
#line 13 "/home/pablin/arduino-programs/torino/torino.ino"
void loop();
#line 6 "/home/pablin/arduino-programs/torino/torino.ino"
void setup()
{
    Serial.begin(9600);
    Serial.println("Initializing...");
    initScreen();
}

void loop()
{
    if (Serial.available())
    {
        String input = commander.getInput();
        commander.exec(input);
    }
}

