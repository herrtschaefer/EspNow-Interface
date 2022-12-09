#pragma once
/*
  This library attempts to unify the esp now interface for esp32 and esp8266 microcontrollers
*/


#include <Arduino.h>
#include <functional>
#include <stdint.h>
#ifdef ESP32
#include <esp_now.h>
#include <WiFi.h>
#elif ESP8266
#include <ESP8266WiFi.h>
#include <c_types.h>
#include <espnow.h>
#endif

/*
    The range of the channel of paired devices is from 0 to 14. If the channel is set to 0,
    data will be sent on the current channel. 
    Otherwise, the channel must be set as the channel that the local device is on.
*/
typedef enum : uint8_t {
  CURRENT_CHANNEL =0, // 
  CH_1 = 1,
  CH_2,
  CH_3,
  CH_4,
  CH_5,
  CH_6,
  CH_7,
  CH_8,
  CH_9,
  CH_10,
  CH_11,
  Ch_12,
  CH_13,
  CH_14,
} now_channel_t;

class ESPNOW_INTERFACE
{
  public:

    virtual int   init() = 0;                                                                                             // < initialize the esp now interface
    virtual int   set_mac(uint8_t *mac) = 0;                                                                              // < change the mac address of this device
    virtual int   set_channel(now_channel_t channel) = 0;                                                                 // < set the esp now channel this device will communicate on. see espressif documentation for further details
    virtual int   set_pmk(uint8_t* pmk);                                                                                  // < set the primary key refer to espressif doc for further information 
    virtual int   add_peer(uint8_t *mac, int channel = 0) = 0;                                                            // < add a device to your known devices list messages can only be send to known devices (except from Broadcast)
    virtual int   add_encrypted_peer(uint8_t *mac, int channel, uint8_t* LMK, size_t key_length) = 0;                     // < add a device and enable encryption refer to espressif doc for further information
    virtual int   remove_peer(uint8_t *mac) = 0;                                                                          // < delete a peer
    virtual int   send_message(uint8_t *mac, uint8_t *data, size_t data_length) = 0;                                      // send a message with esp  now
    
    virtual void  register_on_send_cb(std::function<void(uint8_t* mac,int success)> cb) = 0;                              // < register a function to handle the sucess/failure of a send message
    virtual void  register_on_message_cb(std::function<void(uint8_t *mac, uint8_t *data, uint8_t message_len)> cb) = 0;   // < register a function to handle incoming messages

    
};


extern ESPNOW_INTERFACE &ESPNOW;


