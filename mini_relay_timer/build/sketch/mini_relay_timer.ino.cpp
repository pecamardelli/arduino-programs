#include <Arduino.h>
#line 1 "/home/pablin/arduino_programs/mini_relay_timer/mini_relay_timer.ino"
#include "header.h"

#line 3 "/home/pablin/arduino_programs/mini_relay_timer/mini_relay_timer.ino"
void setup();
#line 41 "/home/pablin/arduino_programs/mini_relay_timer/mini_relay_timer.ino"
void loop();
#line 63 "/home/pablin/arduino_programs/mini_relay_timer/mini_relay_timer.ino"
void relay_check();
#line 3 "/home/pablin/arduino_programs/mini_relay_timer/mini_relay_timer.ino"
void setup()
{
  Serial.begin(9600);
  Serial.println(F("MINI RELAY TIMER STARTED"));

  Wire.begin();
  rtc.begin();

  // External lights
  relayArray[0].type = RELAY_TYPE_SEASON;
  relayArray[0].pin = PIN_EXTERNAL_LIGHTS;

  // Streetlight
  relayArray[1].type = RELAY_TYPE_SEASON;
  relayArray[1].pin = PIN_GARDEN_LIGHTS;

  // Internal lights
  relayArray[2].type = RELAY_TYPE_FIXED;
  relayArray[2].startHour = 23;
  relayArray[2].startMinute = 7;
  relayArray[2].pin = PIN_INTERNAL_LIGHTS;

  pinMode(PIN_EXTERNAL_LIGHTS, OUTPUT);
  digitalWrite(PIN_EXTERNAL_LIGHTS, HIGH);
  pinMode(PIN_GARDEN_LIGHTS, OUTPUT);
  digitalWrite(PIN_GARDEN_LIGHTS, HIGH);
  pinMode(PIN_INTERNAL_LIGHTS, OUTPUT);
  digitalWrite(PIN_INTERNAL_LIGHTS, HIGH);

  if (rtc.lostPower())
  {
    // Fijar a fecha y hora de compilacion
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  Serial.println(sizeof(relayArray) / sizeof(relay));
}

void loop()
{
  DateTime now = rtc.now();

  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(' ');
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();

  relay_check();

  delay(1000);
}

void relay_check()
{
  DateTime now = rtc.now();

  for (uint8_t i = 0; i < ARRAY_LENGTH; i++)
  {
    if (relayArray[i].type != RELAY_TYPE_FIXED)
      continue;

    uint8_t startMins = relayArray[i].startHour * 60 + relayArray[i].startMinute;
    uint8_t endMins = relayArray[i].endHour * 60 + relayArray[i].endMinute;
    uint8_t currentMins = now.hour() * 60 + now.minute();

    if (startMins <= currentMins)
    {
      if (endMins > startMins)
      {
        if (endMins < currentMins)
        {
          digitalWrite(relayArray[i].pin, HIGH);
        }
        else
        {
          digitalWrite(relayArray[i].pin, LOW);
        }
      }
      else
      {
        digitalWrite(relayArray[i].pin, LOW);
      }
    }
    else
    {
      if (startMins < endMins)
      {
        digitalWrite(relayArray[i].pin, HIGH);
      }
      else
      {
        if (endMins > currentMins)
        {
          digitalWrite(relayArray[i].pin, LOW);
        }
        else
        {
          digitalWrite(relayArray[i].pin, HIGH);
        }
      }
    }
  }
}
