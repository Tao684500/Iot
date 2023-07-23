#include <Wire.h>
// Date and time functions using a DS3231 RTC connected via I2C and Wire lib
#include "RTClib.h"
RTC_DS3231 rtc;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

String timenow;

int HH, MM, secs;

int set_HH_open = 12;
int set_MM_open = 0;


int set_HH_close = 13;
int set_MM_close = 0;
char data[50];
int id;
int datas1;
int datas2;
#include<SoftwareSerial.h>
SoftwareSerial mySerial (10, 11);
//-------------------------------
const int pingPin_1 = 9;
int inPin_1 = 8;
int setmode = 2;
int starts = 0;
int setmins = 5;
int sec = 0;
int mins = 0;
int button = 6;
int relay = 7;
void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
  pinMode(relay, OUTPUT);
  digitalWrite(relay, HIGH);

  pinMode(button, INPUT_PULLUP);


  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);
  }

 // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

}

void loop() {


  if ( mySerial.available())
  {
    String datawifi = mySerial.readString();
    Serial.println(" dataWiFi = " + String(datawifi));
    datawifi.toCharArray(data, 50);
    int n = sscanf(data, "%d,%d,%d", &id , &datas1, &datas2 );
    Serial.print(F("n="));
    Serial.println(n);
    Serial.print(F("   id ="));
    Serial.print(id);
    Serial.print(F("   datas1="));
    Serial.print(datas1);
      Serial.print(F("   datas2="));
    Serial.print(datas2);
    Serial.println();
    if ( id == 0 && datas1 == 99 && datas2 == 1 )
    {
      Serial.println("                                            Auto Mode ");
      setmode = 1;
      digitalWrite(relay, HIGH);
      mins = 0;
      sec = 0;
      starts = 0;
    }
    if (id == 0 && datas1 == 99 && datas2 == 0 )
    {
      Serial.println("                                            Blynk Mode ");
      setmode = 0;
      digitalWrite(relay, HIGH);
      mins = 0;
      sec = 0;
      starts = 0;
    }
     if (id == 0 && datas1 == 99 && datas2 == 2 )
    {
      Serial.println("                                            Button Mode ");
      setmode = 2;
      digitalWrite(relay, HIGH);
      mins = 0;
      sec = 0;
      starts = 0;
    }
    //---------------------------------------
    if (id == 3 &&  datas1 ==  99 && datas2 == 2 )
    {
      Serial.println("                                         Blynk / Relay ON ");
      digitalWrite(relay, LOW);
    }
    if ( id == 3 && datas1 == 99 && datas2 == 3 )
    {
      Serial.println("                                         Blynk / Relay OFF ");
      digitalWrite(relay, HIGH);
    }
    if ( id == 1 )
    {
      set_HH_open = datas1;
      set_MM_open = datas2;
      Serial.println("                                   set open > "+String(set_HH_open)+":"+String(set_MM_open));
    }
    if ( id == 2 )
    {
      set_HH_close = datas1;
      set_MM_close = datas2;
      Serial.println("                                   set close > "+String(set_HH_close)+":"+String(set_MM_close));
    }



  } // end mySerial




  DateTime now = rtc.now();

  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(" (");
  Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
  Serial.print(") ");
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();
  HH = now.hour();
  MM = now.minute();
  secs = now.second();
  timenow = String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second());
  if ( HH == set_HH_open && MM == set_MM_open && setmode == 0 && secs == 0  )
  {
    Serial.println("                                                  Timer ON ");
    digitalWrite(relay, LOW);
  }
  if ( HH == set_HH_close && MM == set_MM_close && setmode == 0 && secs == 0 )
  {
    Serial.println("                                                  Timer OFF ");
    digitalWrite(relay, HIGH);
  }
  //------------------------------------------------------------------------------------------------

  long duration_1, cm_1;
  pinMode(pingPin_1, OUTPUT);
  digitalWrite(pingPin_1, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin_1, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingPin_1, LOW);
  pinMode(inPin_1, INPUT);
  duration_1 = pulseIn(inPin_1, HIGH);
  cm_1 = microsecondsToCentimeters(duration_1);
  Serial.print(cm_1);
  Serial.print("cm");
  Serial.println();
  if ( setmode == 1 )
  {
    Serial.println("            Auto");
    if ( cm_1 <= 50 )
    {
      starts = 1;
      Serial.println("                      Cat !!");
      digitalWrite(relay, LOW);
      delay(500);
    }
    if ( starts == 1 )
    {
      sec += 1;
      delay(800);
      if ( sec > 59 )
      {
        mins += 1;
        sec = 0;
      }
      Serial.println("                           จับเวลา > " + String(mins) + ":" + String(sec));

      if ( mins == 1 && sec == 0 ) // ถ้าครบเวลา
      {
        digitalWrite(relay, HIGH);
        mins = 0;
        sec = 0;
        starts = 0;
      }
    } // end starts
  } // end auto
  //----------------------------------------------------------------------
  
    if ( digitalRead(button) == LOW && setmode == 2 )
    {
      Serial.println("                                            !!!!!");
      digitalWrite(relay, LOW);

    }
    if ( digitalRead(button) == HIGH && setmode == 2)
    {
      digitalWrite(relay, HIGH);
    }
  

} // end loop

long microsecondsToCentimeters(long microseconds)
{
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  return microseconds / 29 / 2;
}
