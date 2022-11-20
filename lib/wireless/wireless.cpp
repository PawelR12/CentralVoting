#include "wireless.h"

// address
// destination + msg
// checking

uint8_t* createMessage(uint8_t address, uint8_t msg, uint8_t check_sum){
    uint8_t *ptr;
    ptr = (uint8_t *)calloc(3, sizeof(uint8_t));
    ptr[0] = address;
    ptr[1] = msg;
    ptr[2] = check_sum;

    return ptr;
}
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