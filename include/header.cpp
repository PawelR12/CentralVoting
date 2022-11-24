#include "header.h"


voting_status voting = voting_is_close;
uint8_t number_of_devices = 10;
if (number_of_devices > 63)     number_of_devices = 63;

device_array = (uint8_t*)calloc(number_of_devices, siezof(uint8_t));

for(int n = 1; n<= number_of_devices, n++){
  device_array[n-1] = n;
}

*ack_start = (uint8_t*)calloc(number_of_devices, siezof(uint8_t));
*voting_results= (uint8_t*)calloc(number_of_devices, siezof(uint8_t));