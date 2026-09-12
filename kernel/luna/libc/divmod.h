#ifndef DIVMOD_H
#define DIVMOD_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Signed 64-bit division with remainder */
int64_t __divmoddi4(int64_t numerator, int64_t denominator, int64_t* remainder);

/* Unsigned 64-bit division with remainder */
uint64_t __udivmoddi4(uint64_t numerator, uint64_t denominator, uint64_t* remainder);

/* Unsigned 64-bit division */
uint64_t __udivdi3(uint64_t numerator, uint64_t denominator);

/* Unsigned 64-bit modulo */
uint64_t __umoddi3(uint64_t numerator, uint64_t denominator);

/* Signed 64-bit division */
int64_t __divdi3(int64_t numerator, int64_t denominator);

/* Signed 64-bit modulo */
int64_t __moddi3(int64_t numerator, int64_t denominator);

#ifdef __cplusplus
}
#endif

#endif /* DIVMOD_H */