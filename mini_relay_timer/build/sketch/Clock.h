#line 1 "/home/pablin/arduino_programs/mini_relay_timer/Clock.h"
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
#include "common.h"

/**************************************************************************/
/*!
    @brief  This class defines all utilities to work with dates and time.
*/
/**************************************************************************/
class Clock
{
private:
    uint16_t _year;
    uint8_t _month;
    uint8_t _day;

    uint8_t _hour;
    uint8_t _min;
    uint8_t _sec;

    DateTime now;

public:
    Clock();
    ~Clock();

    const String days[7] = {"DOMINGO", "LUNES", "MARTES", "MIERCOLES", "JUEVES", "VIERNES", "SABADO"};
    // Number of days of each month.
    uint8_t daysInMonth[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    RTC_DS3231 RTC;

    void begin();
    void setDateTime(char *_date, char *_time);
    String getDate();
    String getTime();
    uint8_t exec(char *[]);
    uint16_t calculateDayOfYear();
};

#endif // _CLOCK_H_