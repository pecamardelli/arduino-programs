
#include <Ethernet.h>

// ----------- MACROS ------------- //
#ifndef MACROS_H
#define MACROS_H

#define MAX_COMMAND_LEN 64
#define MAX_COMMAND_ARGS 16
#define MAX_HOSTNAME_LEN 64
#define RELAY_DESC_LEN 32

#endif

// ----------- BOARD RELATED ------------- //
#if defined(ARDUINO_AVR_MEGA2560)
#define BOARD "Mega 2560"
#define DIGITAL_PINS 53
#elif defined(ARDUINO_AVR_UNO)
#define BOARD "Uno"
#define DIGITAL_PINS 13
#endif

#ifndef RELAY_H
#define RELAY_H

class Relay
{
private:
public:
    Relay(uint8_t);
    ~Relay();
    uint8_t setPin(uint8_t);
    uint8_t getPin();
    char *setMode(uint8_t);

    uint8_t pin;
    bool enabled;
    char desc[RELAY_DESC_LEN];
    uint8_t startHour;
    uint8_t startMin;
    uint8_t endHour;
    uint8_t endMin;
    unsigned long startTime;
    bool deleted;
};

typedef struct node
{
    Relay *relay;
    bool changeFlag;
    struct node *next;
} node_t;

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

    int getFreeMemory();
    bool charAllowed(char);
    char *setHostname(char *);
    char *setIpAddress(char *);
    char *setSubnetMask(char *);
    char *setDefaultGateway(char *);
    char *setDnsServer(char *);
    uint8_t getAvailablePin();
    bool isPinAvailable(uint8_t);
    Relay *searchByPin(uint8_t);
    char *createRelay(uint8_t);
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
    Command *Parser::parse(char *);
};

extern Parser parser;

#endif

#ifndef CHANNEL_H
#define CHANNEL_H

template <class T>
class Channel
{
private:
    bool charAllowed(char);
    void getRelayInfo();
    byte specialChars[6];

public:
    Channel(/* args */);
    ~Channel();
    char *exec(Command *);

protected:
    T client;
    char *getInput();
    // char *exec(Command *com);
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

public:
    Telnet(/* args */);
    ~Telnet();
    void commandPrompt();
    void checkAvailable();
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
    void checkAvailable();
};

#endif