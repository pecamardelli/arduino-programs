
#include <Ethernet.h>

// ----------- MACROS ------------- //
#ifndef MACROS_H
#define MACROS_H

#define MAX_COMMAND_LEN 64
#define MAX_COMMAND_ARGS 16

#endif

#ifndef SYSTEM_H
#define SYSTEM_H

#define VERSION F("2.0")
#define BAUD_RATE 9600

class System
{
    byte specialChars[6];

public:
    System();
    String hostname;
    byte macAddress[6];
    IPAddress ipAddress;
    IPAddress subnetMask;
    IPAddress gateway;
    IPAddress dnsServer;

    char *System::getSerialInput();
    int System::getFreeMemory();
    bool System::charAllowed(char c)
};

#endif

#ifndef PARSER_H
#define PARSER_H

class Command
{
private:
public:
    Command(/* args */);
    ~Command();
    char **args;
    uint8_t count;
    void freeMem();
};

class Parser
{
private:
    const char *delimiter;
    Command *Parser::tokenize(char *input);

public:
    Parser(/* args */);
    ~Parser();
    void Parser::parse(char *input);
};

#endif

#ifndef TELNET_H
#define TELNET_H

class Telnet
{
private:
    EthernetClient client;
    boolean connected;
    unsigned long timeOfLastActivity;
    unsigned long allowedConnectTime;

    void Telnet::closeConnection();
    void Telnet::checkConnectionTimeout();
    char *Telnet::getInput();

public:
    Telnet(/* args */);
    ~Telnet();
    void Telnet::commandPrompt();
    void Telnet::checkConnection();
};

#endif