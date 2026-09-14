#include "stdlib.h"

static unsigned long next = 1;

int rand(void) {
    next = next * 1103515245 + 12345;
    return (unsigned int)(next / 65536) % ((unsigned int)RAND_MAX + 1u);
}

void srand(unsigned int seed) {
    next = seed;
}

char *itoa(int value, char *str, int base)
{
    static const char digits[] =
        "0123456789abcdefghijklmnopqrstuvwxyz";

    char *ptr = str;
    char *start;
    unsigned int magnitude;

    if (base < 2 || base > 36) {
        *str = '\0';
        return str;
    }

    if (value < 0 && base == 10) {
        *ptr++ = '-';
        magnitude = (unsigned int)(-(value + 1)) + 1;
    } else {
        magnitude = (unsigned int)value;
    }

    start = ptr;

    do {
        *ptr++ = digits[magnitude % (unsigned int)base];
        magnitude /= (unsigned int)base;
    } while (magnitude);

    *ptr = '\0';

    for (--ptr; start < ptr; ++start, --ptr) {
        char tmp = *start;
        *start = *ptr;
        *ptr = tmp;
    }

    return str;
}