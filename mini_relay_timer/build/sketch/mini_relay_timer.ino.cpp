#include <Arduino.h>
#line 1 "/home/pablin/arduino_programs/mini_relay_timer/mini_relay_timer.ino"
#include "header.h"

Commander commander;
Clock clock;
Relay relays;

#line 7 "/home/pablin/arduino_programs/mini_relay_timer/mini_relay_timer.ino"
void setup();
#line 17 "/home/pablin/arduino_programs/mini_relay_timer/mini_relay_timer.ino"
void loop();
#line 7 "/home/pablin/arduino_programs/mini_relay_timer/mini_relay_timer.ino"
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
}
