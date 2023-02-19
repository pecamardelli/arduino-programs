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
    Command *com = parser.parse(input);
    delete com;
    commandPrompt();
  }

  // Check to see if connection has timed out
  if (connected)
    checkConnectionTimeout();
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
