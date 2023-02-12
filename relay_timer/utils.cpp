#include "header.h"

String parseUpTime(unsigned long startTime) {
  unsigned long diff = millis() - startTime;
  int _days = 0;
  int _hours = 0;
  int _mins = 0;
  int _secs = 0;

  while(diff >= 84600000) {
    diff -= 84600000;
    _days++;
  }

  while(diff >= 3600000){
    diff -= 3600000;
    _hours++;
  }

  while(diff >= 60000){
    diff -= 60000;
    _mins++;
  }

  while(diff >= 1000){
    diff -= 1000;
    _secs++;
  }

  String d = (_days < 10) ? "0" + String(_days) : String(_days);
  String h = (_hours < 10) ? "0" + String(_hours) : String(_hours);
  String m = (_mins < 10) ? "0" + String(_mins) : String(_mins);
  String s = (_secs < 10) ? "0" + String(_secs) : String(_secs);
  
  return d + "d " + h + ":" + m + ":" + s;
}

bool charAllowed(char c) {
  for(byte i=0; i<sizeof(specialChars)/sizeof(byte);i++) {
    if (c == specialChars[i]) return true;
  }
  return false;
}

void clearArgs(char **args) {
  for(byte i=0; i<MAX_COMMAND_ARGS; i++) {
    args[i] = "";
  }
}



String arrayToString(byte array[], unsigned int len) {
  char buffer[20] = "";
  
  for (unsigned int i = 0; i < len; i++) {
      byte nib1 = (array[i] >> 4) & 0x0F;
      byte nib2 = (array[i] >> 0) & 0x0F;
      buffer[i*3+0] = nib1 < 0xA ? '0' + nib1 : 'A' + nib1 - 0xA;
      buffer[i*3+1] = nib2 < 0xA ? '0' + nib2 : 'A' + nib2 - 0xA;
      
      if(i < len-1) {
        buffer[i*3+2] = ':';
      }
  }
  
  buffer[len*3] = '\0';
  return String(buffer);
}
