/**
 * @file main.cpp
 * @author Vladislav Kuznetsov of Slavian-Slaves team (kuznetsovv.e.video@gmail.com)
 * @brief main.cpp file for esp8266 firmware for Mordor project
 * @version 0.2
 * @date 2023-03-20
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <Wire.h>
#include <NfcAdapter.h>
#include <PN532.h>
#include <PN532_I2C.h>

 #define DEBUG
/**
 * @brief wifi and host ip setup
 * 
 */
const char* ssid = "Gorcomnet_plus";
const char* password = "91929394";
const String postServerName = "80.90.186.118/mordorhw";

/**
 * @brief initial neccessary declarations
 * 
 */
PN532_I2C pn532_i2c(Wire);
NfcAdapter nfc = NfcAdapter(pn532_i2c);
String tagId = "None";
String dispTag = "None";
byte nuidPICC[4];

/**
 * @brief read NFC tag if present near reader
 * 
 * @return true: tag read and got payload
 * @return false: tag was not read
 */
boolean readNFC() 
{
 if (nfc.tagPresent())
 {
   NfcTag tag = nfc.read();
   tag.print();
   tagId = tag.getUidString();
   delay(1000);
   return true;
 }
 else return false;
 delay(100);
}

/**
 * @brief Send HTTP GET request with NFC tag number and get response
 * 
 * @param id: NFC tag id in String format
 * @return payload: response payload from server
 */
String httpGet(String id){
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
  return payload;
}

/**
 * @brief Convert NFC tag to String format for convinience
 * 
 * @param id: NFC tag ID
 * @return tagId: tag ID converted to String
 */
String tagToString(byte id[4])
{
  String tagId = "";
  for (byte i = 0; i < 4; i++)
  {
    if (i < 3) tagId += String(id[i]) + ".";
    else tagId += String(id[i]);
  }
  return tagId;
}

/**
 * @brief Initial setup for esp8266, pn532 reader, WI-FI
 * 
 */
void setup() {
  Serial.begin(9600);
  pinMode(D3, OUTPUT);
  digitalWrite(D3, LOW);
  nfc.begin();
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}

/**
 * @brief main loop for esp8266 to run:
 *                                     -Read NFC tag;
 *                                     -If tag not read print debug info and wait
 *                                     -If tag present send a GET request and check payload
 * 
 */
void loop() {
  if(readNFC()){
    //httpPost(tagId);
    if(httpGet(tagId) == "True"){
      Serial.println("Got Positive");
      digitalWrite(D3, HIGH);
      delay(2000);
      digitalWrite(D3, LOW);
    }
    delay(5000);
  }
  else{
    Serial.println("error reading/posting");
    delay(2000);
  }
} 
