#include "header.h"

SerialChannel::SerialChannel(/* args */)
{
    client = &Serial;
}

SerialChannel::~SerialChannel()
{
}

void SerialChannel::checkServer()
{
  if (client->available())
  {
    char *input = getInput();
    Command *com = parser.parse(input);
    exec(com);
    delete com;
  }
}
