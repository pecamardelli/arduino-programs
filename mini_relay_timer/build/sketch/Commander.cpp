#line 1 "/home/pablin/arduino_programs/mini_relay_timer/Commander.cpp"
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

    uint8_t result = clock.exec(args);

    if (result == COMMAND_SUCCESSFUL)
    {
        return;
    }

    result = relays.exec(args);
    if (result == COMMAND_SUCCESSFUL)
    {
        return;
    }

    if (result >= 0)
    {

        printErrorMessage(BAD_COMMAND, args[result]);
    }
}

/**************************************************************************/
/*!
    @brief  Print a standard error message.
    @param  message The error message.
    @return nothing
*/
/**************************************************************************/
void Commander::printErrorMessage(EXEC_STATUSES type, String message)
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
