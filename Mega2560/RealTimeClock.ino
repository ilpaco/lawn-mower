/**
   References:
   https://www.pjrc.com/teensy/td_libs_DS1307RTC.html
   https://www.pjrc.com/teensy/td_libs_Time.html
*/

// DS1307RTC - Version: Latest
#include <DS1307RTC.h>
#include <Wire.h>

//Global Variables
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
int lastRTClockError=0;  //latest error code on Real Time Clock
tmElements_t tm; //real time/date object

/**
   Output= true ok or false KO
   Error=
    999 - DS1307 is stopped. Run SetTime
    998 - DS1307 read error. Check circuitry
  Warning=
    196 - DS1307 Not installed
    TODO va inserito la gestione degli eventi di allarme su cloud.
**/
boolean checkRealTimeClock() {
  String timestamp="";
  if (RTC_ON) {
    if (RTC.read(tm)) {
      if (logEnable)  Serial.println("Debug -- DS1307RTC Read Test");
      timestamp = tm.Day + '/' + tm.Month + '/' + tmYearToCalendar(tm.Year);
      if (logEnable) Serial.print("Date:" + timestamp);
      timestamp = tm.Hour + ':' + tm.Minute + ':' + tm.Second;
      if (logEnable) Serial.println(" Time:" + timestamp);
      return true;
    } else {
      if (RTC.chipPresent()) {
        lastRTClockError=999;
        Serial.println("Error 999");
      } else {
        lastRTClockError=998;
        Serial.println("Error 998");
      }
      return false;
    }
  } else {
    lastRTClockError=196;
    Serial.println("Warning 196");
    return true;
  }
}

/**
   Input=type of date 0=Date, 1=Time, 2=Minutes, 3=day, 4=hours
   Output= String with time requested or error code
   Error=
    999 - DS1307 is stopped. Run SetTime
    998 - DS1307 read error. Check circuitry
    Warning=
    196 - DS1307 Not installed
    TODO va inserito la gestione degli eventi di allarme su cloud.
**/
String getDateTime(int type) {
  String dateTime = "";
  if (RTC_ON) {
    if (RTC.read(tm)) {
      switch (type) {
        case 0: //Date
          dateTime = tm.Day + '/' + tm.Month + '/' + tmYearToCalendar(tm.Year);
          break;
        case 1: //Time hh:mm:ss
          dateTime = tm.Hour + ':' + tm.Minute + ':' + tm.Second;
          break;
        case 2: //Minutes
          dateTime = tm.Minute ;
          break;
        case 3: //day
          dateTime = tm.Day;
          break;
        case 4: //hour
          dateTime = tm.Hour;
          break;
      }
      if (logEnable) Serial.println("Debug - getDateTime:" + dateTime);
    } else {
      if (RTC.chipPresent()) {
        dateTime = "999";
        lastRTClockError=999;
        Serial.println("Error 999");
      } else {
        dateTime = "998";
        lastRTClockError=998;
        Serial.println("Error 998");
      }
    }
  } else {
    dateTime = "196";
    Serial.println("Warning 196");
  }
  return dateTime;
}

/**
   TODO Occorre implementare la chiamata alla funzione
   Input= date and time
   Output= true OK or false if KO
   Error=
    997 - DS1307 write error. Check circuitry
  Warning=
    196 - DS1307 Not installed or disabled by firmware
**/
boolean setDateTime(int day, int month, int year, int hour, int minutes, int second) {
  tm.Hour = hour;
  tm.Minute = minutes;
  tm.Second = second;
  tm.Day = day;
  tm.Month = month;
  tm.Year = year;
  if (RTC_ON) {
    if (RTC.write(tm)) {
      Serial.println("DS1307 date and time setted");
      lastRTClockError=0;
      return true;
    } else {
      Serial.println("Error 997");
      lastRTClockError=997;
      return false;
    }
  } else {
    Serial.println("Warning 196");
    lastRTClockError=196;
    return true;
  }
}
