#include <Arduino.h>
#line 1 "/home/pablin/arduino_programs/torino/torino.ino"
#include "header.h"

Commander commander;
Flowmeter flowmeter;

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

