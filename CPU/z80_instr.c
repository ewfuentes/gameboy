#include "cpu/z80_instr.h"
#include "cpu/mem.h"

#define VALID_OP(x) { if (x<0) {return z80_fail;}}

z80_status z80_nop(z80_t *z80, mem_t *mem) {
    z80->dt = 4;
    return z80_ok;
}

//Load short
//Has no effect on flags
z80_status z80_loadShort(z80_t *z80, mem_t *mem, uint16_t *reg) {
    uint16_t tmp = 0;
    VALID_OP(mem_readShort(mem, z80->pc, &tmp));
    z80->pc += 2;
    *reg = tmp;
    z80->dt = 12;
    return z80_ok;
}

