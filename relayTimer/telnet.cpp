#include "header.h"

extern System sys;
extern EthernetServer server;
extern Parser parser;

Telnet::Telnet()
{
  connected = false;
  client = 0;
  allowedConnectTime = 300000;
};

Telnet::~Telnet()
{
}

void Telnet::commandPrompt()
{
  timeOfLastActivity = millis();
  client.println();
  client.print(sys.hostname);
  client.print(F("#>"));
};

void Telnet::checkConnection()
{
  client = server.available();
  if (client && !connected)
  {
    connected = true;
    client.println(F("ARDUINO RELAY TIMER"));
    client.println(" - " + String(sys.hostname));
    client.print(F("Version "));
    client.println(VERSION);
    commandPrompt();
  }

  // Check to see if text received
  if (client.connected() && client.available())
  {
    char *input = getInput();
    parser.parse(input);
    free(input);
    commandPrompt();
  }
  
  // Check to see if connection has timed out
  if (connected)
    checkConnectionTimeout();
};

char *Telnet::getInput()
{
  uint8_t charIndex = 0;
  const uint8_t charsWaiting = client.available();
  char c;
  
  if (!charsWaiting)
    return;
    
  // Looks like we have a command to parse. Let's do it.
  char *input = (char *)malloc(charsWaiting * sizeof(char));

  while (client.available() > 0)
  {
    c = client.read();
    //Serial.print(String(int(c)) + " ");
    // Include letters, digits, and other allowed chars. Add more allowed characters
    // at the definition of the specialChars array.
    if (isalpha(c) or isdigit(c) or sys.charAllowed(c))
    {
      input[charIndex++] = (char)c;
    }
    else
    { 
      input[charIndex++] = (char)0x20;
    }
  }
  
  return input;
};

void Telnet::closeConnection()
{
  client.println(F("\nBye.\n"));
  client.stop();
  connected = false;
};

void Telnet::checkConnectionTimeout()
{
  if (millis() - timeOfLastActivity > allowedConnectTime)
  {
    client.println();
    client.println(F("Timeout disconnect."));
    closeConnection();
  }
};
