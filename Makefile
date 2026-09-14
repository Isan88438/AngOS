CC := gcc
AS := gcc
LD := ld

CFLAGS := -m64 -ffreestanding -fno-stack-protector -mgeneral-regs-only -Wall -Wextra \
          -Ikernel/include \
          -Ikernel/x86_64

ASFLAGS := -m64 -ffreestanding

BUILD := build

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
    kernel/term/terminal.c \

KERNEL_ASM := \
    kernel/x86_64/cpu/GDT/GDT.S \
    kernel/x86_64/cpu/ints/timer/PIT.S

KERNEL_OBJS := \
    $(KERNEL_C:kernel/%.c=$(BUILD)/%.o) \
    $(KERNEL_ASM:kernel/%.S=$(BUILD)/%.o)

.PHONY: all clean

KERNEL := build/AngOS.elf

.PHONY: all clean

all: $(KERNEL)
	@echo "Kernel linked successfully: $(KERNEL)"

$(BUILD)/%.o: kernel/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/%.o: kernel/%.S
	@mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD)
