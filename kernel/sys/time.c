#include "stdint.h"
#include "sys/time.h"  // For struct timeval and struct timezone

extern dword_t get_timer_value_in_microseconds(void);
extern dword_t get_timer_value_in_milliseconds(void);

// SystemTime stores the UNIX EPOCH time.
// It should be updated by the system periodically.

// Structure representing a broken-down calendar time
struct tm {
    int tm_sec;    // Seconds (0-59)
    int tm_min;    // Minutes (0-59)
    int tm_hour;   // Hours (0-23)
    int tm_mday;   // Day of the month (1-31)
    int tm_mon;    // Month (0-11, 0 = January)
    int tm_year;   // Year (current year minus 1900)
    int tm_wday;   // Day of the week (0-6, 0 = Sunday)
    int tm_yday;   // Day in the year (0-365, 1 = January 1)
    int tm_isdst;  // Daylight saving time flag
};

long int SystemTime = 0;

// Helper function to calculate the number of seconds since the UNIX epoch
// for a given date and time.
static unsigned long _mktime(unsigned int year, unsigned int mon, unsigned int day, unsigned int hour, unsigned int min, unsigned int sec) {
    // Adjust month and year if the month is January or February
    if (mon <= 2) {
        mon += 12;
        year -= 1;
    }

    // Calculate the number of days since March 1, 0000 (proleptic Gregorian calendar)
    unsigned long days = (365 * year) + (year / 4) - (year / 100) + (year / 400) + ((367 * mon) / 12) + day - 719499;

    // Convert days to seconds and add the time of day
    return ((days * 24 + hour) * 60 + min) * 60 + sec;
}

// Convert a tm structure to a time_t value (seconds since the UNIX epoch)
long int mktime(struct tm *tm) {
    if (!tm) {
        return -1; // Invalid input
    }

    // Call the helper function with the tm structure fields
    return _mktime(tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec);
}

// Implementation of gettimeofday
int gettimeofday(struct timeval *tv, struct timezone *tz) {
    if (!tv) {
        return -1; // Invalid input
    }

    // Get the current time in seconds
    tv->tv_sec = SystemTime;

    // Simulate microseconds (for demonstration purposes)
    // In a real implementation, this would come from a high-resolution timer.
    tv->tv_usec = 0;

    // Handle timezone (if provided)
    if (tz) {
        tz->tz_minuteswest = 0; // Assume UTC (no timezone offset)
        tz->tz_dsttime = 0;     // Assume no daylight saving time
    }

    return 0; // Success
}


long int mtime() {
    return get_timer_value_in_milliseconds();
}