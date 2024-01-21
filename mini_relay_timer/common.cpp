#include "common.h"

/**************************************************************************/
/*!
    @brief  Print a standard error message.
    @param  message The error message.
    @return nothing
*/
/**************************************************************************/
void printErrorMessage(EXEC_STATUSES type, String message)
{
    switch (type)
    {
    case BAD_COMMAND:
        Serial.print(F("Bad command: "));
        break;
    case TOO_FEW_ARGUMENTS:
        Serial.print(F("Too few arguments for "));
        break;
    default:
        break;
    }

    Serial.println(message);
}