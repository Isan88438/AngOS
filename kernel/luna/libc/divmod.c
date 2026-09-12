#include "divmod.h"

/* Unsigned 64-bit division */
uint64_t __udivdi3(uint64_t numerator, uint64_t denominator)
{
    return __udivmoddi4(numerator, denominator, 0);
}

/* Unsigned 64-bit modulo */
uint64_t __umoddi3(uint64_t numerator, uint64_t denominator)
{
    uint64_t remainder;
    __udivmoddi4(numerator, denominator, &remainder);
    return remainder;
}

/* Signed 64-bit division */
int64_t __divdi3(int64_t numerator, int64_t denominator)
{
    return __divmoddi4(numerator, denominator, 0);
}

/* Signed 64-bit modulo */
int64_t __moddi3(int64_t numerator, int64_t denominator)
{
    int64_t remainder;
    __divmoddi4(numerator, denominator, &remainder);
    return remainder;
}


uint64_t __udivmoddi4(uint64_t numerator, uint64_t denominator, uint64_t* remainder) {
    uint64_t quotient = 0;
    uint64_t current = 1;
    uint64_t denominator_copy = denominator;
    
    if (denominator == 0) {
        /* Handle division by zero (could trigger exception) */
        return 0;
    }
    
    /* Left align denominator with numerator */
    while ((int64_t)denominator_copy >= 0) {
        denominator_copy <<= 1;
        current <<= 1;
    }

    while (current != 0) {
        if (denominator_copy <= numerator) {
            numerator -= denominator_copy;
            quotient |= current;
        }
        denominator_copy >>= 1;
        current >>= 1;
    }
    
    if (remainder) {
        *remainder = numerator;
    }
    
    return quotient;
}

int64_t __divmoddi4(int64_t numerator, int64_t denominator, int64_t* remainder) {
    int sign = 1;
    uint64_t uquotient;
    uint64_t uremainder;
    uint64_t abs_numerator;
    uint64_t abs_denominator;
    
    /* Take absolute values and save sign */
    if (numerator < 0) {
        abs_numerator = -numerator;
        sign = -sign;
    } else {
        abs_numerator = numerator;
    }
    
    if (denominator < 0) {
        abs_denominator = -denominator;
        sign = -sign;
    } else {
        abs_denominator = denominator;
    }
    
    /* Perform unsigned division */
    uquotient = __udivmoddi4(abs_numerator, abs_denominator, &uremainder);
    
    /* Apply sign to quotient */
    if (sign < 0) {
        uquotient = -uquotient;
    }
    
    /* Remainder has same sign as dividend */
    if (remainder) {
        *remainder = (numerator < 0) ? -uremainder : uremainder;
    }
    
    return uquotient;
}