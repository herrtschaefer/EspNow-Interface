

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

// internal callbacks

void on_data_send( uint8_t *mac_addr, uint8_t status)
{
  Now8266* instance = Now8266::getSingleton(); // get instance of the object
  if(instance){
    uint8_t mac[6];
    memcpy(mac,mac_addr,6);
    instance->on_message_send(mac,(int)status);
  }

}
void on_data_recieved(uint8_t *mac_addr, uint8_t *incomingData, uint8_t len)
{
  uint8_t data_buffer[len];
  uint8_t mac_buffer[6];
  memcpy(mac_buffer, mac_addr, 6);
  memcpy(data_buffer, incomingData, len);
  Now8266* instance = Now8266::getSingleton();
  if(instance && instance->on_message_recieved)
    instance->on_message_recieved(mac_buffer, data_buffer, (uint8_t)len);
  
}

/*#################IMPLEMENTATION########################*/


int  Now8266::init(){

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  auto state = esp_now_init();
  if (state == 0)
    state = esp_now_set_self_role(ESP_NOW_ROLE_COMBO);
  if (state == 0)
    state = esp_now_register_recv_cb(on_data_recieved);
  if (state == 0)
    state = esp_now_register_send_cb(on_data_send);
  return state;
}
int  Now8266::set_mac(uint8_t *mac){
  return wifi_set_macaddr(STATION_IF, mac);
}
int  Now8266::set_channel(now_channel_t channel){
  return wifi_set_channel((uint8_t) channel);
}
int  Now8266::set_pmk(uint8_t* pmk){
  return esp_now_set_kok(pmk,16);
}  
int  Now8266::add_peer(uint8_t *mac, int channel)
{
  auto success = esp_now_add_peer(mac, ESP_NOW_ROLE_COMBO, channel, 0, 0);

  if (!success)
    remove_peer(mac);
  return success;
}
int  Now8266::add_encrypted_peer(uint8_t *mac, int channel, uint8_t* LMK, size_t key_length){
  auto success = esp_now_add_peer(mac, ESP_NOW_ROLE_COMBO, channel, LMK, key_length);
  if (!success)
    remove_peer(mac);
  return success;
}
int  Now8266::remove_peer(uint8_t *mac){
   return esp_now_del_peer(mac); }
int  Now8266::send_message(uint8_t *mac, uint8_t *data, size_t datalen)
{
  return esp_now_send(mac, data, datalen);
}
void Now8266::register_on_message_cb(std::function<void(uint8_t *mac, uint8_t *data, uint8_t message_len)> cb){
   on_message_recieved = cb; }
void Now8266::register_on_send_cb(std::function<void(uint8_t* mac,int success)> cb){
  on_message_send = cb;}








#endif
