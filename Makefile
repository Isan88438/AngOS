CC := gcc
AS := gcc
LD := ld

CFLAGS := -m64 -ffreestanding -fno-stack-protector -mgeneral-regs-only -Wall -Wextra \
          -Ikernel/include \
          -Ikernel/x86_64

ASFLAGS := -m64 -ffreestanding

BUILD := build

KERNEL_C := \
    kernel/x86_64/cpu/GDT/GDT.c \
    kernel/x86_64/cpu/ints/IDT.c \
    kernel/x86_64/cpu/ints/PIC.c \
    kernel/x86_64/cpu/ints/timer/PIT.c \
    kernel/x86_64/memory/paging.c \
    kernel/x86_64/syscall/syscall.c \
    kernel/x86_64/syscall/debug_log.c

KERNEL_ASM := \
    kernel/x86_64/cpu/GDT/GDT.S

KERNEL_OBJS := \
    $(KERNEL_C:kernel/%.c=$(BUILD)/%.o) \
    $(KERNEL_ASM:kernel/%.S=$(BUILD)/%.o)

.PHONY: all clean

all: $(KERNEL_OBJS)
	@echo "Kernel objects compiled successfully."

$(BUILD)/%.o: kernel/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/%.o: kernel/%.S
	@mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD)
