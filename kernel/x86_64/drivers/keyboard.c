#include <types.h>
#include <cpu/IO.h>
#include <cpu/IDT.h>
#include <terminal.h>
#include <keyboard.h>

/*
 * PS/2 keyboard, Set 1 scancodes.
 *
 * For this first version we only:
 *   - read IRQ1
 *   - receive scancodes
 *   - ignore key releases
 *   - translate basic printable keys
 *
 * Character translation will be expanded after the interrupt path
 * is confirmed stable.
 */

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

static void keyboard_interrupt(void)
{
    /*
     * Only read the keyboard data port when the output-buffer
     * status bit says data is available.
     */
    if (!(inb(0x64) & 0x01))
        return;

    u8 scancode = inb(0x60);

    /*
     * Ignore key releases for now.
     */
    if (scancode & 0x80)
        return;

    /*
     * Ignore extended keys for now.
     */
    if (scancode == 0xE0)
        return;

    if (scancode < 128) {
        char c = keymap[scancode];

        if (c != 0)
            terminal_putchar(c);
    }
}

void keyboard_init(void)
{
    /*
     * IRQ1 -> PIC vector 0x21.
     */
    addirq(0x21, (void *)keyboard_interrupt, 0x8E);
    unmaskirq(1);

    terminal_write("KEYBOARD OK\n");
}