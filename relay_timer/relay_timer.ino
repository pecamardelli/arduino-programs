// * ARDUINO RELAY TIMER - February 2023

#include "header.h"
#include "clock.h"
#include "system.h"
#include "network.h"
#include "utils.h"
#include "printFunctions.h"
#include "relayFunctions.h"
#include "parser.h"

void setup() {
  Serial.begin(9600);
  Wire.begin();
  Serial.print("Hola");
}

void loop() {
  network.checkConnection();
  
  if(Serial.available()) {
    if (network.connected) {
      while(Serial.read() != -1) {}
    }
    else {
      output = COMM_SERIAL;
      sys.getSerialInput();
    }
  }
    
  // Check relays every five seconds...
  if(millis() - tstamp >= 5000) {
    checkRelays();
    tstamp = millis();
  }
}
