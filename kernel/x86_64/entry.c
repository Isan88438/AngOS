#include <keyboard.h>
#include <terminal.h>
#include <string.h>
#include <stdio.h>

void execute_command(const char *cmd) {
    if (strcmp(cmd, "help") == 0) {
        printf("AngOS Built-in Commands:\n");
        printf("  help     - Show available commands\n");
        printf("  clear    - Clear terminal screen\n");
        printf("  ver      - Display OS kernel version\n");
        printf("  reboot   - Reboot system\n");
    } else if (strcmp(cmd, "clear") == 0) {
        terminal_clear();
    } else if (strcmp(cmd, "ver") == 0) {
        printf("AngOS v0.1.0 (x86_64 Architecture)\n");
    } else if (strcmp(cmd, "reboot") == 0) {
        printf("Rebooting system...\n");
        outb(0x64, 0xFE); // Pulse CPU reset line via 8042 controller
    } else if (cmd[0] != '\0') {
        printf("Unknown command: '%s'. Type 'help' for commands.\n", cmd);
    }
}

void kernel_main(void) {
    char input_buf[128];

    printf("\n=== AngOS Command Shell ===\n");
    printf("Type 'help' to get started.\n\n");

    while (1) {
        printf("angos> ");
        keyboard_gets(input_buf, sizeof(input_buf));
        execute_command(input_buf);
    }
}