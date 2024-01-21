#line 1 "/home/pablin/arduino_programs/mini_relay_timer/Relay.cpp"
#include "Relay.h"

Relay::Relay(/* args */)
{
}

Relay::~Relay()
{
}

/**************************************************************************/
/*!
    @brief  Initializes all the stuff for the relays.
*/
/**************************************************************************/
void Relay::begin()
{
    // External lights
    relayArray[0].type = RELAY_TYPE_SEASON;
    relayArray[0].pin = pinExternalLights;
    relayArray[0].enabled = true;

    // Streetlight
    relayArray[1].type = RELAY_TYPE_SEASON;
    relayArray[1].pin = pinGardenLights;
    relayArray[1].enabled = true;

    // Internal lights
    relayArray[2].type = RELAY_TYPE_RANGE;
    relayArray[2].duration = 240;
    relayArray[2].pin = pinInternalLights;
    relayArray[2].enabled = true;

    pinMode(pinExternalLights, OUTPUT);
    digitalWrite(pinExternalLights, HIGH);

    pinMode(pinGardenLights, OUTPUT);
    digitalWrite(pinGardenLights, HIGH);

    pinMode(pinInternalLights, OUTPUT);
    digitalWrite(pinInternalLights, HIGH);
}

/**************************************************************************/
/*!
    @brief  Checks the start and end minute for all the relays.
*/
/**************************************************************************/
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
            if (seasonStartMinute - rangeMaxTimeVariation <= currentMinute)
            {
                continue;
            }
            relayArray[i].startMinute = seasonStartMinute - round(random(rangeMaxTimeVariation));
            relayArray[i].endMinute = relayArray[i].startMinute + relayArray[i].duration + round(rangeMaxTimeVariation * random(-1, 1));
            if (relayArray[i].endMinute >= 1440)
                relayArray[i].endMinute = 1438;
            break;
        default:
            break;
        }
}

/**************************************************************************/
/*!
    @brief  Turns on or off the relays.
*/
/**************************************************************************/
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
    float ratio = sin(PI * dayOfTheYear / seasonLongestDayOfTheYear);
    return (uint16_t)seasonMaxStartMinute - round(seasonStartTimespan * ratio);
}

uint16_t Relay::getSeasonEndMins()
{
    uint16_t dayOfTheYear = clock.calculateDayOfYear();
    float ratio = sin(PI * dayOfTheYear / seasonLongestDayOfTheYear);
    return (uint16_t)seasonMinEndMinute + round(seasonStartTimespan * ratio);
}

/**************************************************************************/
/*!
    @brief  Executes a user command.
    @param  command Array of strings representing the command itself and its arguments.
*/
/**************************************************************************/
EXEC_STATUSES Relay::exec(String args[])
{
    if (args[0].equals("show"))
    {
        if (args[1].equals("relay"))
        {
            if (args[2].length() == 0)
            {
                printErrorMessage(TOO_FEW_ARGUMENTS, args[0] + " " + args[1]);
                return TOO_FEW_ARGUMENTS;
            }
            if (args[2].equals("values"))
            {
                Serial.println("Relay values");
                return COMMAND_SUCCESSFUL;
            }
            else
            {
                printErrorMessage(BAD_COMMAND, args[2]);
                return BAD_COMMAND;
            }
        }
        else
        {
            printErrorMessage(BAD_COMMAND, args[1]);
            return BAD_COMMAND;
        }
    }

    return NO_COMMAND;
}