#ifdef __cplusplus
extern "C" {
#endif

#pragma once

#define CHAR_BIT  8
#define SCHAR_MIN (-128)
#define SCHAR_MAX 127
#define UCHAR_MAX 255
#define INT_MIN   (-2147483648)
#define INT_MAX   2147483647
#define UINT_MAX  0xFFFFFFFFU

#define SHRT_MIN  (-32767 - 1)  // Minimum value for an object of type short
#define SHRT_MAX  32767         // Maximum value for an object of type short
#define USHRT_MAX  65535        // Maximum value for an object of type unsigned short

#define LONG_MIN  (-2147483647L - 1)  // Minimum value for an object of type long
#define LONG_MAX  2147483647L         // Maximum value for an object of type long
#define ULONG_MAX  4294967295UL       // Maximum value for an object of type unsigned long

#define LLONG_MIN  (-9223372036854775807LL - 1)  // Minimum value for an object of type long long
#define LLONG_MAX  9223372036854775807LL         // Maximum value for an object of type long long
#define ULLONG_MAX  18446744073709551615ULL      // Maximum value for an object of type unsigned long long

#define CHAR_MIN  (-128)  // Minimum value for an object of type char if char is signed
#define CHAR_MAX  127     // Maximum value for an object of type char if char is signed

#ifdef __cplusplus
}
#endif
