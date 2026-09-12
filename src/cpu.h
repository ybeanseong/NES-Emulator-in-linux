#ifndef CPU_H
#define CPU_H

#include <stdint.h>
#include "bus.h"

#define FLAG_C 0x01
#define FLAG_Z 0x02
#define FLAG_I 0x04
#define FLAG_D 0x08
#define FLAG_B 0x10
#define FLAG_U 0x20
#define FLAG_V 0x40
#define FLAG_N 0x80


typedef enum
{
    CPU_TRAP_NONE = 0,
    CPU_TRAP_INVALID_OPCODE,
    CPU_TRAP_INVALID_MEMORY_ACCESS
} CpuTrap;

typedef struct
{
    uint16_t pc;
    uint8_t  sp;

    uint8_t  a;
    uint8_t  x;
    uint8_t  y;

    uint8_t  status;
    
    CpuTrap trap;
    uint16_t trap_pc;
    uint8_t trap_opcode;

    Bus *bus;

} Cpu6502;

typedef enum
{
    CPU_RESULT_OK = 0,
    CPU_RESULT_TRAP
} CpuResult;

extern void cpu_init(Cpu6502 *cpu, Bus *bus);
extern void cpu_reset(Cpu6502 *cpu);
extern CpuResult cpu_step(Cpu6502 *cpu);

#endif
