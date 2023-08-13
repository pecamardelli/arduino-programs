#include <Arduino.h>
#line 1 "/home/pablin/arduino_programs/relayTimer/relayTimer.ino"
#include "header.h"

RTC_DS3231 RTC;

System sys;
Parser parser;
SerialChannel serial;
NodeList nodes;
Clock clock;

#line 11 "/home/pablin/arduino_programs/relayTimer/relayTimer.ino"
void setup();
#line 19 "/home/pablin/arduino_programs/relayTimer/relayTimer.ino"
void loop();
#line 11 "/home/pablin/arduino_programs/relayTimer/relayTimer.ino"
void setup()
{
  Serial.begin(9600);
  sys.loadSystemData();
  RTC.begin();
  nodes.loadRelays();
}

void loop()
{
  serial.checkAvailable();

  nodes.checkRelays();
}

