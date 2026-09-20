#include <types.h>
#include <stdbool.h>
#include <cpu/IO.h>
#include <cpu/IDT.h>
#include <PIC.h>
#include <terminal.h>
#include <keyboard.h>

static bool shift = false;
static bool caps_lock = false;

static const char keymap[128] = {
    [0x02] = '1',
    [0x03] = '2',
    [0x04] = '3',
    [0x05] = '4',
    [0x06] = '5',
    [0x07] = '6',
    [0x08] = '7',
    [0x09] = '8',
    [0x0A] = '9',
    [0x0B] = '0',

    [0x10] = 'q',
    [0x11] = 'w',
    [0x12] = 'e',
    [0x13] = 'r',
    [0x14] = 't',
    [0x15] = 'y',
    [0x16] = 'u',
    [0x17] = 'i',
    [0x18] = 'o',
    [0x19] = 'p',

    [0x1E] = 'a',
    [0x1F] = 's',
    [0x20] = 'd',
    [0x21] = 'f',
    [0x22] = 'g',
    [0x23] = 'h',
    [0x24] = 'j',
    [0x25] = 'k',
    [0x26] = 'l',

    [0x2C] = 'z',
    [0x2D] = 'x',
    [0x2E] = 'c',
    [0x2F] = 'v',
    [0x30] = 'b',
    [0x31] = 'n',
    [0x32] = 'm',

    [0x39] = ' ',
};

static char shifted_key(u8 scancode)
{
    switch (scancode) {
        case 0x02: return '!';
        case 0x03: return '@';
        case 0x04: return '#';
        case 0x05: return '$';
        case 0x06: return '%';
        case 0x07: return '^';
        case 0x08: return '&';
        case 0x09: return '*';
        case 0x0A: return '(';
        case 0x0B: return ')';

        case 0x0C: return '_';
        case 0x0D: return '+';

        case 0x1A: return '{';
        case 0x1B: return '}';

        case 0x2B: return '|';

        default:
            return 0;
    }
}

static bool is_letter_scancode(u8 scancode)
{
    return
        (scancode >= 0x10 && scancode <= 0x19) ||
        (scancode >= 0x1E && scancode <= 0x26) ||
        (scancode >= 0x2C && scancode <= 0x32);
}

static void keyboard_interrupt(void)
{
    u8 status = inb(0x64);

    if (!(status & 1)) {
        return;
    }

    u8 scancode = inb(0x60);

    /* Key release */
    if (scancode & 0x80) {
        u8 released = scancode & 0x7F;

        if (released == 0x2A || released == 0x36) {
            shift = false;
        }

        piceoi(false);
        return;
    }

    /* Shift */
    if (scancode == 0x2A || scancode == 0x36) {
        shift = true;
        piceoi(false);
        return;
    }

    /* Caps Lock */
    if (scancode == 0x3A) {
        caps_lock = !caps_lock;
        piceoi(false);
        return;
    }

    /* Enter */
    if (scancode == 0x1C) {
        terminal_putchar('\n');
        piceoi(false);
        return;
    }

    /* Backspace */
    if (scancode == 0x0E) {
        terminal_putchar('\b');
        piceoi(false);
        return;
    }

    char c = 0;

    /* Shifted symbols */
    if (shift) {
        c = shifted_key(scancode);
    }

    /* Normal key */
    if (c == 0 && scancode < 128) {
        c = keymap[scancode];
    }

    if (c != 0) {
        if (is_letter_scancode(scancode)) {
            /*
             * Letter case:
             * shift XOR caps = uppercase
             */
            if (shift ^ caps_lock) {
                c = (char)(c - 'a' + 'A');
            }
        } else if (shift) {
            char shifted = shifted_key(scancode);
            if (shifted != 0) {
                c = shifted;
            }
        }

        terminal_putchar(c);
    }

    piceoi(false);
}

void keyboard_init(void)
{
    addirq(0x21, (void *)keyboard_interrupt, 0x8E);
    unmaskirq(1);

    terminal_write("KEYBOARD OK\n");
}