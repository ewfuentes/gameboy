#include "bios.h"
#include <stdio.h>
#include <stdlib.h>

#define BIOS_FILE "gbc_bios.bin"

int32_t bios_load(uint8_t **bufPtr, uint32_t *size){
    FILE *biosFile = fopen(BIOS_FILE, "r");
    if ( biosFile == NULL) {
        printf("Couldn't open file\r\n");
        return -1;
    }
    //Seek to the end of the file
    if (fseek(biosFile, 0, SEEK_END)) {
        fclose(biosFile);
        return -1;
    }

    uint32_t position = ftell(biosFile);
    printf("The file is %x bytes long\r\n", position);
    *size = position;

    fseek(biosFile, 0, SEEK_SET);
    *bufPtr = malloc(*size);
    if (*bufPtr == NULL) {
        printf("Could not allocate space for the bios");
        fclose(biosFile);
        return -1;
    }

    fread(*bufPtr, sizeof(uint8_t), *size, biosFile);

    fclose(biosFile);
    return 0;
}