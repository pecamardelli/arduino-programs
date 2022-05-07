void parseCommand(char *command) {
  // Tokenizing arguments
  char *args[MAX_COMMAND_ARGS];
  clearArgs(args);

  // Using space character as delimiter
  const char *delimiter = " ";
  
  char *token = strtok(command, delimiter);
  byte index = 0;

  while(token != NULL) {
    args[index] = token;
    index++;
    token = strtok(NULL, delimiter);
  }
  //else if(strncmp(args[0], "set", 3) == 0) {
  //  if(strncmp(args[1], "help", 4) == 0 or !strlen(args[1])) setCommandHelp();
  //  else if(strncmp(args[1], "hostname", 8) == 0) setHostname(args[2]);  // By default, args[2] is an empty string. setHostname will deal with it.
  //  else if(strncmp(args[1], "ip", 2) == 0) setAddress(args[2], IP_ADDRESS);
   // else if(strncmp(args[1], "mask", 2) == 0) setAddress(args[2], SUBNET_MASK);
  //  else if(strncmp(args[1], "gateway", 7) == 0) setAddress(args[2], DEFAULT_GATEWAY);
//    else if(strncmp(args[1], "dns", 3) == 0) setAddress(args[2], DNS_SERVER);
    //else if(strncmp(args[1], "mac", 3) == 0) Serial.println(F("Not implemented..."));
    //else if(strncmp(args[1], "datetime", 3) == 0) setDateTime(args[2], args[3]);
    //else {
      //Serial.print(F("Unknown subcommand \""));
  
  if(strncmp(args[0], "help", 4) == 0) {
    Serial.println(F("To be implemented..."));
  }
      //Serial.print(args[1]);
      //Serial.println(F("\""));
    //}
  //}
  else {
    Serial.print(F("Bad command \""));
    Serial.print(args[0]);
    Serial.println(F("\""));
  }
} 
