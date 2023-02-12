#include "header.h"

Network::Network(){
  server.begin();
  connected = false;
  client = 0;
};
 
void Network::commandPrompt() {
  timeOfLastActivity = millis();
  while (this->client.read() != -1) {};
  this->client.println();
  this->client.print(sys.config.hostname);
  this->client.print(F("#>"));
};

void Network::checkConnection() {
  if(server.available() && !this->connected) {
    this->connected = true;
    sys.output = COMM_TELNET;
    client = server.available();
    client.println(F("ARDUINO RELAY TIMER"));
    client.println(" - " + String(sys.config.hostname));
    client.print(F("Version "));
    client.println(VERSION);
    this->commandPrompt();
  }

  // Check to see if text received
  if (client.connected() && client.available()) {
    this->getInput();
    this->commandPrompt();
  }
  
  // Check to see if connection has timed out
  if(this->connected) this->checkConnectionTimeout();
};

void Network::getInput() {
  byte charsWaiting  = 0;
  byte charsReceived = 0;
  char *command = NULL;
  char c;
  
  charsWaiting = client.available();    
  if (!charsWaiting) return;

  if (charsWaiting > MAX_COMMAND_LEN) {
    client.print(F("Command must have a maximum of "));
    client.print(String(MAX_COMMAND_LEN));
    client.println(F(" characters."));
    return;
  }

  // Looks like we have a command to parse. Let's do it.
  command = (char*) malloc (charsWaiting*sizeof(char));
  
  while(charsWaiting > 0) {
    c = client.read();

    // Include letters, digits, and other allowed chars. Add more allowed characters
    // at the definition of the specialChars array.
    if (isalpha(c) or isdigit(c) or charAllowed(c)) {
      command[charsReceived] = (char)c;
      charsReceived++;
    }
    else if (c == 0x0d) {   // Carriage return. Parse command.
      // Add space character to the end of the string in order to give strtok the ability
      // to slice it and avoid taking any garbage from memory.
      // See parser function.
      command[charsReceived] = (char)0x20;
      break;
    }
    charsWaiting--;
  }
};

void Network::closeConnection() {
  client.println(F("\nBye.\n"));
  client.stop();
  this->connected = false;
  sys.output = COMM_SERIAL;
};

void Network::checkConnectionTimeout() {
  if(millis() - timeOfLastActivity > allowedConnectTime) {
    client.println();
    client.println(F("Timeout disconnect."));
    client.stop();
    this->connected = false;
  }
};
