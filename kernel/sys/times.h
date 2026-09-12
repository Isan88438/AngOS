#ifndef _SYS_TIMES_H
#define _SYS_TIMES_H

#include <stdlib.h>  // For clock_t

#ifdef __cplusplus
extern "C" {
#endif

// Structure to store process times
struct tms {
    long int tms_utime;  // User CPU time
    long int tms_stime;  // System CPU time
    long int tms_cutime; // User CPU time of terminated children
    long int tms_cstime; // System CPU time of terminated children
};

// Function to retrieve process times
long int times(struct tms *buf);

#ifdef __cplusplus
}
#endif

#endif // _SYS_TIMES_H