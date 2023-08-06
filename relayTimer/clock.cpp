#include "header.h"

extern RTC_DS3231 RTC;

Clock::Clock(/* args */)
{
}

Clock::~Clock()
{
}

String Clock::setDateTime(String date, String time)
{

    char *dateCharArray;
    char *timeCharArray;

    date.toCharArray(dateCharArray, date.length());
    time.toCharArray(timeCharArray, time.length());

    uint8_t year, month, day, hour, minute, second;

    if (sscanf(dateCharArray, "%d/%d/%d", &year, &month, &day) != 3)
        return F("Bad date format (YYYY/MM/DD)");

    if (sscanf(timeCharArray, "%d:%d:%d", &hour, &minute, &second) != 3)
        return F("Bad time format (HH:MM:SS)");

    RTC.adjust(DateTime(year, month, day, hour, minute, second));
};

char *Clock::getDate()
{
    char *dateBuffer;
    return RTC.now().toString(dateBuffer);
};