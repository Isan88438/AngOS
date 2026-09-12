#ifndef _SYS_TYPES_H
#define _SYS_TYPES_H

#include <stddef.h>  // For size_t and NULL
#include <stdint.h>  // For fixed-width integer types

#ifdef __cplusplus
extern "C" {
#endif

// Process ID type
typedef int pid_t;

// User ID type
typedef unsigned int uid_t;

// Group ID type
typedef unsigned int gid_t;

// File offset type (used for file sizes and positions)
typedef int64_t off_t;

// Signed size type (used for functions that return sizes or error codes)
typedef long ssize_t;

// Time type (used for time values)
typedef long int time_t;

// Device ID type (used for device identifiers)
typedef unsigned long dev_t;

// Inode number type (used for file system inodes)
typedef unsigned long ino_t;

// Mode type (used for file permissions and types)
typedef unsigned int mode_t;

// Number of links type (used for file link counts)
typedef unsigned short nlink_t;

// Block size type (used for file system block sizes)
typedef unsigned long blksize_t;

// Block count type (used for file system block counts)
typedef unsigned long blkcnt_t;

#ifdef __cplusplus
}
#endif

#endif // _SYS_TYPES_H