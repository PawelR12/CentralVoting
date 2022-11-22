#ifndef _HEADER_H_INCLUDED
#define _HEADER_H_INCLUDED

#include <stdint.h>

typedef enum{
  voting_is_open,
  voting_is_close,
} voting_status;

voting_status voting = voting_is_close;

// Array of controlers
uint8_t *device_array;
uint8_t *ack_start;
uint8_t *voting_results;
uint8_t number_of_devices = 10;


#endif