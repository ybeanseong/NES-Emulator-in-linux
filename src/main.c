#include <stdio.h>

#include "cartridge.h"
#include "bus.h"
#include "cpu.h"

int main(void)
{
    Cartridge cart;
    Bus bus;
    Cpu6502 cpu;

    if (cartridge_load(&cart, "roms/nestest.nes") != 0)
    {
        return 1;
    }

    bus_init(&bus, &cart);

    cpu_init(&cpu, &bus);

    cpu_reset(&cpu);

    printf("CPU RESET\n");
    printf("PC     : 0x%04X\n", cpu.pc);
    printf("SP     : 0x%02X\n", cpu.sp);
    printf("STATUS : 0x%02X\n", cpu.status);

    cartridge_free(&cart);

    return 0;
}
