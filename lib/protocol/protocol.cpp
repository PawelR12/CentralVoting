#include "protocol.h"


typedef enum msg_destination{
    voting_open = 10,
    ack_voting_open = 11,
    can_send_voices = 12,
    vote_send = 13,
    ack_vote_send = 14,
    vote_end = 15
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
    validate = false;

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

    uint32_t msg = receiveMessage();
    if(msg != 0){
        Serial.print("Message: "); Serial.println(msg);
     } 
    data_validate(msg);
    
}

void Protocol :: data_validate(uint32_t data){
    validate = true;
    uint8_t received_address = uint8_t((data & 0xFF0000) >> 16);
    uint8_t sender_address = received_address & 0x0F;
    uint8_t receiver_address = received_address >> 4;
    uint8_t received_message = uint8_t((data & 0x00FF00) >> 8);
    uint8_t received_check_sum = uint8_t((data & 0x0000FF));
    // ADDITIONAL CHECKING IN CASE
    if(data != 0){
        Serial.print("Data: ");
        Serial.print(data);
        Serial.print("  Sender address: ");
        Serial.print(sender_address);
        Serial.print("  Receiver address: ");
        Serial.print(receiver_address);
        Serial.print("  Msg: ");
        Serial.print(received_message);
        Serial.print("  check sum: ");
        Serial.print(received_check_sum);
        // Serial.print("Validate before address check: ");
        // Serial.println(validate);
    }
    if(data == 0){
        validate = false;
    }
    if(receiver_address != 0 && receiver_address != own_address){
        validate = false;
    }
    // if(data != 0){
    //     Serial.print("Validate after addres check: ");
    //     Serial.println(validate);
    // }
    // if address is correct
    uint8_t cs = check_sum_func(received_address,received_message);
    if(cs != received_check_sum){
        validate = false;
    }
    // if(data != 0){
    // Serial.print("received sum: ");
    // Serial.println(cs);
    // Serial.print("Validate: ");
    // Serial.println(validate);
    // }
    // if check sum is correct
        
    if(validate){
        address = sender_address; // sender address
    
        if(get_msb(received_message) == 0){
            msg_type = get_lsb(received_message);
            msg = 0;
        }
        else{
            msg_type = get_msb(received_message);
            msg = get_lsb(received_message);
        }
        Serial.print("msg_type is: "); Serial.println(msg_type);
        check_sum = received_check_sum;
        Serial.println("Message is validated");
        msg_execution();
    }
}

void Protocol :: msg_execution(){
    Serial.println("Open msg_execution");
    Serial.print("Voting: ");
    Serial.println(voting);
    Serial.print("Msg type: ");
    Serial.println(msg_type);
    if(voting == voting_is_close && msg_type == voting_open){
        voting_open_func();
        Serial.println("I was there...");
    }
    else if(msg_type == ack_voting_open){
        ack_voting_open_func();
    }
    else if(voting == during_voting){
        switch(msg_type){    
            case can_send_voices:
                void can_send_voices_func();
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

uint8_t Protocol :: get_validate(){
    return validate;
}
void Protocol :: voting_open_func(){
    voting = voting_is_open; // switch on voting mode

    uint8_t response_msg = (ack_voting_open << 4);
    response_msg |= own_address;
    uint8_t destination_addr = 0;
    uint8_t my_addr = (destination_addr << 4);
    my_addr |= own_address;
    uint8_t new_check_sum = check_sum_func(my_addr, response_msg);
    uint8_t* response = createMessage(my_addr, response_msg, new_check_sum);
    Serial.print("Response_msg: "); Serial.println(response_msg);
    Serial.print("Check sum: "); Serial.println(new_check_sum);
    Serial.print("Address: "); Serial.println(my_addr);
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

void Protocol :: can_send_voices_func(){
    // sth lights
    voting = during_voting;
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
    uint32_t x = (uint32_t)destination_address << 24 | (uint32_t)msg << 16 | (uint32_t)check_sum << 8;
    Serial.println(x);
    Serial.print("Address is: ");
    Serial.println(destination_address >> 24);

}

void Protocol :: send_can_vote(uint8_t destination_address){
    voting == during_voting;

    uint8_t msg = can_send_voices << 4;
    uint8_t check_sum = check_sum_func(destination_address, msg);
    
    uint8_t *ptr = createMessage(destination_address, msg, check_sum);
    sendMessage(ptr, 3);
    uint32_t x = (uint32_t)destination_address << 24 | (uint32_t)msg << 16 | (uint32_t)check_sum << 8;

}

void Protocol :: send_voice(vote_possibilites vote){
    uint8_t msg = (vote_send << 4) | vote_yes;
    uint8_t check_sum = check_sum_func(0, msg);

    uint8_t *ptr = createMessage(0, msg, check_sum);
    sendMessage(ptr,3);
    uint32_t x = (uint32_t)msg << 16 | (uint32_t)check_sum << 8;


}

uint8_t Protocol :: check_ack(){
    uint8_t number = 0;
    
    for(int n=0; n<number_of_devices; n++){
        if(ack_start[n] != 0)   number++;
    }
    return number;
}