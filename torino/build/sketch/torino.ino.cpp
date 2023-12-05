#include <Arduino.h>
#line 1 "/home/pablin/arduino_programs/torino/torino.ino"
#include "header.h"

Clock clock;
Commander commander;
Flowmeter flowmeter;

#line 7 "/home/pablin/arduino_programs/torino/torino.ino"
void setup();
#line 26 "/home/pablin/arduino_programs/torino/torino.ino"
void loop();
#line 7 "/home/pablin/arduino_programs/torino/torino.ino"
void setup()
{
    Serial.begin(9600);
    Serial.println("Initializing...");

    // By default, we'll generate the high voltage from the 3.3v line internally! (neat!)
    smallDisplay.begin(SH1106_SWITCHCAPVCC, 0x3C); // initialize with the I2C addr 0x3D (for the 128x64)
    // Init done
    smallDisplay.clearDisplay();
    smallDisplay.display();

    Serial.print("Display width: ");
    Serial.println(smallDisplay.width());
    Serial.print("Display height: ");
    Serial.println(smallDisplay.height());

    clock.begin();
}

void loop()
{
    if (Serial.available())
    {
        String input = commander.getInput();
        commander.exec(input);
    }

    clock.displayTime();
}

