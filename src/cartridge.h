#ifndef CARTRIDGE_H
#define CARTRIDGE_H

#include <stdint.h>
#include <stddef.h>

typedef struct
{
    uint8_t *prg_rom;
    uint8_t *chr_rom;

    size_t prg_rom_size;
    size_t chr_rom_size;

    uint8_t mapper;
    uint8_t mirroring;

} Cartridge;

int cartridge_load(Cartridge *cart, const char *filename);
void cartridge_free(Cartridge *cart);

#endif
