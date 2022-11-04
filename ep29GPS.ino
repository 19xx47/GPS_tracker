#define BLYNK_TEMPLATE_ID "TMPLdhmTmdsL"
#define BLYNK_DEVICE_NAME "Quickstart Device"
#define BLYNK_AUTH_TOKEN "BRkxm37EgHZ8yErtV6w_DTj88zS7ny6o"

#define BLYNK_PRINT Serial
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>


static const int RXPin = 4, TXPin = 5;   
static const uint32_t GPSBaud = 9600; 

TinyGPSPlus gps;
WidgetMap myMap(V0);  

SoftwareSerial ss(RXPin, TXPin);

BlynkTimer timer;

float speeding;       
float state;      

char auth[] = "BRkxm37EgHZ8yErtV6w_DTj88zS7ny6o";               //ใส่ของตัวเอง
char ssid[] = "Galaxy";                                       //ใส่ของตัวเอง
char pass[] = "zzzzzzzz";                                      //ใส่ของตัวเอง
      
unsigned int move_index = 1;       
  

void setup()
{
  Serial.begin(115200);
  Serial.println();
  ss.begin(GPSBaud);
  Blynk.begin(auth, ssid, pass);
  timer.setInterval(5000L, checkGPS); 
}

void checkGPS(){
  if (gps.charsProcessed() < 10)
  {
    Serial.println(F("No GPS detected: check wiring."));
     Blynk.virtualWrite(V4, "GPS ERROR");  // Value Display widget  on V4 if GPS not detected
  }
}

void loop()
{
    while (ss.available() > 0) 
    {
      if (gps.encode(ss.read()))
        displayInfo();
  }
  Blynk.run();
  timer.run();
}

void displayInfo()
{ 
  if (gps.location.isValid() ) 
  {    
    float latitude = (gps.location.lat());     
    float longitude = (gps.location.lng()); 
    
    Serial.print("LAT:  ");
    Serial.println(latitude, 6); 
    Serial.print("LONG: ");
    Serial.println(longitude, 6);
    Blynk.virtualWrite(V1, String(latitude, 6));   
    Blynk.virtualWrite(V2, String(longitude, 6));  
    myMap.location(move_index, latitude, longitude, "รถของฉัน");
    speeding = gps.speed.kmph();        
       Blynk.virtualWrite(V3, speeding);
       state = gps.satellites.value();    
       Blynk.virtualWrite(V4, state);               
  }
}
