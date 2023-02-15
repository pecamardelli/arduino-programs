#include "header.h"

Command::Command(/* args */)
{
  args = new char *[MAX_COMMAND_ARGS];
  count = 0;
}

Command::~Command()
{
}

void Command::freeMem()
{
  for (uint8_t i = 0; i < MAX_COMMAND_ARGS; i++)
  {
    if (args[i])
    {
      free(args[i]);
    }
    args[i] = NULL;
  }
  free(args);
}
