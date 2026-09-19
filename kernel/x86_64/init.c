#include <stdlib.h>
#include <cpu/GDT.h>
#include <cpu/IO.h>
#include <cpu/IDT.h>
#include <cpu/PIT.h>
#include <memory.h>
#include <types.h>
#include "init.h"
#include <syscall.h>
#include <tosaithe.h>
#include <terminal.h>

void init(tosaithe_loader_data *loader_data) {
    fb_addr = loader_data->framebuffer_addr;
    fb_size = loader_data->framebuffer_size;
    fb_w = loader_data->framebuffer_width;
    fb_h = loader_data->framebuffer_height;
    fb_pixperline = loader_data->framebuffer_pitch;

    terminal_init();

    terminal_write("ANGOS KERNEL STARTED\n");
    terminal_write("INITIALIZING HARDWARE\n");

    init_gdt();
    terminal_write("GDT OK\n");

    init_idt();
    terminal_write("IDT OK\n");

    init_timer();
    terminal_write("PIT OK\n");

    terminal_write("TERMINAL OK\n");
    terminal_write("ANGOS IS RUNNING\n");
}
