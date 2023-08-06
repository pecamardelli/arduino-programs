#line 1 "/home/pablin/arduino_programs/relayTimer/relay.cpp"
#include "header.h"

extern NodeList nodes;

Relay::Relay()
{
}

Relay::~Relay()
{
}

void Relay::setParams(relayData_t params)
{
    data.pin = params.pin;
    strcpy(data.desc, params.desc);
    data.status = params.status;

    data.startHour = params.startHour;
    data.startMin = params.startMin;
    data.endHour = params.endHour;
    data.endMin = params.endMin;
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

relayData_t Relay::getData()
{
    return data;
}

uint16_t Relay::getEepromAddress()
{
    return eeAddress;
}

// -------------------------------------------- //

String Relay::setPin(uint8_t newPin)
{
    if (nodes.isPinAvailable(newPin))
    {
        data.pin = newPin;

        nodes.saveRelay(this);

        return "Ok";
    }
    else
    {
        return F("Error: pin not available.");
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

String Relay::setDesc(String newDesc)
{
    if (newDesc.length() > RELAY_DESC_LEN)
        return F("Error: description too long.");

    newDesc.toCharArray(data.desc, RELAY_DESC_LEN);

    nodes.saveRelay(this);

    return "Ok";
}

String Relay::setStartHour(String startHour)
{
    const uint8_t hour = startHour.toInt();

    if (hour < 0 || hour > 23)
        return F("Error: invalid hour.");

    data.startHour = hour;

    nodes.saveRelay(this);

    return "Ok";
}

String Relay::setStartMinute(String startMinute)
{
    const uint8_t minute = startMinute.toInt();

    if (minute < 0 || minute > 59)
        return F("Error: invalid minute.");

    data.startMin = minute;

    nodes.saveRelay(this);

    return "Ok";
}

String Relay::setEndHour(String endHour)
{
    const uint8_t hour = endHour.toInt();

    if (hour < 0 || hour > 23)
        return F("Error: invalid hour.");

    data.endHour = hour;

    nodes.saveRelay(this);

    return "Ok";
}

String Relay::setEndMinute(String endMinute)
{
    const uint8_t minute = endMinute.toInt();

    if (minute < 0 || minute > 59)
        return F("Error: invalid minute.");

    data.endMin = minute;

    nodes.saveRelay(this);

    return "Ok";
}

String Relay::setStatus(RelayStatus newStatus)
{
    data.status = newStatus;

    nodes.saveRelay(this);

    return "Ok";
}