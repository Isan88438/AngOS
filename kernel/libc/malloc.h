#ifdef __cplusplus
extern "C" {
#endif
#pragma once

#include <stddef.h>

extern void* malloc(size_t mem_length);
extern void* calloc(size_t mem_length, size_t num);


extern void* realloc(void* mem_pointer, size_t mem_length);
extern void free(void* mem_pointer);

#ifdef __cplusplus
}
#endif