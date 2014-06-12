#include "cpu/z80.h"

#include <stdio.h>

#include "cpu/mem.h"

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
        case 0x10: // rotate left 1 bit register b
                // the bit that gets shifted out moves to the carry
                // flag, the LSB becomes the value of the carry flag
            printf("9 bit rotate B\r\n");
            z80_RLreg(z80, &(z80->b));
            break;

        case 0x13: // rotate left 1 bit register e
            printf("9 bit rotate E\r\n");
            z80_RLreg(z80, &(z80->e));
            break;

        case 0x37: // Swap High and low nibble of A
            z80->a = (z80->a << 4) | (z80->a >> 4);
            printf("Swap nibbles in a \r\n");
            break;

        case 0x41: // Test 0th bit of c
            printf("Testing bit %d of %s\r\n",0, "c");
            z80_testBit(z80, z80->c, 0);
            break;

        case 0x49: // test 1st bit of C
            printf("Testing bit %d of %s\r\n",1, "c");
            z80_testBit(z80, z80->c, 1);
            break;

        case 0x6C: // test 5th bit of H
            printf("Testing bit %d of %s\r\n",5, "h");
            z80_testBit(z80, z80->h, 5);
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
    memTest(mem_readByte(mem, z80->pc++, &opcode));
    
    printf("Read Opcode: %x\r\n",opcode);
    uint16_t tempS;
    uint8_t tempB;
    switch (opcode) {
        case 0x01: //load the next short into BC
            memTest(mem_readShort(mem, z80->pc, &tempS));
            printf("Loading %x into bc \r\n", tempS);
            z80->pc += 2;
            z80->bc = tempS;

        case 0x05: // Decrement B
            printf("Decrement B\r\n");
            z80->b--;
            if (z80->b == 0) {
                z80->f |= ZERO_FLAG;
            }else {
                z80->f &= ~ZERO_FLAG;
            }
            z80->f |= SUB_FLAG;
            break;

        case 0x06: // Load next byte into B
            memTest(mem_readByte(mem, z80->pc++, &tempB));
            printf("Loading %x into B\r\n", tempB);
            z80->b = tempB;
            break;

        case 0x09: // Add BC to HL
            z80->hl += z80->bc;
            //TODO deal with half carry
            z80->f &= ZERO_FLAG;
            if ((uint32_t)(z80->bc + z80->hl) > 0x0000FFFF) {
                z80->f |= CARRY_FLAG; 
            }

            printf("Add bc to hl\r\n");
            break;

        case 0x0C: // increment C
            printf("Increment C\r\n");
            z80->c++;
            if (z80->c == 0) {
                z80->f |= ZERO_FLAG;
            } else {
                z80->f &= ~ZERO_FLAG;
            }
            z80->f &= ~SUB_FLAG;
            break;

        case 0x0D: // decrement C and check if zero
            printf("Decrement C\r\n");
            z80->c--;
            if (z80->c == 0) {
                z80->f |= ZERO_FLAG;
            }else {
                z80->f &= ~ZERO_FLAG;
            }
            z80->f |= SUB_FLAG;
            break;

        case 0x0E: // load a byte into C
            memTest(mem_readByte(mem, z80->pc++, &tempB));
            

            z80->c = tempB;
            printf("Load C with: %x\r\n", tempB);
            break;

        case 0x11: // Load the next short into DE
            memTest(mem_readShort(mem, z80->pc, &tempS));
            
            z80->pc += 2;

            z80->de = tempS;
            printf("Setting DE to %x\r\n", tempS);
            break;

        case 0x12: // Load A into the byte at DE
            memTest(mem_writeByte(mem, z80->de, z80->a));
            printf("Load a into byte at de\n");
            break;

        case 0x13: // Increment DE
            z80->de++;
            printf("Increment DE\r\n");
            break;

        case 0x15: //Decrement D
            z80->d--;
            if (z80->d == 0) {
                z80->f |= ZERO_FLAG;
            }else {
                z80->f &= ~ZERO_FLAG;
            }
            z80->f |= SUB_FLAG;
            break;

        case 0x16: // Load the next byte into D
            memTest(mem_readByte(mem, z80->pc++, &tempB));
            
            z80->d = tempB;
            printf("Setting D to: %x", tempB);
            break;

        case 0x17: // RL A don't affect zero flag
            z80_RLreg(z80, &(z80->a));
            z80->f &= ~ZERO_FLAG;
            printf("Rotate Right A\r\n");
            break;

         case 0x19: //Add DE to HL
            z80->hl += z80->de;
            //TODO deal with half carry
            z80->f &= ZERO_FLAG;
            if ((uint32_t)(z80->de + z80->hl) > 0x0000FFFF) {
                z80->f |= CARRY_FLAG; 
            }

            printf("Add de to hl\r\n");
            break;

        case 0x1A: // load the byte at DE into A
            memTest(mem_readByte(mem, z80->de, &tempB));
            
            z80->a = tempB;
            printf("Loading byte at %x into A\r\n", z80->de);
            break;

        case 0x1E: // load a byte into E
            memTest(mem_readByte(mem, z80->pc++, &tempB));
            z80->e = tempB;
            printf("Load E with: %x\r\n", tempB);
            break;

        case 0x20: //adjust the PC by the next byte if the Z flag is
                    //not set
            memTest(mem_readByte(mem, z80->pc++, &tempB));
            
            if (z80->f & ZERO_FLAG) {
                printf("Zero flag set, no jump\r\n");
            }else {
                z80->pc += (int8_t) tempB;
                printf("Zero flag not set.");
                printf(" Adjusting PC by %x\r\n", tempB);
            }
            break;

        case 0x21: //Load the next two bytes into HL
            memTest(mem_readShort(mem, z80->pc, &tempS));
            
            z80->pc += 2;
            z80->hl = tempS;
            printf("Load HL with: %x\r\n", tempS);
            break;

        case 0x22: // Load A into the addres at HL and increment HL
            memTest(mem_writeByte(mem, z80->hl++, z80->a));
            
            printf("Load A into address at HL and increment HL\r\n");
            return 0;
            break;

        case 0x23: // Increment HL
            z80->hl++;
            printf("Increment HL\r\n");
            break;

        case 0x26: //Load the next byte into H
            memTest(mem_readByte(mem, z80->pc++, &tempB));
            
            z80->h = tempB;
            printf("Setting H to: %x\r\n", tempB);
            break;

        case 0x28: //Adjust PC by next byte if Z flag is set
            memTest(mem_readByte(mem, z80->pc++, &tempB));
            
            if (z80->f & ZERO_FLAG) {
                printf("Zero flag is set, ");
                printf("adjusting PC by %x\r\n", tempB);
                z80->pc += (int8_t) tempB;
            }
            break;

        case 0x2A: // Load byte at HL into A and increment HL
            memTest(mem_readByte(mem, z80->hl++, &tempB));
            z80->a = tempB;
            printf("Loading %x into a\r\n", tempB);
            break;

        case 0x2E: // Load next byte into L
            memTest(mem_readByte(mem, z80->pc++, &tempB));
            z80->l = tempB;
            printf("Load %x into L\r\n", tempB);
            break;

        case 0x2F: //invert bits of A
                    //sets Half Carry and Subtract flag
            z80->a ^= 0xFF;
            z80->f |= SUB_FLAG | HALF_FLAG;
            printf("Invert A\r\n");
            break;

        case 0x31: //Load stack pointer with the next short
            memTest(mem_readShort(mem, z80->pc, &tempS));
            
            z80->sp = tempS;
            z80->pc += 2;
            printf("Setting Stack pointer to: %x\r\n", tempS);
            break;

        case 0x3C: //Increment A
            printf("Increment A\r\n");
            z80->a++;
            if (z80->a == 0) {
                z80->f |= ZERO_FLAG;
            } else {
                z80->f &= ~ZERO_FLAG;
            }
            z80->f &= ~SUB_FLAG;

            break;

        case 0x3E: //Load A with the next byte
            memTest(mem_readByte(mem, z80->pc++, &tempB));
            
            z80->a = tempB;
            printf("Loading %x into A\r\n", tempB);
            break;

        case 0x43: // move D into B
            z80->b = z80->d;
            printf("Move D into A\r\n");
            break;

        case 0x47: // move A into B
            z80->b = z80->a;
            printf("Move B into A\r\n");
            break;

        case 0x4A: // move D into C
            z80->c = z80->d;
            printf("Move D into C\r\n");
            break;

        case 0x4C: // move H into C
            z80->c = z80->h;
            printf("Move H into C\r\n");
            break;

        case 0x58: // Load B into E
            z80->e = z80->b;
            printf("Move B into E\r\n");
            break;

        case 0x79: //Load C into A
            z80->a = z80->c;
            printf("Load C into A\r\n");
            break;

        case 0x7B: // load E into A
            z80->a = z80->e;
            printf("Load E into A\r\n");
            break;

        case 0x7E: // load the byte at HL into A
            memTest(mem_readByte(mem, z80->hl, &tempB));
            z80->a = tempB;
            printf("Load byte at HL into A\r\n");
            break;

        case 0xAF:
            z80->a ^= z80->a;
            if (z80->a == 0) {
                z80->f = ZERO_FLAG;
            } else {
                z80->f = 0;
            }
            printf("XORing A with A\r\n");
            break;

        case 0xB0: // OR b with a
            printf("a OR b");
            z80->f = 0;
            if ((z80->a | z80->b) == 0) {
                z80->f = ZERO_FLAG;
            }

            break;

        case 0xC1: // Pop BC from the stack
            printf("popping BC from stack\r\n");
            memTest(mem_readShort(mem, z80->sp, &tempS));

            z80->bc = tempS;
            z80->sp += 2;
            break;

        case 0xC3: //Jump if Z flag is not set 
            //temp S contains the jump address if the condition is 
            //satisfied
            memTest(mem_readShort(mem, z80->pc, &tempS));
            
            z80->pc += 2;
            if (z80->f & ZERO_FLAG) {
                //do nothing
            } else {
                //jump to the address
                z80->pc = tempS;    
                printf("Z flag not set, jumping to %x\r\n", tempS);
            } 
            break;

        case 0xC5: // Push BC onto the stack
            z80->sp -= 2;
            mem_writeShort(mem, z80->sp, z80->bc);
            printf("push bc onto the stack\r\n");
            break;

        case 0xC9: //return pop two bytes off of the stack set the pc
            memTest(mem_readShort(mem, z80->sp, &tempS));
            
            z80->sp += 2;
            z80->pc = tempS;
            printf("Returning to: %x\r\n", tempS);
            break;

        case 0xCB: // Instruction Extension OpCode
            return z80_extension(z80, mem);
            break;

        case 0xCD: // Call a function at an address
            //tempS contains the address which we should jump to
            memTest(mem_readShort(mem, z80->pc, &tempS));
            
            z80->pc += 2;

            //push the program counter onto the stack
            z80->sp -= 2;
            mem_writeShort(mem, z80->sp, z80->pc);
            //Set the program counter to the address we read
            z80->pc = tempS;
            printf("Calling function at:%x\r\n", tempS);
            break;

        case 0xD1: // Pop DE from the stack
            printf("popping DE from stack\r\n");
            memTest(mem_readShort(mem, z80->sp, &tempS));

            z80->de = tempS;
            z80->sp += 2;
            break;

        case 0xD5: // Push DE onto the stack
            z80->sp -= 2;
            mem_writeShort(mem, z80->sp, z80->de);
            printf("push DE onto the stack\r\n");
            break;

        case 0xE0: //Load page zero byte with contents of A
            memTest(mem_readByte(mem, z80->pc++, &tempB));
            
            tempS = 0xFF00 | tempB; // location we need to write to
            mem_writeByte(mem, tempS, z80->a);
            printf("Writing %x to %x \r\n",z80->a, tempS);
            break;

        case 0xE1: // Pop HL from the stack
            printf("popping HL from stack\r\n");
            memTest(mem_readShort(mem, z80->sp, &tempS));

            z80->hl = tempS;
            z80->sp += 2;
            break;

        case 0xE2: // Load byte in A at 0xFF00 + C
            memTest(mem_writeByte(mem, 0xFF00 + z80->c, z80->a));
            
            printf("Writing A to page zero at C\r\n");
            break;

        case 0xE5: // Push HL onto the stack
            z80->sp -= 2;
            mem_writeShort(mem, z80->sp, z80->hl);
            printf("push bc onto the stack\r\n");
            break;

        case 0xE6: // And next byte with A
            memTest(mem_readByte(mem, z80->pc++, &tempB));
            printf("Anding A with %x\r\n", tempB);

            z80->f = HALF_FLAG;
            if ((z80->a & tempB) == 0) {
                z80->f |= ZERO_FLAG;
            }
            break;

        case 0xFE: // compare A against the next byte
            memTest(mem_readByte(mem, z80->pc++, &tempB));

            tempB = z80->a - tempB;
            printf("Comparing %x against A\r\n", tempB);
            if (tempB == 0) {
                z80->f = ZERO_FLAG | SUB_FLAG;
            } else {
                z80->f = SUB_FLAG;
            }

            //TODO handle carry flag
            break;
            

        default:
            printf("Opcode not implemented\r\n");
            return -1;
            break;
    }

    return 0;
}


