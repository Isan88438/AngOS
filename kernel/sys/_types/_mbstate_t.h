#ifndef _KERNEL_MBSTATE_H
#define _KERNEL_MBSTATE_H

// not fully implemented yet

/* Opaque type for multibyte conversion state */
typedef struct {
    unsigned long __opaque[4];  // Opaque storage for conversion state
} mbstate_t;

#endif /* _KERNEL_MBSTATE_H */