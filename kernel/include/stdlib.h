#ifndef KERNEL_STDLIB_H
#define KERNEL_STDLIB_H

#include <types.h>

#define RAND_MAX 2147483647

void *malloc(size_t size);
void free(void *ptr);

char *itoa(int value, char *str, int base);

#endif