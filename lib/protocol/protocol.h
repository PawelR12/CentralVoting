#ifndef _WIRELESS_H_INCLUDED
#define _WIRELESS_H_INCLUDED
#include <stdint.h>

uint8_t get_lsb(uint8_t);
uint8_t get_msb(uint8_t);

class Protocol{
    public:
        // uint8_t voting;
        Protocol(uint8_t own_add);
        void divide_message(uint8_t* data);
        void data_validate();
        uint8_t msg_execution();
        


    private:
        uint8_t own_address;
    // received
        uint8_t address;
        uint8_t msg_type;
        uint8_t msg;
        uint8_t check_sum;


        void voting_open_func();
        void ack_voting_open_func();
        void vote_send_func();
        void ack_vote_send_func();
        void vote_end_func();
};


#endif