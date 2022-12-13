#include <Arduino.h>
#include <LoRa.h>
#include <SPI.h>
#include <cstring>
#include <WiFiMulti.h>
#include <cstdlib>

// #include "wireless.h"
#include "protocol.h"
#include "HTTPClient.h" 

// Definition for RFM95W 
#define ss 5
#define rst 14
#define dio0 2

#define ACTIVATE_BUTTON 4
#define ACTION_BUTTON 15
#define GREEN_LED 25
#define YELLOW_LED 26
#define RED_LED 27
//Add WIFI data
#define WIFI_SSID "Redmi Note 9"
#define WIFI_PASSWORD "1234abcd"

using namespace std;

WiFiMulti wifiMulti;
String data_to_send = ""; // for HTTP Client
String tmp = "";
uint8_t encryption_key;
// extern voting_status voting;
Protocol protocol(0x00);
int stage = 0;
int n = 0;
voting_status last_voting_status;
uint8_t previous_yes_votes = 0;
uint8_t previous_no_votes = 0;
uint8_t previous_no_decision_votes = 0;
uint8_t previous_voted_cast = 0;

unsigned long actual_millis, previous_millis;
uint8_t previous_connected_devices = 0;
uint8_t connection = 0; // 0 - no http connection; 1 - http connection

void http_send(String data);


void setup() {

  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  // Set serial communication and communication with LoRa transceiver
  digitalWrite(YELLOW_LED, HIGH);
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


  wifiMulti.addAP(WIFI_SSID, WIFI_PASSWORD);
  while (wifiMulti.run() != WL_CONNECTED) {
    delay(100);
  }
  Serial.println("Connected with Wi-Fi");


  // HTTP Data Setup
  Serial.println("--->First HTTP Data Setup");
  
    if(wifiMulti.run() == WL_CONNECTED){
      HTTPClient http;
      http.begin("https://wirelessvoting.000webhostapp.com/index.php");   //Indicate the destination webpage 
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");          //Prepare the header
      
      data_to_send =  "change_stage_";
      tmp = data_to_send + String(protocol.voting);
      Serial.println("--->Change voting status over wifi");
      http.POST(tmp);
      
      tmp =  "send_devices=1";
      //tmp = data_to_send + String(protocol.voting);
      Serial.println("--->Change device number");
      http.POST(tmp);


      Serial.println("--->Clearing basic values");
      tmp =  "conncected_devices=0";
      //tmp = data_to_send + String(protocol.voting);
      Serial.print("--->  ..conncected devices");
      http.POST(tmp);
      // http.end();

      tmp =  "yes_votes=0";
      //tmp = data_to_send + String(protocol.voting);
      Serial.print("  .. yes_votes");
      http.POST(tmp);
      // http.end();

      tmp =  "no_votes=0";
      //tmp = data_to_send + String(protocol.voting);
      Serial.print("  .. no_votes");
      http.POST(tmp);
      // http.end();

      tmp =  "no_decision_votes=0";
      //tmp = data_to_send + String(protocol.voting);
      Serial.println("  .. no_decision");
      http.POST(tmp);
      http.end();
  }
 
  digitalWrite(YELLOW_LED, LOW);
  digitalWrite(GREEN_LED, HIGH);
  Serial.println("You can start");
}

void loop() {
  

  if(digitalRead(ACTION_BUTTON) && protocol.voting == voting_is_close)
  {
      digitalWrite(GREEN_LED, LOW);
      digitalWrite(YELLOW_LED, HIGH);
      protocol.clear_votes();
      http_send("change_stage_0");
      http_send("conncected_devices=0");
      encryption_key = (rand() % 5) + 1;
      protocol.send_voting_open(0, encryption_key);
      protocol.voting = voting_is_open;
      digitalWrite(YELLOW_LED, LOW);
      digitalWrite(GREEN_LED, HIGH);
      Serial.println("---- START VOTING ----");
    
  }

  if(protocol.voting == voting_is_open){
      protocol.voting_receive();
      if(protocol.get_validate() == 1){
        Serial.print("Address is: ");
        Serial.println(protocol.get_address());
        delay(200);
        Serial.print("Ack number is: ");
        Serial.print(protocol.check_ack());
        Serial.print(" of ");
        Serial.println(protocol.number_of_devices);
      
        }
    }
    if(digitalRead(ACTION_BUTTON) && protocol.voting == voting_is_open && stage == 0){
        if(stage == 0){
          stage = 1;
          digitalWrite(GREEN_LED, LOW);
          digitalWrite(YELLOW_LED, HIGH);
          Serial.println("--->Server communication");
          http_send("change_stage_2");
          data_to_send =  "connected_devices=" + String(protocol.connected_devices);
          http_send(data_to_send);
          protocol.send_can_vote(0);
          // protocol.voting == during_voting;
          digitalWrite(GREEN_LED, HIGH);
          digitalWrite(YELLOW_LED, LOW);
          Serial.println("You can send the voices");
          delay(200);
        }
    }
    if(digitalRead(ACTION_BUTTON) && stage == 1){
          digitalWrite(GREEN_LED, HIGH);
          digitalWrite(YELLOW_LED, LOW);
          protocol.send_close_voting();
          Serial.println("--- VOTING IS CLOSE");    
          Serial.println("--->Server communication");
  
          Serial.print("Voting results" ); Serial.println(protocol.voting_results[1]);
          data_to_send = "casted_votes=" + String(protocol.voted_cast);
          http_send(data_to_send);
          data_to_send = "yes_votes=" + String(protocol.yes_votes_number);
          http_send(data_to_send);
          data_to_send = "no_votes=" + String(protocol.no_votes_number);
          http_send(data_to_send);
          data_to_send = "no_decision_votes=" + String(protocol.no_decision_votes_number);
          http_send(data_to_send);
          http_send("change_stage_1");
      
          stage = 0;
          digitalWrite(GREEN_LED, HIGH);
          digitalWrite(YELLOW_LED, LOW);
          Serial.println("You can clear");
        }
}

void http_send(String data){

  if(wifiMulti.run() == WL_CONNECTED){
    HTTPClient http;
    http.begin("https://wirelessvoting.000webhostapp.com/index.php");   //Indicate the destination webpage 
    http.addHeader("Content-Type", "application/x-www-form-urlencoded"); 
    String x = "--->" + data;
    Serial.println(x);
    http.POST(data);
    http.end();
  }
}


/// OLD version 
 // if( abs(long(actual_millis) - long(previous_millis)) > 200){
    
    // previous_millis = actual_millis;

    // if(protocol.connected_devices != previous_connected_devices){
    //   previous_connected_devices = protocol.connected_devices;  
    //   Serial.println("--->Change connected devices");
    //   data_to_send =  "connected_devices=" + String(protocol.connected_devices);
    //   // tmp = data_to_send + String(protocol.connected_devices);
      
        
    //   http.POST(data_to_send);
      
    // }

    // if(previous_voted_cast != protocol.voted_cast){
    //   previous_voted_cast = protocol.voted_cast;
    //   Serial.println("---> Change voted cast");
    //   data_to_send = "casted_votes=" + String(protocol.voted_cast);
     
    //   http.POST(data_to_send); 
      
    // }
    // if(previous_yes_votes != protocol.yes_votes_number){
    //   previous_voted_cast = protocol.yes_votes_number;
    //   Serial.println("---> Change voted cast");
    //   data_to_send = "yes_votes=" + String(protocol.yes_votes_number);
      
        
    //   http.POST(data_to_send); 
    // }  
    

    // if(previous_yes_votes != protocol.no_votes_number){
    //   previous_voted_cast = protocol.no_votes_number;
    //   Serial.println("---> Change voted cast");
    //   data_to_send = "no_votes=" + String(protocol.no_votes_number);
     
    //   http.POST(data_to_send); 
      
    // }
    // if(previous_yes_votes != protocol.no_decision_votes_number){
    //   previous_voted_cast = protocol.no_decision_votes_number;
    //   Serial.println("---> Change voted cast");
    //   data_to_send = "no_decision_votes=" + String(protocol.no_decision_votes_number);
     
    //   http.POST(data_to_send); 
    // }
    //   http.end();
    //   connection = 0;
    
// }