// Generic help
void printHelp() {
}

void showCommandHelp() {
}

void relayCommandHelp() {
}

void setCommandHelp() {
}

// Send data to the current interface (telnet or serial)
void printData(String data, bool carriageReturn) {
  // output variable is set to COMM_TELNET when a connection arrives. Otherwise, is set to COMM_SERIAL. 
  switch(output) {
    case COMM_SERIAL:
      if(carriageReturn) Serial.println(data);
      else Serial.print(data);
      break;
    case COMM_TELNET:
      //if(carriageReturn) client.println(data);
      //else client.print(data);
      break;
  }
}

void printIpAddress() {
}
