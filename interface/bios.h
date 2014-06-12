#ifndef BIOS_H
#define BIOS_H

#include <stdint.h>

//Will load the bios and pass back a pointer to the location
//If successful, it will return the size of the bios in bytes
//Error if the result is < 0
int32_t bios_load(uint8_t **bufPtr, uint32_t *size);

#endif