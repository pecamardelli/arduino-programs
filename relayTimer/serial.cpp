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
    char *input = getInput();
    Command *com = parser.parse(input);
    client->println(sys.exec(com));
    delete com;
  }
}
