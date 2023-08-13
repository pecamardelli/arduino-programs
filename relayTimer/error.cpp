#include "header.h"

Error::Error(/* args */)
{
}

Error::~Error()
{
}

void Error::print(ErrorTypes type)
{
    Serial.print(F("Error! "));

    switch (type)
    {
    case pinNotAvailable:
        Serial.println(F("pin not available"));
        break;
    case pinOutOfRange:
        Serial.println(F("pin number out of range"));
        break;
    case maxRelayNumberReached:
        Serial.println(F("max relay number reached"));
        break;

    default:
        break;
    }
}
