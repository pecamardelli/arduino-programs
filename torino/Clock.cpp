#include "Clock.h"

Clock::Clock()
{
  props.timeDisplayX = defaultTimeDisplayX;
  props.timeDisplayY = defaultTimeDisplayY;
  props.timeDisplaySize = defaultTimeDisplaySize;

  props.dateDisplayX = defaultDateDisplayX;
  props.dateDisplayY = defaultDateDisplayY;
  props.dateDisplaySize = defaultDateDisplaySize;

  setDisplayData();
}

Clock::Clock(dateTimeProps savedProps)
{
  setDisplayData();
}

Clock::~Clock()
{
}

void Clock::begin()
{
  RTC.begin();
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

/**************************************************************************/
/*!
    @brief  Sets the data to be displayed on the small screen.
*/
/**************************************************************************/
void Clock::setDisplayData()
{
  timeDisplayData = new DataDisplay(5, 0, 4);
  // Lambda function for refreshing the displayed time.
  auto timeRefreshFc = []()
  {
    extern Clock clock;
    clock.timeDisplayData->setText(clock.getTime());
  };

  timeDisplayData->refresh = timeRefreshFc;

  dateDisplayData = new DataDisplay(4, 36, 2);
  // Lambda function for refreshing the displayed date.
  auto dateRefreshFc = []()
  {
    extern Clock clock;
    clock.dateDisplayData->setText(clock.getDate());
  };

  dateDisplayData->refresh = dateRefreshFc;
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
    @brief  Refreshes the data displayed on screen.
*/
/**************************************************************************/
void Clock::refreshDisplayData(){
    // displayData->setText(getTime());
};

/**************************************************************************/
/*!
    @brief  Sets the x coordinate for the time showed on the small display.
    @param  value The x coordinate for the time displayed on screen.
*/
/**************************************************************************/
void Clock::setTimeDisplayX(uint8_t value)
{
  props.timeDisplayX = value;
}

/**************************************************************************/
/*!
    @brief  Sets the y coordinate for the time showed on the small display.
    @param  value The y coordinate for the time displayed on screen.
*/
/**************************************************************************/
void Clock::setTimeDisplayY(uint8_t value)
{
  props.timeDisplayY = value;
}

/**************************************************************************/
/*!
    @brief  Sets the size of time showed on the small display.
    @param  value The size for the time displayed on screen.
*/
/**************************************************************************/
void Clock::setTimeDisplaySize(uint8_t value)
{
  props.timeDisplaySize = value;
}

/**************************************************************************/
/*!
    @brief  Sets the x coordinate for the date showed on the small display.
    @param  value The x coordinate for the date displayed on screen.
*/
/**************************************************************************/
void Clock::setDateDisplayX(uint8_t value)
{
  props.dateDisplayX = value;
}

/**************************************************************************/
/*!
    @brief  Sets the y coordinate for the date showed on the small display.
    @param  value The y coordinate for the date displayed on screen.
*/
/**************************************************************************/
void Clock::setDateDisplayY(uint8_t value)
{
  props.dateDisplayY = value;
}

/**************************************************************************/
/*!
    @brief  Sets the size of date showed on the small display.
    @param  value The size for the date displayed on screen.
*/
/**************************************************************************/
void Clock::setDateDisplaySize(uint8_t value)
{
  props.dateDisplaySize = value;
}

/**************************************************************************/
/*!
    @brief  Resets all properties to their defaults.
*/
/**************************************************************************/
void Clock::resetProps()
{
  props.timeDisplayX = defaultTimeDisplayX;
  props.timeDisplayY = defaultTimeDisplayY;
  props.timeDisplaySize = defaultTimeDisplaySize;

  props.dateDisplayX = defaultDateDisplayX;
  props.dateDisplayY = defaultDateDisplayY;
  props.dateDisplaySize = defaultDateDisplaySize;
}