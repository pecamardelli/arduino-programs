class Clock {
  public:
    Clock(){
      this->days[0] = "Sunday";
      this->days[1] = "Monday";
      this->days[2] = "Tuesday";
      this->days[3] = "Wednesday";
      this->days[4] = "Thursday";
      this->days[5] = "Friday";
      this->days[6] = "Saturday";

      this->RTC.begin();
    }
    
    void setDateTime(char *_date, char *_time) {
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
    }

    String getDate() {
      now = RTC.now();
      return  String(now.hour()) + ":" +
              String(now.minute()) + ":" +
              String(now.second()) + " " +
              days[now.dayOfTheWeek()] + " " +
              String(now.day()) + "/" +
              String(now.month()) + "/" +
              String(now.year());
    }

    DateTime getTime() {
      return this->RTC.now();
    }

  private:
    RTC_DS1307 RTC;
    uint8_t  _hour;
    uint8_t  _min;
    uint8_t  _sec;
    uint8_t  _year;
    uint8_t  _month;
    uint8_t  _day;
    DateTime now;
    String days[7];
 };

Clock clock;
