/**************************************************************************/
/*!
  @file     Flowmeter.h

  Original library by Pablin defining the logic to measure fuel consumption.

  License: MIT (see LICENSE)
*/
/**************************************************************************/

#ifndef _FLOWMETER_H_
#define _FLOWMETER_H_

#include <Arduino.h>

/**************************************************************************/
/*!
    @brief  This class defines the logic to calculate and print fuel
            consumption on the screen.
*/
/**************************************************************************/
class Flowmeter
{
private:
  uint64_t pulses = 0;
  uint16_t pulsesPerLiter = 287; // Default experimental value.
  uint8_t pin = NULL;
  int lastPinState = NULL;
  int currentPinState = NULL;

public:
  Flowmeter(/* args */);
  ~Flowmeter();
  void setPulsesPerLiter(unsigned short);
  unsigned short getPulsesPerLiter();
  void setPin(unsigned char);
  unsigned char getPin();
  void checkPulse();
  void resetPulses();
};

#endif // _FLOWMETER_H_