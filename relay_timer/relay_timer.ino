// * ARDUINO RELAY TIMER - February 2023

#include "header.h"
#include "system.cpp"
#include "network.cpp"
#include "clock.cpp"
#include "relay.cpp"
#include "utils.cpp"
#include "parser.cpp"

void setup() {
  Serial.begin(9600);
  Wire.begin();
  Serial.write("Hola");
}

void loop() {
  network.checkConnection();
  
  if(Serial.available()) {
    if (network.connected) {
      while(Serial.read() != -1) {}
    }
    else {
      sys.output = COMM_SERIAL;
      sys.getSerialInput();
    }
  }
    
  // Check relays every five seconds...
  if(millis() - tstamp >= 5000) {
    checkRelays();
    tstamp = millis();
  }
}
