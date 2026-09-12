#ifdef __cplusplus
extern "C" {
#endif

#ifndef _TIME_H_
#define _TIME_H_

#include <stdint.h>  // For standard types like int64_t
#include <stddef.h>

// Structure representing a time value with seconds and microseconds
struct timeval {
    long int tv_sec;  // Seconds
    long tv_usec;   // Microseconds
};

// Structure representing a time zone
struct timezone {
    int tz_minuteswest;  // Minutes west of Greenwich
    int tz_dsttime;      // Type of DST correction
};

// Get the current time and time zone
int gettimeofday(struct timeval *tv, struct timezone *tz);

long int mtime();

#endif // _TIME_H_

#ifdef __cplusplus
}
#endif