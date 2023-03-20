#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <Wire.h>
#include <NfcAdapter.h>
#include <PN532.h>
#include <PN532_I2C.h>
// #include <PN532_HSU.h>

// #define DEBUG

const char* ssid = "Gorcomnet_plus";
const char* password = "91929394";
const char* serverName = "http://192.168.0.247:5000/test";
// unsigned long lastTime = 0;
// unsigned long timerDelay = 10000;

// PN532_HSU pn532hsu(Serial);
// PN532 nfc(pn532hsu);
PN532_I2C pn532_i2c(Wire);
NfcAdapter nfc = NfcAdapter(pn532_i2c);
String tagId = "None";
String dispTag = "None";
byte nuidPICC[4];

void setup() {
  Serial.begin(9600);
  nfc.begin();
  #ifdef DEBUG
  uint32_t versiondata = nfc.getFirmwareVersion();
  if (! versiondata)
  {
    Serial.print("Didn't Find PN53x Module");
  }
  Serial.print("Found chip PN5");
  #endif
  //nfc.SAMConfig();
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  // Serial.println("Timer set to 10 seconds");
}

bool httpPost(String rfid){
    if(WiFi.status()== WL_CONNECTED){
      WiFiClient client;
      HTTPClient http;
      http.begin(client, serverName);
      http.addHeader("Content-Type", "text/plain");
      String httpRequestData = rfid;
      int httpResponseCode = http.POST(httpRequestData);
      #ifdef DEBUG
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      #endif
      http.end();
      return true;
    }
    else {
      Serial.println("WiFi Disconnected");
      return false;
    }
    // lastTime = millis();
}

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

/*String readNFC()
{
  tagId = "None";
  boolean success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
  uint8_t uidLength;                       // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, &uid[0], &uidLength);
  if (success)
  {
    #ifdef DEBUG
    Serial.print("UID Length: ");
    Serial.print(uidLength, DEC);
    Serial.println(" bytes");
    Serial.print("UID Value: ");
    #endif
    for (uint8_t i = 0; i < uidLength; i++)
    {
      nuidPICC[i] = uid[i];
      #ifdef DEBUG
      Serial.print(" "); Serial.print(uid[i], DEC);
      #endif
    }
    // Serial.println();
    tagId = tagToString(nuidPICC);
    dispTag = tagId;
    #ifdef DEBUG
    Serial.print(F("tagId is : "));
    Serial.println(tagId);
    Serial.println("");
    #endif
    delay(1000); 
  }
  return tagId;
}*/

void readNFC() 
{
 if (nfc.tagPresent())
 {
   NfcTag tag = nfc.read();
   tag.print();
   tagId = tag.getUidString();
   httpPost(tagId);
   delay(5000);
 }
 delay(100);
}

void loop() {
  // String nfcID = readNFC();
  // httpPost(nfcID);
  readNFC();
  delay(1000);
} 
