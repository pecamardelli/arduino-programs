#include "header.h"

Clock clock;
Commander commander;
Flowmeter flowmeter;
Display smallDisplay;

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
