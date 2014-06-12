#include "cpu/z80.h"
#include "cpu/z80_instr.h"
#include <stdio.h>
#include "cpu/mem.h"

#define DEBUG(args...) printf(args)

//Load short


void z80_init(z80_t *z80) {
    z80->af = 0x0000;
    z80->bc = 0x0000;
    z80->de = 0x0000;
    z80->hl = 0x0000;
    z80->sp = 0x0000;
    z80->pc = 0x0000;
}

void z80_testBit(z80_t *z80, uint8_t data, uint8_t bit) {
    z80->f &= ~SUB_FLAG;
    z80->f |= HALF_FLAG; 
    if (data & (1<<bit)) {
        z80->f &= ~ZERO_FLAG;
    }else {
        z80->f |= ZERO_FLAG;
    }
}

void z80_RLreg(z80_t *z80, uint8_t *reg) {
    uint8_t newCarry = (*reg & 0x80) >> 7;
    *reg = *reg << 1;
    if (z80->f & CARRY_FLAG) {
        (*reg)++;
    }

    z80->f = 0;
    if (newCarry) {
        z80->f |= CARRY_FLAG;
    }
    if (*reg == 0) {
        z80->f |= ZERO_FLAG;
    }

}

int8_t z80_extension(z80_t *z80, mem_t *mem) {
    uint8_t opcode;
    memTest(mem_readByte(mem, z80->pc++, &opcode));
    printf("Extension Opcode: %x\r\n", opcode);

    uint8_t tempB = 0;
    uint16_t tempS = 0;
    switch(opcode) {

        default:
            printf("ext opcode not implement: %x\r\n", opcode);
            return -1;
    }

    return 0;
}

int8_t z80_step(z80_t *z80, mem_t *mem) {
    printf("SP: %0.4X PC: %0.4X ", z80->sp, z80->pc);
    printf("AF:%0.4X BC:%0.4X DE:%0.4X", z80->af, z80->bc, z80->de);
    printf(" HL:%0.4X \r\n", z80->hl);

    uint8_t opcode;
    memTest(mem_readByte(mem, z80->pc++, &opcode));
    
    printf("Read Opcode: %x\r\n",opcode);
    uint16_t tempS;
    uint8_t tempB;
    switch (opcode) {

        case 0xCB:
            z80_extension(z80, mem);
            break;

        default:
            printf("Opcode not implemented\r\n");
            return -1;
            break;
    }
    return 0;
}


