
#include <Ethernet.h>

// ----------- MACROS ------------- //
#ifndef MACROS_H
#define MACROS_H

#define MAX_COMMAND_LEN 64
#define MAX_COMMAND_ARGS 16
#define MAX_HOSTNAME_LEN 64
#define RELAY_DESC_LEN 32

#define STATUS_DISABLED 0x00
#define STATUS_ENABLED 0x01
#define STATUS_DELETED 0x02

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

enum RelayStatus
{
    disabled,
    enabled,
    deleted
};

typedef struct _relayData
{
    uint8_t pin;
    uint8_t startHour;
    uint8_t startMin;
    uint8_t endHour;
    uint8_t endMin;
    RelayStatus status;
    char desc[RELAY_DESC_LEN];
} relayData_t;

class Relay
{
private:
    relayData_t data;

public:
    Relay(uint8_t);
    ~Relay();

    unsigned long startTime;

    uint8_t getPin();
    uint8_t getMode();
    String getDesc();
    uint8_t getStartHour();
    uint8_t getStartMinute();
    uint8_t getEndHour();
    uint8_t getEndMinute();
    RelayStatus getStatus();
    String getStartTime();
    String getEndTime();
    String getUptime();

    String setPin(uint8_t);
    String setDesc(char *);
    String setMode(uint8_t);
    String setStartHour(uint8_t);
    String setStartMinute(uint8_t);
    String setEndHour(uint8_t);
    String setEndMinute(uint8_t);
};

#endif

#ifndef COMMAND_H
#define COMMAND_H

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
    String setHostname(char *);
    String setIpAddress(char *);
    String setSubnetMask(char *);
    String setDefaultGateway(char *);
    String setDnsServer(char *);
    String exec(Command *);
};

extern System sys;

#endif

#ifndef PARSER_H
#define PARSER_H

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
    const byte **specialChars;

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

#ifndef CLOCK_H
#define CLOCK_H
// class Clock
// {
// private:
//     const char **days;

// public:
//     Clock(/* args */);
//     ~Clock();
//     RTC_DS1307 RTC;

//     String setDateTime(char *, char *);
//     String getDate();
// };

// extern Clock clock;

#endif

#ifndef NODELIST_H
#define NODELIST_H

typedef struct node
{
    Relay *relay;
    bool changeFlag;
    bool overrided;
    struct node *next;
} node_t;

class NodeList
{
private:
    uint8_t getAvailablePin();
    node_t *first, *last;

public:
    NodeList(/* args */);
    ~NodeList();
    void checkRelays();
    Relay *searchByPin(uint8_t);
    bool isPinAvailable(uint8_t);
    char *createRelay(uint8_t);
    String getRelayInfo();
};

#endif