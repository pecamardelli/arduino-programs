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
    @brief  Turns on or off the relays based on the calculated start an end minute.
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

void Relay::setSeasonMinStartMinute(long minutes)
{

    if (minutes < 0 || minutes > 1440)
    {
        printErrorMessage(VALUE_OUT_OF_RANGE, String(minutes));
        return;
    }

    seasonMinStartMinute = minutes;
}

void Relay::setSeasonMaxStartMinute(long minutes)
{

    if (minutes < 0 || minutes > 1440)
    {
        printErrorMessage(VALUE_OUT_OF_RANGE, String(minutes));
        return;
    }

    seasonMaxStartMinute = minutes;
}

void Relay::setSeasonMinEndMinute(long minutes)
{

    if (minutes < 0 || minutes > 1440)
    {
        printErrorMessage(VALUE_OUT_OF_RANGE, String(minutes));
        return;
    }

    seasonMinEndMinute = minutes;
}

void Relay::setSeasonMaxEndMinute(long minutes)
{

    if (minutes < 0 || minutes > 1440)
    {
        printErrorMessage(VALUE_OUT_OF_RANGE, String(minutes));
        return;
    }

    seasonMaxEndMinute = minutes;
}

void Relay::setSeasonShortestDayOfTheYear(uint16_t day)
{

    if (day < 0 || day > 366)
    {
        printErrorMessage(VALUE_OUT_OF_RANGE, String(day));
        return;
    }

    seasonShortestDayOfTheYear = day;
}

void Relay::setSeasonLongestDayOfTheYear(uint16_t day)
{

    if (day < 0 || day > 366)
    {
        printErrorMessage(VALUE_OUT_OF_RANGE, String(day));
        return;
    }

    seasonLongestDayOfTheYear = day;
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
                printSeparator();
                Serial.print(F("rangeMaxTimeVariation: "));
                Serial.println(rangeMaxTimeVariation);
                printSeparator();

                Serial.print(F("seasonMinStartMinute: "));
                Serial.println(seasonMinStartMinute);
                Serial.print(F("seasonMaxStartMinute: "));
                Serial.println(seasonMaxStartMinute);
                Serial.print(F("seasonStartTimespan: "));
                Serial.println(seasonStartTimespan);
                printSeparator();

                Serial.print(F("seasonMinEndMinute: "));
                Serial.println(seasonMinEndMinute);
                Serial.print(F("seasonMaxEndMinute: "));
                Serial.println(seasonMaxEndMinute);
                Serial.print(F("seasonEndTimespan: "));
                Serial.println(seasonEndTimespan);
                printSeparator();

                Serial.print(F("seasonShortestDayOfTheYear: "));
                Serial.println(seasonShortestDayOfTheYear);
                printSeparator();

                Serial.print(F("seasonLongestDayOfTheYear: "));
                Serial.println(seasonLongestDayOfTheYear);
                printSeparator();

                return COMMAND_SUCCESSFUL;
            }
        }
    }
    else if (args[0].equals(F("set")))
    {
        if (args[1].equals(F("seasonMinStartMinute")))
        {
            if (args[2].length() == 0)
            {
                printErrorMessage(TOO_FEW_ARGUMENTS, args[0] + " " + args[1]);
                return TOO_FEW_ARGUMENTS;
            }

            setSeasonMinStartMinute(args[2].toInt());

            return COMMAND_SUCCESSFUL;
        }
        else if (args[1].equals(F("seasonMaxStartMinute")))
        {
            if (args[2].length() == 0)
            {
                printErrorMessage(TOO_FEW_ARGUMENTS, args[0] + " " + args[1]);
                return TOO_FEW_ARGUMENTS;
            }

            setSeasonMaxStartMinute(args[2].toInt());

            return COMMAND_SUCCESSFUL;
        }
        else if (args[1].equals(F("seasonMinEndMinute")))
        {
            if (args[2].length() == 0)
            {
                printErrorMessage(TOO_FEW_ARGUMENTS, args[0] + " " + args[1]);
                return TOO_FEW_ARGUMENTS;
            }

            setSeasonMinEndMinute(args[2].toInt());

            return COMMAND_SUCCESSFUL;
        }
        else if (args[1].equals(F("seasonMaxEndMinute")))
        {
            if (args[2].length() == 0)
            {
                printErrorMessage(TOO_FEW_ARGUMENTS, args[0] + " " + args[1]);
                return TOO_FEW_ARGUMENTS;
            }

            setSeasonMaxEndMinute(args[2].toInt());

            return COMMAND_SUCCESSFUL;
        }
        else if (args[1].equals(F("seasonShortestDayOfTheYear")))
        {
            if (args[2].length() == 0)
            {
                printErrorMessage(TOO_FEW_ARGUMENTS, args[0] + " " + args[1]);
                return TOO_FEW_ARGUMENTS;
            }

            setSeasonShortestDayOfTheYear(args[2].toInt());

            return COMMAND_SUCCESSFUL;
        }
        else if (args[1].equals(F("seasonLongestDayOfTheYear")))
        {
            if (args[2].length() == 0)
            {
                printErrorMessage(TOO_FEW_ARGUMENTS, args[0] + " " + args[1]);
                return TOO_FEW_ARGUMENTS;
            }

            setSeasonLongestDayOfTheYear(args[2].toInt());

            return COMMAND_SUCCESSFUL;
        }
    }

    return NO_COMMAND;
}