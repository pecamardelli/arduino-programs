#include "header.h"

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