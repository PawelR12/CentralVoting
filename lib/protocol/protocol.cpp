#include "protocol.h"

typedef enum msg_destination{
    voting_open,
    ack_voting_open,
    vote_send,
    ack_vote_send,
    vote_end,
}  msg_destination;

uint8_t get_lsb(uint8_t figure){
    figure = figure & 0x0F;
    return figure;
}

uint8_t get_msb(uint8_t figure){
    figure = figure >> 4;
    return figure;
}

Protocol :: Protocol(uint8_t own_add){
    own_address = own_add;
    address = 0;
    msg_type = 0;
    msg = 0;
    check_sum = 0;
}

void Protocol :: divide_message(uint8_t* data){
    address = data[0];
    msg_type = get_msb(data[1]);
    msg = get_lsb(data[1]);
    check_sum = data[2]; 
}

void Protocol :: data_validate(){
    uint8_t validate = 1;
    // tbd
    // if address is correct
    // if check sum is correct
    if(validate)    msg_execution();
}

void Protocol :: msg_execution(){
    switch(msg_type){
        case voting_open:
            void voting_open_func();
            break;
            
        case ack_voting_open:
            void ack_voting_open_func();
            break;

        case vote_send:
            void vote_send_func();
            break;

        case ack_vote_send:
            void ack_vote_send_func();
            break;

        case vote_end:
            void vote_end_func();
            break;

        default:
            break;
    }
}