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
  return z80_ok;
}

z80_status z80_decShort(z80_t *z80, mem_t *mem, uint16_t *reg) {
  if (reg == NULL) {
    return z80_bad_param;
  }
  (*reg)--;
  z80->dt = 8;
  return z80_ok;
}

z80_status z80_incByte(z80_t *z80, mem_t *mem, void *reg, 
                       uint32_t flag) {
  if (z80 == NULL || mem == NULL || reg == NULL) {
    return z80_bad_param;
  }

  uint8_t *data = (uint8_t *)reg;
  z80->dt = 4;
  if (flag & (z80_srcIsAddr | z80_dstIsAddr)) {
    z80->dt = 12;
    VALID_MEM_OP(mem_getPointer(mem, *((uint16_t *)reg), &data));
  }

  (*data)++;

  z80->f = z80->f & CARRY_FLAG;
  if (*data == 0x00) {
    z80->f |= ZERO_FLAG;
  }

  if ((*data & 0x0F) == 0x00) {
    z80->f |= HALF_FLAG;
  }

  return z80_ok;
}

z80_status z80_decByte(z80_t *z80, mem_t *mem, void *reg, 
                       uint32_t flag) {
  if (z80 == NULL || mem == NULL || reg == NULL) {
    return z80_bad_param;
  }

  uint8_t *data = (uint8_t *)reg;
  z80->dt = 4;
  if (flag & (z80_srcIsAddr | z80_dstIsAddr)) {
    VALID_MEM_OP(mem_getPointer(mem, *((uint16_t *)reg), &data));
    z80->dt = 12;
  }

  (*data)--;

  z80->f = z80->f & CARRY_FLAG;
  z80->f |= z80->f & SUB_FLAG;
  if (*data == 0) {
    z80->f |= ZERO_FLAG;
  }

  if ((*data & 0x0F) == 0x0F) {
    z80->f |= HALF_FLAG;
  }

  return z80_ok;
}

//This fuction will add the lower nibbles together and set the
//appropriate flags
z80_status addNibble(uint8_t a, uint8_t b, uint8_t *flags, 
                     uint8_t *result) {
  if (flags == NULL)  {
    return z80_bad_param;
  }

  a &= 0x0F;
  b &= 0x0F;
  
  *result = a + b;
  if (*flags & CARRY_FLAG) {
    (*result)++;
    *flags = HALF_FLAG;
  }
  if (*result > 0x0F) {
    *flags |= CARRY_FLAG;
  }
  *result &= 0x0F;
  return z80_ok;
}

z80_status z80_addShort(z80_t *z80, mem_t *mem, uint16_t *dst,
                        uint16_t src) {
  if (z80 == NULL || mem == NULL || dst == NULL) {
    return z80_bad_param;
  }

  uint8_t result = 0;
  uint8_t flag = 0;
  addNibble(*dst & 0x0F, src & 0x0F, &flag, &result);
  *dst = (0xFFF0 & *dst) | (src & 0x0F);
  addNibble((*dst>>4) & 0x0F, (src >> 4) & 0x0F, &flag, &result);
  *dst = (*dst & 0xFF0F) | ((result << 4) & 0x00F0);
  addNibble((*dst>>8) & 0x0F, (src >> 8) & 0x0F, &flag, &result);
  *dst = (*dst & 0xF0FF) | ((result << 8) & 0x0F00);
  addNibble((*dst>>12) & 0x0F, (src >> 12) & 0x0F, &flag, &result);
  *dst = (*dst & 0x0FFF) | ((result << 12) & 0xF000);

  z80->f = (z80->f & ZERO_FLAG) | (flag & (HALF_FLAG | CARRY_FLAG));
  z80->dt = 8;
  return z80_ok;
}

z80_status z80_jp(z80_t *z80, mem_t *mem, uint8_t numAddrBytes, 
       uint8_t condition) {
  if (z80 == NULL || mem == NULL || numAddrBytes > 2) {
    return z80_bad_param;
  }

  if (numAddrBytes == 1) {
    int8_t addr = 0;
    VALID_MEM_OP(mem_readByte(mem, z80->pc++, (uint8_t *)&addr));
    if (condition) {
      z80->dt = 12;
      z80->pc += addr;
    }else {
      z80->dt = 8;
    }
  } else {
    int16_t addr = 0;
    VALID_MEM_OP(mem_readShort(mem, z80->pc, (uint16_t *)&addr));
    z80->pc += 2;
    if (condition) {
      z80->dt = 16;
      z80->pc = addr;
    } else {
      z80->dt = 12;
    }
  }
  return z80_ok;
}

z80_status z80_addByte(z80_t *z80, mem_t *mem, uint8_t *dst, 
                       uint8_t *src, uint32_t flag) {
  if (z80 == NULL || mem == NULL || dst == NULL) {
    return z80_bad_param;
  }

  uint8_t srcData = 0;
  if (flag & z80_srcIsAddr) {
    z80->dt = 8;
    VALID_MEM_OP(mem_readByte(mem, *((uint16_t *)src), &srcData));
  } else {
    z80->dt = 4;
    srcData = *src;
  }

  if ((flag & z80_withCarry) && (z80->f & CARRY_FLAG)) {
    srcData++;
  }

  uint8_t opFlags = 0;
  uint8_t result = 0;
  addNibble(*dst, srcData, &opFlags, &result);
  *dst = (*dst & 0xF0) | (result & 0x0F);
  addNibble(*dst >> 4, srcData >> 4, &opFlags, &result);
  *dst = (*dst & 0x0F) | ((result << 4) & 0xF0);

  if (*dst == 0) {
    opFlags |= ZERO_FLAG;
  }

  z80->f = opFlags;
  

  return z80_ok;
}

z80_status subNibble(uint8_t a, uint8_t b, uint8_t *flags, 
                     uint8_t *result) {
  if (flags == NULL || result == NULL)  {
    return z80_bad_param;
  }

  a &= 0x0F;
  b &= 0x0F;
  
  *result = a - b;

  if (*flags & CARRY_FLAG) {
    (*result)--;
    *flags |= HALF_FLAG;
  }
  if (*result > 0x0F) {
    *flags |= CARRY_FLAG;
  }
  *flags |= SUB_FLAG;
  *result &= 0x0F;
  return z80_ok;
}

z80_status z80_subByte(z80_t *z80, mem_t *mem, uint8_t *dst, 
                       uint8_t *src, uint32_t flag) {
  if (z80 == NULL || mem == NULL || dst == NULL) {
    return z80_bad_param;
  }
  uint8_t srcData = 0;
  if (flag & z80_srcIsAddr) {
    z80->dt = 8;
    VALID_MEM_OP(mem_readByte(mem, *((uint16_t *)src), &srcData));
  } else {
    z80->dt = 4;
    srcData = *src;
  }

  if ((flag & z80_withCarry) && (z80->f & CARRY_FLAG)) {
    srcData--;
  }

  uint8_t opFlags = 0;
  uint8_t result = 0;
  subNibble(*dst, srcData, &opFlags, &result);
  *dst = (*dst & 0xF0) | (result & 0x0F);
  subNibble(*dst >> 4, srcData >> 4, &opFlags, &result);
  *dst = (*dst & 0x0F) | ((result << 4) & 0xF0);

  if (*dst == 0) {
    opFlags |= ZERO_FLAG;
  }

  z80->f = opFlags;

  return z80_ok;
}

z80_status z80_and(z80_t *z80, mem_t *mem, uint8_t *reg, 
                   uint32_t flag) {

  if (z80 == NULL || mem == NULL || reg == NULL) {
    return z80_bad_param;
  }
  //The destination register is always going to be A. but the
  //what we are comparing against might be a pointer
  uint8_t srcData = 0;
  if (flag & (z80_srcIsAddr | z80_dstIsAddr)) {
    VALID_MEM_OP(mem_readByte(mem, *((uint16_t *)reg), &srcData));
    z80->dt = 8;
  } else {
    srcData = *reg;
    z80->dt = 4;
  }

  z80->a &= srcData;

  z80->f = HALF_FLAG;
  if (z80->a == 0) {
    z80->f |= ZERO_FLAG;
  }
  return z80_ok;
}

z80_status z80_xor(z80_t *z80, mem_t *mem, uint8_t *reg,
                   uint32_t flag) {
  if (z80 == NULL || mem == NULL || reg == NULL) {
    return z80_bad_param;
  }
  //The destination register is always going to be A. but the
  //what we are comparing against might be a pointer
  uint8_t srcData = 0;
  if (flag & (z80_srcIsAddr | z80_dstIsAddr)) {
    VALID_MEM_OP(mem_readByte(mem, *((uint16_t *)reg), &srcData));
    z80->dt = 8;
  } else {
    srcData = *reg;
    z80->dt = 4;
  }

  z80->a ^= srcData;

  z80->f = 0;
  if (z80->a == 0) {
    z80->f |= ZERO_FLAG;
  }
  return z80_ok;
}

z80_status z80_or(z80_t *z80, mem_t *mem, uint8_t *reg,
                   uint32_t flag) {
  if (z80 == NULL || mem == NULL || reg == NULL) {
    return z80_bad_param;
  }
  //The destination register is always going to be A. but the
  //what we are comparing against might be a pointer
  uint8_t srcData = 0;
  if (flag & (z80_srcIsAddr | z80_dstIsAddr)) {
    VALID_MEM_OP(mem_readByte(mem, *((uint16_t *)reg), &srcData));
    z80->dt = 8;
  } else {
    srcData = *reg;
    z80->dt = 4;
  }

  z80->a |= srcData;

  z80->f = 0;
  if (z80->a == 0) {
    z80->f |= ZERO_FLAG;
  }
  return z80_ok;
}

z80_status z80_cp(z80_t *z80, mem_t *mem, uint8_t *src, 
                  uint32_t flag) {
  if (z80 == NULL || mem == NULL) {
    return z80_bad_param;
  }

  uint8_t srcData = 0;
  if (flag & z80_srcIsAddr) {
    z80->dt = 8;
    VALID_MEM_OP(mem_readByte(mem, *((uint16_t *)src), &srcData));
  } else {
    z80->dt = 4;
    srcData = *src;
  }

  uint8_t opFlags = 0;
  uint8_t result = 0;
  uint8_t temp = z80->a;
  subNibble(temp, srcData, &opFlags, &result);
  temp = (temp & 0xF0) | (result & 0x0F);
  subNibble(temp >> 4, srcData >> 4, &opFlags, &result);
  temp = (temp & 0x0F) | ((result << 4) & 0xF0);

  opFlags |= SUB_FLAG;

  if (temp == 0) {
    opFlags |= ZERO_FLAG;
  }

  z80->f = opFlags;
  
  return z80_ok;
}

z80_status z80_pop(z80_t *z80, mem_t *mem, uint16_t *reg) {
  if (z80 == NULL || mem == NULL || reg == NULL) {
    return z80_bad_param;
  }
  uint16_t temp = 0;
  VALID_MEM_OP(mem_readShort(mem, z80->sp, &temp));
  *reg = temp;
  z80->sp += 2;
  return z80_ok;
}

z80_status z80_push(z80_t *z80, mem_t *mem, uint16_t reg) {
  if (z80 == NULL || mem == NULL) {
    return z80_bad_param;
  }

  z80->sp -= 2;
  VALID_MEM_OP(mem_writeShort(mem, z80->sp, reg));
  
  return z80_ok;
}

z80_status z80_testBit(z80_t *z80, mem_t *mem, uint8_t data, 
                       uint8_t bit) {
  z80->f &= ~SUB_FLAG;
  z80->f |= HALF_FLAG; 
  if (data & (1<<bit)) {
    z80->f &= ~ZERO_FLAG;
  }else {
    z80->f |= ZERO_FLAG;
  }
  return z80_ok;
  z80->dt = 8;
}

z80_status z80_resetBit(z80_t *z80, mem_t *mem, uint8_t *data, 
                       uint8_t bit) {
  *data &= ~1<<bit;
  z80->dt = 8;
  return z80_ok;
}

z80_status z80_setBit(z80_t *z80, mem_t *mem, uint8_t *data, 
                       uint8_t bit) {
  *data |= 1<<bit;
  z80->dt = 8;
  return z80_ok;
}