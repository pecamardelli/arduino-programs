#include "header.h"

extern EthernetServer server;

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
  client->print(sys.hostname);
  client->print(F("#> "));
};

void Telnet::checkAvailable()
{
  client = &server.available();
  if (client && !connected)
  {
    connected = true;
    client->println(F("ARDUINO RELAY TIMER"));
    client->println(" - " + String(sys.hostname));
    client->print(F("Version "));
    client->println(VERSION);
    commandPrompt();
  }

  // Check to see if text received
  if (client->connected() && client->available())
  {
    char *input = getInput();
    Command *com = parser.parse(input);
    client->println(sys.exec(com));
    delete com;
    commandPrompt();
  }
};

void Telnet::closeConnection()
{
  client->println(F("\nBye.\n"));
  client->stop();
  connected = false;
};
