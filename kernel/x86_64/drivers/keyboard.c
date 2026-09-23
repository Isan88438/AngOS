#include <keyboard.h>
#include <cpu/IO.h>
#include <terminal.h>

static bool shift_pressed = false;
static bool caps_lock = false;

static const char scancode_map[] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', ' = ', '\b',
  '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
     0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
     0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ', ', '.', '/', 0,
   '*',   0, ' '
};

static const char scancode_map_shift[] = {
    0,  27, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b',
  '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',
     0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~',
     0, '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0,
   '*',   0, ' '
};

void keyboard_init(void) {

    uint8_t mask = inb(0x21);
    outb(0x21, mask & ~(1 << 1));
}

void keyboard_handler(void) {
    uint8_t scancode = inb(KEYBOARD_DATA_PORT);

    if (scancode & 0x80) {
        uint8_t released_code = scancode & 0x7F;
        if (released_code == 0x2A || released_code == 0x36) {
            shift_pressed = false;
        }
    } else {

        if (scancode == 0x2A || scancode == 0x36) {
            shift_pressed = true;
        } else if (scancode == 0x3A) {
            caps_lock = !caps_lock;
        } else if (scancode < sizeof(scancode_map)) {
            bool use_shift = shift_pressed ^ caps_lock;
            char ch = use_shift ? scancode_map_shift[scancode] : scancode_map[scancode];

            if (ch != 0) {
                terminal_putchar(ch);
            }
        }
    }

    outb(0x20, 0x20);
}
