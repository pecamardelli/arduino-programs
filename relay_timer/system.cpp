#include "header.h"

System::System(){
  eeAddress     = 0;
  changeFlag    = false;
  output        = COMM_SERIAL;
  relayQuantity = 0;
};

void System::saveData() {
  int q = 0;
  node_t *aux = first;
  eeAddress = 0;

  while(aux != NULL) {
    if(aux->changeFlag) {
      EEPROM.put(eeAddress, aux->relay);
      aux->changeFlag = false;
      q++;
    }
    eeAddress += sizeof(relayData);
    aux = aux->next;
  }

  printData(String(q) + " relays updated.", true);
  
  if(this->changeFlag) {
    eeAddress = EEPROM.length() - sizeof(config) - 1;
    /*
      * byte mac[] =     { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
    config.mac[0] = 0x88;
    config.mac[1] = 0xD7;
    config.mac[2] = 0xF6;
    config.mac[3] = 0x3D;
    config.mac[4] = 0xFE;
    config.mac[5] = 0xED;
    */
    EEPROM.put(eeAddress, config);
    this->changeFlag = false;
  }
};

#ifdef __arm__
// should use uinstd.h to define sbrk but Due causes a conflict
extern "C" char *sbrk(int incr);
#else  // __ARM__
extern char *__brkval;
#endif // __arm__

int System::getFreeMemory()
{
  char top;
#ifdef __arm__
  return &top - reinterpret_cast<char *>(sbrk(0));
#elif defined(CORE_TEENSY) || (ARDUINO > 103 && ARDUINO != 151)
  return &top - __brkval;
#else  // __arm__
  return __brkval ? &top - __brkval : &top - __malloc_heap_start;
#endif // __arm__
}


void System::setIpAddress(char *_address, int _type) {
  IPAddress addr;
  // Check validity
  if (!addr.fromString(_address)) return;

  switch(_type) {
    case IP_ADDRESS:
      config.ip[0]  = addr[0];
      config.ip[1]  = addr[1];
      config.ip[2]  = addr[2];
      config.ip[3]  = addr[3];
      break;
    case SUBNET_MASK:
      config.subnet[0] = addr[0];
      config.subnet[1] = addr[1];
      config.subnet[2] = addr[2];
      config.subnet[3] = addr[3];
      break;
    case DEFAULT_GATEWAY:
      config.gateway[0]  = addr[0];
      config.gateway[1]  = addr[1];
      config.gateway[2]  = addr[2];
      config.gateway[3]  = addr[3];
      break;
    case DNS_SERVER:
      config.dns[0]    = addr[0];
      config.dns[1]    = addr[1];
      config.dns[2]    = addr[2];
      config.dns[3]    = addr[3];
      break;
  }
  
  this->changeFlag = true;
};

void System::setHostname(char *_name) {
  if(strlen(_name) == 0) return;
    
  if(strlen(_name) > MAX_HOSTNAME_LEN) return;

  // Looks like we're good...
  printData(_name, true);
  strcpy(this->config.hostname, _name);

  // Inform that changes were made.
  this->changeFlag = true;
};

void System::getSerialInput() {
  char c;
  byte charsWaiting  = 0;
  byte charsReceived = 0;
  char *command = NULL;

  command = (char*) malloc (MAX_COMMAND_LEN*sizeof(char));
  while(Serial.available()) {
    c = (char)Serial.read();
    
    // Include letters, digits, and other allowed chars. Add more allowed characters
    // at the definition of the specialChars array.
    if (isalpha(c) or isdigit(c) or charAllowed(c)) {
      command[charsReceived] = (char)c;
      charsReceived++;
      if (charsReceived >= MAX_COMMAND_LEN) break;
    }
    delay(5);
  }
  command[charsReceived] = (char)0x20;
    
  if (charsReceived) parser(command);
  if (command) free(command);
};

// Send data to the current interface (telnet or serial)
void System::printData(String data, bool carriageReturn) {
  // output variable is set to COMM_TELNET when a connection arrives. Otherwise, is set to COMM_SERIAL. 
  switch(this->output) {
    case COMM_SERIAL:
      if(carriageReturn) Serial.println(data);
      else Serial.print(data);
      break;
    case COMM_TELNET:
      if(carriageReturn) network.client.println(data);
      else network.client.print(data);
      break;
  }
};

void System::loadSystemData() {
  // LOAD SYSTEM CONFIGURATIONS
  // They're stored at the end of the EEPROM.
  // Relay data is stored at the beginning.
  
  eeAddress = EEPROM.length() - sizeof(config) - 1;
  EEPROM.get(eeAddress, config);

  // Set a default mac address if it's not defined.
  if(config.mac[0] == 0xff) config.mac[0] = 0xDE;
  if(config.mac[1] == 0xff) config.mac[1] = 0xAD;
  if(config.mac[2] == 0xff) config.mac[2] = 0xBE;
  if(config.mac[3] == 0xff) config.mac[3] = 0xEF;
  if(config.mac[4] == 0xff) config.mac[4] = 0xF0;
  if(config.mac[5] == 0xff) config.mac[5] = 0x18;

  // Set a default IP address if it's not defined.
  if(config.ip[0] == 0xff) config.ip[0] = 192;
  if(config.ip[1] == 0xff) config.ip[1] = 168;
  if(config.ip[2] == 0xff) config.ip[2] = 0;
  if(config.ip[3] == 0xff) config.ip[3] = 8;
  
  Ethernet.begin(config.mac, config.ip, config.dns, config.gateway, config.subnet);

  byte pos      = 0;
  relayQuantity = 0;
  eeAddress     = 0;

  // All relays are loaded into a dynamic list. This way, more relays can be added later.

  // From byte 0 to 100 is stored the "file system". Every two bytes is an integer
  // that represents a memory address where the relay data is stored on the EEPROM.
  for(byte i=0;i<MAX_RELAY_NUMBER;i++) {
    EEPROM.get(eeAddress, pos);
  }
  
  do {
    node_t *aux = (node_t *)malloc(sizeof(node_t));
    
    if (!aux) break;
    else {
      EEPROM.get(eeAddress, aux->relay);
  
      if(aux->relay.type == 200) {
        if (!aux->relay.deleted) {
          if(first == NULL) {
            first       = aux;
            first->next = NULL;
            last        = first;
          } else {
            last->next  = aux;
            last        = aux;
            last->next  = NULL;
          }
    
          pinMode(aux->relay.pin, OUTPUT);
          digitalWrite(aux->relay.pin, HIGH);
          aux->changeFlag = false;
          aux->overrided  = false;
    
          relayQuantity++;
        }
        else free(aux);

        eeAddress += sizeof(relayData);
      } else break;
    }
  }
  while(1);
};
