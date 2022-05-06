#ifndef _HEADER_H
#define _HEADER_H
  
  // ----------- MISC ------------- //
  
  #define VERSION             F("0.1.0")
  #define BAUD_RATE           9600
  
  
  // ----------- PIN DEFINITION ------------- //
  
  #define RELAY1  2
  #define RELAY2  3
  #define RELAY3  4
  #define RELAY4  5

  #define PUSH_BUTTON         8
  #define PUSH_BUTTON_DELAY   400

  #define TEST_BUTTON1        9
  #define TEST_BUTTON2        10
  
  
  // ----------- GLOBAL VARS ------------- //

  unsigned long pushButtonTimestamp     = 0;
  unsigned long testButton1Timestamp    = 0;
  unsigned long testButton2Timestamp    = 0;
  int           auxiliaryBeamsEnabled   = HIGH;
  int           auxiliaryBeamsLastState = auxiliaryBeamsEnabled;
  int           lowBeams                = HIGH;
  
  
  // ----------- FUNCTIONS ------------- //
  
  void          checkButton           ();
  
#endif // _HEADER_H
