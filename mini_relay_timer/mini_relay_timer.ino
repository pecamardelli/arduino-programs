#include "header.h"

void setup()
{
  Serial.begin(9600);
  Serial.println(F("MINI RELAY TIMER STARTED"));

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
  relayArray[2].enabled = false;

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
      relayArray[i].startMinute = seasonStartMinute - round(random(RANGE_MAX_TIME_VARIATION));
      relayArray[i].endMinute = relayArray[i].startMinute + relayArray[i].duration + round(RANGE_MAX_TIME_VARIATION * random(-1, 1));
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
      digitalWrite(relayArray[i].pin, HIGH);
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
      if (relayArray[i].startMinute < relayArray[i].endMinute)
      {
        digitalWrite(relayArray[i].pin, HIGH);
      }
      else
      {
        if (relayArray[i].endMinute > currentMinute)
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
  float ratio = sin(PI * dayOfTheYear / SEASON_LONGEST_DAY_OF_THE_YEAR);
  return (uint16_t)SEASON_MAX_START_MINUTE - round(SEASON_START_TIMESPAN * ratio);
}

uint16_t getSeasonEndMins()
{
  uint16_t dayOfTheYear = calculateDayOfYear();
  float ratio = sin(PI * dayOfTheYear / SEASON_LONGEST_DAY_OF_THE_YEAR);
  return (uint16_t)SEASON_MIN_END_MINUTE + round(SEASON_START_TIMESPAN * ratio);
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

  while (token != NULL && index < MAX_COMMAND_ARGS)
  {
    args[index] = token;
    index++;
    token = strtok(NULL, DELIMITER);
  }

  if (strncmp(args[0], "date", 4) == 0 || strncmp(args[0], "time", 4) == 0)
  {
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
    Serial.println(now.second(), DEC);
  }
  else if (strncmp(args[0], "relay", 5) == 0)
  {
    // Now check if args[1] is a valid pin number.
    const int relayNumber = atoi(args[1]);

    if (relayNumber < 0 || relayNumber >= MAX_RELAY_NUMBER)
    {
      Serial.print(F("Bad relay number: "));
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
      Serial.print(F("Bad subcommand: "));
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
    Serial.print(F("Bad command: "));
    Serial.println(args[0]);
  }
}