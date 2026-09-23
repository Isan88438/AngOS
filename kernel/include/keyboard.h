#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <types.h>
#include <stddef.h>
#include <stdbool.h>

#define KEYBOARD_DATA_PORT   0x60
#define KEYBOARD_STATUS_PORT 0x64
#define KBD_BUFFER_SIZE      256

void keyboard_init(void);
void keyboard_handler(void);
bool keyboard_has_char(void);
char keyboard_getchar(void);
void keyboard_gets(char *buf, size_t max_len);

__attribute__((interrupt)) void irq1_stub(void *frame);

#endif