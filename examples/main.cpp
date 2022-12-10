#include <Arduino.h>
#include "ESPNOW_INTERFACE.h"
#define line_len 20


 uint8_t mac_addr[3][6] = {
                                {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF},
                                {0x30,0xAE,0xA4,0x73,0xB8,0x18},
                                {0x24,0x0A,0xC4,0xAD,0xFE,0xF0}
                                };


void on_rec(uint8_t* mac,uint8_t* data,uint8_t len){
  Serial.printf("Recieved Message:\nFrom: %02X:%02X:%02X:%02X:%02X:%02X\nLength: %d Bytes\n",mac[0],mac[1],mac[2],mac[3],mac[4],mac[5],len);
  for(int i=0;i<len;i++){
    Serial.print("0x");
    Serial.print(data[i],HEX);
    Serial.print(" ");
    if(i%line_len==0&&i>0)Serial.println();  
  }Serial.println();
   for(int i=0;i<len;i++){
    
    Serial.print((char)data[i]);
    
    if(i%line_len==0)Serial.println();  
  }Serial.println();
}

void on_send(uint8_t* mac,int success){
  Serial.printf("Send Message:\n To: %02X:%02X:%02X:%02X:%02X:%02X\n State: %s\n",mac[0],mac[1],mac[2],mac[3],mac[4],mac[5],(success==0)? "SUCCESS":"FAILED");
}

void setup() {
  Serial.begin(115200);

  ESPNOW.init();
  ESPNOW.set_channel(CH_5);
  ESPNOW.register_on_message_cb(on_rec);
  ESPNOW.register_on_send_cb(on_send);
  for(int i=0;i<3;i++) ESPNOW.add_peer(mac_addr[i],(int)now_channel_t::CURRENT_CHANNEL);

}
auto last = 0L;
#define msg_len 250
uint8_t msg[msg_len];
void loop() {
  
  auto  now = millis();
  if(now-last>4000){
    last=now;
    for(int i=0;i<3;i++){
      String s = "Hello ESP: Time: ";
      memcpy(msg,s.c_str(),s.length());
      ESPNOW.send_message(mac_addr[i],msg,s.length());
    }
  }
}
