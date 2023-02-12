#include <Wire.h>
#include <RTClib.h>
#include <SPI.h>
#include <SD.h>
#include <Ethernet.h>
#include <EEPROM.h>


// ----------- MISC ------------- //
#define MAX_COMMAND_LEN     64
#define MAX_COMMAND_ARGS    10
#define MAX_HOSTNAME_LEN    32
#define MAX_RELAY_NUMBER    50
#define RELAY_DESC_LEN      32

#define SWITCH_OFF          0
#define SWITCH_ON           1

#define TYPE_START_HOUR     10
#define TYPE_END_HOUR       20
#define TYPE_START_MIN      30
#define TYPE_END_MIN        40

// Communication types
#define COMM_SERIAL      100
#define COMM_TELNET      200

// ----------- VARIABLE DECLARATION ----------- //

// ----------- RELAYS ----------- //

byte   relayQuantity     = 0;

typedef struct relayData {
  byte  type;
  byte  pin;
  bool  enabled;
  char  desc[RELAY_DESC_LEN];
  byte  startHour;
  byte  startMin;
  byte  endHour;
  byte  endMin;
  unsigned long startTime;
  bool  deleted;
};

typedef struct node {
    struct  relayData relay;
    byte    memPos;
    bool    changeFlag;
    bool    overrided;
    struct  node * next;
} node_t;

node_t *first = NULL;
node_t *last  = NULL;

// ----------- SYSTEM ----------- //
byte          output          = COMM_SERIAL;
char          *statuses[]     = { "ON", "OFF" };
unsigned long tstamp          = 0;
const byte    unusablePins[]  = { 9, 10, 11, 12, 13 };

// Array of special chars allowed in inputs
const byte    specialChars[]  = { 0x20, 0x2d, 0x2e, 0x2f, 0x3a };

// ----------- TELNET SERVER ----------- //

// we'll use a flag separate from client.connected
// so we can recognize when a new connection has been created
boolean connectFlag = 0;                    

//time in milliseconds of last activity
unsigned long timeOfLastActivity;

//five minutes
unsigned long allowedConnectTime = 300000;
/* -------------------------------------------------------------- */

// ----------- FUNCTION DECLARATIONS ----------- //

void parser(char *command);
void setParam(String param);

// Relay functions - Defined in relayFunctions.h
void setRelayStatus(byte pin, bool _status);
void resumeRelay(byte pin);
node_t *searchRelay(byte pin);
void createRelay(byte pin);
void deleteRelay(byte pin);
void checkRelays();
byte checkPin(char *_pin);
void setRelayParams(byte pin, char *_param, byte type);
void setRelayDesc(byte pin, char *_desc);
void switchRelay(byte pin, uint8_t state, bool overrided);
void changeRelayPin(byte currentPin, char *newPin);

// Getter functions - Defined in getters.h //
void getRelayInfo();
void getReceivedText();

// Setter functions - Defined in setters.h //

// Utilities - Defined in utils.h
String arrayToString(byte array[], unsigned int len);
bool charAllowed(char c);
void clearArgs(char **args);
void loadSystemData();
void checkConnectionTimeout();
void closeConnection();
String parseUpTime(unsigned long startTime);

// Data printing - Defined in printFunctions.h
void printPrompt();
void printData(String data, bool rc);
void printHelp();
void setCommandHelp();
void showCommandHelp();
void relayCommandHelp();
void printIpAddress();
