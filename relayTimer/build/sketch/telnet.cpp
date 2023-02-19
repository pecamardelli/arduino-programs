#line 1 "/home/pablin/arduino_programs/relayTimer/telnet.cpp"
#include "header.h"

extern System sys;
extern EthernetServer server;
extern Parser parser;

Telnet::Telnet()
{
  connected = false;
  this->client = 0;
  allowedConnectTime = 300000;
};

Telnet::~Telnet()
{
}

void Telnet::commandPrompt()
{
  timeOfLastActivity = millis();
  this->client.println();
  this->client.print(sys.hostname);
  this->client.print(F("#>"));
};

void Telnet::checkConnection()
{
  this->client = server.available();
  if (this->client && !connected)
  {
    connected = true;
    this->client.println(F("ARDUINO RELAY TIMER"));
    this->client.println(" - " + String(sys.hostname));
    this->client.print(F("Version "));
    this->client.println(VERSION);
    commandPrompt();
  }

  // Check to see if text received
  if (this->client.connected() && this->client.available())
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

// char *Telnet::getInput()
// {
//   uint8_t charIndex = 0;
//   char c;

//   if (client.available() <= 0)
//     return;

//   // Looks like we have a command to parse. Let's do it.
//   char *input = (char *)calloc(MAX_COMMAND_LEN, sizeof(char));

//   while (client.available() > 0)
//   {
//     c = client.read();
//     //Serial.print(String(int(c)) + " ");
//     // Include letters, digits, and other allowed chars. Add more allowed characters
//     // at the definition of the specialChars array.
//     if (isalpha(c) or isdigit(c) or sys.charAllowed(c))
//     {
//       input[charIndex++] = (char)c;
//     }
//     else
//     {
//       input[charIndex++] = (char)0x20;
//     }
//   }

//   return input;
// };

void Telnet::closeConnection()
{
  this->client.println(F("\nBye.\n"));
  this->client.stop();
  connected = false;
};

void Telnet::checkConnectionTimeout()
{
  if (millis() - timeOfLastActivity > allowedConnectTime)
  {
    this->client.println();
    this->client.println(F("Timeout disconnect."));
    closeConnection();
  }
};
