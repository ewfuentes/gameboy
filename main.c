#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "cpu/Z80.h"
#include "cpu/mem.h"

z80_t z80;
mem_t mem;

void init() {
    z80_init(&z80);
    mem_init(&mem);
}

void cleanup() {
    mem_destroy(&mem);
}

int main(void) {
    init();
    int8_t result = 0;
    while (result >= 0) {
        result = z80_step(&z80, &mem);
    }
    
    cleanup();
}