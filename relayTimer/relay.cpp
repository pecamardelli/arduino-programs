#include "header.h"

extern NodeList nodes;

Relay::Relay(uint8_t newPin)
{
    data.pin = newPin;
    strcpy(data.desc, "No description");
    data.status = disabled;

    data.startHour = 0;
    data.startMin = 0;
    data.endHour = 0;
    data.endMin = 0;
}

Relay::~Relay()
{
}

uint8_t Relay::getPin()
{
    return data.pin;
}

uint8_t Relay::getMode()
{
    return digitalRead(data.pin);
}

String Relay::getDesc()
{
    return data.desc;
}

uint8_t Relay::getStartHour()
{
    return data.startHour;
}

uint8_t Relay::getStartMinute()
{
    return data.startMin;
}

uint8_t Relay::getEndHour()
{
    return data.endHour;
}

uint8_t Relay::getEndMinute()
{
    return data.endMin;
}

RelayStatus Relay::getStatus()
{
    return data.status;
}

String Relay::getStartTime()
{
    String starth = (data.startHour < 10) ? "0" + String(data.startHour) : String(data.startHour);
    String startm = (data.startMin < 10) ? "0" + String(data.startMin) : String(data.startMin);
    return String(starth + ":" + startm);
}

String Relay::getEndTime()
{
    String endh = (data.endHour < 10) ? "0" + String(data.endHour) : String(data.endHour);
    String endm = (data.endMin < 10) ? "0" + String(data.endMin) : String(data.endMin);
    return String(endh + ":" + endm);
}

String Relay::getUptime()
{
    if (getMode())
        return F("--d --:--:--");

    unsigned long diff = millis() - startTime;
    uint8_t _days = 0;
    uint8_t _hours = 0;
    uint8_t _mins = 0;
    uint8_t _secs = 0;

    while (diff >= 84600000)
    {
        diff -= 84600000;
        _days++;
    }

    while (diff >= 3600000)
    {
        diff -= 3600000;
        _hours++;
    }

    while (diff >= 60000)
    {
        diff -= 60000;
        _mins++;
    }

    while (diff >= 1000)
    {
        diff -= 1000;
        _secs++;
    }

    String d = (_days < 10) ? "0" + String(_days) : String(_days);
    String h = (_hours < 10) ? "0" + String(_hours) : String(_hours);
    String m = (_mins < 10) ? "0" + String(_mins) : String(_mins);
    String s = (_secs < 10) ? "0" + String(_secs) : String(_secs);

    return d + "d " + h + ":" + m + ":" + s;
}

// -------------------------------------------- //

String Relay::setPin(uint8_t newPin)
{
    if (nodes.isPinAvailable(newPin))
    {
        data.pin = newPin;
    }
    else
    {
        return F("Error: invalid pin number.");
    }
}

String Relay::switchOn()
{
    digitalWrite(data.pin, LOW);
    startTime = millis();

    return F("Relay switched on.");
}

String Relay::switchOff()
{
    digitalWrite(data.pin, HIGH);
    startTime = 0;

    return F("Relay switched off.");
}

String Relay::setDesc(char *newDesc)
{
    if (strlen(newDesc) > RELAY_DESC_LEN)
        return F("Error: description too long.");

    strcpy(data.desc, newDesc);
    return newDesc;
}

String Relay::setStartHour(uint8_t hour)
{
    if (hour < 0 || hour > 23)
        return F("Error: invalid hour.");

    data.startHour = hour;
}

String Relay::setStartMinute(uint8_t minute)
{
    if (minute < 0 || minute > 59)
        return F("Error: invalid minute.");

    data.startMin = minute;
}

String Relay::setEndHour(uint8_t hour)
{
    if (hour < 0 || hour > 23)
        return F("Error: invalid hour.");

    data.endHour = hour;
}

String Relay::setEndMinute(uint8_t minute)
{
    if (minute < 0 || minute > 59)
        return F("Error: invalid minute.");

    data.endMin = minute;
}