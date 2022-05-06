#ifndef SYSDATA_h
#define SYSDATA_h

#include "Arduino.h"
#include <EEPROM.h>

  /*!
   *  @brief Class containing all the vars and functions used to manage system data
   */

  #define     MAX_BUTTONS     3
  
  class SYSDATA
  {
    public:
      SYSDATA();
      void        loadSysData()     { EEPROM.get(0, sysd);  }
      void        saveSysData()     { EEPROM.put(0, sysd);  }

      size_t      getSize()         { return sizeof(sysd); }
      uint16_t    getNumberWidth()  { return sysd.numWidth; }
      uint16_t    getNumberHeight() { return sysd.numHeight; }
      uint16_t    getCharWidth()    { return sysd.charWidth; }
      uint16_t    getCharHeight()   { return sysd.charHeight; }
      uint16_t    getDotsWidth()    { return sysd.dotsWidth; }
      uint16_t    getDotsHeight()   { return sysd.dotsHeight; }
      uint16_t    getSymbolWidth()  { return sysd.symWidth; }
      uint16_t    getSymbolHeight() { return sysd.symHeight; }

      void        setNumberWidth    (uint16_t width);
      void        setNumberHeight   (uint16_t height);
      void        setCharWidth      (uint16_t width);
      void        setCharHeight     (uint16_t height);
      void        setDotsWidth      (uint16_t width);
      void        setDotsHeight     (uint16_t height);
      void        setSymbolWidth    (uint16_t width);
      void        setSymbolHeight   (uint16_t height);
    
      uint8_t     getButtonPin(uint8_t button);
      void        setButtonPin(uint8_t button, uint8_t pin);
      
      uint16_t    getNoone()        { return sysd.noone; }  
    
    private:

      /*!
       *  @brief Structure containing all variables. Useful to store and retrieve it from EEPROM
       */
       
      typedef struct _sysd
      {
        uint16_t    noone;            // Random number representing an undefined value
        
        /* 
         * ---------- IMAGE SIZE VARS ----------- 
         */
         
        uint8_t     numWidth;         // Big number width
        uint8_t     numHeight;        // Big number height
        uint8_t     charWidth;        // Char width
        uint8_t     charHeight;       // Char height
        uint8_t     dotsWidth;        // Double dot width (used to display the clock)
        uint8_t     dotsHeight;       // Double dot height
        uint8_t     symWidth;         // Symbol width
        uint8_t     symHeight;        // Symbol height
  
        /*
         * ---------- SENSORS DATA ----------- 
         */

        uint8_t     buttonPins[MAX_BUTTONS];      // Array containing the pin numbers for each button.
      };

      _sysd sysd;

      bool          checkPinAvailability(byte pin, bool isDigital);
  };
  
#endif      // SYSDATA_h
