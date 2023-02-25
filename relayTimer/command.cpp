#include "header.h"

Command::Command(/* args */)
{
  // args = new char *[MAX_COMMAND_ARGS](NULL);
  count = 0;
}

Command::~Command()
{
  // freeMem();
}

// void Command::freeMem()
// {
//   for (uint8_t i = 0; i < MAX_COMMAND_ARGS; i++)
//   {
//     if (args[i])
//     {
//       free(args[i]);
//     }
//   }
//   free(args);
// }
