#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
const char* ssid = "Gorcomnet_plus";
const char* password = "91929394";
//const char* serverName = "http://192.168.0.247:5000/test";
#define DEBUG
unsigned long lastTime = 0;
unsigned long timerDelay = 10000;
String id = "None";


void setup() {
  Serial.begin(9600);
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.println("Timer set to 10 seconds");
}

void loop() {
  if ((millis() - lastTime) > timerDelay) {
    if(WiFi.status()== WL_CONNECTED){
      WiFiClient client;
      HTTPClient http;
      String payload = "{}";
      String getServerName = "http://192.168.0.247:5000/test-get?rfid=" + id; 
      http.begin(client, getServerName);
      int httpResponseCode = http.GET();
      if (httpResponseCode>0) {
        #ifdef DEBUG
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        #endif
        payload = http.getString();
        #ifdef DEBUG
        Serial.println(payload);
        #endif
    }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
      http.end();
      Serial.println ("payload");
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
} 
