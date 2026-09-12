#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "cpu.h"
#include "bus.h"

static void test_lda_immediate(void)
{
    Bus bus;
    Cpu6502 cpu;

    memset(&bus, 0, sizeof(bus));

    cpu_init(&cpu, &bus);

    /*
     * Test Program
     *
     * 0x0000 : A9   LDA Immediate
     * 0x0001 : 10   Value = 0x10
     */

    bus.ram[0x0000] = 0xA9;
    bus.ram[0x0001] = 0x10;

    cpu.pc = 0x0000;
    cpu.a  = 0x00;

    cpu_step(&cpu);

    assert(cpu.a == 0x10);
    assert(cpu.pc == 0x0002);

    printf("PASS: LDA Immediate\n");
}
static void test_inx(void)
{
   Bus bus;
   Cpu6502 cpu;
   CpuResult Result;

   memset(&bus, 0, sizeof(bus));
   cpu_init(&cpu, &bus);

   bus.ram[0x0000] = 0xE8;

   cpu.pc = 0x0000;
   cpu.x = 0x10;


   Result = cpu_step(&cpu);

   assert(Result == CPU_RESULT_OK);
   assert(cpu.x == 0x11);
   assert(cpu.pc == 0x0001);

   printf("PASS: INX\n");


}

static void test_inx_overflow(void)
{
   Bus bus;
   Cpu6502 cpu;
   CpuResult Result;

   memset(&bus, 0, sizeof(bus));
   cpu_init(&cpu, &bus);

   bus.ram[0x0000] = 0xE8;

   cpu.pc = 0x0000;
   cpu.x = 0xFF;


   Result = cpu_step(&cpu);

   assert(Result == CPU_RESULT_OK);
   assert(cpu.x == 0x00);
   assert(cpu.pc == 0x0001);
   assert((cpu.status & FLAG_Z) != 0);

   printf("PASS: INX OVF\n");


}
static void test_sta_zero_page(void)
{
    Bus bus;
    Cpu6502 cpu;

    memset(&bus, 0, sizeof(bus));

    cpu_init(&cpu, &bus);

    /* STA $10 */
    bus.ram[0x0000] = 0x8D;
    bus.ram[0x0001] = 0x12;
    bus.ram[0x0002] = 0x34;

    cpu.pc = 0x0000;
    cpu.a  = 0x55;

    CpuResult result = cpu_step(&cpu);

    assert(result == CPU_RESULT_OK);

    assert(bus_read(&bus, 0x0001) == 0x12);

    assert(cpu.a == 0x55);
    assert(cpu.pc == 0x0003);

    printf("PASS: STA Zero Page\n");
}

static void test_jmp_absolute(void)
{
    Bus bus;
    Cpu6502 cpu;

    memset(&bus, 0, sizeof(bus));
    cpu_init(&cpu, &bus);

    /*
     * JMP $1234
     */
    bus.ram[0x0000] = 0x4C;
    bus.ram[0x0001] = 0x34;
    bus.ram[0x0002] = 0x12;

    cpu.pc = 0x0000;

    CpuResult result = cpu_step(&cpu);

    assert(result == CPU_RESULT_OK);
    assert(cpu.pc == 0x1234);
}

static void test_beq_taken(void)
{
    Bus bus;
    Cpu6502 cpu;

    memset(&bus, 0, sizeof(bus));
    cpu_init(&cpu, &bus);

    /*
     * BEQ +5
     */
    bus.ram[0x0000] = 0xF0;
    bus.ram[0x0001] = 0x05;

    cpu.pc = 0x0000;

    /* Zero Flag ON */
    cpu.status |= FLAG_Z;

    CpuResult result = cpu_step(&cpu);

    assert(result == CPU_RESULT_OK);

    /*
     * opcode + operand를 읽은 뒤 PC = 0x0002
     * 거기에 +5
     */
    assert(cpu.pc == 0x0007);
}

static void test_beq_not_taken(void)
{
    Bus bus;
    Cpu6502 cpu;

    memset(&bus, 0, sizeof(bus));
    cpu_init(&cpu, &bus);

    bus.ram[0x0000] = 0xF0;
    bus.ram[0x0001] = 0x05;

    cpu.pc = 0x0000;

    /* Zero Flag OFF */
    cpu.status &= ~FLAG_Z;

    CpuResult result = cpu_step(&cpu);

    assert(result == CPU_RESULT_OK);

    assert(cpu.pc == 0x0002);
}
static void test_dex_zero_flag(void)
{
    Bus bus;
    Cpu6502 cpu;

    memset(&bus, 0, sizeof(bus));
    cpu_init(&cpu, &bus);

    bus.ram[0x0000] = 0xCA;

    cpu.pc = 0x0000;
    cpu.x = 0x01;

    CpuResult result = cpu_step(&cpu);

    assert(result == CPU_RESULT_OK);
    assert(cpu.x == 0x00);
    assert((cpu.status & FLAG_Z) != 0);
}
int main(void)
{
    test_lda_immediate();
    //test_lda_zero_flag();
    //test_lda_negative_flag();

    test_sta_zero_page();
    test_inx();
    test_inx_overflow();
    test_jmp_absolute();
    
    test_beq_taken();
    test_beq_not_taken();
    
    test_dex_zero_flag();
    printf("All CPU tests passed\n");

    return 0;
}
