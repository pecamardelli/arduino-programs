#include "Clock.h"

Clock::Clock()
{
}

Clock::~Clock()
{
}

void Clock::begin()
{
  RTC.begin();
  // Si se ha perdido la corriente, fijar fecha y hora
  if (RTC.lostPower())
  {
    // Fijar a fecha y hora de compilacion
    RTC.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
}

/**************************************************************************/
/*!
    @brief  Configures the date and time of the Real Time Clock.
    @param  _date The date in format YYYY/MM/DD.
    @param  _time The time in format HH:MM:SS.
*/
/**************************************************************************/
void Clock::setDateTime(char *_date, char *_time)
{
  if (sscanf(_date, "%d/%d/%d", &_year, &_month, &_day) != 3)
  {
    Serial.println(F("Bad date format (YYYY/MM/DD)"));
    return;
  }

  if (sscanf(_time, "%d:%d:%d", &_hour, &_min, &_sec) != 3)
  {
    Serial.println(F("Bad time format (HH:MM:SS)"));
    return;
  }

  if (_day >= 0 || _day <= 31)
  {
    if (_day == 31 &&
        (_month == 2 ||
         _month == 4 ||
         _month == 6 ||
         _month == 9 ||
         _month == 11))
    {
      Serial.print(F("ERROR! Month "));
      Serial.print(String(_month));
      Serial.println(F(" does not have 31 days."));
      return;
    }

    if (_month == 2 && _day == 29)
    {
      if (_year % 4 != 0)
      {
        Serial.println(F("ERROR! Year "));
        Serial.println(String(_year));
        Serial.println(F(" is not a leap-year.\n February cannot have 29 days."));
        return;
      }
    }
  }
  else
    return;

  // For some weird reason, the value of the _year variable is not recognized.
  // Adding 0 to it solved the issue. Yeah, I know...
  RTC.adjust(DateTime(_year + 0, _month, _day, _hour, _min, _sec));
};

/**************************************************************************/
/*!
    @brief  Returns the current date in string format.
    @return The current date.
*/
/**************************************************************************/
String Clock::getDate()
{
  return RTC.now().timestamp(now.TIMESTAMP_DATE);
};

/**************************************************************************/
/*!
    @brief  Returns the current date with the day in string format.
    @return The current date with the day.
*/
/**************************************************************************/
String Clock::getFullDate()
{
  String fullDate;
  now = RTC.now();

  fullDate += days[now.dayOfTheWeek()] + " ";
  fullDate += now.day() >= 10 ? String(now.day()) : "0" + String(now.day());
  fullDate += "/";
  fullDate += now.month() >= 10 ? String(now.month()) : "0" + String(now.month());
  fullDate += "/";
  fullDate += now.year();

  return fullDate;
};

/**************************************************************************/
/*!
    @brief  Returns the current time formatted to hours and minutes.
    @return The current time in format HH:MM.
*/
/**************************************************************************/
String Clock::getTime()
{
  return RTC.now().timestamp(now.TIMESTAMP_TIME).substring(0, 5);
}

/**************************************************************************/
/*!
    @brief  Executes a user command.
    @param  command Array of strings representing the command itself and its arguments.
*/
/**************************************************************************/
uint8_t Clock::exec(char *args[])
{
  if (strncmp(args[0], "date", 4) == 0 || strncmp(args[0], "time", 4) == 0)
  {
    Serial.print(getDate() + " " + getTime());
    return 0;
  }
  else if (strncmp(args[0], "set", 3) == 0)
  {
    if (strncmp(args[1], "date", 4) == 0)
    {
      setDateTime(args[2], args[3]);
      return 0;
    }
  }

  return 1;
}