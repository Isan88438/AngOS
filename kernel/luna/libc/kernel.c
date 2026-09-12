#include "stdlib.h"

static unsigned long next = 1;

int rand(void) {
    next = next * 1103515245 + 12345;
    return (unsigned int)(next / 65536) % (RAND_MAX + 1);
}

void srand(unsigned int seed) {
    next = seed;
}