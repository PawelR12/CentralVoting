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
uint32_t receiveMessage(){
    int packetSize = LoRa.parsePacket();
    uint32_t msg = 0;

    if (packetSize){
        uint8_t n = 0;
      
        if (packetSize){
            while(LoRa.available()){
    
                msg |= uint32_t((uint32_t(LoRa.read()) << (8*n)));
                n++;
            }
        }
        uint32_t tmp1, tmp2, tmp3;
        tmp1 = (msg & 0x0000FF) << 16;
        tmp3 = (msg & 0x00FF00);
        tmp2 = (msg & 0xFF0000) >> 16;
        msg = tmp1 | tmp2 | tmp3;
       
        return msg;
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