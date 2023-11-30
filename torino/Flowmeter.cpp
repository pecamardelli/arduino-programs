#include "Flowmeter.h"

Flowmeter::Flowmeter(/* args */)
{
}

Flowmeter::~Flowmeter()
{
}

/**************************************************************************/
/*!
    @brief  Sets the number of pulses per liter of the fuel meter.
*/
/**************************************************************************/
void Flowmeter::setPulsesPerLiter(unsigned short pulses)
{
    pulsesPerLiter = pulses;
}

/**************************************************************************/
/*!
    @brief  Gets the number of pulses per liter of the fuel meter.
    @return The current number of pulses per liter.
*/
/**************************************************************************/
unsigned short Flowmeter::getPulsesPerLiter()
{
    return pulsesPerLiter;
}

/**************************************************************************/
/*!
    @brief  Sets the pin number where the flowmeter should be connected.
*/
/**************************************************************************/
void Flowmeter::setPin(unsigned char newPin)
{
    pin = newPin;
}

/**************************************************************************/
/*!
    @brief  Gets the pin number where the flowmenter should be connected.
    @return The current pin number of the flowmenter.
*/
/**************************************************************************/
unsigned char Flowmeter::getPin()
{
    return pin;
}

/**************************************************************************/
/*!
    @brief  Checks the flowmenter for pulses.
*/
/**************************************************************************/
void Flowmeter::checkPulse()
{
    currentPinState = digitalRead(pin);
    if (currentPinState != lastPinState)
    {
        lastPinState = currentPinState;
        pulses++;
    }
}

/**************************************************************************/
/*!
    @brief  Sets the number of registered pulses to 0.
*/
/**************************************************************************/
void Flowmeter::resetPulses()
{
    pulses = 0;
}
