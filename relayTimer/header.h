
#include <Ethernet.h>
// #include <RTClib.h>

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
    char *getDesc();
    uint8_t getStartHour();
    uint8_t getStartMinute();
    uint8_t getEndHour();
    uint8_t getEndMinute();
    RelayStatus getStatus();
    String getStartTime();
    String getEndTime();
    String getUptime();

    char *setPin(uint8_t);
    char *setDesc(char *);
    char *setMode(uint8_t);
    char *setStartHour(uint8_t);
    char *setStartMinute(uint8_t);
    char *setEndHour(uint8_t);
    char *setEndMinute(uint8_t);
};

typedef struct node
{
    Relay *relay;
    bool changeFlag;
    bool overrided;
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
    char *createRelay(uint8_t);
    uint8_t getAvailablePin();
    bool isPinAvailable(uint8_t);
    Relay *searchByPin(uint8_t);
    // void checkRelays();
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
    const byte **specialChars;

public:
    Channel(/* args */);
    ~Channel();
    char *exec(Command *);

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