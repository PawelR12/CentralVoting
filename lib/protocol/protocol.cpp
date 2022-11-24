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

    voting = voting_is_close;
    number_of_devices = 10;

    device_array = (uint8_t*)calloc(number_of_devices, sizeof(uint8_t));

    for(int n = 1; n<= number_of_devices; n++){
    device_array[n-1] = n;
    }

    ack_start = (uint8_t*)calloc(number_of_devices, sizeof(uint8_t));
    voting_results= (uint8_t*)calloc(number_of_devices, sizeof(uint8_t));

}

void Protocol :: voting_receive(){

    uint8_t* data = receiveMessage();
    data_validate(data);

}
void Protocol :: divide_message(uint8_t* data){
    address = data[0];
    msg_type = get_msb(data[1]);
    msg = get_lsb(data[1]);
    check_sum = data[2]; 
}

void Protocol :: data_validate(uint8_t* data){
    uint8_t validate = true;
    // tbd
    if(data[0] != 0 || data[0] != own_address){
        validate = false;
    }
    // if address is correct
    uint8_t cs = check_sum_func(data[0],data[1]);
    if(cs != data[2]){
        validate = false;
    }
    // if check sum is correct
        
    if(validate){
         divide_message(data);
         msg_execution();
    }
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
uint8_t Protocol :: check_sum_func(uint8_t val1, uint8_t val2){
    int16_t sum;
    int8_t ans;
    sum = (uint16_t)val1 + (uint16_t)val2;
    if(sum % 2)     sum++;
    ans = (uint8_t)(sum/2);
    return ans;
}

uint8_t Protocol :: get_address(){
    return address;
}

uint8_t Protocol :: get_msg_type()
{
    return msg_type;
}

uint8_t Protocol :: get_msg(){
    return msg;
}

uint8_t Protocol :: get_check_sum(){
    return check_sum;
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

void Protocol :: vote_send_func(vote_possibilites last_vote){
    // add this vote to list of votes
    if(address > 0 && address <= number_of_devices){
        voting_results[address] = last_vote;
    }
    // send ack msg
    uint8_t voting_msg = (ack_vote_send << 4);
    uint8_t new_check_sum = check_sum_func(address, voting_msg);
    uint8_t* ack_your_vote = createMessage(address, voting_msg , new_check_sum);
    sendMessage(ack_your_vote, 3);
}

void Protocol :: ack_vote_send_func(){
    // if(address > 0 && address <= number_of_devices){
    //     voting_results[address] = address;
    // }
    // tbd
}

void Protocol :: vote_end_func(){
    voting = voting_is_close;
    
}

uint8_t Protocol :: check_fill(uint8_t* arr,uint8_t number_of_elements){
    uint8_t num;
    for(int n = 0; n < number_of_devices; n++){
        if(!arr[n]) num++;
    }
    return num;
}

void Protocol :: send_voting_open(uint8_t destination_address){
    
    uint8_t msg = voting_open << 4;
    uint8_t check_sum = check_sum_func(destination_address, msg);
    
    uint8_t *ptr = createMessage(destination_address, msg, check_sum);
    sendMessage(ptr, 3);

}