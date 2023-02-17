#include "header.h"

System::System()
{
  hostname = "arduino";
  ipAddress.fromString("192.168.40.8");
  subnetMask.fromString("255.255.255.0");
  gateway.fromString("192.168.40.1");
  dnsServer.fromString("179.42.171.21");

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

char *System::getSerialInput()
{
  char c;
  uint8_t charIndex = 0;
  const uint8_t charsWaiting = Serial.available();

  if (charsWaiting <= 0)
    return;

  char *input = (char *)malloc(charsWaiting * sizeof(char));

  while (Serial.available() > 0)
  {
    c = Serial.read();

    // Include letters, digits, and other allowed chars. Add more allowed characters
    // at the definition of the specialChars array.
    if (isalpha(c) or isdigit(c) or charAllowed(c))
    {
      input[charIndex++] = (char)c;
    }
    else
    {
      input[charIndex++] = (char)0x20;
    }
      
    delay(5);
  }
  
  return input;
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
