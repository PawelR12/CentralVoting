#include <Arduino.h>
#include <LoRa.h>
#include <SPI.h>
#include "header.h"
#include "wireless.h"
#include "protocol.h"

// Definition for RFM95W 
#define ss 5
#define rst 14
#define dio0 2
#define ACTIVATE_BUTTON 2
extern voting_status voting;

void setup() {

  // Set serial communication and communication with LoRa transceiver
  Serial.begin(115200);
  LoRa.setPins(ss, rst, dio0);
  while (!LoRa.begin(866E6)){
    Serial.println("Lora Communication Fail");
    delay(300);
  }
  Serial.println("Success communication with LoRa");

  LoRa.setSyncWord(0xA7); // We can only receive and send messages to the devices
  // with the same SyncWord number

}

void loop() {
  
  uint8_t* msg = createMessage(255,255,250);
  sendMessage(msg, 3);
  delay(500);
}