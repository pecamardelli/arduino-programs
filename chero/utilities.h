bool isCharValid(char c) {
  for(byte i = 0; i < sizeof(specialChars) / sizeof(byte); i++) {
    if (c == specialChars[i]) return true;
  }
  return false;
}

void clearArgs(char **args) {
  for(byte i = 0; i < MAX_COMMAND_ARGS; i++) {
    args[i] = "";
  }
}
