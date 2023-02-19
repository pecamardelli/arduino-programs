
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
    bool System::charAllowed(char c);
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

public:
    Parser(/* args */);
    ~Parser();
    Command *Parser::parse(char *input);
};

#endif

#ifndef CHANNEL_H
#define CHANNEL_H

template <class T>
class Channel
{
private:
    bool charAllowed(char c);
    byte specialChars[6];

public:
    Channel(/* args */);
    ~Channel();

protected:
    T client;
    char *getInput();
};

#endif

#ifndef TELNET_H
#define TELNET_H

class Telnet : public Channel<EthernetClient>
{
private:
    boolean connected;
    unsigned long timeOfLastActivity;
    unsigned long allowedConnectTime;

    void checkConnectionTimeout();

public:
    Telnet(/* args */);
    ~Telnet();
    void commandPrompt();
    void checkConnection();
    void closeConnection();
};

#endif
