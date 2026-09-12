#include "cpu.h"


/*function list */
void cpu_init(Cpu6502 *cpu, Bus *bus);
void cpu_reset(Cpu6502 *cpu);
CpuResult cpu_step(Cpu6502 *cpu);

/*Internal function */
static void cpu_update_zn(Cpu6502 *cpu, uint8_t value);
static void cpu_raise_trap(Cpu6502 *cpu,CpuTrap trap,uint16_t pc,uint8_t opcode);
static uint8_t cpu_fetch8(Cpu6502 *cpu);

void cpu_init(Cpu6502 *cpu, Bus *bus)
{
    cpu->pc = 0;
    cpu->sp = 0;

    cpu->a = 0;
    cpu->x = 0;
    cpu->y = 0;

    cpu->status = 0;

    cpu->bus = bus;
}

void cpu_reset(Cpu6502 *cpu)
{
    uint8_t lo;
    uint8_t hi;

    lo = bus_read(cpu->bus, 0xFFFC);
    hi = bus_read(cpu->bus, 0xFFFD);

    cpu->pc = ((uint16_t)hi << 8) | lo;

    cpu->sp = 0xFD;
    cpu->status = 0x24;
}

CpuResult cpu_step(Cpu6502 *cpu)
{
    uint8_t opcode;
    uint16_t opcode_pc = cpu->pc;
    opcode = bus_read(cpu->bus, cpu->pc);

    cpu->pc++;

    switch (opcode)
    {
        /* opcode 처리 */
	case 0xA9 :
	{
           /* 0xA9 : LDA Immediate*/
           /* Insert A register of value*/
	   uint8_t value;
	   value = cpu_fetch8(cpu);
	   cpu -> a = value;
	   cpu_update_zn(cpu, cpu->a);
	   break;
	}
	case 0x85 :
	{  uint8_t addr;
	   /*0x85 : STA Zero page*/
	   /*Read 1 byte data*/
	   addr = cpu_fetch8(cpu);	
	   bus_write(cpu->bus,addr,cpu->a);
	   break;
	   
	}
	case 0x8D :
	{
	   /* 0x8D : STA Absolute Page*/
	   /* Read 2byte address */
	   uint8_t lo;
	   uint8_t hi;
	   uint16_t addr;

	   lo = cpu_fetch8(cpu);
	   hi = cpu_fetch8(cpu);

	   addr =((uint16_t)hi <<8)|lo;
	   bus_write(cpu->bus,addr,cpu->a);
	   break;

	}
	case 0xA2 :
	{  /*0xA2 : LDX */
	   /*Insert x register of value*/
           uint8_t value;

	   value = cpu_fetch8(cpu);
	   cpu->x = value;
	   cpu_update_zn(cpu,cpu->x);
           break;

	}
	case 0xE8:
	{
	   /*INX : add x register*/
       	   cpu->x++;
	   cpu_update_zn(cpu,cpu->x);
	   break;
	}
	case 0x4C :
	{
	   /*0x4C JMP : Jump to specific address*/
	   uint8_t lo;
	   uint8_t hi;
	   uint16_t target;
	   
	   lo = cpu_fetch8(cpu);
	   hi = cpu_fetch8(cpu);
	   
	   target = ((uint16_t)hi << 8) | lo;
	   
	   cpu->pc = target;
	   
	   break;
	}
	case 0xF0 :
	{
	  /*0xF0 BEQ : If codition satisfied*/
	  /* Z : previous instruction trasfer status of zero*/
	  int8_t offset;
	  
	  offset = (int8_t)cpu_fetch8(cpu); /*relative address*/
	  
	  if((cpu->status & FLAG_Z) != 0)
	  {
	    cpu->pc = cpu->pc + offset;
	  }
	  
	  break;
	}
	case 0xD0 :
	{
	  int8_t offset;
	  /*0xF0 BEQ : If codition not satisfied*/
	  offset = (int_8)cpu_fetch8(cpu); /*relative address*/
	  
	  if ((cpu->status & FLAG_Z) == 0)
	  {
	     cpu->pc = cpu->pc + offset;
	  }
	}
	case 0xCA :
	{
	   /* 0xCA : Decrement X */
	   /* Decrese x register */
	   cpu-> x--;
	   cpu_update_zn(cpu,cpu->x);
	   break;
	}
	case 0xC9 :
	{
	   /*0xC9 : CMP */
	   uint8_t value;
	   uint8_t result;
	   
	   value = cpu_fetch8(cpu);
	   result = cpu->a - value;
	   
	   /* Zero flag */
	   if(cpu->a == value)
	   {
	      cpu->status |= FLAG_Z;
	   }
	   else
	   {
	      cpu->status &= ~FLAG_Z;
	   }
	   
	   /* Carry flag*/
	   if(cpu->a >= value)
	   {
	      cpu->status |= FLAG_C;
	   }
	   else
	   {
	      cpu->status &= ~FLAG_C;
	   }  
	   
	   /* Negative flag*/
	   if(result & 0x80)
	   {
	      cpu->status |= FLAG_N;
	   }
	   else
	   {
	      cpu->status &= ~FLAG_N;
	   }
	   
	   break;
	}
        default:
             /* Invalid OPCODE : Trap*/
            cpu_raise_trap(cpu,
                           CPU_TRAP_INVALID_OPCODE,
                           opcode_pc,
                           opcode);

            return CPU_RESULT_TRAP;
            break;
    }
    return CPU_RESULT_OK;
}

static void cpu_update_zn(Cpu6502 *cpu, uint8_t value)
{
    if (value == 0)
        cpu->status |= FLAG_Z;
    else
        cpu->status &= ~FLAG_Z;

    if (value & 0x80)
        cpu->status |= FLAG_N;
    else
        cpu->status &= ~FLAG_N;
}

static void cpu_raise_trap(Cpu6502 *cpu,
                           CpuTrap trap,
                           uint16_t pc,
                           uint8_t opcode)
{
    cpu->trap = trap;
    cpu->trap_pc = pc;
    cpu->trap_opcode = opcode;
}

static uint8_t cpu_fetch8(Cpu6502 *cpu)
{
    uint8_t value;

    value = bus_read(cpu->bus, cpu->pc);
    cpu->pc++;

    return value;
}

