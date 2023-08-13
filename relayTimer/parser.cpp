#include "header.h"

Parser::Parser(/* args */)
{
  // Using space character as delimiter
  delimiter = " ";
}

Parser::~Parser()
{
}

Command *Parser::parse(String input)
{
  Command *com = new Command();
  char buffer[MAX_COMMAND_LEN];

  input.toCharArray(buffer, MAX_COMMAND_LEN);

  char *token = strtok(buffer, delimiter);

  while (token != NULL && com->count < MAX_COMMAND_ARGS)
  {
    com->args[com->count].concat(token);
    com->count++;
    token = strtok(NULL, delimiter);
  }

  return com;
}
