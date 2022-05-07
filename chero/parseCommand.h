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
  
  if(strncmp(args[0], "help", 4) == 0) showCommands();
  else if(strncmp(args[0], "sysinfo", 7) == 0) systemInfo();
  else if(strncmp(args[0], "show", 4) == 0) {
    if(strncmp(args[1], "help", 4) == 0 or !strlen(args[1])) showHelp();
    else if(strncmp(args[1], "pins", 4) == 0) pinInfo();
    else {
      Serial.print(F("Unknown subcommand \""));
      Serial.print(args[1]);
      Serial.println(F("\""));
    }
  }
  else {
    Serial.print(F("Bad command \""));
    Serial.print(args[0]);
    Serial.println(F("\""));
  }

  free(command);
} 
