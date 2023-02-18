#include "header.h"

extern Telnet telnet;

Parser::Parser(/* args */)
{
  // Using space character as delimiter
  delimiter = " ";
}

Parser::~Parser()
{
}

Command *Parser::parse(char *input)
{
  //Serial.println("Parsing: " + String(input));
  Command *com = new Command();

  char *token = strtok(input, delimiter);

  while (token != NULL && com->count < MAX_COMMAND_ARGS)
  {
    com->args[com->count] = (char *)malloc(sizeof(char) * strlen(token));
    strcpy(com->args[com->count], token);
    com->count++;
    token = strtok(NULL, delimiter);
  }

  // ----- Do some debugging ----- //
  
  for (uint8_t i = 0; i < com->count; i++)
  {
    Serial.print("Argument " + String(i) + ": ");
    Serial.println(com->args[i]);
  }
  Serial.println("Total args: " + String(com->count));
  
  // ---------------------------- //

  free(input);
  
  return com;
}
