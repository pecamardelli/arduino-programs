
#include <Ethernet.h>
#include <EEPROM.h>

// ----------- MACROS ------------- //
#ifndef MACROS_H
#define MACROS_H

#define VERSION F("2.0")
#define BAUD_RATE 9600

#define MAX_COMMAND_LEN 64
#define MAX_COMMAND_ARGS 16
#define MAX_HOSTNAME_LEN 64
#define RELAY_DESC_LEN 32

// Leaving this amount of memory reserved for future use.
#define EEPROM_RESERVED_BYTES 128

// Random integer to identify a relay in the EEPROM
#define RELAY_IDENTIFIER 0xfa67b9c2

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
    uint32_t identifier;
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
    uint16_t eeAddress;

public:
    Relay(uint8_t);
    Relay(relayData_t);
    ~Relay();

    unsigned long startTime;
    bool overrided;

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
    uint8_t getIndex();
    relayData_t getData();
    uint16_t getEepromAddress();

    String setPin(uint8_t);
    String setDesc(char *);
    String setStartHour(uint8_t);
    String setStartMinute(uint8_t);
    String setEndHour(uint8_t);
    String setEndMinute(uint8_t);
    void setEepromAddress(uint16_t);

    String switchOn();
    String switchOff();
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
    String args[MAX_COMMAND_ARGS];
    uint8_t count;
    void freeMem();
};

#endif

#ifndef SYSTEM_H
#define SYSTEM_H

typedef struct _ethernet
{
    byte macAddress[6];
    IPAddress ipAddress;
    IPAddress subnetMask;
    IPAddress gateway;
    IPAddress dnsServer;

} ethernet_t;

typedef struct _system
{
    char hostname[MAX_HOSTNAME_LEN];
    uint16_t relayCheckInterval;
    ethernet_t ethernetConfig;
} system_t;

class System
{
private:
    byte specialChars[6];
    bool configChanged;
    uint16_t eeAddress;

    void loadSystemData();
    String ipToString(IPAddress);
    String resetConfig();
    void saveSystemData();

public:
    System();
    system_t config;

    int getFreeMemory();
    bool charAllowed(char);
    String setHostname(String);
    String setIpAddress(String);
    String setSubnetMask(String);
    String setDefaultGateway(String);
    String setDnsServer(String);
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
    Command *Parser::parse(String);
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
    String getInput();
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
    struct node *next;
} node_t;

class NodeList
{
private:
    uint8_t getAvailablePin();
    node_t *first, *last;
    unsigned long lastCheckTimeStamp;

    void saveRelay(Relay *);

public:
    NodeList(/* args */);
    ~NodeList();
    void checkRelays();
    Relay *searchByPin(uint8_t);
    bool isPinAvailable(uint8_t);
    String addNode(Relay *);
    Relay *createRelay(uint8_t);
    String getRelayInfo();
    uint8_t getNodeNumber();
    void loadRelays();
};

#endif