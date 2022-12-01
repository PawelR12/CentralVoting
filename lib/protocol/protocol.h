#ifndef _PROTOCOL_H_INCLUDED
#define _PROTOCOL_H_INCLUDED
// #include "../../include/header.h"
#include "../wireless/wireless.h"
#include <stdint.h>

typedef enum{
  voting_is_open,
  voting_is_close,
  during_voting,
} voting_status;

uint8_t get_lsb(uint8_t);
uint8_t get_msb(uint8_t);

// extern voting_status voting;
// extern uint8_t *device_array;
// extern uint8_t *ack_start;
// extern uint8_t *voting_results;
// extern uint8_t number_of_devices;

typedef enum vote_possibilites{
    vote_yes = 1,
    vote_no = 2,
    vote_no_decision = 3,
} vote_possibilites;

class Protocol{
    public:        
        uint8_t ack_start_fill;
        uint8_t ack_vote_fill;
        uint8_t number_of_devices;
        uint8_t* device_array;
        uint8_t* ack_start;
        uint8_t* voting_results;
        voting_status voting;

        Protocol(uint8_t own_add);
        void divide_message(uint8_t* data);
        void data_validate(uint32_t data);
        void msg_execution();
        uint8_t check_sum_func(uint8_t val1, uint8_t val2);


        void voting_receive();
        uint8_t check_fill(uint8_t* arr,uint8_t number_of_elements);
        uint8_t check_ack();

        void send_voting_open(uint8_t destination_address);
        void send_can_vote(uint8_t destination_address);
        void send_voice(vote_possibilites vote);
        uint8_t get_address();
        uint8_t get_msg_type();
        uint8_t get_msg();
        uint8_t get_check_sum();
        uint8_t get_validate();
    private:
        uint8_t own_address; // address of the device
    // received
        uint8_t address; // address of last received message
        uint8_t msg_type;
        uint8_t msg;
        uint8_t check_sum;
        uint8_t validate;
        void voting_open_func();
        void ack_voting_open_func();
        void can_send_voices_func();
        void vote_send_func(vote_possibilites own_vote);
        void ack_vote_send_func(); // tbd
        void vote_end_func();
        

        
};


#endif