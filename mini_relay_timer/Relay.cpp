#include "Relay.h"

Relay::Relay(/* args */)
{
}

Relay::~Relay()
{
}

void Relay::begin()
{
    // External lights
    relayArray[0].type = RELAY_TYPE_SEASON;
    relayArray[0].pin = PIN_EXTERNAL_LIGHTS;
    relayArray[0].enabled = true;

    // Streetlight
    relayArray[1].type = RELAY_TYPE_SEASON;
    relayArray[1].pin = PIN_GARDEN_LIGHTS;
    relayArray[1].enabled = true;

    // Internal lights
    relayArray[2].type = RELAY_TYPE_RANGE;
    relayArray[2].duration = 240;
    relayArray[2].pin = PIN_INTERNAL_LIGHTS;
    relayArray[2].enabled = true;

    pinMode(PIN_EXTERNAL_LIGHTS, OUTPUT);
    digitalWrite(PIN_EXTERNAL_LIGHTS, HIGH);

    pinMode(PIN_GARDEN_LIGHTS, OUTPUT);
    digitalWrite(PIN_GARDEN_LIGHTS, HIGH);

    pinMode(PIN_INTERNAL_LIGHTS, OUTPUT);
    digitalWrite(PIN_INTERNAL_LIGHTS, HIGH);
}

void Relay::relay_check()
{
    for (uint8_t i = 0; i < MAX_RELAY_NUMBER; i++)
        switch (relayArray[i].type)
        {
        case RELAY_TYPE_FIXED:
            continue;
        case RELAY_TYPE_SEASON:
            relayArray[i].startMinute = getSeasonStartMins();
            relayArray[i].endMinute = getSeasonEndMins();
            break;
        case RELAY_TYPE_RANGE:
            uint16_t currentMinute = clock.RTC.now().hour() * 60 + clock.RTC.now().minute();
            uint16_t seasonStartMinute = getSeasonStartMins();
            if (seasonStartMinute - RANGE_MAX_TIME_VARIATION <= currentMinute)
            {
                continue;
            }
            relayArray[i].startMinute = seasonStartMinute - round(random(RANGE_MAX_TIME_VARIATION));
            relayArray[i].endMinute = relayArray[i].startMinute + relayArray[i].duration + round(RANGE_MAX_TIME_VARIATION * random(-1, 1));
            if (relayArray[i].endMinute >= 1440)
                relayArray[i].endMinute = 1438;
            break;
        default:
            break;
        }
}

void Relay::relay_watch()
{
    uint16_t currentMinute = clock.RTC.now().hour() * 60 + clock.RTC.now().minute();

    for (uint8_t i = 0; i < MAX_RELAY_NUMBER; i++)
    {
        if (!relayArray[i].enabled)
        {
            digitalWrite(relayArray[i].pin, HIGH);
            continue;
        }

        if (isnan(relayArray[i].startMinute) || isnan(relayArray[i].endMinute))
            continue;

        if (relayArray[i].startMinute <= currentMinute)
        {
            if (relayArray[i].endMinute > relayArray[i].startMinute)
            {
                if (relayArray[i].endMinute < currentMinute)
                {
                    digitalWrite(relayArray[i].pin, HIGH);
                }
                else
                {
                    digitalWrite(relayArray[i].pin, LOW);
                }
            }
            else
            {
                digitalWrite(relayArray[i].pin, LOW);
            }
        }
        else
        {
            if (relayArray[i].startMinute < relayArray[i].endMinute)
            {
                digitalWrite(relayArray[i].pin, HIGH);
            }
            else
            {
                if (relayArray[i].endMinute > currentMinute)
                {
                    digitalWrite(relayArray[i].pin, LOW);
                }
                else
                {
                    digitalWrite(relayArray[i].pin, HIGH);
                }
            }
        }
    }
}

uint16_t Relay::getSeasonStartMins()
{
    uint16_t dayOfTheYear = clock.calculateDayOfYear();
    float ratio = sin(PI * dayOfTheYear / SEASON_LONGEST_DAY_OF_THE_YEAR);
    return (uint16_t)SEASON_MAX_START_MINUTE - round(SEASON_START_TIMESPAN * ratio);
}

uint16_t Relay::getSeasonEndMins()
{
    uint16_t dayOfTheYear = clock.calculateDayOfYear();
    float ratio = sin(PI * dayOfTheYear / SEASON_LONGEST_DAY_OF_THE_YEAR);
    return (uint16_t)SEASON_MIN_END_MINUTE + round(SEASON_START_TIMESPAN * ratio);
}