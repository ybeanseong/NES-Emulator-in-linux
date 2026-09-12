#include "bus.h"

#include <stdio.h>
#include <string.h>

void bus_init(Bus *bus, Cartridge *cartridge)
{
    memset(bus->ram, 0, sizeof(bus->ram));

    bus->cartridge = cartridge;
}

uint8_t bus_read(Bus *bus, uint16_t addr)
{
    /* Internal RAM */
    if (addr <= 0x1FFF)
    {
        return bus->ram[addr & 0x07FF];
    }

    /* PPU Registers */
    if (addr >= 0x2000 && addr <= 0x3FFF)
    {
        /* 아직 PPU 구현 안 함 */
        return 0;
    }

    /* Cartridge PRG ROM */
    if (addr >= 0x8000)
    {
        uint32_t mapped_addr;

        if (bus->cartridge->prg_rom_size == 16384)
        {
            mapped_addr = addr & 0x3FFF;
        }
        else
        {
            mapped_addr = addr & 0x7FFF;
        }

        return bus->cartridge->prg_rom[mapped_addr];
    }

    return 0;
}

void bus_write(Bus *bus,
               uint16_t addr,
               uint8_t data)
{
    /* Internal RAM */
    if (addr <= 0x1FFF)
    {
        bus->ram[addr & 0x07FF] = data;
        return;
    }

    /* PPU Registers */
    if (addr >= 0x2000 && addr <= 0x3FFF)
    {
        /* 나중에 구현 */
        return;
    }

    /*
     * 0x8000 이상은 현재 Mapper 0 PRG ROM이므로
     * 일반적인 RAM처럼 쓰지 않는다.
     */
}
