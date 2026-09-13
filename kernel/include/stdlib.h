#ifndef KERNEL_STDLIB_H
#define KERNEL_STDLIB_H

#include <types.h>

void *malloc(size_t size);
void free(void *ptr);

char *itoa(int value, char *str, int base);

#endif