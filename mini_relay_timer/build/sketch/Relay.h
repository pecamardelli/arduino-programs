#line 1 "/home/pablin/arduino_programs/mini_relay_timer/Relay.h"
/**************************************************************************/
/*!
  @file     Relay.h

  Original library by Pablin defining all variables, structs and functions
  used by the relays.

  License: MIT (see LICENSE)
*/
/**************************************************************************/

#ifndef _RELAY_H_
#define _RELAY_H_

#include <Wire.h> //https://www.arduino.cc/en/reference/wire
#include "common.h"
#include "Clock.h"

extern Clock clock;

typedef struct _relay
{
    // Time vars
    uint16_t startMinute = NAN;
    uint16_t endMinute = NAN;
    // Only used when type is RANGE
    uint16_t duration;
    // General vars
    uint8_t type;
    uint8_t pin;
    bool enabled = false;
} relay;

// Relay types
//  - FIXED: starts at a fixed time of the day.
//  - AUTO: starts at a variable time of the day depending on year season.
//  - RANGE: starts at a variable time within a specified range.
const uint8_t RELAY_TYPE_FIXED = 0x00;
const uint8_t RELAY_TYPE_SEASON = 0x01;
const uint8_t RELAY_TYPE_RANGE = 0x02;

const uint8_t MAX_RELAY_NUMBER = 4;

class Relay
{
private:
    /* data */
    uint16_t getSeasonStartMins();
    uint16_t getSeasonEndMins();
    // Defining array of relays.
    relay relayArray[MAX_RELAY_NUMBER];
    // Max variation in minutes of the start and end time when type is RANGE
    uint8_t rangeMaxTimeVariation = 30;
    // Season time range
    uint16_t seasonMinStartMinute = 19 * 60;      // 19:00 hs
    uint16_t seasonMaxStartMinute = 21 * 60 + 15; // 20:45 hs
    uint16_t seasonStartTimespan = seasonMaxStartMinute - seasonMinStartMinute;
    uint16_t seasonMinEndMinute = 6 * 60;      // 06:00 hs
    uint16_t seasonMaxEndMinute = 8 * 60 + 20; // 08:20 hs
    uint16_t seasonEndTimespan = seasonMaxEndMinute - seasonMinEndMinute;
    // June 21st is the shortest day of the year.
    uint16_t seasonShortestDayOfTheYear = 31 * 3 + 28 + 30 + 21;
    // December 21st would be the longest one.
    uint16_t seasonLongestDayOfTheYear = 355;
    // Relay Pins
    uint8_t pinExternalLights = 5;
    uint8_t pinGardenLights = 6;
    uint8_t pinInternalLights = 7;

public:
    Relay(/* args */);
    ~Relay();
    void begin();
    void relay_check();
    void relay_watch();
    EXEC_STATUSES exec(String[]);
};

#endif // _RELAY_H_