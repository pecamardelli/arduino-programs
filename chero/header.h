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
  
  
  // ----------- GLOBAL VARS ------------- //

  unsigned long pushButtonTimestamp   = 0;
  bool          auxiliaryBeamsEnabled = false;
  
  
  // ----------- FUNCTIONS ------------- //
  
  void          checkButton           ();
  
#endif // _HEADER_H
