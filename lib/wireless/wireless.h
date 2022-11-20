#ifndef _WIRELESS_H_INCLUDED
#define _WIRELESS_H_INCLUDED


#include <LoRa.h>
#include <SPI.h>


void sendMessage(uint8_t* msg, uint8_t msgSize);
uint8_t* receiveMessage();

#endif