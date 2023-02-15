#include "header.h"

extern System sys;
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
  while (client.read() != -1)
  {
  };
  client.println();
  client.print(sys.hostname);
  client.print(F(" >"));
};

void Telnet::checkConnection()
{
  if (server.available() && !connected)
  {
    connected = true;
    client = server.available();
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
    Serial.println(input);
    commandPrompt();
  }

  // Check to see if connection has timed out
  if (connected)
    checkConnectionTimeout();
};

char *Telnet::getInput()
{
  uint8_t charsWaiting = 0;
  uint8_t charsReceived = 0;
  char *input = NULL;
  char c;

  charsWaiting = client.available();
  if (!charsWaiting)
    return;

  if (charsWaiting > MAX_COMMAND_LEN)
  {
    client.print(F("Command must have a maximum of "));
    client.print(String(MAX_COMMAND_LEN));
    client.println(F(" characters."));
    return;
  }

  // Looks like we have a command to parse. Let's do it.
  input = (char *)malloc(charsWaiting * sizeof(char));

  while (charsWaiting > 0)
  {
    c = client.read();

    // Include letters, digits, and other allowed chars. Add more allowed characters
    // at the definition of the specialChars array.
    if (isalpha(c) or isdigit(c) or sys.charAllowed(c))
    {
      input[charsReceived] = (char)c;
      charsReceived++;
    }
    else if (c == 0x0d)
    { // Carriage return. Parse command.
      // Add space character to the end of the string in order to give strtok the ability
      // to slice it and avoid taking any garbage from memory.
      // See parser function.
      input[charsReceived] = (char)0x20;
      break;
    }
    charsWaiting--;
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
    client.stop();
    connected = false;
  }
};