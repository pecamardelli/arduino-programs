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
}

void Clock::setDateTime(char *_date, char *_time)
{
  Serial.println(_date);
  Serial.println(_time);

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
      Serial.println(F("ERROR! Month "));
      Serial.println(String(_month));
      Serial.println(F(" does not have 31 days."));
      return;
    }

    if (_month == 2 && _day == 29)
    {
      int x = _year % 4;
      if (x != 0)
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

  RTC.adjust(DateTime(_year, _month, _day, _hour, _min, _sec));
};

String Clock::getDate()
{
  return this->RTC.now().timestamp();
};

DateTime Clock::getTime()
{
  return this->RTC.now().TIMESTAMP_TIME;
};
