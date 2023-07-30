#ifndef HEADER_H
#define HEADER_H

#include <Wire.h> //https://www.arduino.cc/en/reference/wire
#include <RTClib.h>

RTC_DS3231 rtc;

typedef struct _relay
{
    uint8_t startHour;
    uint8_t endHour;
    uint8_t startMinute;
    uint8_t endMinute;
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

// Defining array of relays.
const size_t ARRAY_LENGTH = 4;
relay relayArray[ARRAY_LENGTH];

#endif