#include <stdio.h>

#include "cpu/z80.h"
#include "cpu/z80_instr.h"
#include "cpu/mem.h"

#define DEBUG(args...) printf(args)
#define state z80, mem
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
  VALID_MEM_OP(mem_readByte(mem, z80->pc++, &opcode));
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
  VALID_MEM_OP(mem_readByte(mem, z80->pc++, &opcode));
  
  printf("Read Opcode: %x\r\n",opcode);
  uint16_t tempS;
  uint8_t tempB;
  switch (opcode) {
    case zNOP: // 0x00
      z80_nop(state);
      break;

    case zLD_bc_d16: // 0x01
      z80_loadShort(state, &(z80->bc));
      break;

    case zLD_BC_a: // 0x02
      z80_movGen(state, &(z80->bc), &(z80->a), z80_srcIsAddr);
      break;

    case zINC_bc: // 0x03
      z80_incShort(state, &(z80->bc));
      break;

    case zINC_b: // 0x04
      z80_incByte(state, &(z80->b), 0);
      break;

    case zDEC_b: // 0x05
      z80_decByte(state, &(z80->c), 0);
      break;

    case zLD_b_d8: // 0x06
      z80_movGen(state, &(z80->b), &(z80->pc), 
                 z80_srcIsAddr | z80_srcInc);
      break;

    case zADD_hl_bc: // 0x09
      z80_addShort(state, &(z80->hl), z80->bc);
      break;

    case zLD_a_BC: // 0x0A
      z80_movGen(state, &(z80->a), &(z80->bc), z80_srcIsAddr);
      break;

    case zDEC_bc: // 0x0B
      z80_decShort(state, &(z80->bc));
      break;

    case zINC_c: // 0x0C
      z80_incByte(state, &(z80->c), 0);
      break;

    case zDEC_c: // 0x0D
      z80_decByte(state, &(z80->c), 0);
      break;

    case zLD_c_d8: // 0x0E
      z80_movGen(state, &(z80->c), &(z80->pc), 
                 z80_srcIsAddr | z80_srcInc);
      break;

    case zLD_de_d16: // 0x11
      z80_loadShort(state, &(z80->de));
      break;

    case zLD_DE_a: // 0x12
      z80_movGen(state, &(z80->de), &(z80->a), z80_srcIsAddr);
      break;

    case zINC_de: // 0x13
      z80_incShort(state, &(z80->de));
      break;

    case zINC_d: // 0x14
      z80_incByte(state, &(z80->d), 0);
      break;

    case zDEC_d: // 0x15
      z80_decByte(state, &(z80->d), 0);
      break;

    case zLD_d_d8: // 0x16
      z80_movGen(state, &(z80->d), &(z80->pc), 
                 z80_srcIsAddr | z80_srcInc);
      break;
      
    case zJR_r8: // 0x18
      z80_jp(state, 1, 1);
      break;

    case zADD_hl_de: // 0x19
      z80_addShort(state, &(z80->hl), z80->de);
      break;

    case zLD_a_DE: // 0x1A
      z80_movGen(state, &(z80->a), &(z80->de), z80_srcIsAddr);
      break;

    case zDEC_de: // 0x1B
      z80_decShort(state, &(z80->de));
      break;

    case zINC_e: // 0x1C
      z80_incByte(state, &(z80->e), 0);
      break;

    case zDEC_e: // 0x1D
      z80_decByte(state, &(z80->e), 0);
      break;

    case zLD_e_d8: // 0x1E
      z80_movGen(state, &(z80->e), &(z80->pc), 
                 z80_srcIsAddr | z80_srcInc);
      break;

    case zJRNZ_r8: // 0x20
      z80_jp(state, 1, (z80->f & ZERO_FLAG) == 0);
      break;

    case zLD_hl_d16: // 0x21
      z80_loadShort(state, &(z80->hl));
      break;

    case zLD_HLI_a: // 0x22
      z80_movGen(state, &(z80->hl), &(z80->a), 
                z80_srcIsAddr | z80_srcInc);
      break;

    case zINC_hl: // 0x23
      z80_incShort(state, &(z80->hl));
      break;

    case zINC_h: // 0x24
      z80_incByte(state, &(z80->h), 0);
      break;

    case zDEC_h: // 0x25
      z80_decByte(state, &(z80->h), 0);
      break;

    case zLD_h_d8: // 0x26
      z80_movGen(state, &(z80->h), &(z80->pc),
                 z80_srcIsAddr | z80_srcInc);
      break;

    case zJRZ_r8: // 0x28
      z80_jp(state, 1, z80->f & ZERO_FLAG);
      break;

    case zADD_hl_hl: // 0x29
      z80_addShort(state, &(z80->hl), z80->hl);
      break;

    case zLD_a_HLI: // 0x2A
      z80_movGen(state, &(z80->a), &(z80->hl), 
                 z80_srcIsAddr | z80_srcInc);
      break;

    case zDEC_hl: // 0x2B
      z80_decShort(state, &(z80->hl));
      break;

    case zINC_l: // 0x2C
      z80_incByte(state, &(z80->l), 0);
      break;

    case zDEC_l: // 0x2D
      z80_decByte(state, &(z80->l), 0);
      break;

    case zLD_l_d8: // 0x2E
      z80_movGen(state, &(z80->l), &(z80->pc), 
                 z80_srcIsAddr | z80_srcInc);
      break;

    case zJRNC_r8: // 0x30
      z80_jp(state, 1, (z80->f & CARRY_FLAG) == 0);
      break;

    case zLD_sp_d16: // 0x31
      z80_loadShort(state, &(z80->sp));
      break;

    case zLD_HLD_a: // 0x32
      z80_movGen(state, &(z80->hl), &(z80->a),
                z80_srcIsAddr | z80_srcDec);
      break;

    case zINC_sp: // 0x33
      z80_incShort(state, &(z80->sp));
      break;

    case zINC_HL: // 0x34
      z80_incByte(state, &(z80->hl), z80_dstIsAddr);
      break;

    case zDEC_HL: // 0x35
      z80_decByte(state, &(z80->hl), z80_dstIsAddr);
      break;

    case zLD_HL_d8: // 0x36
      z80_movGen(state, &(z80->hl), &(z80->pc),
                 z80_srcIsAddr | z80_srcInc | z80_dstIsAddr);
      break;

    case zJRC_r8: // 0x38
      z80_jp(state, 1, z80->f & CARRY_FLAG);
      break;

    case zADD_hl_sp: // 0x39
      z80_addShort(state, &(z80->hl), z80->sp);
      break;

    case zLD_a_HLD: // 0x3A
      z80_movGen(state, &(z80->a), &(z80->hl), 
                 z80_srcIsAddr | z80_srcDec);
      break;

    case zDEC_sp: // 0x3B
      z80_decShort(state, &(z80->sp));
      break;

    case zINC_a: // 0x3C
      z80_incByte(state, &(z80->a), 0);
      break;

    case zDEC_a: // 0x3D
      z80_decByte(state, &(z80->a), 0);
      break;

    case zLD_a_d8: // 0x3E
      z80_movGen(state, &(z80->a), &(z80->pc), 
                 z80_srcIsAddr | z80_srcInc);
      break;

    case zLD_b_b:   // 0x40
      z80_mov(state, &(z80->b), z80->b);
      break;

    case zLD_b_c:   // 0x41
      z80_mov(state, &(z80->b), z80->c);
      break;

    case zLD_b_d:   // 0x42
      z80_mov(state, &(z80->b), z80->d);
      break;

    case zLD_b_e:   // 0x43
      z80_mov(state, &(z80->b), z80->e);
      break;

    case zLD_b_h:   // 0x44
      z80_mov(state, &(z80->b), z80->h);
      break;

    case zLD_b_l:   // 0x45
      z80_mov(state, &(z80->b), z80->l);
      break;

    case zLD_b_HL:  // 0x46
      z80_movGen(state, &(z80->b), &(z80->hl), z80_srcIsAddr);
      break;

    case zLD_b_a:   // 0x47
      z80_mov(state, &(z80->b), z80->a);
      break;

    case zLD_c_b:   // 0x48
      z80_mov(state, &(z80->c), z80->b);
      break;

    case zLD_c_c:   // 0x49
      z80_mov(state, &(z80->c), z80->c);
      break;

    case zLD_c_d:   // 0x4A
      z80_mov(state, &(z80->c), z80->d);
      break;

    case zLD_c_e:   // 0x4B
      z80_mov(state, &(z80->c), z80->e);
      break;

    case zLD_c_h:   // 0x4C
      z80_mov(state, &(z80->c), z80->h);
      break;

    case zLD_c_l:   // 0x4D
      z80_mov(state, &(z80->c), z80->l);
      break;

    case zLD_c_HL:  // 0x4E
      z80_movGen(state, &(z80->c), &(z80->hl), z80_srcIsAddr);
      break;

    case zLD_c_a:   // 0x4F
      z80_mov(state, &(z80->c), z80->a);
      break;

    case zLD_d_b:   // 0x50
      z80_mov(state, &(z80->d), z80->b);
      break;

    case zLD_d_c:   // 0x51
      z80_mov(state, &(z80->d), z80->c);
      break;

    case zLD_d_d:   // 0x52
      z80_mov(state, &(z80->d), z80->d);
      break;

    case zLD_d_e:   // 0x53
      z80_mov(state, &(z80->d), z80->e);
      break;

    case zLD_d_h:   // 0x54
      z80_mov(state, &(z80->d), z80->h);
      break;

    case zLD_d_l:   // 0x55
      z80_mov(state, &(z80->d), z80->l);
      break;

    case zLD_d_HL:  // 0x56
      z80_movGen(state, &(z80->d), &(z80->hl), z80_srcIsAddr);
      break;

    case zLD_d_a:   // 0x57
      z80_mov(state, &(z80->d), z80->a);
      break;

    case zLD_e_b:   // 0x58
      z80_mov(state, &(z80->e), z80->b);
      break;

    case zLD_e_c:   // 0x59
      z80_mov(state, &(z80->e), z80->c);
      break;

    case zLD_e_d:   // 0x5A
      z80_mov(state, &(z80->e), z80->d);
      break;

    case zLD_e_e:   // 0x5B
      z80_mov(state, &(z80->e), z80->e);
      break;

    case zLD_e_h:   // 0x5C
      z80_mov(state, &(z80->e), z80->h);
      break;

    case zLD_e_l:   // 0x5D
      z80_mov(state, &(z80->e), z80->l);
      break;

    case zLD_e_HL:  // 0x5E
      z80_movGen(state, &(z80->e), &(z80->hl), z80_srcIsAddr);
      break;

    case zLD_e_a:   // 0x5F
      z80_mov(state, &(z80->e), z80->a);
      break;

    case zLD_h_b:   // 0x60
      z80_mov(state, &(z80->h), z80->b);
      break;

    case zLD_h_c:   // 0x61
      z80_mov(state, &(z80->h), z80->c);
      break;

    case zLD_h_d:   // 0x62
      z80_mov(state, &(z80->h), z80->d);
      break;

    case zLD_h_e:   // 0x63
      z80_mov(state, &(z80->h), z80->e);
      break;

    case zLD_h_h:   // 0x64
      z80_mov(state, &(z80->h), z80->h);
      break;

    case zLD_h_l:   // 0x65
      z80_mov(state, &(z80->h), z80->l);
      break;

    case zLD_h_HL:  // 0x66
      z80_movGen(state, &(z80->h), &(z80->hl), z80_srcIsAddr);
      break;

    case zLD_h_a:   // 0x67
      z80_mov(state, &(z80->h), z80->a);
      break;

    case zLD_l_b:   // 0x68
      z80_mov(state, &(z80->l), z80->b);
      break;

    case zLD_l_c:   // 0x69
      z80_mov(state, &(z80->l), z80->c);
      break;

    case zLD_l_d:   // 0x6A
      z80_mov(state, &(z80->l), z80->d);
      break;

    case zLD_l_e:   // 0x6B
      z80_mov(state, &(z80->l), z80->e);
      break;

    case zLD_l_h:   // 0x6C
      z80_mov(state, &(z80->l), z80->h);
      break;

    case zLD_l_l:   // 0x6D
      z80_mov(state, &(z80->l), z80->l);
      break;

    case zLD_l_HL:  // 0x6E
      z80_movGen(state, &(z80->l), &(z80->hl), z80_srcIsAddr);
      break;

    case zLD_l_a:   // 0x6F
      z80_mov(state, &(z80->l), z80->a);
      break;

    case zLD_HL_b:   // 0x70
      z80_movGen(state, &(z80->hl), &(z80->b), z80_dstIsAddr);
      break;

    case zLD_HL_c:   // 0x71
      z80_movGen(state, &(z80->hl), &(z80->c), z80_dstIsAddr);
      break;

    case zLD_HL_d:   // 0x72
      z80_movGen(state, &(z80->hl), &(z80->d), z80_dstIsAddr);
      break;

    case zLD_HL_e:   // 0x73
      z80_movGen(state, &(z80->hl), &(z80->e), z80_dstIsAddr);
      break;

    case zLD_HL_h:   // 0x74
      z80_movGen(state, &(z80->hl), &(z80->h), z80_dstIsAddr);
      break;

    case zLD_HL_l:   // 0x75
      z80_movGen(state, &(z80->hl), &(z80->l), z80_dstIsAddr);
      break;

    case zHALT:  // 0x76
      //TODO Halt Instruction
      break;

    case zLD_HL_a:   // 0x77
      z80_movGen(state, &(z80->hl), &(z80->a), z80_dstIsAddr);
      break;

    case zLD_a_b:   // 0x78
      z80_mov(state, &(z80->a), z80->b);
      break;

    case zLD_a_c:   // 0x79
      z80_mov(state, &(z80->a), z80->c);
      break;

    case zLD_a_d:   // 0x7A
      z80_mov(state, &(z80->a), z80->d);
      break;

    case zLD_a_e:   // 0x7B
      z80_mov(state, &(z80->a), z80->e);
      break;

    case zLD_a_h:   // 0x7C
      z80_mov(state, &(z80->a), z80->h);
      break;

    case zLD_a_l:   // 0x7D
      z80_mov(state, &(z80->a), z80->l);
      break;

    case zLD_a_HL:  // 0x7E
      z80_movGen(state, &(z80->a), &(z80->hl), z80_srcIsAddr);
      break;

    case zLD_a_a:   // 0x7F
      z80_mov(state, &(z80->a), z80->a);
      break;

    case zADD_a_b: // 0x80
      z80_addByte(state, &(z80->a), &(z80->b), 0);
      break;

    case zADD_a_c: // 0x81
      z80_addByte(state, &(z80->a), &(z80->c), 0);
      break;

    case zADD_a_d: // 0x82
      z80_addByte(state, &(z80->a), &(z80->d), 0);
      break;

    case zADD_a_e: // 0x83
      z80_addByte(state, &(z80->a), &(z80->e), 0);
      break;

    case zADD_a_h: // 0x84
      z80_addByte(state, &(z80->a), &(z80->h), 0);
      break;

    case zADD_a_l: // 0x85
      z80_addByte(state, &(z80->a), &(z80->l), 0);
      break;

    case zADD_a_HL: // 0x86
      z80_addByte(state, &(z80->a), &(z80->hl), z80_srcIsAddr);
      break;

    case zADD_a_a: // 0x87
      z80_addByte(state, &(z80->a), &(z80->a), 0);
      break;

    case zADC_a_b: // 0x88
      z80_addByte(state, &(z80->a), &(z80->b), z80_withCarry);
      break;

    case zADC_a_c: // 0x89
      z80_addByte(state, &(z80->a), &(z80->c), z80_withCarry);
      break;

    case zADC_a_d: // 0x8A
      z80_addByte(state, &(z80->a), &(z80->d), z80_withCarry);
      break;

    case zADC_a_e: // 0x8B
      z80_addByte(state, &(z80->a), &(z80->e), z80_withCarry);
      break;

    case zADC_a_h: // 0x8C
      z80_addByte(state, &(z80->a), &(z80->h), z80_withCarry);
      break;

    case zADC_a_l: // 0x8D
      z80_addByte(state, &(z80->a), &(z80->l), z80_withCarry);
      break;

    case zADC_a_HL: // 0x8E
      z80_addByte(state, &(z80->a), &(z80->hl), 
                  z80_srcIsAddr | z80_withCarry);
      break;

    case zADC_a_a: // 0x8F
      z80_addByte(state, &(z80->a), &(z80->a), z80_withCarry);
      break;
      

    case 0xCB:      
      z80_extension(z80, mem);
      break;

    default:
      DEBUG("Opcode not implemented\r\n");
      return -1;
      break;
  }
  return 0;
}


