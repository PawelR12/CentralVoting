#ifndef _PROTOCOL_H_INCLUDED
#define _PROTOCOL_H_INCLUDED
#include "../../include/header.h"
#include <stdint.h>
#include "../wireless/wireless.h"

uint8_t get_lsb(uint8_t);
uint8_t get_msb(uint8_t);

extern voting_status voting;
extern uint8_t *device_array;
extern uint8_t *ack_start;
extern uint8_t *voting_results;
extern uint8_t number_of_devices;

typedef enum vote_possibilites{
    vote_yes = 1,
    vote_no = 2,
    vote_no_decision = 3,
} vote_possibilites;

class Protocol{
    public:        
        uint8_t ack_start_fill;
        uint8_t ack_vote_fill;

        Protocol(uint8_t own_add);
        void divide_message(uint8_t* data);
        void data_validate();
        void msg_execution();
        uint8_t check_sum_func(uint8_t val1, uint8_t val2);
    private:
        uint8_t own_address; // address of the device
    // received
        uint8_t address; // address of last received message
        uint8_t msg_type;
        uint8_t msg;
        uint8_t check_sum;

        void voting_open_func();
        void ack_voting_open_func();
        void vote_send_func(vote_possibilites own_vote);
        void ack_vote_send_func(); // tbd
        void vote_end_func();

        uint8_t check_fill(uint8_t* arr,uint8_t number_of_elements);
};


#endif