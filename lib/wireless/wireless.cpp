#include "wireless.h"

// address
// destination + msg
// checking

uint8_t* receiveMessage(){
    int packetSize = LoRa.parsePacket();
    
    if (packetSize){
        static uint8_t *ptr;
        uint8_t n = 0;

        ptr = (uint8_t *)calloc(packetSize,sizeof(uint8_t));
      
        if (packetSize){
            while(LoRa.available()){
                ptr[n] = LoRa.read();
                n++;
            }
        }

        return ptr;
    }
    else{
        return 0;
    }
}


void sendMessage(uint8_t* msg, uint8_t msgSize){
    
    LoRa.beginPacket();
    for(int n=0; n<msgSize; n++){
        LoRa.write(msg[n]);
    }
    LoRa.endPacket();
}