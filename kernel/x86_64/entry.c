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
