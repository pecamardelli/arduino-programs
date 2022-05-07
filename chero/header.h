#ifndef _HEADER_H
#define _HEADER_H
  
  // ----------- MISC ------------- //
  
  #define VERSION             F("0.9.1")
  #define BAUD_RATE           9600
  
  
  // ----------- GLOBAL VARS ------------- //

  unsigned long pushButtonTimestamp     = 0;
  unsigned long testButton1Timestamp    = 0;
  unsigned long testButton2Timestamp    = 0;
  
  bool          auxiliaryBeamsEnabled   = false;
  int           auxiliaryBeamsLastState = auxiliaryBeamsEnabled;
  int           lowBeams                = HIGH;

  const int     lowBeamsRelayPin        = 2;
  const int     highBeamsRelayPin       = 3;
  const int     buttonLedRelayPin       = 4;
  
  const int     lowBeamsSensorPin       = A0;
  const int     highBeamsSensorPin      = A1;

  const int     pushButtonPin           = 8;
  const int     pushButtonDelay         = 400;

  float         lowBeamsSensorValue     = 0;
  float         highBeamsSensorValue    = 0;

  const float   auxiliaryBeamsVoltageThreshold  = 11.0;
  
  
  // ----------- FUNCTIONS ------------- //
  
  float measureVoltage  (const int pin);
  
#endif // _HEADER_H
