
#include <RTClib.h>
#include <Ethernet.h>
#include <EEPROM.h>

// ----------- MACROS ------------- //
#ifndef MACROS_H
  #define MACROS_h
  
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
  #define COMM_SERIAL      0x00
  #define COMM_TELNET      0xff
#endif

#ifndef SYSTEM_H
  #define SYSTEM_H

  #define VERSION             F("2.0")
  #define BAUD_RATE           9600

  // Type of address passed to setAddress function
  #define IP_ADDRESS       0
  #define SUBNET_MASK      1
  #define DEFAULT_GATEWAY  2
  #define DNS_SERVER       3

  typedef struct SysConfig {
    char   hostname[32];
    byte   mac[6];
    byte   ip[4];
    byte   subnet[4];
    byte   gateway[4];
    byte   dns[4];
    byte   resetPin;
  };

  class System {
    unsigned int  eeAddress;
    bool          changeFlag;
    void          loadSystemData();

    public:
      System();
      SysConfig config;
      byte output;
      byte relayQuantity;
      void saveData();
      void setIpAddress(char *_address, int _type);
      void setHostname(char *_name);
      void getSerialInput();
      void printData(String data, bool carriageReturn);
  };

  System sys;
#endif

#ifndef NETWORK_H
  #define NETWORK_H

  EthernetServer server(23); // Telnet listens on port 23

  class Network {
    public:
      Network();
      boolean connected;
      EthernetClient client = 0;
      void commandPrompt();
      void checkConnection();
      void getInput();
      void closeConnection();
      void checkConnectionTimeout();
  };

  Network network;
#endif

#ifndef CLOCK_H
  #define CLOCK_H
  class Clock {
    RTC_DS1307 RTC;
    uint8_t  _hour;
    uint8_t  _min;
    uint8_t  _sec;
    uint8_t  _year;
    uint8_t  _month;
    uint8_t  _day;
    DateTime now;
    const String days[7];

    public:
      Clock();
      void setDateTime(char *_date, char *_time);
      String getDate();
      DateTime getTime();
  };

  Clock clock;
#endif

#ifndef RELAY_H
  #define RELAY_H
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

  class Relay{
    public:
      Relay();  
  };

  Relay relay;
#endif

#ifndef MISC_H
  #define MISC_H
  // ----------- SYSTEM ----------- //
  char          *statuses[]     = { "ON", "OFF" };
  unsigned long tstamp          = 0;
  const byte    unusablePins[]  = { 9, 10, 11, 12, 13 };

  // Array of special chars allowed in inputs
  const byte    specialChars[]  = { 0x20, 0x2d, 0x2e, 0x2f, 0x3a };

  // ----------- TELNET SERVER ----------- //               

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
  String parseUpTime(unsigned long startTime);
#endif