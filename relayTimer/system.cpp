#include "header.h"

extern NodeList nodes;

System::System()
{
  strcpy(hostname, "arduino");
  ipAddress.fromString(F("192.168.40.8"));
  subnetMask.fromString(F("255.255.255.0"));
  gateway.fromString(F("192.168.40.1"));
  dnsServer.fromString(F("179.42.171.21"));
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

String System::setHostname(char *newHostname)
{
  if (strlen(newHostname) == 0)
    return F("Error: hostname cannot be empty.");

  if (strlen(newHostname) > MAX_HOSTNAME_LEN)
    return F("Error: hostname exceedes maximum.");

  // Looks like we're good...
  strncpy(hostname, newHostname, MAX_HOSTNAME_LEN);

  return newHostname;
};

String System::setIpAddress(char *newIpAddress)
{
  if (!ipAddress.fromString(newIpAddress))
    return F("Error: invalid ip address.");

  Ethernet.setLocalIP(ipAddress);

  return newIpAddress;
};

String System::setSubnetMask(char *newSubnetMask)
{
  if (!subnetMask.fromString(newSubnetMask))
    return F("Error: invalid subnet mask.");

  Ethernet.setSubnetMask(subnetMask);

  return newSubnetMask;
};

String System::setDefaultGateway(char *newDefaultGateway)
{
  if (!gateway.fromString(newDefaultGateway))
    return F("Error: invalid default gateway.");

  Ethernet.setGatewayIP(gateway);

  return newDefaultGateway;
};

String System::setDnsServer(char *newDnsServer)
{
  if (!dnsServer.fromString(newDnsServer))
    return F("Error: invalid DNS server.");

  Ethernet.setDnsServerIP(dnsServer);

  return newDnsServer;
};

String System::exec(Command *com)
{
  String output = "";
  // ----- Do some debugging ----- //

  // for (uint8_t i = 0; i < com->count; i++)
  // {
  //     output += "Argument " + String(i) + ": ");
  //     output += com->args[i]);
  // }
  // output += "Total args: " + String(com->count));

  // ---------------------------- //

  // output += "Free memory before: " + String(sys.getFreeMemory()));

  if (strncmp(com->args[0], "help", 4) == 0)
  {
    output += F("time\t\t\t--> Returns the current time.\n");
    output += F("relay (pinNumber)\t--> Runs command on specified relay. Use 'relay help' for further options.\n");
    output += F("set (parameter)\t\t--> Sets a given parameter. Use 'set help' for further options.\n");
    output += F("show (parameter)\t--> Shows specified information. Use 'show help' for further options.\n");
    output += F("save\t\t\t--> Save changes.\n");
    output += F("exit\t\t\t--> Close connection (telnet only).\n");
    output += F("quit\t\t\t--> Close connection (telnet only).\n");
  }
  else if (strncmp(com->args[0], "set", 3) == 0)
  {
    if (strncmp(com->args[1], "help", 4) == 0 or !strlen(com->args[1]))
    {
      output += F("hostname\t\t--> Sets the system name.\n");
      output += F("ip\t\t--> Sets system's ip address.\n");
      output += F("mask\t\t--> Sets system's subnet mask.\n");
      output += F("gateway\t\t--> Sets system's default gateway.\n");
      output += F("dns\t\t--> Sets system's dns server.\n");
      output += F("mac\t\t--> Sets system's mac address.\n");
    }
    else if (strncmp(com->args[1], "hostname", 8) == 0)
      output = sys.setHostname(com->args[2]); // By default, args[2] is an empty string. setHostname will deal with it.
    else if (strncmp(com->args[1], "ip", 2) == 0)
      output = sys.setIpAddress(com->args[2]);
    else if (strncmp(com->args[1], "mask", 2) == 0)
      output = sys.setSubnetMask(com->args[2]);
    else if (strncmp(com->args[1], "gateway", 2) == 0)
      output = sys.setDefaultGateway(com->args[2]);
    else if (strncmp(com->args[1], "dns", 2) == 0)
      output = sys.setDnsServer(com->args[2]);
    else if (strncmp(com->args[1], "mac", 2) == 0)
      output = F("Not implemented");

    else if (strncmp(com->args[0], "relay", 5) == 0)
    {
      output += F("Unknown subcommand \"");
      output += com->args[1];
      output += F("\"");
    }
  }
  else if (strncmp(com->args[0], "relay", 5) == 0)
  {
    if (strncmp(com->args[1], "help", 4) == 0 or !strlen(com->args[1]))
    {
      output += F("relay info\t\t\t\t-> Shows this information.\n");
      output += F("relay (pinNumber) create\t\t-> Creates a relay at the specified pin.\n");
      output += F("relay (pinNumber) delete \t\t-> Deletes the specified relay.\n");
      output += F("relay (pinNumber) enable\t\t-> Enables the specified relay.\n");
      output += F("relay (pinNumber) disable\t\t-> Disables the specified relay.\n");
      output += F("relay (pinNumber) resume\t\t-> Resumes operation of the specified relay.\n");
      output += F("relay (pinNumber) desc (string)\t\t-> Adds a description to the specified relay.\n");
      output += F("relay (pinNumber) starth (hour)\t\t-> Sets starting hour.\n");
      output += F("relay (pinNumber) endh (hour)\t\t-> Sets the ending hour.\n");
      output += F("relay (pinNumber) startm (minute)\t-> Sets the starting minute.\n");
      output += F("relay (pinNumber) endm (minute)\t\t-> Sets the ending minute.\n");
      output += F("relay (pinNumber) pin (pinNumber)\t-> Sets a new pin for the specified relay.\n");
      output += F("relay (pinNumber) on (pinNumber)\t-> Turns on the specified relay.\n");
      output += F("relay (pinNumber) off (pinNumber)\t-> Turns off the specified relay.\n");
      return output;
    }
    else if (strncmp(com->args[1], "info", 4) == 0)
    {
      output = nodes.getRelayInfo();
      return output;
    }

    const uint8_t pin = atoi(com->args[1]);
    // output += "Pin: " + String(pin));

    if (strncmp(com->args[2], "create", 6) == 0)
    {
      output = nodes.createRelay(pin);
    }
    else if (strncmp(com->args[2], "on", 2) == 0)
    {
      Relay *relay = nodes.searchByPin(pin);
      if (relay == NULL)
      {
        output = F("No relay defined on pin ");
        output += pin;
        return output;
      }

      relay->setMode(LOW);

      output = F("Relay ");
      output += pin;
      output += F(" on");
    }
    else if (strncmp(com->args[2], "off", 3) == 0)
    {
      Relay *relay = nodes.searchByPin(pin);
      if (relay == NULL)
      {
        output = F("No relay defined on pin ");
        output += pin;
        return output;
      }

      relay->setMode(HIGH);

      output = F("Relay ");
      output += pin;
      output += F(" off");
    }
    else
    {
      output += F("Unknown subcommand: ");
      output += com->args[1];
      output += "\n";
    }
  }
  else if (strncmp(com->args[0], "freemem", 7) == 0)
  {
    output = F("Free memory: ");
    output += String(sys.getFreeMemory());
    output += F(" bytes.");
  }
  else if (strncmp(com->args[0], "hostname", 8) == 0)
    output = sys.hostname;
  // else if (strncmp(com->args[0], "date", 4) == 0)
  //     output += clock.getDate());
  else if (strncmp(com->args[0], "exit", 4) == 0 || strncmp(com->args[0], "quit", 4) == 0)
  {
    telnet.closeConnection();
  }
  else
  {
    output = F("Bad command: ");
    output += String(com->args[0]) + "\n";
  }

  return output;
  // output += "Free memory after: " + String(sys.getFreeMemory()));
};