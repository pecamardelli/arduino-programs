/**************************************************************************/
/*!
  @file     Clock.h

  Original library by Pablin defining the logic for date and time resources.

  License: MIT (see LICENSE)
*/
/**************************************************************************/

#ifndef _CLOCK_H_
#define _CLOCK_H_

#include <RTClib.h>

/**************************************************************************/
/*!
    @brief  This class defines all utilities to work with dates and time.
*/
/**************************************************************************/
class Clock
{
private:
    uint8_t _hour;
    uint8_t _min;
    uint8_t _sec;
    uint8_t _year;
    uint8_t _month;
    uint8_t _day;

    DateTime now;

    const String days[7] = {"Dom", "Lun", "Mar", "Mie", "Jue", "Vie", "Sab"};

public:
    Clock();
    ~Clock();

    RTC_DS3231 RTC;

    void begin();
    void setDateTime(char *_date, char *_time);
    String getDate();
    String getTime();
};

#endif // _CLOCK_H_