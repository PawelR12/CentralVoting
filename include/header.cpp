#include "header.h"


if (number_of_devices > 63)     number_of_devices = 63;

device_array = (uint8_t*)calloc(number_of_devices, siezof(uint8_t));

for(int n = 1; n<= number_of_devices, n++){
  device_array[n-1] = n;
}

*ack_start = (uint8_t*)calloc(number_of_devices, siezof(uint8_t));
*ack_vote= (uint8_t*)calloc(number_of_devices, siezof(uint8_t));