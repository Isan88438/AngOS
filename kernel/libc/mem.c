#include <stddef.h>

void *memset(void *dest, int value, size_t count)
{
    unsigned char *ptr = dest;

    while (count--)
        *ptr++ = (unsigned char)value;

    return dest;
}