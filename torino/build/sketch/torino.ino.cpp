#include <Arduino.h>
#line 1 "/home/pablin/arduino_programs/torino/torino.ino"
#include "header.h"

SmallDisplay smallDisplay;
Clock clock;
Commander commander;
Flowmeter flowmeter;
TempSensor tempSensor;

#line 9 "/home/pablin/arduino_programs/torino/torino.ino"
void setup();
#line 23 "/home/pablin/arduino_programs/torino/torino.ino"
void loop();
#line 9 "/home/pablin/arduino_programs/torino/torino.ino"
void setup()
{
    Serial.begin(9600);
    Serial.println("Initializing...");

    smallDisplay.begin();
    clock.begin();

    smallDisplay.setDataSlot(0, clock.timeDisplayData);
    smallDisplay.setDataSlot(1, clock.dateDisplayData);

    tempSensor.begin();
}

void loop()
{
    if (Serial.available())
    {
        String input = commander.getInput();
        commander.exec(input);
    }

    smallDisplay.display();
    // clock.displayTime();
    // tempSensor.printTemp();
}

