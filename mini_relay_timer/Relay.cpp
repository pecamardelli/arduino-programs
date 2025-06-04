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
    relayArray[2].enabled = false;

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
    {
        // Skip if relay is disabled
        if (!relayArray[i].enabled)
        {
            continue;
        }

        switch (relayArray[i].type)
        {
        case RELAY_TYPE_FIXED:
            // Fixed type doesn't need updates
            continue;

        case RELAY_TYPE_SEASON:
            // Simple seasonal adjustment
            relayArray[i].startMinute = getSeasonStartMins();
            relayArray[i].endMinute = getSeasonEndMins();
            break;

        case RELAY_TYPE_RANGE:
            uint16_t currentMinute = clock.RTC.now().hour() * 60 + clock.RTC.now().minute();
            uint16_t seasonStartMinute = getSeasonStartMins();

            // Calculate new random start time
            int16_t randomVariation = round(random(rangeMaxTimeVariation));
            if (seasonStartMinute > randomVariation)
            {
                relayArray[i].startMinute = seasonStartMinute - randomVariation;
            }
            else
            {
                relayArray[i].startMinute = 0;
            }

            // Calculate end time with bounds checking
            int16_t endTimeVariation = round(rangeMaxTimeVariation * random(-1, 1));
            uint16_t calculatedEnd = relayArray[i].startMinute + relayArray[i].duration + endTimeVariation;
            relayArray[i].endMinute = min(1439, calculatedEnd); // 1439 = 23:59
            break;

        default:
            // Log unexpected relay type
            Serial.print(F("Warning: Unknown relay type for relay "));
            Serial.println(i);
            break;
        }
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
        // Skip disabled relays
        if (!relayArray[i].enabled)
        {
            digitalWrite(relayArray[i].pin, HIGH); // Turn OFF
            continue;
        }

        // Skip invalid times
        if (isnan(relayArray[i].startMinute) || isnan(relayArray[i].endMinute))
            continue;

        bool shouldBeOn = false;

        // Normal case (e.g., 18:00 to 23:00)
        if (relayArray[i].startMinute < relayArray[i].endMinute)
        {
            shouldBeOn = (currentMinute >= relayArray[i].startMinute && currentMinute < relayArray[i].endMinute);
        }
        // Overnight case (e.g., 23:00 to 06:00)
        else
        {
            shouldBeOn = (currentMinute >= relayArray[i].startMinute || currentMinute < relayArray[i].endMinute);
        }

        digitalWrite(relayArray[i].pin, shouldBeOn ? LOW : HIGH); // CORRECTED COMMENT: LOW = ON, HIGH = OFF
    }
}

uint16_t Relay::getSeasonStartMins()
{
    uint16_t dayOfTheYear = clock.calculateDayOfYear();
    float ratio = sin(PI * dayOfTheYear / 365.0);
    return (uint16_t)seasonMinStartMinute + round(seasonStartTimespan * ratio);
}

uint16_t Relay::getSeasonEndMins()
{
    uint16_t dayOfTheYear = clock.calculateDayOfYear();
    float ratio = sin(PI * dayOfTheYear / 365.0);
    return (uint16_t)seasonMaxEndMinute - round(seasonEndTimespan * ratio);
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
            else if (args[2].equals("status"))
            {
                uint16_t currentMinute = clock.RTC.now().hour() * 60 + clock.RTC.now().minute();
                uint16_t dayOfYear = clock.calculateDayOfYear();

                printSeparator();
                Serial.print(F("Current time: "));
                Serial.print(currentMinute / 60);
                Serial.print(F(":"));
                if (currentMinute % 60 < 10)
                    Serial.print("0");
                Serial.print(currentMinute % 60);
                Serial.print(F(" ("));
                Serial.print(currentMinute);
                Serial.println(F(" minutes)"));

                Serial.print(F("Day of year: "));
                Serial.println(dayOfYear);
                printSeparator();

                for (uint8_t i = 0; i < MAX_RELAY_NUMBER; i++)
                {
                    Serial.print(F("Relay "));
                    Serial.print(i);
                    Serial.print(F(" ("));
                    switch (relayArray[i].type)
                    {
                    case RELAY_TYPE_FIXED:
                        Serial.print(F("FIXED"));
                        break;
                    case RELAY_TYPE_SEASON:
                        Serial.print(F("SEASON"));
                        break;
                    case RELAY_TYPE_RANGE:
                        Serial.print(F("RANGE"));
                        break;
                    default:
                        Serial.print(F("UNKNOWN"));
                        break;
                    }
                    Serial.println(F(")"));

                    Serial.print(F("  Enabled: "));
                    Serial.println(relayArray[i].enabled ? F("Yes") : F("No"));

                    Serial.print(F("  Start: "));
                    Serial.print(relayArray[i].startMinute / 60);
                    Serial.print(F(":"));
                    if (relayArray[i].startMinute % 60 < 10)
                        Serial.print("0");
                    Serial.print(relayArray[i].startMinute % 60);
                    Serial.print(F(" ("));
                    Serial.print(relayArray[i].startMinute);
                    Serial.println(F(" minutes)"));

                    Serial.print(F("  End: "));
                    Serial.print(relayArray[i].endMinute / 60);
                    Serial.print(F(":"));
                    if (relayArray[i].endMinute % 60 < 10)
                        Serial.print("0");
                    Serial.print(relayArray[i].endMinute % 60);
                    Serial.print(F(" ("));
                    Serial.print(relayArray[i].endMinute);
                    Serial.println(F(" minutes)"));

                    Serial.print(F("  State: "));
                    Serial.println(digitalRead(relayArray[i].pin) == LOW ? F("ON") : F("OFF"));
                    printSeparator();
                }
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