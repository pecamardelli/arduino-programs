#line 1 "/home/pablin/arduino_programs/relayTimer/parser.cpp"
#include "header.h"

Parser::Parser(/* args */)
{
  // Using space character as delimiter
  delimiter = (char *)0x20;
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
    com->args[com->count] = token;
    Serial.println(com->args[com->count]);
    com->count++;
    token = strtok(NULL, delimiter);
  }

  return com;
}
