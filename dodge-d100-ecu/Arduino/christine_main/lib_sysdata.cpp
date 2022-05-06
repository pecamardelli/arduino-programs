#include "lib_sysdata.h"

/*
 * We assume that we'll never use a screen larger than that
 */
#define   MAX_IMAGE_WIDTH   1024
#define   MAX_IMAGE_HEIGHT  1024

#if defined(ARDUINO_AVR_MEGA2560)
  #define   MAX_DIGITAL_PINS        53
  #define   MAX_ANALOG_PINS         16
#elif defined(ARDUINO_AVR_UNO)
  #define   MAX_DIGITAL_PINS        13
  #define   MAX_ANALOG_PINS         6
#endif

SYSDATA::SYSDATA()
{}

void  SYSDATA::setNumberWidth(uint16_t width)
{
  if(width > 0 && width < MAX_IMAGE_WIDTH)
  {
    sysd.numWidth  = width;
    Serial.print(F("Number width set correctly: "));
    Serial.println(sysd.numWidth);
  }
  else
  {
    sysd.numWidth  = 0;
    Serial.print(F("Number width not set. Value out of range: "));
    Serial.println(width);
  }
}

void  SYSDATA::setNumberHeight(uint16_t height)
{
  if(height > 0 && height < MAX_IMAGE_HEIGHT)
  {
    sysd.numHeight  = height;
    Serial.print(F("Number height set correctly: "));
    Serial.println(sysd.numHeight);
  }
  else
  {
    sysd.numHeight  = 0;
    Serial.print(F("Number height not set. Value out of range: "));
    Serial.println(height);
  }
}

void  SYSDATA::setCharWidth(uint16_t width)
{
  if(width > 0 && width < MAX_IMAGE_WIDTH)
  {
    sysd.charWidth  = width;
    Serial.print(F("Char width set correctly: "));
    Serial.println(sysd.charWidth);
  }
  else
  {
    sysd.charWidth  = 0;
    Serial.print(F("Char width not set. Value out of range: "));
    Serial.println(width);
  }
}

void  SYSDATA::setCharHeight(uint16_t height)
{
  if(height > 0 && height < MAX_IMAGE_HEIGHT)
  {
    sysd.charHeight  = height;
    Serial.print(F("Char height set correctly: "));
    Serial.println(sysd.charHeight);
  }
  else
  {
    sysd.charHeight  = 0;
    Serial.print(F("Char height not set. Value out of range: "));
    Serial.println(height);
  }
}

void  SYSDATA::setDotsWidth(uint16_t width)
{
  if(width > 0 && width < MAX_IMAGE_WIDTH)
  {
    sysd.dotsWidth  = width;
    Serial.print(F("Double dots width set correctly: "));
    Serial.println(sysd.dotsWidth);
  }
  else
  {
    sysd.dotsWidth  = 0;
    Serial.print(F("Double dots width not set. Value out of range: "));
    Serial.println(width);
  }
}

void  SYSDATA::setDotsHeight(uint16_t height)
{
  if(height > 0 && height < MAX_IMAGE_HEIGHT)
  {
    sysd.dotsHeight  = height;
    Serial.print(F("Double dots height set correctly: "));
    Serial.println(sysd.dotsHeight);
  }
  else
  {
    sysd.dotsHeight  = 0;
    Serial.print(F("Double dots height not set. Value out of range: "));
    Serial.println(height);
  }
}

void  SYSDATA::setSymbolWidth(uint16_t width)
{
  if(width > 0 && width < MAX_IMAGE_WIDTH)
  {
    sysd.symWidth  = width;
    Serial.print(F("Symbol width set correctly: "));
    Serial.println(sysd.symWidth);
  }
  else
  {
    sysd.symWidth  = 0;
    Serial.print(F("Symbol width not set. Value out of range: "));
    Serial.println(width);
  }
}

void  SYSDATA::setSymbolHeight(uint16_t height)
{
  if(height > 0 && height < MAX_IMAGE_HEIGHT)
  {
    sysd.symHeight  = height;
    Serial.print(F("Symbol height set correctly: "));
    Serial.println(sysd.symHeight);
  }
  else
  {
    sysd.symHeight  = 0;
    Serial.print(F("Symbol height not set. Value out of range: "));
    Serial.println(height);
  }
}

uint8_t  SYSDATA::getButtonPin(uint8_t button)
{
  if(button < MAX_BUTTONS)
  {
    return sysd.buttonPins[button - 1];
  }
  else
  {
    Serial.print(F("Button number out of range. Max is "));
    Serial.println(MAX_BUTTONS - 1);
  }
}

void  SYSDATA::setButtonPin(uint8_t button, uint8_t pin)
{
  if(button < MAX_BUTTONS)
  {
    if(pin < MAX_DIGITAL_PINS)
    {
      sysd.buttonPins[button - 1] = pin;
      Serial.println(F("Button pin set correctly."));
    }
    else
    {
      Serial.print(F("Pin out of range. Max is "));
      Serial.println(MAX_DIGITAL_PINS);
    }
  }
  else
  {
    Serial.print(F("Button number out of range. Max is "));
    Serial.println(MAX_BUTTONS - 1);
  }
}

bool    SYSDATA::checkPinAvailability(byte pin, bool isDigital)
{
  uint16_t  eeAddress;
  uint8_t   max_pins;
  
  if(isDigital)
  {
    eeAddress = sizeof(sysd) + pin - 1;
    max_pins  = MAX_DIGITAL_PINS;
  }
  else
  {
    eeAddress = sizeof(sysd) + MAX_DIGITAL_PINS + pin - 1;
    max_pins  = MAX_ANALOG_PINS;
  }
  
  if(pin > 0 && pin <= max_pins)
  {
    byte  state;
    EEPROM.get(eeAddress, state);

    if(state == 0)
    {
      return true;
    }
    else
    {
      Serial.println(F("Pin is in use."));
      return false;
    }
  }
  else
  {
    Serial.println(F("Invalid pin number."));
    return false;
  }
}
