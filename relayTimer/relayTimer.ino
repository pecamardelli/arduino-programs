#include "header.h"

RTC_DS3231 RTC;

System sys;
Parser parser;
SerialChannel serial;
NodeList nodes;
Clock clock;

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
