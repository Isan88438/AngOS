#ifdef __cplusplus
extern "C" {
#endif

#pragma once

#include <stdbool.h>
#include <stdint.h>

// Filesystem initialization and directory listing
bool init_filesystem();

// File operations (abstracted)
bool create_directory(const char* path);
bool open_directory(const char* path);

bool read_file(const char* path, void* buffer, uint32_t size);

// Creates a new file at the specified path
bool create_file(const char* path);

// Writes data to an existing file (overwrites contents)
bool write_file(const char* path, const void* data, uint32_t size);

// Deletes a file at the specified path
bool delete_file(const char* path);
#ifdef __cplusplus
}
#endif
