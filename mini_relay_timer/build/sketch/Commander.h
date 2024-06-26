#line 1 "/home/pablin/arduino_programs/mini_relay_timer/Commander.h"
/**************************************************************************/
/*!
  @file     Commander.h

  Original library by Pablin defining basic commands to be sent via serial port.

  License: MIT (see LICENSE)
*/
/**************************************************************************/

#ifndef _COMMANDER_H_
#define _COMMANDER_H_

#include "common.h"
#include "Clock.h"
#include "Relay.h"

extern Clock clock;
extern Relay relays;

/**************************************************************************/
/*!
    @brief  The main class containing basic command parsing and sanitizing
            logic.
*/
/**************************************************************************/
class Commander
{
private:
    /**
        Array of special chars allowed in inputs
    */
    const byte specialChars[5] = {0x20, 0x2d, 0x2e, 0x2f, 0x3a};

    const uint8_t MAX_COMMAND_LEN = 64;
    const uint8_t MAX_COMMAND_ARGS = 8;
    const char *DELIMITER = " ";
    bool charAllowed(char c);

public:
    Commander(/* args */);
    ~Commander();
    String getInput();
    void exec(String);
};

#endif // _COMMANDER_H_