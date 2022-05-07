#ifndef _HEADER_H
#define _HEADER_H
  
  // ----------- MISC ------------- //
  
  #define VERSION             F("0.1.0")
  #define BAUD_RATE           9600
  
  
  // ----------- PIN DEFINITION ------------- //

  #define PUSH_BUTTON         8
  #define PUSH_BUTTON_DELAY   400

  #define TEST_BUTTON1        9
  #define TEST_BUTTON2        10
  
  
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

  float         lowBeamsSensorValue     = 0;
  float         highBeamsSensorValue    = 0;

  const float   auxiliaryBeamsVoltageThreshold  = 11.0;
  
  
  // ----------- FUNCTIONS ------------- //
  
  float measureVoltage  (const int pin);
  
#endif // _HEADER_H
