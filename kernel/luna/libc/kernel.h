#ifdef __cplusplus
extern "C" {
#endif

#pragma once

#include <stddef.h>
#include <sys/types.h>

#include <pdclib/_PDCLIB_glue.h>

#define RAND_MAX 32767

int rand(void);
void srand(unsigned int seed);

ssize_t write(int fd, const void *buf, size_t count);

int close(int fd);
int unlink(const char *pathname);
void _exit(int status);

// Device driver write function type
typedef ssize_t (*device_write_fn)(int fd, const void *buf, size_t count);

// Initialize standard I/O devices
void kernel_io_init(void);

void *luna_malloc(size_t size);

void *luna_realloc(void *ptr, size_t size);

void luna_free(void *ptr);

#ifdef __cplusplus
}
#endif

