//Betanzos Reyes Gustavo Noel
//Mediante Sinric Pro, se creo un dispositivo que enciende y apaga tres leds de la tarjeta con el ESP32, de manera de switch individual, es decir, uno por cada uno. Uno de ellos tiene un temporizador de 5 segundos prendido, el otro 10 y el último 15.
//Mediante las aplicaciones Google Home y Google Assistant, se configuro el dispositivo creado del punto anterior para encenderlo y apagarlo mediante instrucciones con voz utilizando el asistente de Google.

#ifdef ENABLE_DEBUG
   #define DEBUG_ESP_PORT Serial
   #define NODEBUG_WEBSOCKETS
   #define NDEBUG
#endif 

#include <Arduino.h>
#if defined(ESP8266)
  #include <ESP8266WiFi.h>
#elif defined(ESP32) || defined(ARDUINO_ARCH_RP2040)
  #include <WiFi.h>
#endif

#include "SinricPro.h"
#include "SinricProSwitch.h"

#define WIFI_SSID         //Nombre de la RED
#define WIFI_PASS         //Contraseña de la RED
#define APP_KEY           "598b875a-f329-45d9-be0a-046a623da058"
#define APP_SECRET        "6d4de75b-39d0-4051-bb9b-8ed100e23511-b70a2f9e-f62d-4b8a-af1d-afa485b22237"

#define SWITCH_ID_1       "65428b2b66e5895b9b5d83bf"
#define RELAYPIN_1        27

#define SWITCH_ID_2       "654287ef2254d187b0547225"
#define RELAYPIN_2        26

#define SWITCH_ID_3       "653fd27363039b95af23a7f6"
#define RELAYPIN_3        25

#define BAUD_RATE         115200                // Change baudrate to your need

bool onPowerState1(const String &deviceId, bool &state) {
 Serial.printf("Device 1 turned %s", state?"on":"off");
 digitalWrite(RELAYPIN_1, state ? HIGH:LOW);
 return true; // request handled properly
}

bool onPowerState2(const String &deviceId, bool &state) {
 Serial.printf("Device 2 turned %s", state?"on":"off");
 digitalWrite(RELAYPIN_2, state ? HIGH:LOW);
 return true; // request handled properly
}

bool onPowerState3(const String &deviceId, bool &state) {
 Serial.printf("Device 3 turned %s", state?"on":"off");
 digitalWrite(RELAYPIN_3, state ? HIGH:LOW);
 return true; // request handled properly
}

// setup function for WiFi connection
void setupWiFi() {
  Serial.printf("\r\n[Wifi]: Connecting");

  #if defined(ESP8266)
    WiFi.setSleepMode(WIFI_NONE_SLEEP); 
    WiFi.setAutoReconnect(true);
  #elif defined(ESP32)
    WiFi.setSleep(false); 
    WiFi.setAutoReconnect(true);
  #endif

  WiFi.begin(WIFI_SSID, WIFI_PASS);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.printf(".");
    delay(250);
  }

  Serial.printf("connected!\r\n[WiFi]: IP-Address is %s\r\n", WiFi.localIP().toString().c_str());
}

// setup function for SinricPro
void setupSinricPro() {
  // add devices and callbacks to SinricPro
  pinMode(RELAYPIN_1, OUTPUT);
  pinMode(RELAYPIN_2, OUTPUT);
  pinMode(RELAYPIN_3, OUTPUT);
    
  SinricProSwitch& mySwitch1 = SinricPro[SWITCH_ID_1];
  mySwitch1.onPowerState(onPowerState1);
  
  SinricProSwitch& mySwitch2 = SinricPro[SWITCH_ID_2];
  mySwitch2.onPowerState(onPowerState2);
  
  SinricProSwitch& mySwitch3 = SinricPro[SWITCH_ID_3];
  mySwitch3.onPowerState(onPowerState3);
  
  
  // setup SinricPro
  SinricPro.onConnected([](){ Serial.printf("Connected to SinricPro\r\n"); }); 
  SinricPro.onDisconnected([](){ Serial.printf("Disconnected from SinricPro\r\n"); });
   
  SinricPro.begin(APP_KEY, APP_SECRET);
}

// main setup function
void setup() {
  Serial.begin(BAUD_RATE); Serial.printf("\r\n\r\n");
  setupWiFi();
  setupSinricPro();
}

void loop() {
  SinricPro.handle();
}
