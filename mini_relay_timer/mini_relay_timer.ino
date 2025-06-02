#include "header.h"

Commander commander;
Clock clock;
Relay relays;

void setup()
{
  Serial.begin(9600);
  Serial.println(F("MINI RELAY TIMER STARTED"));

  Wire.begin();
  clock.begin();
  relays.begin();
}

void loop()
{
  if (Serial.available())
  {
    String input = commander.getInput();
    commander.exec(input);
  }

  relays.relay_check();
  relays.relay_watch();
  delay(1000); // Check every second
}