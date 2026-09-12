#ifdef __cplusplus
extern "C" {
#endif

#ifndef __STDARG_H__
#define __STDARG_H__

// Define the va_list type
typedef char* va_list;

// Alignment for types on the stack
#define __va_align(type) \
    (((sizeof(type) + sizeof(int) - 1) / sizeof(int)) * sizeof(int))

// Initialize a va_list
#define va_start(ap, last_arg) \
    ((ap) = (va_list)&(last_arg) + __va_align(last_arg))

// Clean up a va_list
#define va_end(ap) \
    ((void)0) // No cleanup needed for this implementation

// Fetch the next argument of the given type
#define va_arg(ap, type) \
    ((ap) += __va_align(type), \
     *(type*)((ap) - __va_align(type)))

// Copy a va_list
#define va_copy(dest, src) \
    ((dest) = (src))

// Optional: Support for printf-like format checking (GCC/Clang extension)
#ifdef __GNUC__
#define __ATTRIBUTE_FORMAT_PRINTF(fmt_idx, first_arg_idx) \
    __attribute__((format(printf, fmt_idx, first_arg_idx)))
#else
#define __ATTRIBUTE_FORMAT_PRINTF(fmt_idx, first_arg_idx)
#endif

#endif // __STDARG_H__

#ifdef __cplusplus
}
#endif
