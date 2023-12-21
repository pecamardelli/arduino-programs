#include "header.h"

SmallDisplay smallDisplay;
Clock clock;
Commander commander;
Flowmeter flowmeter;
TempSensor tempSensor;

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
