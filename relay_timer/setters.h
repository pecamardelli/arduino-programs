void setHostname(char *_name) {
  if(strlen(_name) == 0) return;
    
  if(strlen(_name) > MAX_HOSTNAME_LEN) return;

  // Looks like we're good...
  printData(_name, true);
  strcpy(sys.hostname, _name);

  // Inform that changes were made.
  sysChangeFlag = true;
}

void setAddress(char *_address, int _type) {
  IPAddress addr;
  // Check validity
  if (!addr.fromString(_address)) return;

  switch(_type) {
    case IP_ADDRESS:
      sys.ip[0]        = addr[0];
      sys.ip[1]        = addr[1];
      sys.ip[2]        = addr[2];
      sys.ip[3]        = addr[3];
      break;
    case SUBNET_MASK:
      sys.subnet[0] = addr[0];
      sys.subnet[1] = addr[1];
      sys.subnet[2] = addr[2];
      sys.subnet[3] = addr[3];
      break;
    case DEFAULT_GATEWAY:
      sys.gateway[0]  = addr[0];
      sys.gateway[1]  = addr[1];
      sys.gateway[2]  = addr[2];
      sys.gateway[3]  = addr[3];
      break;
    case DNS_SERVER:
      sys.dns[0]    = addr[0];
      sys.dns[1]    = addr[1];
      sys.dns[2]    = addr[2];
      sys.dns[3]    = addr[3];
      break;
  }
  
  sysChangeFlag = true;
}

void setDateTime(char *_date, char *_time) {
  int  _hour;
  int  _min;
  int  _sec;
  int  _year;
  int  _month;
  int  _day;

  if(sscanf(_date, "%d/%d/%d", &_year, &_month, &_day) != 3)
    return printData(F("Bad date format (YYYY/MM/DD)"), true);

  if(sscanf(_time, "%d:%d:%d", &_hour, &_min, &_sec) != 3)
    return printData(F("Bad time format (HH:MM:SS)"), true);

  if(_day >= 0 || _day <= 31) {
    if(_day == 31 &&
       (_month == 2 ||
       _month == 4 ||
       _month == 6 ||
       _month == 9 ||
       _month == 11)) {
      printData(F("ERROR! Month "), false);
      printData(String(_month), false);
      printData(F(" does not have 31 days."), true);
      return;
    }

    if(_month == 2 && _day == 29) {
      int x = _year % 4;
      if(x != 0) {
        printData(F("ERROR! Year "), false);
        printData(String(_year), false);
        printData(F(" is not a leap-year.\n February cannot have 29 days."), true);
        return;
      }
    }        
  }
  else return;
  
  RTC.adjust(DateTime(_year,_month,_day,_hour,_min,_sec));
  printData(F("Date successfully updated: "), false);
  printData(String(getDate()), true);
}
