#include <keyboard.h>
#include <terminal.h>
#include <string.h>
#include <cpu/IO.h>

void execute_command(const char *cmd) {
    if (strcmp(cmd, "help") == 0) {
        terminal_writestring("AngOS Built-in Commands:\n");
        terminal_writestring("  help     - Show available commands\n");
        terminal_writestring("  clear    - Clear terminal screen\n");
        terminal_writestring("  ver      - Display OS kernel version\n");
        terminal_writestring("  reboot   - Reboot system\n");
    } else if (strcmp(cmd, "clear") == 0) {
        terminal_clear();
    } else if (strcmp(cmd, "ver") == 0) {
        terminal_writestring("AngOS v0.1.0 (x86_64 Architecture)\n");
    } else if (strcmp(cmd, "reboot") == 0) {
        terminal_writestring("Rebooting system...\n");
        outb(0x64, 0xFE); // Pulse CPU reset line via 8042 controller
    } else if (cmd[0] != '\0') {
        terminal_writestring("Unknown command. Type 'help' for commands.\n");
    }
}

void kernel_main(void) {
    char input_buf[128];

    terminal_writestring("\n=== AngOS Command Shell ===\n");
    terminal_writestring("Type 'help' to get started.\n\n");

    while (1) {
        terminal_writestring("angos> ");
        keyboard_gets(input_buf, sizeof(input_buf));
        execute_command(input_buf);
    }
}