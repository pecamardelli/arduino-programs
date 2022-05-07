char *readSerial() {
  char c;
  byte charsWaiting   = 0;
  byte charsReceived  = 0;
  char *line          = NULL;
  
  line = (char*) malloc (MAX_LINE_LEN*sizeof(char));
  
  while(Serial.available()) {
    c = (char)Serial.read();
    
    // Include letters, digits, and other allowed chars. Add more allowed characters
    // at the definition of the specialChars array.
    if (isalpha(c) or isdigit(c) or isCharValid(c)) {
      line[charsReceived] = (char)c;
      charsReceived++;
      if (charsReceived >= MAX_LINE_LEN) break;
    }
    delay(5);
  }
  
  line[charsReceived] = (char)0x20;
    
  if (charsReceived > 0) return line;
  else {
    free(line);
    return NULL;
  }
}
