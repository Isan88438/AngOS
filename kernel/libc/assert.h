#ifndef _KERNEL_ASSERT_H
#define _KERNEL_ASSERT_H

#include <stdio.h>  // For kernel panic function

// Define NDEBUG to disable assertions
#ifdef NDEBUG
#define assert(condition) ((void)0)
#else
#define assert(condition) \
    do { \
        if (!(condition)) { \
            printf("Assertion failed: %s, file %s, line %d\n", \
                   #condition, __FILE__, __LINE__); \
            panic(); \
        } \
    } while (0)
#endif // NDEBUG

#endif // _KERNEL_ASSERT_H