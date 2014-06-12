#ifndef Z80_INSTR_H
#define Z80_INSTR_H

#include "cpu/z80.h"

typedef enum {
    z80_fail = -1,
    z80_ok
} z80_status;

typedef enum {
    zNOP,               //0x00
    zLD_bc_d16,
    zLD_BC_a,
    zINC_bc,
    zINC_b,
    zDEC_b,
    zLD_b_d8,
    zRLCA,
    zLD_a16_sp,
    zADD_hl_bc,
    zLD_a_BC,
    zDEC_bc,
    zINC_c,
    zDEC_c,
    zLD_c_d8,
    zRCCA,
    zSTOP,              //0x10
    zLD_de_d16,
    zLD_DE_a,
    zINC_de,
    zINC_d,
    zDEC_d,
    zLD_d_d8,
    zRLA,
    zJR_r8,
    zADD_hl_de,
    zLD_a_DE,
    zDEC_de,
    zINC_e,
    zDEC_e,
    zLD_e_d8,
    zRRA,
    zJRNZ_r8,           //0x20
    zLD_hl_d16,
    zLD_HLI_a,
    zINC_hl,
    zINC_h,
    zDEC_h,
    zLD_h_d8,
    zDAA,
    zJRZ_r8,
    zADD_hl_hl,
    zLD_a_HLI,
    zDEC_hl,
    zINC_l,
    zDEC_l,
    zLD_l_d8,
    zCPL,
    zJRNC_r8,           //0x30
    zLD_sp_d16,
    zLD_HLD_a,
    zINC_sp,
    zINC_HL,
    zDEC_HL,
    zLD_HL_d8,
    zSCF,
    zJRC_r8,
    zADD_hl_sp,
    zLD_a_HLD,
    zDEC_sp,
    zINC_a,
    zDEC_a,
    zLD_a_d8,
    zCCF,
    zLD_b_b,            //0x40
    zLD_b_c,
    zLD_b_d,
    zLD_b_e,
    zLD_b_h,
    zLD_b_l,
    zLD_b_HL,
    zLD_b_a,
    zLD_c_b,
    zLD_c_c,
    zLD_c_d,
    zLD_c_e,
    zLD_c_h,
    zLD_c_l,
    zLD_c_HL,
    zLD_c_a,
    zLD_d_b,            //0x50
    zLD_d_c,
    zLD_d_d,
    zLD_d_e,
    zLD_d_h,
    zLD_d_l,
    zLD_d_HL,
    zLD_d_a,
    zLD_e_b,
    zLD_e_c,
    zLD_e_d,
    zLD_e_e,
    zLD_e_h,
    zLD_e_l,
    zLD_e_HL,
    zLD_e_a,
    zLD_h_b,            //0x60
    zLD_h_c,
    zLD_h_d,
    zLD_h_e,
    zLD_h_h,
    zLD_h_HL,
    zLD_h_a,
    zLD_l_b,
    zLD_l_c,
    zLD_l_d,
    zLD_l_e,
    zLD_l_h,
    zLD_l_l,
    zLD_l_HL,
    zLD_l_a,
    zLD_HL_b,           //0x70
    zLD_HL_c,
    zLD_HL_d,
    zLD_HL_e,
    zLD_HL_h,
    zLD_HL_l,
    zHALT,
    zLD_HL_a,
    zLD_a_b,
    zLD_a_c,
    zLD_a_d,
    zLD_a_e,
    zLD_a_h,
    zLD_a_l,
    zLD_a_HL,
    zLD_a_a,
    zADD_a_b,           //0x80
    zADD_a_c,
    zADD_a_d,
    zADD_a_e,
    zADD_a_h,
    zADD_a_l,
    zADD_a_HL,
    zADD_a_a,
    zADC_a_b,
    zADC_a_c,
    zADC_a_d,
    zADC_a_e,
    zADC_a_h,
    zADC_a_l,
    zADC_a_HL,
    zADC_a_a,
    zSUB_b,             //0x90
    zSUB_c,
    zSUB_d,
    zSUB_e,
    zSUB_h,
    zSUB_l,
    zSUB_HL,
    zSUB_a,
    zSBC_a_b,
    zSBC_a_c,
    zSBC_a_d,
    zSBC_a_e,
    zSBC_a_h,
    zSBC_a_l,
    zSBC_a_HL,
    zSBC_a_a,
    zAND_b,             //0xA0
    zAND_c,
    zAND_d,
    zAND_e,
    zAND_h,
    zAND_l,
    zAND_HL,
    zAND_a,
    zXOR_b,
    zXOR_c,
    zXOR_d,
    zXOR_e,
    zXOR_h,
    zXOR_l,
    zXOR_HL,
    zXOR_a,
    zOR_b,              //0xB0
    zOR_c,
    zOR_d,
    zOR_e,
    zOR_h,
    zOR_l,
    zOR_HL,
    zOR_a,
    zCP_b,
    zCP_c,
    zCP_d,
    zCP_e,
    zCP_h,
    zCP_l,
    zCP_HL,
    zCP_a,
    zRETNZ,             //0xC0
    zPOP_bc,
    zJPNZ_a16,
    zJP_a16,
    zCALLNZ_a16,
    zPUSH_bc,
    zADD_a_d8,
    zRST_00,
    zRETZ,
    zRET,
    zJPZ_a16,
    zEXT,
    zCALLZ_a16,
    zCALL_a16,
    zADC_a_d8,
    zRST_08,
    zRETNC,             //0xD0
    zPOP_de,
    zJPNC_a16,
    zCALLNC_a16 = 0xD4,
    zPUSH_de,
    zSUB_d8,
    zRST_10,
    zRETC,
    zRETI,
    zJPC_a16,
    zCALLC_a16 = 0xDC,
    zSBC_a_d8 = 0xDE,
    zRST_18,
    zLDH_a8_a,          //0xE0
    zPOP_hl,
    zLD_C_a,
    zPUSH_hl = 0xE5,
    zAND_d8,
    zRST_20,
    zADD_sp_r8,
    zJP_HL,
    zLD_a16_a,
    zXOR_d8,
    zRST_28,
    zLDH_a_a8,          //0xF0
    zPOP_af,
    zLD_a_C,
    zDI,
    zPUSH_af = 0xF5,
    zOR_d8,
    zRST_30,
    zLD_hl_sp_r8,
    zLD_sp_hl,
    zLD_a_a16,
    zEI,
    zCP_d8 = 0xFE,
    zRST_38
} z80_instr;

typedef enum {
    zRLC_b,             //0x00
    zRLC_c,
    zRLC_d,
    zRLC_e,
    zRLC_h,
    zRLC_l,
    zRLC_HL,
    zRLC_a,
    zRRC_b,
    zRRC_c,
    zRRC_d,
    zRRC_e,
    zRRC_h,
    zRRC_l,
    zRRC_HL,
    zRRC_a,
    zRL_b,              //0x10
    zRL_c,
    zRL_d,
    zRL_e,
    zRL_h,
    zRL_l,
    zRL_HL,
    zRL_a,
    zRR_b,
    zRR_c,
    zRR_d,
    zRR_e,
    zRR_h,
    zRR_l,
    zRR_HL,
    zRR_a,
    zSLA_b,             //0x20
    zSLA_c,
    zSLA_d,
    zSLA_e,
    zSLA_h,
    zSLA_l,
    zSLA_HL,
    zSLA_a,
    zSRA_b,
    zSRA_c,
    zSRA_d,
    zSRA_e,
    zSRA_h,
    zSRA_l,
    zSRA_HL,
    zSRA_a,
    zSWAP_b,            //0x30
    zSWAP_c,
    zSWAP_d,
    zSWAP_e,
    zSWAP_h,
    zSWAP_l,
    zSWAP_HL,
    zSWAP_a,
    zSRL_b,
    zSRL_c,
    zSRL_d,
    zSRL_e,
    zSRL_h,
    zSRL_l,
    zSRL_HL,
    zSRL_a,
    zBIT0_b,            //0x40
    zBIT0_c,
    zBIT0_d,
    zBIT0_e,
    zBIT0_h,
    zBIT0_l,
    zBIT0_HL,
    zBIT0_a,
    zBIT1_b,
    zBIT1_c,
    zBIT1_d,
    zBIT1_e,
    zBIT1_h,
    zBIT1_l,
    zBIT1_HL,
    zBIT1_a,
    zBIT2_b,            //0x50
    zBIT2_c,
    zBIT2_d,
    zBIT2_e,
    zBIT2_h,
    zBIT2_l,
    zBIT2_HL,
    zBIT2_a,
    zBIT3_b,
    zBIT3_c,
    zBIT3_d,
    zBIT3_e,
    zBIT3_h,
    zBIT3_l,
    zBIT3_HL,
    zBIT3_a,
    zBIT4_b,            //0x60
    zBIT4_c,
    zBIT4_d,
    zBIT4_e,
    zBIT4_h,
    zBIT4_l,
    zBIT4_HL,
    zBIT4_a,
    zBIT5_b,
    zBIT5_c,
    zBIT5_d,
    zBIT5_e,
    zBIT5_h,
    zBIT5_l,
    zBIT5_HL,
    zBIT5_a,
    zBIT6_b,            //0x70
    zBIT6_c,
    zBIT6_d,
    zBIT6_e,
    zBIT6_h,
    zBIT6_l,
    zBIT6_HL,
    zBIT6_a,
    zBIT7_b,            
    zBIT7_c,
    zBIT7_d,
    zBIT7_e,
    zBIT7_h,
    zBIT7_l,
    zBIT7_HL,
    zBIT7_a,
    zRES0_b,            //0x80
    zRES0_c,
    zRES0_d,
    zRES0_e,
    zRES0_h,
    zRES0_l,
    zRES0_HL,
    zRES0_a,
    zRES1_b,
    zRES1_c,
    zRES1_d,
    zRES1_e,
    zRES1_h,
    zRES1_l,
    zRES1_HL,
    zRES1_a,
    zRES2_b,            //0x90
    zRES2_c,
    zRES2_d,
    zRES2_e,
    zRES2_h,
    zRES2_l,
    zRES2_HL,
    zRES2_a,
    zRES3_b,
    zRES3_c,
    zRES3_d,
    zRES3_e,
    zRES3_h,
    zRES3_l,
    zRES3_HL,
    zRES3_a,
    zRES4_b,            //0xA0
    zRES4_c,
    zRES4_d,
    zRES4_e,
    zRES4_h,
    zRES4_l,
    zRES4_HL,
    zRES4_a,
    zRES5_b,
    zRES5_c,
    zRES5_d,
    zRES5_e,
    zRES5_h,
    zRES5_l,
    zRES5_HL,
    zRES5_a,
    zRES6_b,            //0xB0
    zRES6_c,
    zRES6_d,
    zRES6_e,
    zRES6_h,
    zRES6_l,
    zRES6_HL,
    zRES6_a,
    zRES7_b,            
    zRES7_c,
    zRES7_d,
    zRES7_e,
    zRES7_h,
    zRES7_l,
    zRES7_HL,
    zRES7_a,
    zSET0_b,            //0xC0
    zSET0_c,
    zSET0_d,
    zSET0_e,
    zSET0_h,
    zSET0_l,
    zSET0_HL,
    zSET0_a,
    zSET1_b,
    zSET1_c,
    zSET1_d,
    zSET1_e,
    zSET1_h,
    zSET1_l,
    zSET1_HL,
    zSET1_a,
    zSET2_b,            //0xD0
    zSET2_c,
    zSET2_d,
    zSET2_e,
    zSET2_h,
    zSET2_l,
    zSET2_HL,
    zSET2_a,
    zSET3_b,
    zSET3_c,
    zSET3_d,
    zSET3_e,
    zSET3_h,
    zSET3_l,
    zSET3_HL,
    zSET3_a,
    zSET4_b,            //0xE0
    zSET4_c,
    zSET4_d,
    zSET4_e,
    zSET4_h,
    zSET4_l,
    zSET4_HL,
    zSET4_a,
    zSET5_b,
    zSET5_c,
    zSET5_d,
    zSET5_e,
    zSET5_h,
    zSET5_l,
    zSET5_HL,
    zSET5_a,
    zSET6_b,            //0xF0
    zSET6_c,
    zSET6_d,
    zSET6_e,
    zSET6_h,
    zSET6_l,
    zSET6_HL,
    zSET6_a,
    zSET7_b,            
    zSET7_c,
    zSET7_d,
    zSET7_e,
    zSET7_h,
    zSET7_l,
    zSET7_HL,
    zSET7_a,
} z80_extInstr;

z80_status z80_nop(z80_t *z80, mem_t *mem);

//This function will load the next two bytes at the program counter
//and store it into the specified register.
//Takes care of instructions:
// 0x01, 0x11, 0x21, 0x31
z80_status z80_loadShort(z80_t *z80, mem_t *mem, uint16_t *reg);

#endif