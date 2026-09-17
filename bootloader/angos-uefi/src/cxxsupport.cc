#include <stddef.h>

#include "tosaithe-util.h"

/*
void debug_write(void *p)
{
    con_write((uint64_t)p);
}

void debug_write(const char16_t *msg)
{
    con_write((CHAR16 *)msg);
}
*/

extern "C"
void abort(void) {
    con_write(L"** Aborted! **\r\n");
    while (true) {
        asm volatile ("hlt\n");
    }
}

extern "C"
void *malloc(size_t size) {
    return alloc_pool(size);
}

extern "C"
void free(void *v) {
    return free_pool(v);
}


extern "C"
int memcmp(const void *a, const void *b, size_t n)
{
    const unsigned char *aa = (const unsigned char *)a;
    const unsigned char *bb = (const unsigned char *)b;

    for (size_t i = 0; i < n; ++i) {
        if (aa[i] < bb[i])
            return -1;
        if (aa[i] > bb[i])
            return 1;
    }

    return 0;
}

extern "C"
void *memcpy(void *dest, const void *src, size_t n)
{
    unsigned char *d = (unsigned char *)dest;
    const unsigned char *s = (const unsigned char *)src;

    for (size_t i = 0; i < n; ++i)
        d[i] = s[i];

    return dest;
}

extern "C"
void *memset(void *dest, int value, size_t n)
{
    unsigned char *d = (unsigned char *)dest;

    for (size_t i = 0; i < n; ++i)
        d[i] = (unsigned char)value;

    return dest;
}
