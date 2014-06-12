//Written by: Erick Fuentes
#ifndef Z80_H
#define Z80_H

#include <stdint.h>
#include "cpu/mem.h"

#define ZERO_FLAG 0x80
#define SUB_FLAG 0x40
#define HALF_FLAG 0x20
#define CARRY_FLAG 0x10

typedef struct{
    union {
        uint16_t af;
        struct {
            uint8_t f;
            uint8_t a;
        };
    };
    union {
        uint16_t bc;
        struct {
            uint8_t c;
            uint8_t b;
        };
    };
    union {
        uint16_t de;
        struct {
            uint8_t e;
            uint8_t d;
        };
    };
    union {
        uint16_t hl;
        struct {
            uint8_t l;
            uint8_t h;
        };
    };
    uint16_t sp;
    uint16_t pc;
    uint32_t dt;
} z80_t;

//Takes in an instance of the z80 struct and zeros out all of the 
//registers
void z80_init(z80_t *z80);

//Takes a step by reading the byte at the current program counter and 
//decoding the instruction and modifying the registers as necessary
int8_t z80_step(z80_t *z80, mem_t *mem);

#endif