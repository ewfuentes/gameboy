#include "cpu/z80_instr.h"
#include "cpu/mem.h"

#define NULL (void *) 0

z80_status z80_nop(z80_t *z80, mem_t *mem) {
  z80->dt = 4;
  return z80_ok;
}

//Load short
//Has no effect on flags
z80_status z80_loadShort(z80_t *z80, mem_t *mem, uint16_t *reg) {
  if (z80 == NULL || mem == NULL || reg == NULL){
    return z80_bad_param;
  }
  uint16_t tmp = 0;
  VALID_MEM_OP(mem_readShort(mem, z80->pc, &tmp));
  z80->pc += 2;
  *reg = tmp;
  z80->dt = 12;
  return z80_ok;
}

z80_status z80_mov(z80_t *z80, mem_t *mem, uint8_t *dst, 
                    uint8_t src) {
  return z80_movGen(z80, mem, dst, &src, 0);
}

z80_status z80_movGen(z80_t *z80, mem_t *mem, void *dst,
                    void *src, uint32_t flags) {
  if (z80 == NULL || dst == NULL) {
    return z80_bad_param;
  }
  //First let's get the source data
  uint8_t srcData = 0;
  if (flags & z80_srcPage0) {
    VALID_MEM_OP(mem_readByte(mem, 0xFF00 |  *((uint8_t*)src), 
                                &srcData));
  } else if (flags & z80_srcIsAddr) {
    VALID_MEM_OP(mem_readByte(mem, *((uint16_t *)src), 
                                &srcData));
  } else {
    //since the source isn't a page zero location or general
    //memory location, it must be a register value
    srcData = *((uint8_t *)src);
  }

  //Now let's write to our destination
  if (flags & z80_dstPage0) {
    VALID_MEM_OP(mem_writeByte(mem, 0xFF00 | (uint8_t) dst,
                srcData));
  } else if (flags & z80_dstIsAddr) {
    VALID_MEM_OP(mem_writeByte(mem, (uint16_t) dst, srcData));
  } else {
    //Since the destination isn't a page zero location or general
    //memory location, it must be a pointer to a register
    *((uint8_t *)dst) = srcData;
  }

  z80->dt = 4;
  if (flags & (z80_dstIsAddr | z80_srcIsAddr)) {
    z80->dt = 8;
  }
  if (flags & (z80_dstPage0 | z80_srcPage0)) {
    z80->dt = 12;
  }

  if (flags & z80_dstInc) {
    (*((uint16_t *)dst))++;
  }

  if (flags & z80_dstDec) {
    (*((uint16_t *)dst))--;
  }

  if (flags & z80_srcInc) {
    (*((uint16_t *)src))++;
  }

  if (flags & z80_srcDec) {
    (*((uint16_t *)src))--;
  }
  return z80_ok;
}

z80_status z80_incShort(z80_t *z80, mem_t *mem, uint16_t *reg) {
  if (reg == NULL) {
    return z80_bad_param;
  }
  (*reg)++;
  z80->dt = 8;
}

z80_status z80_decShort(z80_t *z80, mem_t *mem, uint16_t *reg) {
  if (reg == NULL) {
    return z80_bad_param;
  }
  (*reg)--;
  z80->dt = 8;
}