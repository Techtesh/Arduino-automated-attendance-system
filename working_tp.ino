#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>
#define USE_SERIAL Serial
ESP8266WiFiMulti WiFiMulti;

#define SS_PIN 4  //D2
#define RST_PIN 5 //D1
#include <SPI.h>
#include <MFRC522.h>

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
int statuss = 0;
String buf;
int out = 0;
String payload;


void setup() 
{
  Serial.begin(115200);   // Initiate a serial communication
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
   pinMode(LED_BUILTIN, OUTPUT); // FOR LED
     
  
    WiFiMulti.addAP("TP-LINK_633A", "etrx@205");
    
    for(uint8_t t = 4; t > 0; t--) {
        USE_SERIAL.printf("waiting for connection %d...\n", t);
        USE_SERIAL.flush();
        delay(1000);
    }
      if(WiFiMulti.run() == WL_CONNECTED)
     Serial.println("connected");
     digitalWrite(LED_BUILTIN, LOW);   delay(500);              
  digitalWrite(LED_BUILTIN, HIGH);  
}

void database()
{
  
  // wait for WiFi connection
    if((WiFiMulti.run() == WL_CONNECTED)) {

        HTTPClient http;

        USE_SERIAL.print("[HTTP] begin...\n");
        // configure traged server and url
      String abc = "http://192.168.110.73/adduser.php?regno=&name=" + buf + "&addr=&class=&mobileno=";
        http.begin(abc); //HTTP

        USE_SERIAL.print("[HTTP] GET...\n");
        // start connection and send HTTP header
        int httpCode = http.GET();

        // httpCode will be negative on error
        if(httpCode > 0) {
            // HTTP header has been send and Server response header has been handled
            USE_SERIAL.printf("[HTTP] GET... code: %d\n", httpCode);

            // file found at server
            if(httpCode == HTTP_CODE_OK) {
                payload = http.getString();
                USE_SERIAL.println(payload);
                  subss();                    
            }
        } else {
            USE_SERIAL.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }

        http.end();
    }

    delay(1000);
}

void subss()
{
   if (payload.substring(68, 75) == "allowed") {
    Serial.println("allowed");
     digitalWrite(LED_BUILTIN, LOW);   
     delay(1000);                      
  digitalWrite(LED_BUILTIN, HIGH); 
  }
  else {
    Serial.println("failed");
   // Serial.println(payload.length());
    //Serial.println(payload.substring(68, 75));
     digitalWrite(LED_BUILTIN, LOW);   
     delay(5000);                      
  digitalWrite(LED_BUILTIN, HIGH);  
  }
}



void loop() 
{
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Show UID on serial monitor
  //Serial.print(" UID tag :");
  String content= "";

  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     content.concat(String(mfrc522.uid.uidByte[i]));
  }
buf=content.substring(0,content.length()-1);
  
  Serial.print(buf);

  delay(1000);
  database();

} 
