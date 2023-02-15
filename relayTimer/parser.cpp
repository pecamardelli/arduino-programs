#include "header.h"

Parser::Parser(/* args */)
{
    // Using space character as delimiter
    delimiter = " ";
}

Parser::~Parser()
{
}

void Parser::parse(char *input)
{
    Command *com = tokenize(input);
    // ----- Do some debugging ----- //
    /*
    for (byte i = 0; i < com->count; i++)
    {
        Serial.print("Argument " + String(i) + ": ");
        Serial.println(com->args[i]);
    }
    Serial.println("Total args: " + String(com->count));
    */
    // ---------------------------- //

    if (strncmp(com->args[0], "help", 4) == 0)
    {
        Serial.println("Help wanted...");
    }

    com->freeMem();
    delete com;
}

Command *Parser::tokenize(char *input)
{
    // Tokenizing arguments
    Command *com = new Command();

    char *token = strtok(input, delimiter);

    while (token != NULL && com->count < MAX_COMMAND_ARGS)
    {
        com->args[com->count] = (char *)malloc(sizeof(char) * strlen(token));
        strcpy(com->args[com->count], token);
        com->count++;
        token = strtok(NULL, delimiter);
    }
    return com;
};
