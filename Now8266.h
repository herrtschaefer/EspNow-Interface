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
    static std::function<void(uint8_t *, uint8_t *, uint8_t)> on_message_recieved;

public:
    NOW8266() { this.singleton8266 = this; }
    now_send_success_t send_success;
    static Now8266 *getSingleton() { return singleton8266; }

    virtual int init();
    virtual int add_peer(uint8_t *mac, int channel);
    virtual int remove_peer(uint8_t *mac);
    virtual int send_message(uint8_t *mac, uint8_t *data, size_t data_length);
    virtual int set_mac(uint8_t *mac);

    virtual void register_on_message_cb(std::function<void(uint8_t *mac, uint8_t *data, uint8_t message_len)> cb) { on_message_recieved = cb; }
    virtual void register_on_send_cb(std::function<void(uint8_t* mac,int success)> cb){on_message_send = cb;}
    // virtual void auto_find_channel();
    // virtual void set_channel(uint8_t channel);
};

Now8266 *Now8266::singleton8266 = nullptr;

// Create instances
Now8266 esp8266 = Now8266();
ESPNOW_INTERFACE &ESPNOW = esp8266;

#endif

#endif