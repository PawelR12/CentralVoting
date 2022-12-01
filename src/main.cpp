#include <Arduino.h>
#include <LoRa.h>
#include <SPI.h>
// #include "header.h"
// #include "wireless.h"
#include "protocol.h"

// Definition for RFM95W 
#define ss 5
#define rst 14
#define dio0 2
#define ACTIVATE_BUTTON 4
#define ACTION_BUTTON 15
#define YES_BUTTON 4
#define NO_BUTTON 5
#define NO_DECSION_BUTTON 6
// extern voting_status voting;
Protocol protocol(0x00);

int n = 0;
void setup() {

  // Set serial communication and communication with LoRa transceiver
  Serial.begin(115200);
  LoRa.setPins(ss, rst, dio0);
  while (!LoRa.begin(866E6)){
    Serial.println("Lora Communication Fail");
    delay(300);
  }
  LoRa.setSyncWord(0xA7);
  Serial.println("Success communication with LoRa");
  
   // We can only receive and send messages to the devices
  // with the same SyncWord number
  pinMode(ACTIVATE_BUTTON, INPUT);
  pinMode(ACTION_BUTTON, INPUT);
}

void loop() {
  
  if(digitalRead(ACTIVATE_BUTTON) && protocol.voting == voting_is_close)
  {
    // if(protocol.voting == voting_is_close){
      protocol.send_voting_open(0);
      protocol.voting = voting_is_open;
      // uint8_t msg = 10 << 4;
      // uint8_t check_sum = protocol.check_sum_func(0, msg);
      // Serial.println(msg);
      // Serial.println(check_sum);
      Serial.println("---- START VOTING ----");
  // }
  }

    if(protocol.voting == voting_is_open){
      protocol.voting_receive();
      if(protocol.get_validate() == 1){
      Serial.print("Address is: ");
      Serial.println(protocol.get_address());
      delay(500);
      Serial.print("Ack number is: ");
      Serial.print(protocol.check_ack());
      Serial.print(" of ");
      Serial.println(protocol.number_of_devices);
      // n=1;     
      }
      
      if(digitalRead(ACTION_BUTTON) && protocol.voting == voting_is_open){
        //protocol.voting == during_voting;
        protocol.send_can_vote(0);

        Serial.println("You can send the voices");
      }
    }
}