#line 1 "/home/pablin/arduino_programs/relayTimer/serial.cpp"
#include "header.h"

SerialChannel::SerialChannel(/* args */)
{
  client = &Serial;
}

SerialChannel::~SerialChannel()
{
}

void SerialChannel::checkAvailable()
{
  if (client->available())
  {
    String input = getInput();
    Command *com = parser.parse(input);
    client->println(sys.exec(com));

    if (com)
      delete com;
  }
}
