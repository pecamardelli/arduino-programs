#include "header.h"

extern node_t *first;
extern node_t *last;

System::System()
{
  strcpy(hostname, "arduino");
  ipAddress.fromString("192.168.40.8");
  subnetMask.fromString("255.255.255.0");
  gateway.fromString("192.168.40.1");
  dnsServer.fromString("179.42.171.21");
  configChanged = false;

  macAddress[0] = 0xDE;
  macAddress[1] = 0xAD;
  macAddress[2] = 0xBE;
  macAddress[3] = 0xEF;
  macAddress[4] = 0xF0;
  macAddress[5] = 0x18;

  // Array of special chars allowed on inputs
  specialChars[0] = 0x20;
  specialChars[1] = 0x2d;
  specialChars[2] = 0x2e;
  specialChars[3] = 0x2f;
  specialChars[4] = 0x3a;
};

bool System::charAllowed(char c)
{
  for (byte i = 0; i < sizeof(specialChars) / sizeof(byte); i++)
  {
    if (c == specialChars[i])
      return true;
  }
  return false;
}

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

char *System::setHostname(char *newHostname)
{
  if (strlen(newHostname) == 0)
    return "Error: hostname cannot be empty.";

  if (strlen(newHostname) > MAX_HOSTNAME_LEN)
    return "Error: hostname exceedes maximum.";

  // Looks like we're good...
  strncpy(hostname, newHostname, MAX_HOSTNAME_LEN);

  return newHostname;
};

char *System::setIpAddress(char *newIpAddress)
{
  if (!ipAddress.fromString(newIpAddress))
    return "Error: invalid ip address.";

  Ethernet.setLocalIP(ipAddress);

  return newIpAddress;
};

char *System::setSubnetMask(char *newSubnetMask)
{
  if (!subnetMask.fromString(newSubnetMask))
    return "Error: invalid subnet mask.";

  Ethernet.setSubnetMask(subnetMask);

  return newSubnetMask;
};

char *System::setDefaultGateway(char *newDefaultGateway)
{
  if (!gateway.fromString(newDefaultGateway))
    return "Error: invalid default gateway.";

  Ethernet.setGatewayIP(gateway);

  return newDefaultGateway;
};

char *System::setDnsServer(char *newDnsServer)
{
  if (!dnsServer.fromString(newDnsServer))
    return "Error: invalid DNS server.";

  Ethernet.setDnsServerIP(dnsServer);

  return newDnsServer;
};

uint8_t System::getAvailablePin()
{
  // Leaving pins 1, 2 and 13 unused
  for (uint8_t k = 3; k < DIGITAL_PINS - 1; k++)
  {
    if (searchByPin(k) == NULL)
      return k;
  }

  return 0;
}

bool System::isPinAvailable(uint8_t pin)
{
  if (pin <= 2 || pin >= DIGITAL_PINS)
    return false;

  if (searchByPin(pin) != NULL)
    return false;

  return true;
}

Relay *System::searchByPin(uint8_t pin)
{

  node_t *aux = first;
  while (aux != NULL)
  {
    if (aux->relay->getPin() == pin)
    {
      return aux->relay;
    }
    aux = aux->next;
  }

  return NULL;
}

char *System::createRelay(uint8_t pin)
{
  if (!isPinAvailable(pin))
    return "Error: pin not available";

  node_t *aux = (node_t *)malloc(sizeof(node_t));
  aux->next = NULL; // Set to NULL to inform that it's a new list entry.

  pinMode(pin, OUTPUT);
  digitalWrite(pin, HIGH);

  aux->relay = new Relay(pin);

  if (aux->next == NULL)
  {
    if (first == NULL)
    {
      first = aux;
      last = aux;
    }
    else
    {
      last->next = aux;
      last = aux;
      // last->next  = NULL;  In this case, next has been set to NULL when memory was allocated.
    }
  }

  return "Relay created.";
}