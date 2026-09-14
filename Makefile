CC := gcc
AS := gcc
LD := ld

CFLAGS := -m64 -ffreestanding -fno-stack-protector -mgeneral-regs-only \
          -fno-asynchronous-unwind-tables -fno-unwind-tables \
          -Wall -Wextra \
          -Ikernel/include \
          -Ikernel/x86_64

ASFLAGS := -m64 -ffreestanding

BUILD := build
KERNEL := $(BUILD)/AngOS.elf

KERNEL_C := \
    kernel/x86_64/init.c \
    kernel/x86_64/entry.c \
    kernel/x86_64/cpu/GDT/GDT.c \
    kernel/x86_64/cpu/ints/IDT.c \
    kernel/x86_64/cpu/ints/PIC.c \
    kernel/x86_64/cpu/ints/timer/PIT.c \
    kernel/x86_64/cpu/IO.c \
    kernel/x86_64/memory/paging.c \
    kernel/x86_64/syscall/syscall.c \
    kernel/x86_64/syscall/debug_log.c \
    kernel/memory/memory.c \
    kernel/term/terminal.c

KERNEL_ASM := \
    kernel/x86_64/cpu/GDT/GDT.S \
    kernel/x86_64/cpu/ints/timer/PIT.S \
    kernel/x86_64/memory/paging.asm

KERNEL_C_OBJS := $(KERNEL_C:kernel/%.c=$(BUILD)/%.o)

KERNEL_ASM_OBJS := \
    $(patsubst kernel/%.S,$(BUILD)/%_asm.o,$(KERNEL_ASM)) \
    $(patsubst kernel/%.asm,$(BUILD)/%_asm.o,$(KERNEL_ASM))

KERNEL_OBJS := \
    $(KERNEL_C_OBJS) \
    $(KERNEL_ASM_OBJS)

.PHONY: all clean

all: $(KERNEL)
	@echo "Kernel linked successfully: $(KERNEL)"

$(KERNEL): $(KERNEL_OBJS) kernel/linker.ld
	@mkdir -p $(dir $@)
	$(LD) -nostdlib -T kernel/linker.ld $(KERNEL_OBJS) -o $@

$(BUILD)/%.o: kernel/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/%_asm.o: kernel/%.S
	@mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) -c $< -o $@

$(BUILD)/%_asm.o: kernel/%.asm
	@mkdir -p $(dir $@)
	nasm -f elf64 $< -o $@

clean:
	rm -rf $(BUILD)