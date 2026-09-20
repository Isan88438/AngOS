#include <types.h>
#include <cpu/IO.h>
#include <cpu/IDT.h>
#include <terminal.h>

#define KEYBOARD_DATA   0x60
#define KEYBOARD_STATUS 0x64
#define KEYBOARD_IRQ    1

static bool shift_down;
static bool caps_lock;

static const char scancode[128] = {
    0,   27,  '1', '2', '3', '4', '5', '6',
    '7', '8',  '9', '0', '-', ' = ', '\b', '\t',

    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i',
    'o', 'p', '[', ']', '\n', 0,   'a', 's',

    'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',
    '\'', '`', 0,   '\\', 'z', 'x', 'c', 'v',

    'b', 'n', 'm', ', ', '.', '/', 0,   '*',
    0,   ' ',  0,   0,   0,   0,   0,   0,

    0,   0,    0,   0,   0,   0,   0,   '7',
    '8', '9',  '-', '4', '5', '6', '+', '1',
    '2', '3',  '0', '.', 0,   0,   0,   0,

    0,   0,    0,   0,   0,   0,   0,   0,
    0,   0,    0,   0,   0,   0,   0,   0
};

static const char shifted[128] = {
    0,   27,  '!', '@', '#', '$', '%', '^',
    '&', '*',  '(', ')', '_', '+', '\b', '\t',

    'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I',
    'O', 'P', '{', '}', '\n', 0,   'A', 'S',

    'D', 'F', 'G', 'H', 'J', 'K', 'L', ':',
    '"', '~', 0,   '|', 'Z', 'X', 'C', 'V',

    'B', 'N', 'M', '<', '>', '?', 0,   '*',
    0,   ' ',  0,   0,   0,   0,   0,   0,

    0,   0,    0,   0,   0,   0,   0,   '7',
    '8', '9',  '-', '4', '5', '6', '+', '1',
    '2', '3',  '0', '.', 0,   0,   0,   0,

    0,   0,    0,   0,   0,   0,   0,   0,
    0,   0,    0,   0,   0,   0,   0,   0
};

__attribute__((interrupt))
static void keyboard_interrupt(void *frame __attribute__((unused))) {
    u8 code = inb(KEYBOARD_DATA);

    static bool extended;
    if (code == 0xE0) {
        extended = true;
        piceoi(false);
        return;
    }

    if (extended) {
        extended = false;
        piceoi(false);
        return;
    }

    if (code & 0x80) {
        u8 released = code & 0x7F;

        if (released == 0x2A || released == 0x36)
            shift_down = false;

        piceoi(false);
        return;
    }

    if (code == 0x2A || code == 0x36) {
        shift_down = true;
        piceoi(false);
        return;
    }

    if (code == 0x3A) {
        caps_lock = !caps_lock;
        piceoi(false);
        return;
    }

    if (code < 128) {
        char c = shift_down ? shifted[code] : scancode[code];

        if (caps_lock &&
            ((c >= 'a' && c <= 'z') ||
             (c >= 'A' && c <= 'Z'))) {
            if (c >= 'a' && c <= 'z')
                c = (char)(c - 'a' + 'A');
            else
                c = (char)(c - 'A' + 'a');
        }

        if (c)
            terminal_putchar(c);
    }

    piceoi(false);
}

void keyboard_init(void) {

    addirq(0x21, (void *)keyboard_interrupt, 0x8E);

    unmaskirq(KEYBOARD_IRQ);

    terminal_write("KEYBOARD OK\n");
}
