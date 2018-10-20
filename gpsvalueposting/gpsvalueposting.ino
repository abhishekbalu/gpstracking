/**
 * Arduino Code to post data to webpage using WEMOSd1
 * 
 * posting data to web server.
 * 
 * Done by Abhishek B (www.abhishekbalu.com)
 */




#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>

#define USE_SERIAL Serial

ESP8266WiFiMulti WiFiMulti;

  //String data;
 int hi = 2203;
 int sp =100;
String latitude ;
String longitude ;
 


 static const int RXPin = 12, TXPin = 13;
 static const uint32_t GPSBaud = 9600;

// The TinyGPS++ object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);

void setup() {
      USE_SERIAL.begin(19200);
   // USE_SERIAL.setDebugOutput(true);0
      ss.begin(GPSBaud);
      USE_SERIAL.println(TinyGPSPlus::libraryVersion());

      USE_SERIAL.println();
      USE_SERIAL.println();
      USE_SERIAL.println();
 //USE_SERIAL.print(gps.location.lat(), 6);
    for(uint8_t t = 4; t > 0; t--) {
        USE_SERIAL.printf("[SETUP] WAIT %d...\n", t);
        //USE_SERIAL.flush();
        delay(1000);
    }

    WiFiMulti.addAP("abhishek", "Abhishekehsihba@22");
    
}

  void loop()
  {
    // This sketch displays information every time a new sentence is correctly encoded.
   // USE_SERIAL.flush();
   while (ss.available() > 0)
      if (gps.encode(ss.read()))
      {
         displayInfo();
               
       }
     
      if (millis() > 10000 && gps.charsProcessed() < 10)
    {
      USE_SERIAL.println(F("No GPS detected: check wiring."));
      while(true);
    }
  
     if((WiFiMulti.run() == WL_CONNECTED)) {
     
      postRequest(gps.location.lat(),  gps.location.lng());
   
     }
  }

  void displayInfo()
  {
        USE_SERIAL.print(F("Location: ")); 
     delay(50);
    if (gps.location.isValid())
    {
              USE_SERIAL.print("2020");

        USE_SERIAL.print(gps.location.lat(), 6);
//        latitude=gps.location.lat();
//       USE_SERIAL.print(latitude);
       USE_SERIAL.print(F(","));
      USE_SERIAL.print(gps.location.lng(), 6);
//      longitude=gps.location.lng();
//       USE_SERIAL.print(longitude);
  
      sp=gps.speed.kmph();
   }
    else
    {
       USE_SERIAL.print(F("INVALID"));
    }
  
     USE_SERIAL.print(F("  Date/Time: "));
    if (gps.date.isValid())
    {
       USE_SERIAL.print(gps.date.month());
       USE_SERIAL.print(F("/"));
       USE_SERIAL.print(gps.date.day());
       USE_SERIAL.print(F("/"));
       USE_SERIAL.print(gps.date.year());
    }
    else
    {
       USE_SERIAL.print(F("INVALID"));
    }
  
     USE_SERIAL.print(F(" "));
    if (gps.time.isValid())
    {
      if (gps.time.hour() < 10)  USE_SERIAL.print(F("0"));
       USE_SERIAL.print(gps.time.hour());
       USE_SERIAL.print(F(":"));
      if (gps.time.minute() < 10)  USE_SERIAL.print(F("0"));
       USE_SERIAL.print(gps.time.minute());
       USE_SERIAL.print(F(":"));
      if (gps.time.second() < 10)  USE_SERIAL.print(F("0"));
       USE_SERIAL.print(gps.time.second());
       USE_SERIAL.print(F("."));
      if (gps.time.centisecond() < 10)  USE_SERIAL.print(F("0"));
       USE_SERIAL.print(gps.time.centisecond());
    }
    else
    {
       USE_SERIAL.print(F("INVALID"));
    }
  
     USE_SERIAL.println();
  }
  void postRequest(float latitude1, float longitude1)
  {
          int speeds =0;
          HTTPClient http;
  
          USE_SERIAL.print("[HTTP] begin...\n");
  
          
          http.begin("your webpage link");     // enter the web server to which u want to post data.
         // delay(10000);
          http.addHeader("Content-Type", "application/x-www-form-urlencoded", false, true);    // use "application/x-www-form-urlencoded" as universal format 
            
          USE_SERIAL.print("[HTTP] POST...\n");        // start connection and send HTTP header
          
         int httpCode = http.POST("device_id=" +String(hi) + "&latitude=" +float(latitude1)+ "&longitude=" +float(longitude1) + "&speed=" +int(sp) );        // httpCode will be negative on error
        //int httpCode = http.POST("hi");
          if(httpCode > 0) {
            
              USE_SERIAL.printf("[HTTP] POST... code: %d\n", httpCode);
  
             
              
              if(httpCode == HTTP_CODE_OK) {
                
                  String payload = http.getString();
                  
                  USE_SERIAL.println(payload);
                  
              }
              
         } 
         else {
            
              USE_SERIAL.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
          }
  
        http.end();
        delay(4500);
      }
   
      
  
