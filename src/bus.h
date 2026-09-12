#ifndef BUS_H
#define BUS_H

#include <stdint.h>
#include "cartridge.h"

typedef struct
{
    uint8_t ram[2048];

    Cartridge *cartridge;

} Bus;

void bus_init(Bus *bus, Cartridge *cartridge);

uint8_t bus_read(Bus *bus, uint16_t addr);

void bus_write(Bus *bus,
               uint16_t addr,
               uint8_t data);

#endif
