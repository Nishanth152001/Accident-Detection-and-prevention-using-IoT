#include <SoftwareSerial.h>
#include <stdlib.h>
#include <TinyGPS++.h>

#define x A1

#define y A2

#define z A3
#define vib_sensor A4
#define MQ3 A5
#define Thres_Val 350

int xsample=0;

int ysample=0;

int zsample=0;
int vob_data=0;

#define samples 10


#define minVal -60

#define MaxVal 60


int i=0,k=0;
 
static const int RXPin = 3, TXPin = 4;
static const uint32_t GPSBaud = 9600;
 
TinyGPSPlus gps;

SoftwareSerial ss(RXPin, TXPin); 
SoftwareSerial SIM900(10,11);
 

  
  char inchar; 
  String textForSMS; 
  
  double longitude; 
  double latitude;  
  char buff[10];
  
  String mylong = ""; 
  String mylati = ""; 
 
   int gpsflag = 0;
  
   int mycounter = 0;
  
 
   
void setup()
{
  SIM900.begin(19200);
  Serial.begin(9600);
  ss.begin(GPSBaud);
  delay(5000);  
  for(int i=0;i<samples;i++)

  {

    xsample+=analogRead(x);

    ysample+=analogRead(y);

    zsample+=analogRead(z);

  }


  xsample/=samples;

  ysample/=samples;

  zsample/=samples;


  Serial.println(xsample);

  Serial.println(ysample);

  Serial.println(zsample);

  delay(1000);
  randomSeed(analogRead(0));
  pinMode(MQ3, INPUT);
  pinMode(vib_sensor, INPUT);
}
 
 
 
void sendSMS(String message)
{
  SIM900.print("AT+CMGF=1\r");                     
  delay(100);
 SIM900.println("AT+CMGS=\"+917358844341\"");  
  delay(100);
  SIM900.println(message);                     
  delay(100);
  SIM900.println((char)26);                    
  delay(100); 
  SIM900.println();
  delay(5000);                                 
                                  
}
 
void loop()
{
  int value1=analogRead(x);

    int value2=analogRead(y);

    int value3=analogRead(z);
    

    int xValue=xsample-value1;

    int yValue=ysample-value2;

    int zValue=zsample-value3;

    int alc  = analogRead(MQ3);
    int vib  = analogRead(vib_sensor);

    

    Serial.print("x=");

    Serial.println(xValue);
 
    Serial.print("y=");

    Serial.println(yValue);

    Serial.print("z=");

    Serial.println(zValue);

    Serial.print("Alcohol: ");

    Serial.println(alc);
    Serial.print("Vibration Sensor: ");

    Serial.println(vib);
 
 
  while (ss.available() > 0)
    if ((gps.encode(ss.read())) && (gpsflag == 0))
    {
     displayInfo();    
    gpsflag = 1;
    }
 
  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
    while(true);
  }
 
  if(xValue < minVal || xValue > MaxVal  || yValue < minVal || yValue > MaxVal  || zValue < minVal || zValue > MaxVal)
  {
 displayInfo();
 latitude = gps.location.lat(), 6 ;
 longitude = gps.location.lng(), 6 ;
// for latitude

  mylati = dtostrf(latitude, 3, 6, buff);
  mylong = dtostrf(longitude, 3, 6, buff);
  
 //textForSMS = textForSMS + "Latitude:" + "," + mylati + "," + "Longitude:  "+ mylong + ",";
 textForSMS = "Accident:\n Latitude:"+ mylati + "\nLongitude:"+mylong;
 textForSMS = textForSMS + "\nhttp://www.google.com/maps/place/" + mylati + "," + mylong ; 
  sendSMS(textForSMS);
    textForSMS = "";
  Serial.println(textForSMS);
  Serial.println("message sent.");
  delay(2000);
  }
 
 if(alc>Thres_Val)
  {
 displayInfo();
 latitude = gps.location.lat(), 6 ;
 longitude = gps.location.lng(), 6 ;
// for latitude
  mylati = dtostrf(latitude, 3, 6, buff);
  mylong = dtostrf(longitude, 3, 6, buff);
  
 //textForSMS = textForSMS + "Latitude:" + "," + mylati + "," + "Longitude:  "+ mylong + ",";
 textForSMS = "Alcohol Alert:\n Latitude:"+ mylati + "\nLongitude:"+mylong;
 textForSMS = textForSMS + "\nhttp://www.google.com/maps/place/" + mylati + "," + mylong ; 
  sendSMS(textForSMS);
    textForSMS = "";
  Serial.println(textForSMS);
  Serial.println("message sent.");
  delay(2000);
  }
 
 
 
mycounter = mycounter + 1; 
 
if ( (mycounter > 50) && ( mycounter < 100))
{
  gpsflag = 0; 
  ss.begin(GPSBaud);
}
 
 
 
if ( mycounter > 100 )
mycounter = 0;
 
 
 
}
 
 
 
 
void displayInfo()
{
  Serial.print(F("Location: ")); 
  if (gps.location.isValid())
  {
    Serial.print(gps.location.lat(), 6);
    Serial.print(F(","));
    Serial.print(gps.location.lng(), 6);
    Serial.print(" ");
    Serial.print(F("Speed:"));
    Serial.print(gps.speed.kmph());
  }
  else
  {
    Serial.print(F("INVALID"));
  }
 
  Serial.print(F("  Date/Time: "));
  if (gps.date.isValid())
  {
    Serial.print(gps.date.month());
    Serial.print(F("/"));
    Serial.print(gps.date.day());
    Serial.print(F("/"));
    Serial.print(gps.date.year());
  }
  else
  {
    Serial.print(F("INVALID"));
  }
 
  Serial.print(F(" "));
  if (gps.time.isValid())
  {
    if (gps.time.hour() < 10) Serial.print(F("0"));
    Serial.print(gps.time.hour());
    Serial.print(F(":"));
    if (gps.time.minute() < 10) Serial.print(F("0"));
    Serial.print(gps.time.minute());
    Serial.print(F(":"));
    if (gps.time.second() < 10) Serial.print(F("0"));
    Serial.print(gps.time.second());
    Serial.print(F("."));
    if (gps.time.centisecond() < 10) Serial.print(F("0"));
    Serial.print(gps.time.centisecond());
  }
  else
  {
    Serial.print(F("INVALID"));
  }
 
  Serial.println();
 
}void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
