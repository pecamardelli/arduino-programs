
#include <EEPROM.h>
#include <RTClib.h>

// ----------- MACROS ------------- //
#ifndef CONSTANTS_H
#define CONSTANTS_H

#define VERSION F("2.0")

const uint8_t MAX_COMMAND_LEN = 64;
const uint8_t MAX_COMMAND_ARGS = 16;
const uint8_t MAX_HOSTNAME_LEN = 64;
const uint8_t RELAY_DESC_LEN = 32;

// Define max relay number
const uint8_t MAX_RELAY_NUMBER = 4;
// Board pin number
const uint8_t DIGITAL_PINS = 13;
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
    uint16_t eeAddress;

public:
    Relay();
    ~Relay();

    unsigned long startTime;
    bool overrided;

    bool setParams(relayData_t);
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
    String setDesc(String);
    String setStartHour(String);
    String setStartMinute(String);
    String setEndHour(String);
    String setEndMinute(String);
    String setStatus(RelayStatus);

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

typedef struct _system
{
    char hostname[MAX_HOSTNAME_LEN];
    uint16_t relayCheckInterval;
} system_t;

class System
{
private:
    bool configChanged;
    uint16_t eeAddress;

    String resetConfig();
    void saveSystemData();

public:
    System();
    system_t config;

    void loadSystemData();
    int getFreeMemory();
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

class Clock
{
private:
    String days[7];

public:
    Clock(/* args */);
    ~Clock();

    String setDateTime(String, String);
    String getDate();
};

extern Clock clock;

#endif

#ifndef NODELIST_H
#define NODELIST_H

class NodeList
{
private:
    uint8_t getAvailablePin();
    unsigned long lastCheckTimeStamp;

public:
    NodeList(/* args */);
    ~NodeList();

    Relay relayArray[MAX_RELAY_NUMBER];
    uint8_t validRelays;

    void addRelay(uint8_t);
    void checkRelays();
    uint8_t searchByPin(uint8_t);
    bool isPinAvailable(uint8_t);
    String getRelayInfo();
    void saveRelay(Relay *);
    void loadRelays();
    void eraseRelaysFromEEPROM();
};

#endif

#ifndef ERROR_H
#define ERROR_H

enum ErrorTypes
{
    pinNotAvailable,
    pinOutOfRange,
    maxRelayNumberReached
};

class Error
{
private:
    /* data */
public:
    Error(/* args */);
    ~Error();

    static void print(ErrorTypes);
};

#endif