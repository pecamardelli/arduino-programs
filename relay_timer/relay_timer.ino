// * ARDUINO RELAY TIMER - September 25th 2018

#include "header.h"
#include "utils.h"
#include "getters.h"
#include "setters.h"
#include "printFunctions.h"
#include "saveData.h"
#include "relayFunctions.h"
#include "parser.h"

void setup() {
  Serial.begin(BAUD_RATE);
  Wire.begin();
  RTC.begin();
  server.begin();
  
  loadSystemData();
}

void loop() {
  if(server.available() && !connectFlag) {
    connectFlag = 1;
    output = COMM_TELNET;
    client = server.available();
    client.println(F("ARDUINO RELAY TIMER"));
    client.println(" - " + String(sys.hostname));
    client.print(F("Version "));
    client.println(VERSION);
    printPrompt();
  }

  // Check to see if text received
  if (client.connected() && client.available()) {
    getReceivedText();
    printPrompt();
  }

  // Check to see if connection has timed out
  if(connectFlag) checkConnectionTimeout();

  if(Serial.available()) {
    if (connectFlag) {
      while(Serial.read() != -1) {}
    }
    else {
      output = COMM_SERIAL;
      getReceivedText();
    }
  }
    
  // Check relays every five seconds...
  if(millis() - tstamp >= 5000) {
    checkRelays();
    tstamp = millis();
  }
}
