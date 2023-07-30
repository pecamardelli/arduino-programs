# 1 "/home/pablin/arduino_programs/mini_relay_timer/mini_relay_timer.ino"
# 2 "/home/pablin/arduino_programs/mini_relay_timer/mini_relay_timer.ino" 2

void setup()
{
  Serial.begin(9600);
  Serial.println((reinterpret_cast<const __FlashStringHelper *>(
# 6 "/home/pablin/arduino_programs/mini_relay_timer/mini_relay_timer.ino" 3
                (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 6 "/home/pablin/arduino_programs/mini_relay_timer/mini_relay_timer.ino"
                "MINI RELAY TIMER STARTED"
# 6 "/home/pablin/arduino_programs/mini_relay_timer/mini_relay_timer.ino" 3
                ); &__c[0];}))
# 6 "/home/pablin/arduino_programs/mini_relay_timer/mini_relay_timer.ino"
                )));

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

  pinMode(PIN_EXTERNAL_LIGHTS, 0x1);
  digitalWrite(PIN_EXTERNAL_LIGHTS, 0x1);
  pinMode(PIN_GARDEN_LIGHTS, 0x1);
  digitalWrite(PIN_GARDEN_LIGHTS, 0x1);
  pinMode(PIN_INTERNAL_LIGHTS, 0x1);
  digitalWrite(PIN_INTERNAL_LIGHTS, 0x1);

  if (rtc.lostPower())
  {
    // Fijar a fecha y hora de compilacion
    rtc.adjust(DateTime((reinterpret_cast<const __FlashStringHelper *>(
# 35 "/home/pablin/arduino_programs/mini_relay_timer/mini_relay_timer.ino" 3
                       (__extension__({static const char __c[] __attribute__((__progmem__)) = ("Jul 29 2023"); &__c[0];}))
# 35 "/home/pablin/arduino_programs/mini_relay_timer/mini_relay_timer.ino"
                       )), (reinterpret_cast<const __FlashStringHelper *>(
# 35 "/home/pablin/arduino_programs/mini_relay_timer/mini_relay_timer.ino" 3
                                    (__extension__({static const char __c[] __attribute__((__progmem__)) = ("23:08:31"); &__c[0];}))
# 35 "/home/pablin/arduino_programs/mini_relay_timer/mini_relay_timer.ino"
                                    ))));
  }

  Serial.println(sizeof(relayArray) / sizeof(relay));
}

void loop()
{
  DateTime now = rtc.now();

  Serial.print(now.year(), 10);
  Serial.print('/');
  Serial.print(now.month(), 10);
  Serial.print('/');
  Serial.print(now.day(), 10);
  Serial.print(' ');
  Serial.print(now.hour(), 10);
  Serial.print(':');
  Serial.print(now.minute(), 10);
  Serial.print(':');
  Serial.print(now.second(), 10);
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
          digitalWrite(relayArray[i].pin, 0x1);
        }
        else
        {
          digitalWrite(relayArray[i].pin, 0x0);
        }
      }
      else
      {
        digitalWrite(relayArray[i].pin, 0x0);
      }
    }
    else
    {
      if (startMins < endMins)
      {
        digitalWrite(relayArray[i].pin, 0x1);
      }
      else
      {
        if (endMins > currentMins)
        {
          digitalWrite(relayArray[i].pin, 0x0);
        }
        else
        {
          digitalWrite(relayArray[i].pin, 0x1);
        }
      }
    }
  }
}
