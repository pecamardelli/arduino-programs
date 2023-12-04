#include <Arduino.h>
#line 1 "/home/pablin/arduino_programs/torino/torino.ino"
#include "header.h"

Clock clock;
Commander commander;
Flowmeter flowmeter;
Display smallDisplay;

#line 8 "/home/pablin/arduino_programs/torino/torino.ino"
void setup();
#line 16 "/home/pablin/arduino_programs/torino/torino.ino"
void loop();
#line 8 "/home/pablin/arduino_programs/torino/torino.ino"
void setup()
{
    Serial.begin(9600);
    Serial.println("Initializing...");
    smallDisplay.begin();
    clock.begin();
}

void loop()
{
    if (Serial.available())
    {
        String input = commander.getInput();
        commander.exec(input);
    }
}

