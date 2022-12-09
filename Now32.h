#pragma once

#ifdef ESP32
  #include <Arduino.h>
  #include "ESPNOW_INTERFACE.h"
  #include <cstring>
  #include <list>
  #include <esp_system.h>
  #include <WiFi.h>
  #include <esp_now.h>
  #include <functional>
  #include <stdint.h>

/*The implementation for ESP32 based devices*/
  class Now32 : public ESPNOW_INTERFACE
  {
    private:
      static Now32* singleton32;
      std::list<esp_now_peer_info_t> peers;

      std::function<void(uint8_t *mac, uint8_t *data, uint8_t message_len)> on_message_recieved = nullptr;
      std::function<void(uint8_t* mac,int success)> on_message_send = nullptr;

  public:
      Now32() { this->singleton32 = this; }
      static Now32 *getSingleton() { return singleton32; }
    
      virtual int   init();                                                                                          
      virtual int   set_mac(uint8_t *mac);                                                                           
      virtual int   set_channel(now_channel_t channel);                                                              
      virtual int   set_pmk(uint8_t* pmk);                                                                               
      virtual int   add_peer(uint8_t *mac, int channel);                                                         
      virtual int   add_encrypted_peer(uint8_t *mac, int channel, uint8_t* LMK, size_t key_length);                  
      virtual int   remove_peer(uint8_t *mac);                                                                       
      virtual int   send_message(uint8_t *mac, uint8_t *data, size_t data_length);                                   
      
      virtual void  register_on_send_cb(std::function<void(uint8_t* mac,int success)> cb);                           
      virtual void  register_on_message_cb(std::function<void(uint8_t *mac, uint8_t *data, uint8_t message_len)> cb);

    
  };

  Now32* Now32::singleton32 = nullptr;

  // create the oject
  Now32 espnow = Now32();
  ESPNOW_INTERFACE &ESPNOW = espnow;


#endif
