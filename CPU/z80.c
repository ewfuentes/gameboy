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
  uint8_t *tempP = NULL;
  switch(opcode) {
    case zRLC_b: // 0x00
      z80_rlc(state, &(z80->b), 0);
      break;
    case zRLC_c: // 0x01
      z80_rlc(state, &(z80->c), 0);
      break;
    case zRLC_d: // 0x02
      z80_rlc(state, &(z80->d), 0);
      break;
    case zRLC_e: // 0x03
      z80_rlc(state, &(z80->e), 0);
      break;
    case zRLC_h: // 0x04
      z80_rlc(state, &(z80->h), 0);
      break;
    case zRLC_l: // 0x05
      z80_rlc(state, &(z80->l), 0);
      break;
    case zRLC_HL: // 0x06
      z80_rlc(state, (uint8_t *)&(z80->hl), z80_srcIsAddr);
      break;
    case zRLC_a: // 0x07
      z80_rlc(state, &(z80->a), 0);
      break;
    case zRRC_b: // 0x08
      z80_rrc(state, &(z80->b), 0);
      break;
    case zRRC_c: // 0x09
      z80_rrc(state, &(z80->c), 0);
      break;
    case zRRC_d: // 0x0A
      z80_rrc(state, &(z80->d), 0);
      break;
    case zRRC_e: // 0x0B
      z80_rrc(state, &(z80->e), 0);
      break;
    case zRRC_h: // 0x0C
      z80_rrc(state, &(z80->h), 0);
      break;
    case zRRC_l: // 0x0D
      z80_rrc(state, &(z80->l), 0);
      break;
    case zRRC_HL: // 0x0E
      z80_rrc(state, (uint8_t *)&(z80->hl), z80_srcIsAddr);
      break;
    case zRRC_a: // 0x0F
      z80_rrc(state, &(z80->a), 0);
      break;
    case zRL_b: // 0x10
      z80_rl(state, &(z80->b), 0);
      break;
    case zRL_c: // 0x11
      z80_rl(state, &(z80->c), 0);
      break;
    case zRL_d: // 0x12
      z80_rl(state, &(z80->d), 0);
      break;
    case zRL_e: // 0x13
      z80_rl(state, &(z80->e), 0);
      break;
    case zRL_h: // 0x14
      z80_rl(state, &(z80->h), 0);
      break;
    case zRL_l: // 0x15
      z80_rl(state, &(z80->l), 0);
      break;
    case zRL_HL: // 0x16
      z80_rl(state, (uint8_t *)&(z80->hl), z80_srcIsAddr);
      break;
    case zRL_a: // 0x17
      z80_rl(state, &(z80->a), 0);
      break;
    case zRR_b: // 0x18
      z80_rr(state, &(z80->b), 0);
      break;
    case zRR_c: // 0x19
      z80_rr(state, &(z80->c), 0);
      break;
    case zRR_d: // 0x1A
      z80_rr(state, &(z80->d), 0);
      break;
    case zRR_e: // 0x1B
      z80_rr(state, &(z80->e), 0);
      break;
    case zRR_h: // 0x1C
      z80_rr(state, &(z80->h), 0);
      break;
    case zRR_l: // 0x1D
      z80_rr(state, &(z80->l), 0);
      break;
    case zRR_HL: // 0x1E
      z80_rr(state, (uint8_t *)&(z80->hl), z80_srcIsAddr);
      break;
    case zRR_a: // 0x1F
      z80_rr(state, &(z80->a), 0);
      break;
    case zSLA_b: // 0x20
      z80_sla(state, &(z80->b), 0);
      break;
    case zSLA_c: // 0x21
      z80_sla(state, &(z80->c), 0);
      break;
    case zSLA_d: // 0x22
      z80_sla(state, &(z80->d), 0);
      break;
    case zSLA_e: // 0x23
      z80_sla(state, &(z80->e), 0);
      break;
    case zSLA_h: // 0x24
      z80_sla(state, &(z80->h), 0);
      break;
    case zSLA_l: // 0x25
      z80_sla(state, &(z80->l), 0);
      break;
    case zSLA_HL: // 0x26
      z80_sla(state, (uint8_t *)&(z80->hl), z80_srcIsAddr);
      break;
    case zSLA_a: // 0x27
      z80_sla(state, &(z80->a), 0);
      break;
    case zSRA_b: // 0x28
      z80_sra(state, &(z80->b), 0);
      break;
    case zSRA_c: // 0x29
      z80_sra(state, &(z80->c), 0);
      break;
    case zSRA_d: // 0x2A
      z80_sra(state, &(z80->d), 0);
      break;
    case zSRA_e: // 0x2B
      z80_sra(state, &(z80->e), 0);
      break;
    case zSRA_h: // 0x2C
      z80_sra(state, &(z80->h), 0);
      break;
    case zSRA_l: // 0x2D
      z80_sra(state, &(z80->l), 0);
      break;
    case zSRA_HL: // 0x2E
      z80_sra(state, (uint8_t *)&(z80->hl), z80_srcIsAddr);
      break;
    case zSRA_a: // 0x2F
      z80_sra(state, &(z80->a), 0);
      break;

    case zSWAP_b: // 0x30
      z80_swapNibbles(state, &(z80->b), 0);
      break;
    case zSWAP_c: // 0x31
      z80_swapNibbles(state, &(z80->c), 0);
      break;
    case zSWAP_d: // 0x32
      z80_swapNibbles(state, &(z80->d), 0);
      break;
    case zSWAP_e: // 0x33
      z80_swapNibbles(state, &(z80->e), 0);
      break;
    case zSWAP_h: // 0x34
      z80_swapNibbles(state, &(z80->h), 0);
      break;
    case zSWAP_l: // 0x35
      z80_swapNibbles(state, &(z80->l), 0);
      break;
    case zSWAP_HL: // 0x36
      z80_swapNibbles(state, (uint8_t *)&(z80->hl), z80_srcIsAddr);
      break;
    case zSWAP_a: // 0x37
      z80_swapNibbles(state, &(z80->a), 0);
      break;
    case zSRL_b: // 0x38
      z80_srl(state, &(z80->b), 0);
      break;
    case zSRL_c: // 0x39
      z80_srl(state, &(z80->c), 0);
      break;
    case zSRL_d: // 0x3A
      z80_srl(state, &(z80->d), 0);
      break;
    case zSRL_e: // 0x3B
      z80_srl(state, &(z80->e), 0);
      break;
    case zSRL_h: // 0x3C
      z80_srl(state, &(z80->h), 0);
      break;
    case zSRL_l: // 0x3D
      z80_srl(state, &(z80->l), 0);
      break;
    case zSRL_HL: // 0x3E
      z80_srl(state, (uint8_t *)&(z80->hl), z80_srcIsAddr);
      break;
    case zSRL_a: // 0x3F
      z80_srl(state, &(z80->a), 0);
      break;

    case zBIT0_b: // 0x40
      z80_testBit(state, z80->b, 0);
      break;
    case zBIT0_c: // 0x41
      z80_testBit(state, z80->c, 0);
      break;
    case zBIT0_d: // 0x42
      z80_testBit(state, z80->d, 0);
      break;
    case zBIT0_e: // 0x43
      z80_testBit(state, z80->e, 0);
      break;
    case zBIT0_h: // 0x44
      z80_testBit(state, z80->h, 0);
      break;
    case zBIT0_l: // 0x45
      z80_testBit(state, z80->l, 0);
      break;
    case zBIT0_HL: // 0x46
      VALID_MEM_OP(mem_readByte(mem, z80->hl, &tempB));
      z80_testBit(state, tempB, 0);
      z80->dt = 16;
      break;
    case zBIT0_a: // 0x47
      z80_testBit(state, z80->a, 0);
      break;
    case zBIT1_b: // 0x48
      z80_testBit(state, z80->b, 1);
      break;
    case zBIT1_c: // 0x49
      z80_testBit(state, z80->c, 1);
      break;
    case zBIT1_d: // 0x4A
      z80_testBit(state, z80->d, 1);
      break;
    case zBIT1_e: // 0x4B
      z80_testBit(state, z80->e, 1);
      break;
    case zBIT1_h: // 0x4C
      z80_testBit(state, z80->h, 1);
      break;
    case zBIT1_l: // 0x4D
      z80_testBit(state, z80->l, 1);
      break;
    case zBIT1_HL: // 0x4E
      VALID_MEM_OP(mem_readByte(mem, z80->hl, &tempB));
      z80_testBit(state, tempB, 1);
      z80->dt = 16;
      break;
    case zBIT1_a: // 0x4F
      z80_testBit(state, z80->a, 1);
      break;
    case zBIT2_b: // 0x50
      z80_testBit(state, z80->b, 2);
      break;
    case zBIT2_c: // 0x51
      z80_testBit(state, z80->c, 2);
      break;
    case zBIT2_d: // 0x52
      z80_testBit(state, z80->d, 2);
      break;
    case zBIT2_e: // 0x53
      z80_testBit(state, z80->e, 2);
      break;
    case zBIT2_h: // 0x54
      z80_testBit(state, z80->h, 2);
      break;
    case zBIT2_l: // 0x55
      z80_testBit(state, z80->l, 2);
      break;
    case zBIT2_HL: // 0x56
      VALID_MEM_OP(mem_readByte(mem, z80->hl, &tempB));
      z80_testBit(state, tempB, 2);
      z80->dt = 16;
      break;
    case zBIT2_a: // 0x57
      z80_testBit(state, z80->a, 2);
      break;
    case zBIT3_b: // 0x58
      z80_testBit(state, z80->b, 3);
      break;
    case zBIT3_c: // 0x59
      z80_testBit(state, z80->c, 3);
      break;
    case zBIT3_d: // 0x5A
      z80_testBit(state, z80->d, 3);
      break;
    case zBIT3_e: // 0x5B
      z80_testBit(state, z80->e, 3);
      break;
    case zBIT3_h: // 0x5C
      z80_testBit(state, z80->h, 3);
      break;
    case zBIT3_l: // 0x5D
      z80_testBit(state, z80->l, 3);
      break;
    case zBIT3_HL: // 0x5E
      VALID_MEM_OP(mem_readByte(mem, z80->hl, &tempB));
      z80_testBit(state, tempB, 3);
      z80->dt = 16;
      break;
    case zBIT3_a: // 0x5F
      z80_testBit(state, z80->a, 3);
      break;
    case zBIT4_b: // 0x60
      z80_testBit(state, z80->b, 4);
      break;
    case zBIT4_c: // 0x61
      z80_testBit(state, z80->c, 4);
      break;
    case zBIT4_d: // 0x62
      z80_testBit(state, z80->d, 4);
      break;
    case zBIT4_e: // 0x63
      z80_testBit(state, z80->e, 4);
      break;
    case zBIT4_h: // 0x64
      z80_testBit(state, z80->h, 4);
      break;
    case zBIT4_l: // 0x65
      z80_testBit(state, z80->l, 4);
      break;
    case zBIT4_HL: // 0x66
      VALID_MEM_OP(mem_readByte(mem, z80->hl, &tempB));
      z80_testBit(state, tempB, 4);
      z80->dt = 16;
      break;
    case zBIT4_a: // 0x67
      z80_testBit(state, z80->a, 4);
      break;
    case zBIT5_b: // 0x68
      z80_testBit(state, z80->b, 5);
      break;
    case zBIT5_c: // 0x69
      z80_testBit(state, z80->c, 5);
      break;
    case zBIT5_d: // 0x6A
      z80_testBit(state, z80->d, 5);
      break;
    case zBIT5_e: // 0x6B
      z80_testBit(state, z80->e, 5);
      break;
    case zBIT5_h: // 0x6C
      z80_testBit(state, z80->h, 5);
      break;
    case zBIT5_l: // 0x6D
      z80_testBit(state, z80->l, 5);
      break;
    case zBIT5_HL: // 0x6E
      VALID_MEM_OP(mem_readByte(mem, z80->hl, &tempB));
      z80_testBit(state, tempB, 5);
      z80->dt = 16;
      break;
    case zBIT5_a: // 0x6F
      z80_testBit(state, z80->a, 5);
      break;
    case zBIT6_b: // 0x70
      z80_testBit(state, z80->b, 6);
      break;
    case zBIT6_c: // 0x71
      z80_testBit(state, z80->c, 6);
      break;
    case zBIT6_d: // 0x72
      z80_testBit(state, z80->d, 6);
      break;
    case zBIT6_e: // 0x73
      z80_testBit(state, z80->e, 6);
      break;
    case zBIT6_h: // 0x74
      z80_testBit(state, z80->h, 6);
      break;
    case zBIT6_l: // 0x75
      z80_testBit(state, z80->l, 6);
      break;
    case zBIT6_HL: // 0x76
      VALID_MEM_OP(mem_readByte(mem, z80->hl, &tempB));
      z80_testBit(state, tempB, 6);
      z80->dt = 16;
      break;
    case zBIT6_a: // 0x77
      z80_testBit(state, z80->a, 6);
      break;
    case zBIT7_b: // 0x78
      z80_testBit(state, z80->b, 7);
      break;
    case zBIT7_c: // 0x79
      z80_testBit(state, z80->c, 7);
      break;
    case zBIT7_d: // 0x7A
      z80_testBit(state, z80->d, 7);
      break;
    case zBIT7_e: // 0x7B
      z80_testBit(state, z80->e, 7);
      break;
    case zBIT7_h: // 0x7C
      z80_testBit(state, z80->h, 7);
      break;
    case zBIT7_l: // 0x7D
      z80_testBit(state, z80->l, 7);
      break;
    case zBIT7_HL: // 0x7E
      VALID_MEM_OP(mem_readByte(mem, z80->hl, &tempB));
      z80_testBit(state, tempB, 7);
      z80->dt = 16;
      break;
    case zBIT7_a: // 0x7F
      z80_testBit(state, z80->a, 7);
      break;
    case zRES0_b: // 0x80
      z80_resetBit(state, &(z80->b), 0);
      break;
    case zRES0_c: // 0x81
      z80_resetBit(state, &(z80->c), 0);
      break;
    case zRES0_d: // 0x82
      z80_resetBit(state, &(z80->d), 0);
      break;
    case zRES0_e: // 0x83
      z80_resetBit(state, &(z80->e), 0);
      break;
    case zRES0_h: // 0x84
      z80_resetBit(state, &(z80->h), 0);
      break;
    case zRES0_l: // 0x85
      z80_resetBit(state, &(z80->l), 0);
      break;
    case zRES0_HL: // 0x86
      VALID_MEM_OP(mem_getPointer(mem, z80->hl, &tempP));
      z80_resetBit(state, tempP, 0);
      z80->dt = 16;
      break;
    case zRES0_a: // 0x87
      z80_resetBit(state, &(z80->a), 0);
      break;
    case zRES1_b: // 0x88
      z80_resetBit(state, &(z80->b), 1);
      break;
    case zRES1_c: // 0x89
      z80_resetBit(state, &(z80->c), 1);
      break;
    case zRES1_d: // 0x8A
      z80_resetBit(state, &(z80->d), 1);
      break;
    case zRES1_e: // 0x8B
      z80_resetBit(state, &(z80->e), 1);
      break;
    case zRES1_h: // 0x8C
      z80_resetBit(state, &(z80->h), 1);
      break;
    case zRES1_l: // 0x8D
      z80_resetBit(state, &(z80->l), 1);
      break;
    case zRES1_HL: // 0x8E
      VALID_MEM_OP(mem_getPointer(mem, z80->hl, &tempP));
      z80_resetBit(state, tempP, 1);
      z80->dt = 16;
      break;
    case zRES1_a: // 0x8F
      z80_resetBit(state, &(z80->a), 1);
      break;
    case zRES2_b: // 0x90
      z80_resetBit(state, &(z80->b), 2);
      break;
    case zRES2_c: // 0x91
      z80_resetBit(state, &(z80->c), 2);
      break;
    case zRES2_d: // 0x92
      z80_resetBit(state, &(z80->d), 2);
      break;
    case zRES2_e: // 0x93
      z80_resetBit(state, &(z80->e), 2);
      break;
    case zRES2_h: // 0x94
      z80_resetBit(state, &(z80->h), 2);
      break;
    case zRES2_l: // 0x95
      z80_resetBit(state, &(z80->l), 2);
      break;
    case zRES2_HL: // 0x96
      VALID_MEM_OP(mem_getPointer(mem, z80->hl, &tempP));
      z80_resetBit(state, tempP, 2);
      z80->dt = 16;
      break;
    case zRES2_a: // 0x97
      z80_resetBit(state, &(z80->a), 2);
      break;
    case zRES3_b: // 0x98
      z80_resetBit(state, &(z80->b), 3);
      break;
    case zRES3_c: // 0x99
      z80_resetBit(state, &(z80->c), 3);
      break;
    case zRES3_d: // 0x9A
      z80_resetBit(state, &(z80->d), 3);
      break;
    case zRES3_e: // 0x9B
      z80_resetBit(state, &(z80->e), 3);
      break;
    case zRES3_h: // 0x9C
      z80_resetBit(state, &(z80->h), 3);
      break;
    case zRES3_l: // 0x9D
      z80_resetBit(state, &(z80->l), 3);
      break;
    case zRES3_HL: // 0x9E
      VALID_MEM_OP(mem_getPointer(mem, z80->hl, &tempP));
      z80_resetBit(state, tempP, 3);
      z80->dt = 16;
      break;
    case zRES3_a: // 0x9F
      z80_resetBit(state, &(z80->a), 3);
      break;
    case zRES4_b: // 0xA0
      z80_resetBit(state, &(z80->b), 4);
      break;
    case zRES4_c: // 0xA1
      z80_resetBit(state, &(z80->c), 4);
      break;
    case zRES4_d: // 0xA2
      z80_resetBit(state, &(z80->d), 4);
      break;
    case zRES4_e: // 0xA3
      z80_resetBit(state, &(z80->e), 4);
      break;
    case zRES4_h: // 0xA4
      z80_resetBit(state, &(z80->h), 4);
      break;
    case zRES4_l: // 0xA5
      z80_resetBit(state, &(z80->l), 4);
      break;
    case zRES4_HL: // 0xA6
      VALID_MEM_OP(mem_getPointer(mem, z80->hl, &tempP));
      z80_resetBit(state, tempP, 4);
      z80->dt = 16;
      break;
    case zRES4_a: // 0xA7
      z80_resetBit(state, &(z80->a), 4);
      break;
    case zRES5_b: // 0xA8
      z80_resetBit(state, &(z80->b), 5);
      break;
    case zRES5_c: // 0xA9
      z80_resetBit(state, &(z80->c), 5);
      break;
    case zRES5_d: // 0xAA
      z80_resetBit(state, &(z80->d), 5);
      break;
    case zRES5_e: // 0xAB
      z80_resetBit(state, &(z80->e), 5);
      break;
    case zRES5_h: // 0xAC
      z80_resetBit(state, &(z80->h), 5);
      break;
    case zRES5_l: // 0xAD
      z80_resetBit(state, &(z80->l), 5);
      break;
    case zRES5_HL: // 0xAE
      VALID_MEM_OP(mem_getPointer(mem, z80->hl, &tempP));
      z80_resetBit(state, tempP, 5);
      z80->dt = 16;
      break;
    case zRES5_a: // 0xAF
      z80_resetBit(state, &(z80->a), 5);
      break;
    case zRES6_b: // 0xB0
      z80_resetBit(state, &(z80->b), 6);
      break;
    case zRES6_c: // 0xB1
      z80_resetBit(state, &(z80->c), 6);
      break;
    case zRES6_d: // 0xB2
      z80_resetBit(state, &(z80->d), 6);
      break;
    case zRES6_e: // 0xB3
      z80_resetBit(state, &(z80->e), 6);
      break;
    case zRES6_h: // 0xB4
      z80_resetBit(state, &(z80->h), 6);
      break;
    case zRES6_l: // 0xB5
      z80_resetBit(state, &(z80->l), 6);
      break;
    case zRES6_HL: // 0xB6
      VALID_MEM_OP(mem_getPointer(mem, z80->hl, &tempP));
      z80_resetBit(state, tempP, 6);
      z80->dt = 16;
      break;
    case zRES6_a: // 0xB7
      z80_resetBit(state, &(z80->a), 6);
      break;
    case zRES7_b: // 0xB8
      z80_resetBit(state, &(z80->b), 7);
      break;
    case zRES7_c: // 0xB9
      z80_resetBit(state, &(z80->c), 7);
      break;
    case zRES7_d: // 0xBA
      z80_resetBit(state, &(z80->d), 7);
      break;
    case zRES7_e: // 0xBB
      z80_resetBit(state, &(z80->e), 7);
      break;
    case zRES7_h: // 0xBC
      z80_resetBit(state, &(z80->h), 7);
      break;
    case zRES7_l: // 0xBD
      z80_resetBit(state, &(z80->l), 7);
      break;
    case zRES7_HL: // 0xBE
      VALID_MEM_OP(mem_getPointer(mem, z80->hl, &tempP));
      z80_resetBit(state, tempP, 7);
      z80->dt = 16;
      break;
    case zRES7_a: // 0xBF
      z80_setBit(state, &(z80->a), 7);
      break;
    case zSET0_b: // 0xC0
      z80_setBit(state, &(z80->b), 0);
      break;
    case zSET0_c: // 0xC1
      z80_setBit(state, &(z80->c), 0);
      break;
    case zSET0_d: // 0xC2
      z80_setBit(state, &(z80->d), 0);
      break;
    case zSET0_e: // 0xC3
      z80_setBit(state, &(z80->e), 0);
      break;
    case zSET0_h: // 0xC4
      z80_setBit(state, &(z80->h), 0);
      break;
    case zSET0_l: // 0xC5
      z80_setBit(state, &(z80->l), 0);
      break;
    case zSET0_HL: // 0xC6
      VALID_MEM_OP(mem_getPointer(mem, z80->hl, &tempP));
      z80_setBit(state, tempP, 0);
      z80->dt = 16;
      break;
    case zSET0_a: // 0xC7
      z80_setBit(state, &(z80->a), 0);
      break;
    case zSET1_b: // 0xC8
      z80_setBit(state, &(z80->b), 1);
      break;
    case zSET1_c: // 0xC9
      z80_setBit(state, &(z80->c), 1);
      break;
    case zSET1_d: // 0xCA
      z80_setBit(state, &(z80->d), 1);
      break;
    case zSET1_e: // 0xCB
      z80_setBit(state, &(z80->e), 1);
      break;
    case zSET1_h: // 0xCC
      z80_setBit(state, &(z80->h), 1);
      break;
    case zSET1_l: // 0xCD
      z80_setBit(state, &(z80->l), 1);
      break;
    case zSET1_HL: // 0xCE
      VALID_MEM_OP(mem_getPointer(mem, z80->hl, &tempP));
      z80_setBit(state, tempP, 1);
      z80->dt = 16;
      break;
    case zSET1_a: // 0xCF
      z80_setBit(state, &(z80->a), 1);
      break;
    case zSET2_b: // 0xD0
      z80_setBit(state, &(z80->b), 2);
      break;
    case zSET2_c: // 0xD1
      z80_setBit(state, &(z80->c), 2);
      break;
    case zSET2_d: // 0xD2
      z80_setBit(state, &(z80->d), 2);
      break;
    case zSET2_e: // 0xD3
      z80_setBit(state, &(z80->e), 2);
      break;
    case zSET2_h: // 0xD4
      z80_setBit(state, &(z80->h), 2);
      break;
    case zSET2_l: // 0xD5
      z80_setBit(state, &(z80->l), 2);
      break;
    case zSET2_HL: // 0xD6
      VALID_MEM_OP(mem_getPointer(mem, z80->hl, &tempP));
      z80_setBit(state, tempP, 2);
      z80->dt = 16;
      break;
    case zSET2_a: // 0xD7
      z80_setBit(state, &(z80->a), 2);
      break;
    case zSET3_b: // 0xD8
      z80_setBit(state, &(z80->b), 3);
      break;
    case zSET3_c: // 0xD9
      z80_setBit(state, &(z80->c), 3);
      break;
    case zSET3_d: // 0xDA
      z80_setBit(state, &(z80->d), 3);
      break;
    case zSET3_e: // 0xDB
      z80_setBit(state, &(z80->e), 3);
      break;
    case zSET3_h: // 0xDC
      z80_setBit(state, &(z80->h), 3);
      break;
    case zSET3_l: // 0xDD
      z80_setBit(state, &(z80->l), 3);
      break;
    case zSET3_HL: // 0xDE
      VALID_MEM_OP(mem_getPointer(mem, z80->hl, &tempP));
      z80_setBit(state, tempP, 3);
      z80->dt = 16;
      break;
    case zSET3_a: // 0xDF
      z80_setBit(state, &(z80->a), 3);
      break;
    case zSET4_b: // 0xE0
      z80_setBit(state, &(z80->b), 4);
      break;
    case zSET4_c: // 0xE1
      z80_setBit(state, &(z80->c), 4);
      break;
    case zSET4_d: // 0xE2
      z80_setBit(state, &(z80->d), 4);
      break;
    case zSET4_e: // 0xE3
      z80_setBit(state, &(z80->e), 4);
      break;
    case zSET4_h: // 0xE4
      z80_setBit(state, &(z80->h), 4);
      break;
    case zSET4_l: // 0xE5
      z80_setBit(state, &(z80->l), 4);
      break;
    case zSET4_HL: // 0xE6
      VALID_MEM_OP(mem_getPointer(mem, z80->hl, &tempP));
      z80_setBit(state, tempP, 4);
      z80->dt = 16;
      break;
    case zSET4_a: // 0xE7
      z80_setBit(state, &(z80->a), 4);
      break;
    case zSET5_b: // 0xE8
      z80_setBit(state, &(z80->b), 5);
      break;
    case zSET5_c: // 0xE9
      z80_setBit(state, &(z80->c), 5);
      break;
    case zSET5_d: // 0xEA
      z80_setBit(state, &(z80->d), 5);
      break;
    case zSET5_e: // 0xEB
      z80_setBit(state, &(z80->e), 5);
      break;
    case zSET5_h: // 0xEC
      z80_setBit(state, &(z80->h), 5);
      break;
    case zSET5_l: // 0xED
      z80_setBit(state, &(z80->l), 5);
      break;
    case zSET5_HL: // 0xEE
      VALID_MEM_OP(mem_getPointer(mem, z80->hl, &tempP));
      z80_setBit(state, tempP, 5);
      z80->dt = 16;
      break;
    case zSET5_a: // 0xEF
      z80_setBit(state, &(z80->a), 5);
      break;
    case zSET6_b: // 0xF0
      z80_setBit(state, &(z80->b), 6);
      break;
    case zSET6_c: // 0xF1
      z80_setBit(state, &(z80->c), 6);
      break;
    case zSET6_d: // 0xF2
      z80_setBit(state, &(z80->d), 6);
      break;
    case zSET6_e: // 0xF3
      z80_setBit(state, &(z80->e), 6);
      break;
    case zSET6_h: // 0xF4
      z80_setBit(state, &(z80->h), 6);
      break;
    case zSET6_l: // 0xF5
      z80_setBit(state, &(z80->l), 6);
      break;
    case zSET6_HL: // 0xF6
      VALID_MEM_OP(mem_getPointer(mem, z80->hl, &tempP));
      z80_setBit(state, tempP, 6);
      z80->dt = 16;
      break;
    case zSET6_a: // 0xF7
      z80_setBit(state, &(z80->a), 6);
      break;
    case zSET7_b: // 0xF8
      z80_setBit(state, &(z80->b), 7);
      break;
    case zSET7_c: // 0xF9
      z80_setBit(state, &(z80->c), 7);
      break;
    case zSET7_d: // 0xFA
      z80_setBit(state, &(z80->d), 7);
      break;
    case zSET7_e: // 0xFB
      z80_setBit(state, &(z80->e), 7);
      break;
    case zSET7_h: // 0xFC
      z80_setBit(state, &(z80->h), 7);
      break;
    case zSET7_l: // 0xFD
      z80_setBit(state, &(z80->l), 7);
      break;
    case zSET7_HL: // 0xFE
      VALID_MEM_OP(mem_getPointer(mem, z80->hl, &tempP));
      z80_setBit(state, tempP, 7);
      z80->dt = 16;
      break;
    case zSET7_a: // 0xFF
      z80_setBit(state, &(z80->a), 7);
      break;

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
      z80_movGen(state, (uint8_t *)&(z80->bc), &(z80->a),
                 z80_srcIsAddr);
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
      z80_movGen(state, &(z80->b), (uint8_t *)&(z80->pc), 
                 z80_srcIsAddr | z80_srcInc);
      break;

    case zRLCA: // 0x07
      z80_rlc(state, &(z80->a), 0);
      z80->dt = 4;
      z80->f &= CARRY_FLAG;
      break;

    case zLD_a16_sp: // 0x08
      VALID_MEM_OP(mem_readShort(mem, z80->pc, &tempS));
      z80->pc += 2;
      VALID_MEM_OP(mem_writeShort(mem, tempS, z80->sp));
      z80->dt = 20;
      break;

    case zADD_hl_bc: // 0x09
      z80_addShort(state, &(z80->hl), z80->bc);
      break;

    case zLD_a_BC: // 0x0A
      z80_movGen(state, &(z80->a), (uint8_t *)&(z80->bc), 
                 z80_srcIsAddr);
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
      z80_movGen(state, &(z80->c), (uint8_t *)&(z80->pc), 
                 z80_srcIsAddr | z80_srcInc);
      break;

    case zRRCA: // 0x0F
      z80_rrc(state, &(z80->a), 0);
      z80->dt = 4;
      break;

    case zLD_de_d16: // 0x11
      z80_loadShort(state, &(z80->de));
      break;

    case zLD_DE_a: // 0x12
      z80_movGen(state, (uint8_t *)&(z80->de), &(z80->a),
                 z80_srcIsAddr);
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
      z80_movGen(state, &(z80->d), (uint8_t *)&(z80->pc), 
                 z80_srcIsAddr | z80_srcInc);
      break;

    case zRLA: // 0x17
      z80_rl(state, &(z80->a), 0);
      z80->dt = 4;
      z80->f &= CARRY_FLAG;
      break;
      
    case zJR_r8: // 0x18
      z80_jp(state, 1, 1);
      break;

    case zADD_hl_de: // 0x19
      z80_addShort(state, &(z80->hl), z80->de);
      break;

    case zLD_a_DE: // 0x1A
      z80_movGen(state, &(z80->a), (uint8_t *)&(z80->de), 
                 z80_srcIsAddr);
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
      z80_movGen(state, &(z80->e), (uint8_t *)&(z80->pc), 
                 z80_srcIsAddr | z80_srcInc);
      break;

    case zRRA: // 0x1F
      z80_rr(state, &(z80->a), 0);
      z80->dt = 4;
      z80->f &= CARRY_FLAG;
      break;

    case zJRNZ_r8: // 0x20
      z80_jp(state, 1, (z80->f & ZERO_FLAG) == 0);
      break;

    case zLD_hl_d16: // 0x21
      z80_loadShort(state, &(z80->hl));
      break;

    case zLD_HLI_a: // 0x22
      z80_movGen(state, (uint8_t *)&(z80->hl), &(z80->a), 
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
      z80_movGen(state, &(z80->h), (uint8_t *)&(z80->pc),
                 z80_srcIsAddr | z80_srcInc);
      break;

    case zJRZ_r8: // 0x28
      z80_jp(state, 1, z80->f & ZERO_FLAG);
      break;

    case zADD_hl_hl: // 0x29
      z80_addShort(state, &(z80->hl), z80->hl);
      break;

    case zLD_a_HLI: // 0x2A
      z80_movGen(state, &(z80->a), (uint8_t *)&(z80->hl), 
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
      z80_movGen(state, &(z80->l), (uint8_t *)&(z80->pc), 
                 z80_srcIsAddr | z80_srcInc);
      break;

    case zCPL: // 0x2F
      z80->a = ~z80->a;
      z80->dt = 4;
      break;

    case zJRNC_r8: // 0x30
      z80_jp(state, 1, (z80->f & CARRY_FLAG) == 0);
      break;

    case zLD_sp_d16: // 0x31
      z80_loadShort(state, &(z80->sp));
      break;

    case zLD_HLD_a: // 0x32
      z80_movGen(state, (uint8_t *)&(z80->hl), &(z80->a),
                z80_srcIsAddr | z80_srcDec);
      break;

    case zINC_sp: // 0x33
      z80_incShort(state, &(z80->sp));
      break;

    case zINC_HL: // 0x34
      z80_incByte(state, (uint8_t *)&(z80->hl), z80_dstIsAddr);
      break;

    case zDEC_HL: // 0x35
      z80_decByte(state, (uint8_t *)&(z80->hl), z80_dstIsAddr);
      break;

    case zLD_HL_d8: // 0x36
      z80_movGen(state, (uint8_t *)&(z80->hl), (uint8_t *)&(z80->pc),
                 z80_srcIsAddr | z80_srcInc | z80_dstIsAddr);
      break;

    case zSCF: // 0x37
      z80->f = (z80->f & ZERO_FLAG) | CARRY_FLAG;
      z80->dt = 4;
      break;

    case zJRC_r8: // 0x38
      z80_jp(state, 1, z80->f & CARRY_FLAG);
      break;

    case zADD_hl_sp: // 0x39
      z80_addShort(state, &(z80->hl), z80->sp);
      break;

    case zLD_a_HLD: // 0x3A
      z80_movGen(state, &(z80->a), (uint8_t *)&(z80->hl), 
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
      z80_movGen(state, &(z80->a), (uint8_t *)&(z80->pc), 
                 z80_srcIsAddr | z80_srcInc);
      break;

    case zCCF: // 0x3F
      z80->f ^= CARRY_FLAG;
      z80->f &= (CARRY_FLAG | ZERO_FLAG);
      z80->dt = 4;
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
      z80_movGen(state, &(z80->b), (uint8_t *)&(z80->hl), 
                 z80_srcIsAddr);
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
      z80_movGen(state, &(z80->c), (uint8_t *)&(z80->hl),
                 z80_srcIsAddr);
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
      z80_movGen(state, &(z80->d), (uint8_t *)&(z80->hl),
                 z80_srcIsAddr);
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
      z80_movGen(state, &(z80->e), (uint8_t *)&(z80->hl),
                 z80_srcIsAddr);
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
      z80_movGen(state, &(z80->h), (uint8_t *)&(z80->hl),
                 z80_srcIsAddr);
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
      z80_movGen(state, &(z80->l), (uint8_t *)&(z80->hl), 
                 z80_srcIsAddr);
      break;

    case zLD_l_a:   // 0x6F
      z80_mov(state, &(z80->l), z80->a);
      break;

    case zLD_HL_b:   // 0x70
      z80_movGen(state, (uint8_t *)&(z80->hl), &(z80->b),
                 z80_dstIsAddr);
      break;

    case zLD_HL_c:   // 0x71
      z80_movGen(state, (uint8_t *)&(z80->hl), &(z80->c),
                 z80_dstIsAddr);
      break;

    case zLD_HL_d:   // 0x72
      z80_movGen(state, (uint8_t *)&(z80->hl), &(z80->d),
                 z80_dstIsAddr);
      break;

    case zLD_HL_e:   // 0x73
      z80_movGen(state, (uint8_t *)&(z80->hl), &(z80->e), 
                 z80_dstIsAddr);
      break;

    case zLD_HL_h:   // 0x74
      z80_movGen(state, (uint8_t *)&(z80->hl), &(z80->h),
                 z80_dstIsAddr);
      break;

    case zLD_HL_l:   // 0x75
      z80_movGen(state, (uint8_t *)&(z80->hl), &(z80->l),
                 z80_dstIsAddr);
      break;

    case zHALT:  // 0x76
      //TODO Halt Instruction
      break;

    case zLD_HL_a:   // 0x77
      z80_movGen(state, (uint8_t *)&(z80->hl), &(z80->a),
                 z80_dstIsAddr);
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
      z80_movGen(state, &(z80->a), (uint8_t *)&(z80->hl),
                 z80_srcIsAddr);
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
      z80_addByte(state, &(z80->a), (uint8_t *)&(z80->hl), z80_srcIsAddr);
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
      z80_addByte(state, &(z80->a), (uint8_t *)&(z80->hl), 
                  z80_srcIsAddr | z80_withCarry);
      break;

    case zADC_a_a: // 0x8F
      z80_addByte(state, &(z80->a), &(z80->a), z80_withCarry);
      break;
      
    case zSUB_b: // 0x90
      z80_subByte(state, &(z80->a), (uint8_t *)&(z80->b), 0);
      break;

    case zSUB_c: // 0x91
      z80_subByte(state, &(z80->a), &(z80->c), 0);
      break;

    case zSUB_d: // 0x92
      z80_subByte(state, &(z80->a), &(z80->d), 0);
      break;

    case zSUB_e: // 0x93
      z80_subByte(state, &(z80->a), &(z80->e), 0);
      break;

    case zSUB_h: // 0x94
      z80_subByte(state, &(z80->a), &(z80->h), 0);
      break;

    case zSUB_l: // 0x95
      z80_subByte(state, &(z80->a), &(z80->l), 0);
      break;

    case zSUB_HL: // 0x96
      z80_subByte(state, &(z80->a), (uint8_t *)&(z80->hl), 
                  z80_srcIsAddr);
      break;

    case zSUB_a: // 0x97
      z80_subByte(state, &(z80->a), &(z80->a), 0);
      break;

    case zSBC_a_b: // 0x98
      z80_subByte(state, &(z80->a), &(z80->b), z80_withCarry);
      break;

    case zSBC_a_c: // 0x99
      z80_subByte(state, &(z80->a), &(z80->c), z80_withCarry);
      break;

    case zSBC_a_d: // 0x9A
      z80_subByte(state, &(z80->a), &(z80->d), z80_withCarry);
      break;

    case zSBC_a_e: // 0x9B
      z80_subByte(state, &(z80->a), &(z80->e), z80_withCarry);
      break;

    case zSBC_a_h: // 0x9C
      z80_subByte(state, &(z80->a), &(z80->h), z80_withCarry);
      break;

    case zSBC_a_l: // 0x9D
      z80_subByte(state, &(z80->a), &(z80->l), z80_withCarry);
      break;

    case zSBC_a_HL: // 0x9E
      z80_subByte(state, &(z80->a), (uint8_t *)&(z80->hl), 
                  z80_srcIsAddr | z80_withCarry);
      break;

    case zSBC_a_a: // 0x9F
      z80_subByte(state, &(z80->a), &(z80->a), z80_withCarry);
      break;

    case zAND_b: // 0xA0
      z80_and(state, &(z80->b), 0);
      break;

    case zAND_c: // 0xA1
      z80_and(state, &(z80->c), 0);
      break;

    case zAND_d: // 0xA2
      z80_and(state, &(z80->d), 0);
      break;

    case zAND_e: // 0xA3
      z80_and(state, &(z80->e), 0);
      break;

    case zAND_h: // 0xA4
      z80_and(state, &(z80->h), 0);
      break;

    case zAND_l: // 0xA5
      z80_and(state, &(z80->l), 0);
      break;

    case zAND_HL: // 0xA6
      z80_and(state, (uint8_t *)&(z80->hl), z80_srcIsAddr);
      break;

    case zAND_a: // 0xA7
      z80_and(state, &(z80->a), 0);
      break;

    case zXOR_b: // 0xA8
      z80_xor(state, &(z80->b), 0);
      break;

    case zXOR_c: // 0xA9
      z80_xor(state, &(z80->c), 0);
      break;

    case zXOR_d: // 0xAA
      z80_xor(state, &(z80->d), 0);
      break;

    case zXOR_e: // 0xAB
      z80_xor(state, &(z80->e), 0);
      break;

    case zXOR_h: // 0xAC
      z80_xor(state, &(z80->h), 0);
      break;

    case zXOR_l: // 0xAD
      z80_xor(state, &(z80->l), 0);
      break;

    case zXOR_HL: // 0xAE
      z80_xor(state, (uint8_t *)&(z80->hl), z80_srcIsAddr);
      break;

    case zXOR_a: // 0xAF
      z80_xor(state, &(z80->a), 0);
      break;

    case zOR_b: // 0xB0
      z80_or(state, &(z80->b), 0);
      break;

    case zOR_c: // 0xB1
      z80_or(state, &(z80->c), 0);
      break;

    case zOR_d: // 0xB2
      z80_or(state, &(z80->d), 0);
      break;

    case zOR_e: // 0xB3
      z80_or(state, &(z80->e), 0);
      break;

    case zOR_h: // 0xB4
      z80_or(state, &(z80->h), 0);
      break;

    case zOR_l: // 0xB5
      z80_or(state, &(z80->l), 0);
      break;

    case zOR_HL: // 0xB6
      z80_or(state, (uint8_t *)&(z80->hl), z80_srcIsAddr);
      break;

    case zOR_a: // 0xB7
      z80_or(state, &(z80->a), 0);
      break;

    case zCP_b: // 0xB8
      z80_cp(state, &(z80->b), 0);
      break;

    case zCP_c: // 0xB9
      z80_cp(state, &(z80->c), 0);
      break;

    case zCP_d: // 0xBA
      z80_cp(state, &(z80->d), 0);
      break;

    case zCP_e: // 0xBB
      z80_cp(state, &(z80->e), 0);
      break;

    case zCP_h: // 0xBC
      z80_cp(state, &(z80->h), 0);
      break;

    case zCP_l: // 0xBD
      z80_cp(state, &(z80->l), 0);
      break;

    case zCP_HL: // 0xBE
      z80_cp(state, (uint8_t *)&(z80->hl), z80_srcIsAddr);
      break;

    case zCP_a: // 0xBF
      z80_cp(state, &(z80->a), 0);
      break;

    case zRETNZ: // 0xC0
      z80_ret(state, !(z80->f & ZERO_FLAG));
      break;

    case zPOP_bc: // 0xC1
      z80_pop(state, &(z80->bc));
      break;


    case zJPNZ_a16: // 0xC2
      z80_jp(state, 2, !(z80->f & ZERO_FLAG));
      break;

    case zJP_a16: // 0xC2
      z80_jp(state, 2, 1);
      break;

    case zCALLNZ_a16: // 0xC4
      VALID_MEM_OP(mem_readShort(mem, z80->pc, &tempS));
      z80->pc += 2;
      z80_call(state, !(z80->f & ZERO_FLAG), tempS);
      break;

    case zPUSH_bc: // 0xC5
      z80_push(state, z80->bc);
      break;

    case zADD_a_d8: // 0xC6
      z80_addByte(state, &(z80->a), (uint8_t *)&(z80->pc), 
                  z80_srcIsAddr);
      z80->pc++;
      break;

    case zRST_00: // 0xC7
      z80_call(state, 1, 0x0000);
      z80->dt = 16;
      break;

    case zRETZ: // 0xC8
      z80_ret(state, z80->f & ZERO_FLAG);
      break;

    case zRET: // 0xC9
      z80_ret(state, 1);
      break;

    case zJPZ_a16: // 0xCA
      z80_jp(state, 2, z80->f & ZERO_FLAG);
      break;

    case 0xCB: // 0xCB
      z80_extension(z80, mem);
      break;

    case zCALLZ_a16: // 0xCC
      VALID_MEM_OP(mem_readShort(mem, z80->pc, &tempS));
      z80->pc += 2;
      z80_call(state, (z80->f & ZERO_FLAG), tempS);
      break;

    case zCALL_a16: // 0xCD
      VALID_MEM_OP(mem_readShort(mem, z80->pc, &tempS));
      z80->pc += 2;
      z80_call(state, 1, tempS);
      break;

    case zADC_a_d8: // 0xCE
      z80_addByte(state, &(z80->a), (uint8_t *)&(z80->pc), 
                  z80_srcIsAddr | z80_withCarry);
      z80->pc++;
      break;

    case zRST_08: // 0xCF
      z80_call(state, 1, 0x0008);
      z80->dt = 16;
      break;

    case zRETNC: // 0xD0
      z80_ret(state, !(z80->f & CARRY_FLAG));
      break;

    case zPOP_de: // 0xD1
      z80_pop(state, &(z80->de));
      break;

    case zJPNC_a16: // 0xD2
      z80_jp(state, 2, !(z80->f & CARRY_FLAG));
      break;

    case zCALLNC_a16: // 0xD4
      VALID_MEM_OP(mem_readShort(mem, z80->pc, &tempS));
      z80->pc += 2;
      z80_call(state, !(z80->f & CARRY_FLAG), tempS);
      break;

    case zPUSH_de: // 0xD5
      z80_push(state, z80->de);
      break;

    case zSUB_d8: // 0xD6
      z80_subByte(state, &(z80->a), (uint8_t *)&(z80->pc),
                  z80_srcIsAddr);
      z80->pc++;
      break;

    case zRST_10: // 0xD7
      z80_call(state, 1, 0x0010);
      z80->dt = 16;
      break;

    case zRETC: // 0xD8
      z80_ret(state, z80->f & CARRY_FLAG);
      break;

    case zJPC_a16: // 0xDA
      z80_jp(state, 2, z80->f & CARRY_FLAG);
      break;

    case zCALLC_a16: // 0xDC
      VALID_MEM_OP(mem_readShort(mem, z80->pc, &tempS));
      z80->pc += 2;
      z80_call(state, (z80->f & CARRY_FLAG), tempS);
      break;

    case zSBC_a_d8: // 0xDE
      z80_subByte(state, &(z80->a), (uint8_t *)&(z80->pc), 
                  z80_srcIsAddr | z80_withCarry);
      z80->pc++;
      break;

    case zRST_18: // 0xDF
      z80_call(state, 1, 0x0018);
      z80->dt = 16;
      break;

    case zLDH_a8_a: // 0xE0
      z80_movGen(state, (uint8_t *)&(z80->pc), &(z80->a), z80_dstPage0);
      break;

    case zPOP_hl: // 0xE1
      z80_pop(state, &(z80->hl));
      break;

    case zLD_C_a: // 0xE2
      z80_movGen(state, &(z80->c), &(z80->a), z80_dstPage0);
      break;

    case zPUSH_hl: // 0xE5
      z80_push(state, z80->hl);
      break;

    case zAND_d8: // 0xE6
      z80_and(state, (uint8_t *)&(z80->pc), z80_srcIsAddr);
      z80->pc++;
      break;

    case zRST_20: // 0xE7
      z80_call(state, 1, 0x0020);
      z80->dt = 16;
      break;

    case zADD_sp_r8: // 0xE8
      VALID_MEM_OP(mem_readByte(mem, z80->pc++, &tempB));
      z80->sp += (int8_t)tempB;
      z80->dt = 16;
      break;

    case zJP_HL: // 0xE9
      z80->pc = z80->hl;
      z80->dt = 4;
      break;

    case zLD_a16_a: // 0xEA
      VALID_MEM_OP(mem_readShort(mem, z80->pc, &tempS));
      z80->pc += 2;
      VALID_MEM_OP(mem_writeByte(mem, tempS, z80->a));
      z80->dt = 16;
      break;

    case zXOR_d8: // 0xEE
      z80_xor(state, (uint8_t *)&(z80->pc), 
              z80_srcIsAddr | z80_withCarry);
      z80->pc++;
      break;

    case zRST_28: // 0xEF
      z80_call(state, 1, 0x0028);
      z80->dt = 16;
      break;

    case zLDH_a_a8: // 0xF0
      VALID_MEM_OP(mem_readByte(mem, z80->pc++, &tempB));
      z80_movGen(state, &(z80->a), &tempB, z80_srcPage0);
      break;

    case zPOP_af: // 0xF1
      z80_pop(state, &(z80->af));
      break;

    case zLD_a_C: // 0xF2
      z80_movGen(state, &(z80->a), &(z80->c), z80_srcPage0);
      break;

    case zPUSH_af: // 0xF5
      z80_push(state, z80->af);
      break;

    case zOR_d8: // 0xF6
      z80_or(state, (uint8_t *)&(z80->pc), z80_srcIsAddr);
      z80->pc++;
      break;

    case zRST_30: // 0xF7
      z80_call(state, 1, 0x0030);
      z80->dt = 16;
      break;

    case zLD_hl_sp_r8:
      VALID_MEM_OP(mem_readByte(mem, z80->pc++, &tempB));
      z80->hl = z80->sp + (int8_t) tempB;
      z80->dt = 12;
      break;

    case zLD_sp_hl: // 0xF9
      z80->sp = z80->hl;
      z80->dt = 8;
      break;

    case zLD_a_a16: // 0xFA
      VALID_MEM_OP(mem_readShort(mem, z80->pc, &tempS));
      z80->pc += 2;
      VALID_MEM_OP(mem_readByte(mem, tempS, &(z80->a)));
      z80->dt = 16;
      break;

    case zCP_d8: // 0xFE
      z80_cp(state, (uint8_t *)&(z80->pc), 
             z80_srcIsAddr | z80_withCarry);
      z80->pc++;
      break;

    case zRST_38: // 0xFF
      z80_call(state, 1, 0x0038);
      z80->dt = 16;
      break;


    default:
      DEBUG("Opcode not implemented\r\n");
      return -1;
      break;
  }
  return 0;
}


