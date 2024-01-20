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
  relayArray[0].enabled = true;

  // Streetlight
  relayArray[1].type = RELAY_TYPE_SEASON;
  relayArray[1].pin = PIN_GARDEN_LIGHTS;
  relayArray[1].enabled = true;

  // Internal lights
  relayArray[2].type = RELAY_TYPE_RANGE;
  relayArray[2].duration = 240;
  relayArray[2].pin = PIN_INTERNAL_LIGHTS;
  relayArray[2].enabled = true;

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
# 39 "/home/pablin/arduino_programs/mini_relay_timer/mini_relay_timer.ino" 3
                       (__extension__({static const char __c[] __attribute__((__progmem__)) = ("Jan 20 2024"); &__c[0];}))
# 39 "/home/pablin/arduino_programs/mini_relay_timer/mini_relay_timer.ino"
                       )), (reinterpret_cast<const __FlashStringHelper *>(
# 39 "/home/pablin/arduino_programs/mini_relay_timer/mini_relay_timer.ino" 3
                                    (__extension__({static const char __c[] __attribute__((__progmem__)) = ("20:44:09"); &__c[0];}))
# 39 "/home/pablin/arduino_programs/mini_relay_timer/mini_relay_timer.ino"
                                    ))));
  }
}

void loop()
{
  now = rtc.now();

  if (Serial.available())
  {
    String input = getInput();
    parse(input);
  }

  relay_check();
  relay_watch();
}

void relay_check()
{
  for (uint8_t i = 0; i < MAX_RELAY_NUMBER; i++)
    switch (relayArray[i].type)
    {
    case RELAY_TYPE_FIXED:
      continue;
    case RELAY_TYPE_SEASON:
      relayArray[i].startMinute = getSeasonStartMins();
      relayArray[i].endMinute = getSeasonEndMins();
      break;
    case RELAY_TYPE_RANGE:
      uint16_t currentMinute = now.hour() * 60 + now.minute();
      uint16_t seasonStartMinute = getSeasonStartMins();
      if (seasonStartMinute - RANGE_MAX_TIME_VARIATION <= currentMinute)
      {
        continue;
      }
      relayArray[i].startMinute = seasonStartMinute - ((random(RANGE_MAX_TIME_VARIATION))>=0?(long)((random(RANGE_MAX_TIME_VARIATION))+0.5):(long)((random(RANGE_MAX_TIME_VARIATION))-0.5));
      relayArray[i].endMinute = relayArray[i].startMinute + relayArray[i].duration + ((RANGE_MAX_TIME_VARIATION * random(-1, 1))>=0?(long)((RANGE_MAX_TIME_VARIATION * random(-1, 1))+0.5):(long)((RANGE_MAX_TIME_VARIATION * random(-1, 1))-0.5));
      if (relayArray[i].endMinute >= 1440)
        relayArray[i].endMinute = 1438;
      break;
    default:
      break;
    }
}

void relay_watch()
{
  uint16_t currentMinute = now.hour() * 60 + now.minute();

  for (uint8_t i = 0; i < MAX_RELAY_NUMBER; i++)
  {
    if (!relayArray[i].enabled)
    {
      digitalWrite(relayArray[i].pin, 0x1);
      continue;
    }

    if (isnan(relayArray[i].startMinute) || isnan(relayArray[i].endMinute))
      continue;

    if (relayArray[i].startMinute <= currentMinute)
    {
      if (relayArray[i].endMinute > relayArray[i].startMinute)
      {
        if (relayArray[i].endMinute < currentMinute)
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
      if (relayArray[i].startMinute < relayArray[i].endMinute)
      {
        digitalWrite(relayArray[i].pin, 0x1);
      }
      else
      {
        if (relayArray[i].endMinute > currentMinute)
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

uint16_t calculateDayOfYear()
{
  const uint8_t day = now.day();
  const uint8_t month = now.month();
  const uint16_t year = now.year();
  // Given a day, month, and year (4 digit), returns
  // the day of year. Errors return 999.

  // Verify we got a 4-digit year
  if (year < 1000)
  {
    return 999;
  }

  // Check if it is a leap year, this is confusing business
  // See: https://support.microsoft.com/en-us/kb/214019
  if (year % 4 == 0)
  {
    if (year % 100 != 0)
    {
      daysInMonth[1] = 29;
    }
    else
    {
      if (year % 400 == 0)
      {
        daysInMonth[1] = 29;
      }
    }
  }

  // Make sure we are on a valid day of the month
  if (day < 1)
  {
    return 999;
  }
  else if (day > daysInMonth[month - 1])
  {
    return 999;
  }

  uint16_t doy = 0;
  for (uint8_t i = 0; i < month - 1; i++)
  {
    doy += daysInMonth[i];
  }

  doy += day;
  return doy;
}

uint16_t getSeasonStartMins()
{
  uint16_t dayOfTheYear = calculateDayOfYear();
  float ratio = sin(3.1415926535897932384626433832795 * dayOfTheYear / SEASON_LONGEST_DAY_OF_THE_YEAR);
  return (uint16_t)SEASON_MAX_START_MINUTE - ((SEASON_START_TIMESPAN * ratio)>=0?(long)((SEASON_START_TIMESPAN * ratio)+0.5):(long)((SEASON_START_TIMESPAN * ratio)-0.5));
}

uint16_t getSeasonEndMins()
{
  uint16_t dayOfTheYear = calculateDayOfYear();
  float ratio = sin(3.1415926535897932384626433832795 * dayOfTheYear / SEASON_LONGEST_DAY_OF_THE_YEAR);
  return (uint16_t)SEASON_MIN_END_MINUTE + ((SEASON_START_TIMESPAN * ratio)>=0?(long)((SEASON_START_TIMESPAN * ratio)+0.5):(long)((SEASON_START_TIMESPAN * ratio)-0.5));
}

String getInput()
{
  if (Serial.available() <= 0)
    return;

  char c;
  String input;

  while (Serial.available() > 0)
  {
    c = Serial.read();
    // Serial.print(String(int(c)) + " ");
    //  Include letters, digits, and other allowed chars. Add more allowed characters
    //  at the definition of the specialChars array.
    if (isalpha(c) || isdigit(c) || charAllowed(c))
      input.concat(c);
    else
      input.concat((char)0x20);

    delay(5);
  }

  return input;
};

bool charAllowed(char c)
{
  for (uint8_t i = 0; i < sizeof(specialChars) / sizeof(byte); i++)
  {
    if ((int)c == specialChars[i])
      return true;
  }
  return false;
};

void parse(String input)
{
  // Tokenizing arguments
  char *args[MAX_COMMAND_ARGS];
  char buffer[MAX_COMMAND_LEN];
  uint8_t index = 0;

  input.toCharArray(buffer, MAX_COMMAND_LEN);

  char *token = strtok(buffer, DELIMITER);

  while (token != 
# 250 "/home/pablin/arduino_programs/mini_relay_timer/mini_relay_timer.ino" 3 4
                 __null 
# 250 "/home/pablin/arduino_programs/mini_relay_timer/mini_relay_timer.ino"
                      && index < MAX_COMMAND_ARGS)
  {
    args[index] = token;
    index++;
    token = strtok(
# 254 "/home/pablin/arduino_programs/mini_relay_timer/mini_relay_timer.ino" 3 4
                  __null
# 254 "/home/pablin/arduino_programs/mini_relay_timer/mini_relay_timer.ino"
                      , DELIMITER);
  }

  if (strncmp(args[0], "date", 4) == 0 || strncmp(args[0], "time", 4) == 0)
  {
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
    Serial.println(now.second(), 10);
  }
  else if (strncmp(args[0], "relay", 5) == 0)
  {
    // Now check if args[1] is a valid pin number.
    const int relayNumber = atoi(args[1]);

    if (relayNumber < 0 || relayNumber >= MAX_RELAY_NUMBER)
    {
      Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 278 "/home/pablin/arduino_programs/mini_relay_timer/mini_relay_timer.ino" 3
                  (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 278 "/home/pablin/arduino_programs/mini_relay_timer/mini_relay_timer.ino"
                  "Bad relay number: "
# 278 "/home/pablin/arduino_programs/mini_relay_timer/mini_relay_timer.ino" 3
                  ); &__c[0];}))
# 278 "/home/pablin/arduino_programs/mini_relay_timer/mini_relay_timer.ino"
                  )));
      Serial.println(relayNumber);
      return;
    }
    else if (strncmp(args[2], "enable", 6) == 0)
    {
      relayArray[relayNumber].enabled = true;
    }
    else if (strncmp(args[2], "disable", 7) == 0)
    {
      relayArray[relayNumber].enabled = false;
    }
    else
    {
      Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 292 "/home/pablin/arduino_programs/mini_relay_timer/mini_relay_timer.ino" 3
                  (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 292 "/home/pablin/arduino_programs/mini_relay_timer/mini_relay_timer.ino"
                  "Bad subcommand: "
# 292 "/home/pablin/arduino_programs/mini_relay_timer/mini_relay_timer.ino" 3
                  ); &__c[0];}))
# 292 "/home/pablin/arduino_programs/mini_relay_timer/mini_relay_timer.ino"
                  )));
      Serial.println(args[2]);
    }
  }
  else if (strncmp(args[0], "set", 3) == 0)
  {
    if (strncmp(args[1], "date", 4) == 0)
    {
      String dateString;
      dateString.concat(args[2]);
      dateString.concat(" ");
      dateString.concat(args[3]);
      dateString.concat(" ");
      dateString.concat(args[4]);

      char dateCharArray[dateString.length()];
      dateString.toCharArray(dateCharArray, dateString.length() + 1);
      rtc.adjust(DateTime(dateCharArray, args[5]));
    }
  }
  else
  {
    Serial.print((reinterpret_cast<const __FlashStringHelper *>(
# 314 "/home/pablin/arduino_programs/mini_relay_timer/mini_relay_timer.ino" 3
                (__extension__({static const char __c[] __attribute__((__progmem__)) = (
# 314 "/home/pablin/arduino_programs/mini_relay_timer/mini_relay_timer.ino"
                "Bad command: "
# 314 "/home/pablin/arduino_programs/mini_relay_timer/mini_relay_timer.ino" 3
                ); &__c[0];}))
# 314 "/home/pablin/arduino_programs/mini_relay_timer/mini_relay_timer.ino"
                )));
    Serial.println(args[0]);
  }
}
