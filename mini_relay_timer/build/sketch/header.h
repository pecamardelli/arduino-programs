#line 1 "/home/pablin/arduino_programs/mini_relay_timer/header.h"
#ifndef HEADER_H
#define HEADER_H

#include <Wire.h> //https://www.arduino.cc/en/reference/wire
#include <RTClib.h>

RTC_DS3231 rtc;

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
} relay;

// Relay Pins
const uint8_t PIN_EXTERNAL_LIGHTS = 5;
const uint8_t PIN_GARDEN_LIGHTS = 6;
const uint8_t PIN_INTERNAL_LIGHTS = 7;

// Relay types
//  - FIXED: starts at a fixed time of the day.
//  - AUTO: starts at a variable time of the day depending on year season.
//  - RANGE: starts at a variable time within a specified range.
const uint8_t RELAY_TYPE_FIXED = 0x00;
const uint8_t RELAY_TYPE_SEASON = 0x01;
const uint8_t RELAY_TYPE_RANGE = 0x02;

// Number of days of each month.
uint8_t daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
// Season time range
const uint16_t SEASON_MIN_START_MINUTE = 18 * 60 + 30; // 18:30 hs
const uint16_t SEASON_MAX_START_MINUTE = 20 * 60 + 45; // 20:45 hs
const uint16_t SEASON_START_TIMESPAN = SEASON_MAX_START_MINUTE - SEASON_MIN_START_MINUTE;

const uint16_t SEASON_MIN_END_MINUTE = 6 * 60; // 06:00 hs
const uint16_t SEASON_MAX_END_MINUTE = 8 * 60; // 08:00 hs
const uint16_t SEASON_END_TIMESPAN = SEASON_MAX_END_MINUTE - SEASON_MIN_END_MINUTE;
// June 21st is the shortest day of the year.
const uint16_t SEASON_SHORTEST_DAY_OF_THE_YEAR = 31 * 3 + 28 + 30 + 21;
// December 21st would be the longest one, but we'll use 365 instead.
const uint16_t SEASON_LONGEST_DAY_OF_THE_YEAR = 355;

// Max variation in minutes of the start and end time when type is RANGE
const uint8_t RANGE_MAX_TIME_VARIATION = 30;

// Defining array of relays.
const size_t ARRAY_LENGTH = 4;
relay relayArray[ARRAY_LENGTH];

// Datetime variable used globally
DateTime now;

#endif