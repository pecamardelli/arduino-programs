#line 1 "/home/pablin/arduino_programs/relayTimer/header.h"

#include <Ethernet.h>

// ----------- MACROS ------------- //
#ifndef MACROS_H
#define MACROS_H

#define MAX_COMMAND_LEN 64
#define MAX_COMMAND_ARGS 16
#define MAX_HOSTNAME_LEN 64

#endif

#ifndef SYSTEM_H
#define SYSTEM_H

#define VERSION F("2.0")
#define BAUD_RATE 9600

class System
{
private:
    byte specialChars[6];
    bool configChanged;

public:
    System();
    char hostname[MAX_HOSTNAME_LEN];
    byte macAddress[6];
    IPAddress ipAddress;
    IPAddress subnetMask;
    IPAddress gateway;
    IPAddress dnsServer;

    char *getSerialInput();
    int getFreeMemory();
    bool charAllowed(char c);
    char *setHostname(char *newHostname);
};

extern System sys;

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

extern Parser parser;

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
    char *exec(Command *com);
};

#endif

#ifndef TELNET_H
#define TELNET_H

class Telnet : public Channel<EthernetClient *>
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

extern Telnet telnet;

#endif

#ifndef SERIAL_CHANNEL_H
#define SERIAL_CHANNEL_H

class SerialChannel : public Channel<Stream *>
{
private:
    /* data */
public:
    SerialChannel(/* args */);
    ~SerialChannel();
    void checkServer();
};

#endif
