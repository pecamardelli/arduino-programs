#include "header.h"

Clock::Clock(){
    this->days[0] = "Sunday";
    this->days[1] = "Monday";
    this->days[2] = "Tuesday";
    this->days[3] = "Wednesday";
    this->days[4] = "Thursday";
    this->days[5] = "Friday";
    this->days[6] = "Saturday";
    this->RTC.begin();
};

void Clock::setDateTime(char *_date, char *_time) {
  if(sscanf(_date, "%d/%d/%d", &_year, &_month, &_day) != 3)
    return sys.printData(F("Bad date format (YYYY/MM/DD)"), true);

  if(sscanf(_time, "%d:%d:%d", &_hour, &_min, &_sec) != 3)
    return sys.printData(F("Bad time format (HH:MM:SS)"), true);

  if(_day >= 0 || _day <= 31) {
    if(_day == 31 &&
        (_month == 2 ||
        _month == 4 ||
        _month == 6 ||
        _month == 9 ||
        _month == 11)) {
      sys.printData(F("ERROR! Month "), false);
      sys.printData(String(_month), false);
      sys.printData(F(" does not have 31 days."), true);
      return;
    }

    if(_month == 2 && _day == 29) {
      int x = _year % 4;
      if(x != 0) {
        sys.printData(F("ERROR! Year "), false);
        sys.printData(String(_year), false);
        sys.printData(F(" is not a leap-year.\n February cannot have 29 days."), true);
        return;
      }
    }        
  }
  else return;
  
  RTC.adjust(DateTime(_year,_month,_day,_hour,_min,_sec));
};

String Clock::getDate() {
  now = RTC.now();
  return  String(now.hour()) + ":" +
          String(now.minute()) + ":" +
          String(now.second()) + " " +
          days[now.dayOfTheWeek()] + " " +
          String(now.day()) + "/" +
          String(now.month()) + "/" +
          String(now.year());
};

DateTime Clock::getTime() {
  return this->RTC.now();
};
