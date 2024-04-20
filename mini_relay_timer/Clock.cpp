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
void Clock::setDateTime(String date, String time)
{
  char _date[date.length() + 1];
  date.toCharArray(_date, date.length() + 1);

  char _time[time.length() + 1];
  time.toCharArray(_time, time.length() + 1);

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
    @brief  Calculates the day number of the year.
    @return  the current day number of the year.
*/
/**************************************************************************/
uint16_t Clock::calculateDayOfYear()
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

/**************************************************************************/
/*!
    @brief  Executes a user command.
    @param  command Array of strings representing the command itself and its arguments.
*/
/**************************************************************************/
EXEC_STATUSES Clock::exec(String args[])
{
  if (args[0].equals("date") || args[0].equals("time"))
  {
    Serial.print(getDate() + " " + getTime());
    return COMMAND_SUCCESSFUL;
  }
  else if (args[0].equals("set"))
  {
    if (args[1].equals("date"))
    {
      setDateTime(args[2], args[3]);
      return COMMAND_SUCCESSFUL;
    }
  }

  return NO_COMMAND;
}