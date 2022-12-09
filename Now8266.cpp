

#ifdef ESP8266
#include "Now8266.h"
#include <Arduino.h>
#include "ESPNOW_INTERFACE.h"
#include <cstring>
#include <list>
#include <memory>
#include <user_interface.h>
#include <ESP8266WiFi.h>
#include <functional>
#include <stdint.h>
#include <c_types.h>
#include <espnow.h>

int Now8266::add_peer(uint8_t *mac, int channel)
{
  auto success = esp_now_add_peer(mac, ESP_NOW_ROLE_COMBO, channel, 0, 0);

  if (!success)
    remove_peer(mac);
  return success;
}

int Now8266::remove_peer(uint8_t *mac) { return esp_now_del_peer(mac); }
int Now8266::send_message(uint8_t *mac, uint8_t *data, size_t datalen)
{
  return esp_now_send(mac, data, datalen);
}

int Now8266::set_mac(uint8_t *mac)
{
  return wifi_set_macaddr(STATION_IF, mac);
}

void on_data_recieved(const uint8_t *mac_addr, const uint8_t *incomingData, uint8_t len)
{
  uint8_t buffer[len];
  uint8_t mac_buffer[6];
  memcpy(mac_buffer, mac_addr, 6);
  memcpy(buffer, incomingData, len);

  Now8266::on_message_recieved(mac_buffer, buffer, (uint8_t)len);
  Serial.println("Recieved Packed");
}

void on_data_send(const uint8_t *mac_addr, uint8_t status)
{
  Serial.println(status == 0 ? "success" : "failed");
}

int Now8266::init()
{

  WiFi.mode(WIFI_STA);
  Wifi.disconnect();

  auto state = esp_now_init();
  if (state == 0)
    state = esp_now_set_self_role(ESP_NOW_ROLE_COMBO);
  if (state == 0)
    state = esp_now_register_recv_cb(on_data_recieved);
  if (state == 0)
    state = esp_now_register_send_cb(on_data_send);
  if (state == 0)
    state = add_peer(broadcast_mac);
}

#endif