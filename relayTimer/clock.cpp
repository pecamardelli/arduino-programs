#include "header.h"

extern RTC_DS1307 RTC;

Clock::Clock(/* args */)
{
    days[0] = F("Sunday");
    days[1] = F("Monday");
    days[2] = F("Tuesday");
    days[3] = F("Wednesday");
    days[4] = F("Thursday");
    days[5] = F("Friday");
    days[6] = F("Saturday");
}

Clock::~Clock()
{
}

String Clock::setDateTime(String date, String time)
{
    Serial.println("date " + date + " - " + "time " + time);

    char *dateCharArray;
    char *timeCharArray;

    date.toCharArray(dateCharArray, date.length());
    time.toCharArray(timeCharArray, date.length());

    uint8_t year, month, day, hour, minute, second;

    if (sscanf(dateCharArray, "%d/%d/%d", &year, &month, &day) != 3)
        return F("Bad date format (YYYY/MM/DD)");

    if (sscanf(timeCharArray, "%d:%d:%d", &hour, &minute, &second) != 3)
        return F("Bad time format (HH:MM:SS)");

    if (day >= 0 || day <= 31)
    {
        if (day == 31 &&
            (month == 2 ||
             month == 4 ||
             month == 6 ||
             month == 9 ||
             month == 11))
        {
            return String("ERROR! Month " + String(month) + " does not have 31 days.");
        }

        if (month == 2 && day == 29)
        {
            if (year % 4 != 0)
                return String("ERROR! Year " + String(year) + " is not a leap-year.\n February cannot have 29 days.");
        }
    }
    else
        return;

    RTC.adjust(DateTime(year, month, day, hour, minute, second));
};

String Clock::getDate()
{
    DateTime now = RTC.now();
    return String(now.hour()) + ":" +
           String(now.minute()) + ":" +
           String(now.second()) + " " +
           days[now.dayOfTheWeek()] + " " +
           String(now.day()) + "/" +
           String(now.month()) + "/" +
           String(now.year());
};