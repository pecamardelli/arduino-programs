#include "header.h"

Relay::Relay(uint8_t newPin)
{
    pin = newPin;
    strcpy(desc, "No description");
    deleted = false;
    enabled = false;

    startHour = 0;
    startMin = 0;
    endHour = 0;
    endMin = 0;
}

Relay::~Relay()
{
}

uint8_t Relay::setPin(uint8_t newPin)
{
}

uint8_t Relay::getPin()
{
    return pin;
}

char *Relay::setMode(uint8_t mode)
{
    if (mode == HIGH || mode == LOW)
        digitalWrite(pin, mode);
    else
        return "Error: invalid mode";
}