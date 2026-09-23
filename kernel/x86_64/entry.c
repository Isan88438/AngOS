#include <types.h>
#include <tosaithe.h>

extern void init(tosaithe_loader_data *loader_data);

static u8 kernel_stack[16384] __attribute__((aligned(16)));

__attribute__((section(".tsbp"), used))
const tosaithe_entry_header angos_tsbp_header = {
    .signature = 0x50425354,
    .version = 1,
    .min_reqd_version = 1,
    .flags = 0,
    .stack_ptr = (uintptr_t)(kernel_stack + sizeof(kernel_stack))
};

void angos_entry(tosaithe_loader_data *loader_data) {
    init(loader_data);

    for (;;) {
        asm volatile ("hlt");
    }
}

[bits 64]
global irq1_stub
extern keyboard_handler

irq1_stub:
    push rbx
    push rcx
    push rdx
    push rbp
    push rdi
    push rsi
    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15

    call keyboard_handler

    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8
    pop rsi
    pop rdi
    pop rbp
    pop rdx
    pop rcx
    pop rbx
    iretq