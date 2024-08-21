#include <espnow.h>
#include <ESP8266WiFi.h>
#include "OTA.h"

#define Relay 3

bool ota_done = false;

uint8_t Repcounter_Address[] = {0xC8, 0xC9, 0xA3, 0x5D, 0x14, 0x84}; //ESP-01 MAC

typedef struct struct_message
{
  bool shock = false;
  bool reset = false;
  bool ota_mode = false;

} struct_message;
struct_message Data;

void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&Data, incomingData, sizeof(Data));

  if(Data.shock)
  {
    digitalWrite(Relay, HIGH);
    delay(500);
    digitalWrite(Relay, LOW);
    Data.shock = false;
  }
}

void setup() {

  pinMode(Relay, OUTPUT);

  WiFi.mode(WIFI_STA);
  esp_now_init();
  esp_now_set_self_role(ESP_NOW_ROLE_COMBO);
  //esp_now_register_send_cb(OnDataSent);
  esp_now_register_recv_cb(OnDataRecv);
  esp_now_add_peer(Repcounter_Address, ESP_NOW_ROLE_COMBO, 0, NULL, 0);
  esp_now_send(Repcounter_Address, (uint8_t *) &Data, sizeof(Data));
}

void loop() {
  if(!Data.ota_mode)
  {
  }
  else
  {
    if(!ota_done) setupOTA("ShockBracelet", "WIFI_SSID_HERE", "WIFI_PASSWORD_HERE");
    ota_done = true;
    ArduinoOTA.handle();
  }
}