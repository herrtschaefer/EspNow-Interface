#ifndef NOW8266_H
#define NOW8266_H

#ifdef ESP8266

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


class Now8266 : public ESPNOW_INTERFACE
{

private:
    static Now8266 *singleton8266;

public:
    std::function<void(uint8_t *mac, uint8_t *data, uint8_t message_len)> on_message_recieved = nullptr;
    std::function<void(uint8_t* mac,int success)> on_message_send = nullptr;

    Now8266() { this->singleton8266 = this; }
    static Now8266 *getSingleton() { return singleton8266; }

    virtual int init();
    virtual int set_mac(uint8_t *mac);
    virtual int set_channel(now_channel_t channel); 
    virtual int set_pmk(uint8_t* pmk);  
    virtual int add_peer(uint8_t *mac, int channel);
    virtual int add_encrypted_peer(uint8_t *mac, int channel, uint8_t* LMK, size_t key_length);
    virtual int remove_peer(uint8_t *mac);
    virtual int send_message(uint8_t *mac, uint8_t *data, size_t data_length);

    virtual void register_on_message_cb(std::function<void(uint8_t *mac, uint8_t *data, uint8_t message_len)> cb);
    virtual void register_on_send_cb(std::function<void(uint8_t* mac,int success)> cb);

};

Now8266 *Now8266::singleton8266 = nullptr;

// Create instances
Now8266 nowesp8266 = Now8266();
ESPNOW_INTERFACE &ESPNOW = nowesp8266;

#endif

#endif