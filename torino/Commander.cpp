/**************************************************************************/
/*!
  @file     Commander.cpp

  @section intro Introduction

  Base library intended to set a basic command logic.

  @section license License

  Original library by Pablin defining basic commands to be sent via serial port.

  This version: MIT (see LICENSE)
*/
/**************************************************************************/

#include "Commander.h"

float average = 0;
float lastAverage = 0;

Commander::Commander(/* args */)
{
}

Commander::~Commander()
{
}

/**************************************************************************/
/*!
    @brief  Reads the serial port and sanitizes the input.
    @return Stringified and sanitized input.
*/
/**************************************************************************/
String Commander::getInput()
{
    if (Serial.available() <= 0)
        return;

    char c;
    String input;

    while (Serial.available() > 0)
    {
        c = Serial.read();
        // Serial.print(String(int(c)) + " ");
        //  Include letters, digits, and other allowed chars. Add more allowed characters
        //  at the definition of the specialChars array.
        if (isalpha(c) || isdigit(c) || charAllowed(c))
            input.concat(c);
        else
            input.concat((char)0x20);

        delay(5);
    }

    return input;
};

/**************************************************************************/
/*!
    @brief  Check if a received char is allowed. It checks if it's included
            in the 'specialChars' array.
    @param  c The received character.
    @return boolean value
*/
/**************************************************************************/
bool Commander::charAllowed(char c)
{
    for (uint8_t i = 0; i < sizeof(specialChars) / sizeof(byte); i++)
    {
        if ((int)c == specialChars[i])
            return true;
    }
    return false;
};

/**************************************************************************/
/*!
    @brief  Process the command and prints the result.
    @param  input The command.
    @return nothing
*/
/**************************************************************************/
void Commander::exec(String input)
{
    // Tokenizing arguments
    char *args[MAX_COMMAND_ARGS];
    char buffer[MAX_COMMAND_LEN];
    uint8_t index = 0;

    input.toCharArray(buffer, MAX_COMMAND_LEN);

    char *token = strtok(buffer, DELIMITER);

    while (token != NULL && index < MAX_COMMAND_ARGS)
    {
        args[index] = token;
        index++;
        token = strtok(NULL, DELIMITER);
    }

    if (strncmp(args[0], "date", 4) == 0 || strncmp(args[0], "time", 4) == 0)
    {
        Serial.println(clock.getDate());
    }
    else if (strncmp(args[0], "avg", 3) == 0)
    {
        average = atof(args[1]);

        if (average != lastAverage)
        {
            lastAverage = average;
            // drawFuelConsumption(average);
        }
    }
    else if (strncmp(args[0], "set", 3) == 0)
    {
        if (strncmp(args[1], "ppl", 3) == 0)
        {
            flowmeter.setPulsesPerLiter(atoi(args[2]));
        }
        else if (strncmp(args[1], "date", 4) == 0)
        {
            clock.setDateTime(args[2], args[3]);
        }
        else if (strncmp(args[1], "clock", 5) == 0)
        {
            if (strncmp(args[2], "display", 7) == 0)
            {
                if (strncmp(args[3], "time_x", 6) == 0)
                {
                    clock.setTimeDisplayX(atoi(args[4]));
                }
                else if (strncmp(args[3], "time_y", 6) == 0)
                {
                    clock.setTimeDisplayY(atoi(args[4]));
                }
                else if (strncmp(args[3], "time_size", 9) == 0)
                {
                    clock.setTimeDisplaySize(atoi(args[4]));
                }
                else
                {
                    printErrorMessage(BAD_COMMAND, args[3]);
                }
            }
            else
            {
                printErrorMessage(BAD_COMMAND, args[2]);
            }
        }
        else
        {
            printErrorMessage(BAD_COMMAND, args[1]);
        }
    }
    else if (strncmp(args[0], "reset", 5) == 0)
    {
        if (strncmp(args[1], "clock", 5) == 0)
        {
            if (strncmp(args[2], "display", 7) == 0)
            {
                clock.resetProps();
            }
            else
            {
                printErrorMessage(BAD_COMMAND, args[2]);
            }
        }
        else
        {
            printErrorMessage(BAD_COMMAND, args[1]);
        }
    }
    else if (strncmp(args[0], "show", 4) == 0)
    {
        if (strncmp(args[1], "ppl", 3) == 0)
        {
            Serial.print(F("Current pulses per liter: "));
            Serial.println(flowmeter.getPulsesPerLiter());
        }
        else
        {
            printErrorMessage(BAD_COMMAND, args[1]);
        }
    }
    else if (strncmp(args[0], "reset", 5) == 0)
    {
        if (strncmp(args[1], "pulses", 6) == 0)
        {
            flowmeter.resetPulses();
            Serial.println(F("Pulses reset."));
        }
        else
        {
            printErrorMessage(BAD_COMMAND, args[1]);
        }
    }
    else
    {
        printErrorMessage(BAD_COMMAND, args[0]);
    }
}

/**************************************************************************/
/*!
    @brief  Print a standard error message.
    @param  message The error message.
    @return nothing
*/
/**************************************************************************/
void Commander::printErrorMessage(ERROR_TYPES type, String message)
{
    switch (type)
    {
    case BAD_COMMAND:
        Serial.print(F("Bad command: "));
        break;
    default:
        break;
    }

    Serial.println(message);
}
