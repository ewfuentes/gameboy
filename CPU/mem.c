#include "mem.h"
#include <stdio.h>
#include <stdlib.h>
#include "interface/bios.h"

#define GBC_RAM_SIZE 0x8000
#define GBC_GFX_RAM_SIZE 0x2000

void mem_ramInit(uint8_t **ram, uint32_t *ramSize) {
  *ram = (uint8_t *)malloc(GBC_RAM_SIZE);
  *ramSize = GBC_RAM_SIZE;
  printf("Allocating RAM\r\n");
  printf("Allocated 0x%X bytes at 0x%llX\r\n", 
         *ramSize, (uint64_t)*ram);
}

void mem_gfxRamInit(uint8_t **gfxRam, uint32_t *ramSize) {
  *gfxRam = (uint8_t *)malloc(GBC_GFX_RAM_SIZE);
  *ramSize = GBC_GFX_RAM_SIZE;
  printf("Allocating Video RAM\r\n");
  printf("Allocated 0x%X bytes at 0x%llX\r\n", 
         *ramSize, (uint64_t)*gfxRam);
}

int8_t mem_init(mem_t *mem) {
  bios_load(&(mem->bios), &(mem->biosSize));
  mem_ramInit(&(mem->ram), &(mem->ramSize));
  mem_gfxRamInit(&(mem->gfxRam), &(mem->gfxRamSize));
  return 0;
}

int8_t mem_readByte(mem_t *mem, uint16_t location, uint8_t *data) {
  switch (0xF000 & location) {
    case 0x0000:
    case 0x1000:
    case 0x2000:
    case 0x3000:
      //read from bank 0 or from bios
      if (location < mem->biosSize) {
        *data = mem->bios[location];
        return 0;
      }
      while(1);
      break;
    
    case 0x4000:
    case 0x5000:
    case 0x6000:
    case 0x7000:
      //read from other bank
      break;

    case 0x8000:
    case 0x9000:
      //Graphics RAM
      *data = mem->gfxRam[location-0x8000];
      return 0;
      break;

    case 0xA000:
    case 0xB000:
      //Cartridge RAM
      break;

    case 0xC000:
    case 0xD000:
      //Working RAM
      *data = mem->ram[location - 0xC000];
      return 0;
      break;

    case 0xE000:
    case 0xF000:
      //Working RAM shadow
      //Graphics: Sprite information
      //Memory Mapped I/O
      //Zero Page RAM
      if (location > 0xFF80){
        *data = mem->ram[location];
      }
      return 0;
      break;
  }
  printf("Failed to read from: %x\r\n", location);
  return -1;
}

int8_t mem_writeByte(mem_t *mem, uint16_t location, uint8_t data) {
  switch (0xF000 & location) {
    case 0x0000:
    case 0x1000:
    case 0x2000:
    case 0x3000:
      //read from bank 0 or from bios
      if (location < mem->biosSize) {
        mem->bios[location] = data;
        return 0;
      }
      break;
    
    case 0x4000:
    case 0x5000:
    case 0x6000:
    case 0x7000:
      //read from other bank
      break;

    case 0x8000:
    case 0x9000:
      //Graphics RAM
      mem->gfxRam[location-0x8000] = data;
      return 0;
      break;

    case 0xA000:
    case 0xB000:
      //Cartridge RAM
      break;

    case 0xC000:
    case 0xD000:
      //Working RAM
      mem->ram[location - 0xC000] = data;
      return 0;
      break;

    case 0xE000:
    case 0xF000: 
      //Working RAM shadow
      //Graphics: Sprite information
      //Memory Mapped I/O
      //Zero Page RAM
      if (location > 0xFF80){
        mem->ram[location] = data;
      }
      return 0;    
      break;
  }
  printf("Failed to write to: %x\r\n", location);
  return -1;
}

int8_t mem_getPointer(mem_t *mem, uint16_t location, uint8_t **ptr) {
    switch (0xF000 & location) {
      case 0x0000:
      case 0x1000:
      case 0x2000:
      case 0x3000:
        //read from bank 0 or from bios
        if (location < mem->biosSize) {
          *ptr = &(mem->bios[location]);
          return 0;
        }
        break;
      
      case 0x4000:
      case 0x5000:
      case 0x6000:
      case 0x7000:
        //read from other bank
        break;

      case 0x8000:
      case 0x9000:
        //Graphics RAM
        *ptr = &(mem->gfxRam[location-0x8000]);
        return 0;
        break;

      case 0xA000:
      case 0xB000:
        //Cartridge RAM
        break;

      case 0xC000:
      case 0xD000:
        //Working RAM
        *ptr = &(mem->ram[location - 0xC000]);
        return 0;
        break;

      case 0xE000:
      case 0xF000: 
        //Working RAM shadow
        //Graphics: Sprite information
        //Memory Mapped I/O
        //Zero Page RAM
        if (location > 0xFF80){
          *ptr = &(mem->ram[location]);
        }
        return 0;    
        break;
  }
  printf("Failed to get pointer for: %x\r\n", location);
  return -1;
}

int8_t mem_readShort(mem_t *mem, uint16_t location, uint16_t *data) {
    uint8_t temp[] = {0,0};
    int8_t result = mem_readByte(mem, location, &temp[0]);
    if (result < 0) {
        goto fail;
    }
    result = mem_readByte(mem, location + 1, &temp[1]);
    if (result < 0) {
        goto fail;
    }

    *data = temp[0] | temp[1] << 8;
    return 0;
    fail:
        return -1;

}

int8_t mem_writeShort(mem_t *mem, uint16_t location, uint16_t data) {
    int8_t result = mem_writeByte(mem, location, data & 0x00FF);
    if (result < 0) {
        goto fail;
    }

    result = mem_writeByte(mem, location + 1, data>>8);
    if (result < 0) {
        goto fail;
    }

    return 0;

    fail: 
        return -1;

}

void mem_destroy(mem_t *mem) {
    free(mem->bios);
    mem->biosSize = 0;
    free(mem->gfxRam);
    mem->gfxRamSize = 0;

}

