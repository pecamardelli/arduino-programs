#ifndef _HEADER_H
#define _HEADER_H

  #include "boardTypes.h"
  
  // ----------- MISC ------------- //
  
  #define VERSION         F("0.93.1")
  #define BAUD_RATE       9600

  #define MAX_LINE_LEN      64
  #define MAX_COMMAND_ARGS  10
  
  
  // ----------- GLOBAL VARS ------------- //

  unsigned long pushButtonTimestamp     = 0;
  unsigned long testButton1Timestamp    = 0;
  unsigned long testButton2Timestamp    = 0;
  
  bool  auxiliaryBeamsEnabled   = false;

  int   lowBeamsRelayPin        = 2;
  int   highBeamsRelayPin       = 3;
  int   buttonLedRelayPin       = 4;
  
  int   lowBeamsSensorPin       = A0;
  int   highBeamsSensorPin      = A1;

  int   pushButtonPin           = 8;
  int   pushButtonDelay         = 400;

  float lowBeamsSensorValue     = 0;
  float highBeamsSensorValue    = 0;

  float auxiliaryBeamsVoltageThreshold  = 11.0;
  
  // ----------- MISC ------------- //

  // Array of special chars allowed on inputs
  const byte    specialChars[]  = { 0x20, 0x2d, 0x2e, 0x2f, 0x3a };
  
#endif // _HEADER_H
