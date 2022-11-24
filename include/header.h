#ifndef _HEADER_H_INCLUDED
#define _HEADER_H_INCLUDED

#include <stdint.h>
// #include "../lib/protocol/protocol.h"

typedef enum{
  voting_is_open,
  voting_is_close,
} voting_status;


voting_status voting;

// Array of controlers
uint8_t *device_array;
uint8_t *ack_start;
uint8_t *voting_results;
uint8_t number_of_devices;
 


#endif