#include "Clock.h"

Clock::Clock()
{
  props.timeDisplayX = defaultTimeDisplayX;
  props.timeDisplayY = defaultTimeDisplayY;
  props.timeDisplaySize = defaultTimeDisplaySize;

  props.dateDisplayX = defaultDateDisplayX;
  props.dateDisplayY = defaultDateDisplayY;
  props.dateDisplaySize = defaultDateDisplaySize;
}

Clock::Clock(dateTimeProps savedProps)
{
  lastUnixtime = 0;
  refreshInterval = 1; // Amount of seconds between display refreshes.

  if (savedProps.timeDisplayX > smallDisplay.width())
  {
    props.timeDisplayX = defaultTimeDisplayX;
  }
  else
  {
    props.timeDisplayX = savedProps.timeDisplayX;
  }

  if (savedProps.timeDisplayY > smallDisplay.height())
  {
    props.timeDisplayY = defaultTimeDisplayY;
  }
  else
  {
    props.timeDisplayY = savedProps.timeDisplayY;
  }

  if (savedProps.timeDisplaySize > 9)
  {
    props.timeDisplaySize = defaultTimeDisplaySize;
  }
  else
  {
    props.timeDisplaySize = savedProps.timeDisplaySize;
  }

  if (savedProps.dateDisplayX > smallDisplay.width())
  {
    props.dateDisplayX = defaultDateDisplayX;
  }
  else
  {
    props.dateDisplayX = savedProps.dateDisplayX;
  }

  if (savedProps.dateDisplayY > smallDisplay.height())
  {
    props.dateDisplayY = defaultDateDisplayY;
  }
  else
  {
    props.dateDisplayY = savedProps.dateDisplayY;
  }

  if (savedProps.dateDisplaySize > 9)
  {
    props.dateDisplaySize = defaultDateDisplaySize;
  }
  else
  {
    props.dateDisplaySize = savedProps.dateDisplaySize;
  }
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
    @brief  Returns the current date in string format.
    @return The current date.
*/
/**************************************************************************/
String Clock::getDate()
{
  return this->RTC.now().timestamp();
};

/**************************************************************************/
/*!
    @brief  Prints the date and time on the small display.
*/
/**************************************************************************/
void Clock::displayTime()
{
  now = this->RTC.now();

  if (lastUnixtime + refreshInterval > now.unixtime())
  {
    return;
  }

  smallDisplay.clearDisplay();

  // Print time
  smallDisplay.setCursor(props.timeDisplayX, props.timeDisplayY);
  smallDisplay.setTextSize(props.timeDisplaySize);
  smallDisplay.setTextColor(WHITE);
  smallDisplay.print(now.timestamp(now.TIMESTAMP_TIME).substring(0, 5));

  // Print date
  smallDisplay.setCursor(props.dateDisplayX, props.dateDisplayY);
  smallDisplay.setTextSize(props.dateDisplaySize);
  smallDisplay.print(now.timestamp(now.TIMESTAMP_DATE));

  smallDisplay.display();

  lastUnixtime = now.unixtime();
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