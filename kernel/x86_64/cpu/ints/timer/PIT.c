#include <types.h>
#include <cpu/IDT.h>
#include <cpu/IO.h>

#define CHAN0 0x40
#define CMD   0x43
#define HZ    1000

volatile u64 timerticks = 0;

__attribute__((interrupt))
static void pitirq(void *frame)
{
    (void)frame;

    ++timerticks;
    piceoi(false);
}

void init_timer(void)
{
    u32 div = 1193180 / HZ;

    outb(CMD, 0x36);
    outb(CHAN0, div & 0xFF);
    outb(CHAN0, (div >> 8) & 0xFF);

    addirq(0x20, pitirq, 0x8E);
    unmaskirq(0);
}

void sleep(unsigned long ticks)
{
    u64 end = timerticks + ticks;

    while (timerticks < end)
        __asm__ volatile("hlt");
}