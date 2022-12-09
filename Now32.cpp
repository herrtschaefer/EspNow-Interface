


#ifdef ESP32
#include "Now32.h"
#include <Arduino.h>
#include "ESPNOW_INTERFACE.h"
#include <cstring>
#include <list>
#include <esp_system.h>
#include <WiFi.h>
#include <esp_now.h>
#include <functional>
#include <stdint.h>
#include <esp_wifi.h>

// internal callback functions

void on_data_send(const unsigned char *mac_addr, esp_now_send_status_t status)
{
  Now32* instance = Now32::getSingleton(); // get instance of the object
  if(instance){
    uint8_t mac[6];
    memcpy(mac,mac_addr,6);
    instance->on_message_send(mac,(int)status);
  }

}
void on_data_recieved(const unsigned char *mac_addr, const unsigned char *incomingData, int len)
{
  uint8_t data_buffer[len];
  uint8_t mac_buffer[6];
  memcpy(mac_buffer, mac_addr, 6);
  memcpy(data_buffer, incomingData, len);
  Now32* instance = Now32::getSingleton();
  if(instance && instance->on_message_recieved)
    instance->on_message_recieved(mac_buffer, data_buffer, (uint8_t)len);
  
}



/*#################IMPLEMENTATION########################*/


int  Now32::init()
{

  WiFi.mode(WIFI_MODE_STA);
  WiFi.disconnect();

  int state = esp_now_init();
  if (state == 0)
    state = esp_now_register_recv_cb(on_data_recieved);
  if (state == 0)
    state = esp_now_register_send_cb(on_data_send);
  return state;
}
int  Now32::set_mac(uint8_t* mac){
  return (int) esp_base_mac_addr_set(mac);
}
int  Now32::set_channel(now_channel_t channel){
    return (int) esp_wifi_set_channel((uint8_t)channel, WIFI_SECOND_CHAN_NONE);
}  
int  Now32::set_pmk(uint8_t* pmk){
  return (int) esp_now_set_pmk(pmk);
} 
int  Now32::add_peer(uint8_t *mac, int channel)
{
  peers.push_back(esp_now_peer_info_t{});
  esp_now_peer_info_t &peer = peers.back();
  std::memcpy(peer.peer_addr, mac, sizeof(peer.peer_addr));
  peer.channel = channel;
  peer.ifidx = WIFI_IF_STA;
  auto success = esp_now_add_peer(&peers.back());
  if (success != ESP_OK)
    remove_peer(mac);
  return (int)success;
}
int  Now32::add_encrypted_peer(uint8_t *mac, int channel, uint8_t* LMK, size_t key_length){
    peers.push_back(esp_now_peer_info_t{});
    esp_now_peer_info_t &peer = peers.back();
    std::memcpy(peer.peer_addr, mac, sizeof(peer.peer_addr));
    peer.channel = channel;
    peer.ifidx = WIFI_IF_STA;

    peer.encrypt = true;
    memcpy(peer.lmk,LMK,key_length);

   auto success = esp_now_add_peer(&peers.back());
   if (success != ESP_OK)
      remove_peer(mac);
    return (int)success;
}
int  Now32::remove_peer(uint8_t *mac)
{
  // find peer in peers
  for (auto it = peers.begin(); it != peers.end();)
  {
    if (0 == memcmp(mac, it->peer_addr, sizeof(it->peer_addr)))
    {
      it = peers.erase(it); // remove it
    }
  }
  return (int)esp_now_del_peer(mac);
}
int  Now32::send_message(uint8_t *mac, uint8_t *data, size_t datalen)
{
  return esp_now_send(mac, data, datalen);
}
void Now32::register_on_send_cb(std::function<void(uint8_t* mac,int success)> cb){
    this->on_message_send = cb;
}                          
void Now32::register_on_message_cb(std::function<void(uint8_t *mac, uint8_t *data, uint8_t message_len)> cb){
  this->on_message_recieved = cb;
}






#endif