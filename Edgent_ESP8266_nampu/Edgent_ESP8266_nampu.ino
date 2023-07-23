
// Fill-in information from your Blynk Template here
#define BLYNK_TEMPLATE_ID "TMPLzg12ynAx"
#define BLYNK_DEVICE_NAME "WiFi"

#define BLYNK_FIRMWARE_VERSION        "0.1.0"

#define BLYNK_PRINT Serial
//#define BLYNK_DEBUG

#define APP_DEBUG

// Uncomment your board, or configure a custom board in Settings.h
//#define USE_SPARKFUN_BLYNK_BOARD
//#define USE_NODE_MCU_BOARD
//#define USE_WITTY_CLOUD_BOARD
//#define USE_WEMOS_D1_MINI

#include "BlynkEdgent.h"
#include <SoftwareSerial.h>
SoftwareSerial mySerial(D5, D6);

long currentMillis = 0;
long previousMillis = 0;
int interval = 1000;
int setmode = 0;

#define pingPin_1 D0
#define inPin_1 D7

int set_HH_open;
int set_MM_open;

int set_HH_close;
int set_MM_close;

void setup()
{
  Serial.begin(9600);
  delay(100);
  mySerial.begin(9600);
 
  BlynkEdgent.begin();
}

void loop() {
  BlynkEdgent.run();
  currentMillis = millis();
  if (currentMillis - previousMillis > interval) {
    previousMillis = millis();

   
   
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
      Blynk.virtualWrite(V5, cm_1);


  } // end millis
} // end loop

BLYNK_WRITE(V1) // set
{
  setmode = param.asInt();
  Serial.println("                                               setmode = " + String(setmode));
  if ( setmode == 1 )
  {
    mySerial.println("0,99,1");
    delay(1000);
  }
  if ( setmode == 0 )
  {
    mySerial.println("0,99,0");
    delay(1000);
  }
   if ( setmode == 2 )
  {
    mySerial.println("0,99,2");
    delay(1000);
  }
}
BLYNK_WRITE(V2) // set
{
  int relay = param.asInt();
  Serial.println("                                               relay = " + String(relay));
  if ( relay == 1 && setmode == 0 )
  {
    mySerial.println("3,99,2");
    delay(1000);
  }
  if ( relay == 0 && setmode == 0)
  {
    mySerial.println("3,99,3");
    delay(1000);
  }
}

BLYNK_WRITE(V3)
{

  TimeInputParam t(param);
  Serial.println(String("Start  : ") + t.getStartHour() + ":" + t.getStartMinute());
  set_HH_open = t.getStartHour() ;
  set_MM_open = t.getStartMinute() ;
  mySerial.println("1," + String(set_HH_open) +"," + String(set_MM_open) );
  delay(1000);
}
BLYNK_WRITE(V0)
{

  TimeInputParam t(param);
  Serial.println(String("Stop  : ") + t.getStartHour() + ":" + t.getStartMinute());
  set_HH_close = t.getStartHour() ;
  set_MM_close = t.getStartMinute() ;
   mySerial.println( "2," + String(set_HH_close) +"," + String(set_MM_close) );
  delay(1000);
}

long microsecondsToCentimeters(long microseconds)
{
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  return microseconds / 29 / 2;
}
