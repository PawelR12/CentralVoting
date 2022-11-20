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
    ack_start_fill = 0;
    ack_vote_fill = 0;
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
    
    if(voting == voting_is_close && msg_type == voting_open){
        voting_open_func();
    }
    else if(msg_type == ack_voting_open){
        ack_voting_open_func();
    }
    else if(voting == voting_is_open){
        switch(msg_type){    
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
}
uint8_t check_sum_func(uint8_t val1, uint8_t val2){
    // tbd
    return 255;
}
void Protocol :: voting_open_func(){
    voting = voting_is_open; // switch on voting mode

    uint8_t response_msg = (ack_voting_open << 4);
    uint8_t new_check_sum = check_sum_func(own_address, response_msg); // tbd
    uint8_t* response = createMessage(own_address, response_msg, new_check_sum);
    sendMessage(response, 3);
}

void Protocol :: ack_voting_open_func(){
    if(address > 0 && address <= number_of_devices){
        ack_start[address] = address;
    }
}
uint8_t Protocol :: check_fill(uint8_t* arr,uint8_t number_of_elements){
    uint8_t num;
    for(int n = 0; n < number_of_devices; n++){
        if(!arr[n]) num++;
    }
    return num;
}