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
#include "DataDisplay.h"

typedef struct _dateTimeProps
{
    uint8_t timeDisplayX;
    uint8_t timeDisplayY;
    uint8_t timeDisplaySize;

    uint8_t dateDisplayX;
    uint8_t dateDisplayY;
    uint8_t dateDisplaySize;
} dateTimeProps;

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

    uint32_t lastUnixtime;
    uint8_t refreshInterval;

    const uint8_t defaultTimeDisplayX = 5;
    const uint8_t defaultTimeDisplayY = 0;
    const uint8_t defaultTimeDisplaySize = 4;

    const uint8_t defaultDateDisplayX = 4;
    const uint8_t defaultDateDisplayY = 36;
    const uint8_t defaultDateDisplaySize = 2;

    dateTimeProps props;

    const String days[7] = {"Dom", "Lun", "Mar", "Mie", "Jue", "Vie", "Sab"};

    void setDisplayData();

public:
    Clock();
    Clock(dateTimeProps);
    ~Clock();

    RTC_DS3231 RTC;
    DataDisplay *dateDisplayData;
    DataDisplay *timeDisplayData;

    void begin();
    void setDateTime(char *_date, char *_time);
    String getDate();
    String getTime();
    void refreshDisplayData();

    void setTimeDisplayX(uint8_t);
    void setTimeDisplayY(uint8_t);
    void setTimeDisplaySize(uint8_t);

    void setDateDisplayX(uint8_t);
    void setDateDisplayY(uint8_t);
    void setDateDisplaySize(uint8_t);

    void resetProps();
};

#endif // _CLOCK_H_