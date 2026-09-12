#include "stddef.h"
#include "stdio.h"
#include "stdlib.h"
// #include "drivers/ufat.h"
// #include "drivers/voltron.h"
#include <stdlib.h>
#include <pdclib/_PDCLIB_glue.h>
#include <pdclib/_PDCLIB_internal.h>
#include <pdclib/_PDCLIB_config.h>

#include <string.h>

// PDCLIB internal flags (adjust according to your PDCLIB configuration)
#define _PDCLIB_FOPEN   0x0001
#define _PDCLIB_FBYTE   0x0008
#define _PDCLIB_ERROR   0x0010

// extern struct ufat uf;

// typedef struct {
//     struct ufat_file file;
//     uint32_t pos;
//     bool is_appending;
// } ufat_file_handle;

//-----------------------------------------------------------------
// fopen: Open a file with the given mode
//-----------------------------------------------------------------
_PDCLIB_PUBLIC FILE *fopen(const char *pathname, const char *mode) {
    // if (!pathname || !mode) return NULL;

    // // Parse mode flags
    // bool read = false, write = false, append = false, trunc = false, plus = false;
    // for (; *mode; ++mode) {
    //     switch (*mode) {
    //         case 'r': read = true; break;
    //         case 'w': write = true; trunc = true; break;
    //         case 'a': append = true; write = true; break;
    //         case '+': plus = true; break;
    //         case 'b': break;
    //         default: return NULL;
    //     }
    // }

    // // Validate mode combinations
    // if ((append && trunc) || (!read && !write && !plus)) return NULL;

    // // Check file existence and get dirent
    // struct ufat_directory dir;
    // struct ufat_dirent ent;
    // const char *remaining;
    // ufat_open_root(&uf, &dir);
    // int ret = ufat_dir_find_path(&dir, pathname, &ent, &remaining);
    // bool file_exists = (ret == UFAT_OK);

    // // Handle directories and invalid paths
    // if (file_exists && (ent.attributes & UFAT_ATTR_DIRECTORY))
    //     return NULL;

    // // File creation/truncation logic
    // if (write) {
    //     if (file_exists) {
    //         if (trunc) {
    //             struct ufat_file tmp;
    //             if (ufat_open_file(&uf, &tmp, &ent) != UFAT_OK) return NULL;
    //             if (ufat_file_truncate(&tmp) != UFAT_OK) {
    //                 return NULL;
    //             }
    //         }
    //     } else {
    //         if (!create_file(pathname)) return NULL;
    //         ufat_open_root(&uf, &dir);
    //         if (ufat_dir_find_path(&dir, pathname, &ent, &remaining) != UFAT_OK)
    //             return NULL;
    //     }
    // } else if (read && !file_exists) {
    //     return NULL;
    // }

    // // Allocate FILE structure
    // FILE *stream = (FILE*)calloc(1, sizeof(FILE));
    // if (!stream) return NULL;

    // // Allocate ufat handle
    // ufat_file_handle *handle = (ufat_file_handle*)calloc(1, sizeof(ufat_file_handle));
    // if (!handle) {
    //     free(stream);
    //     return NULL;
    // }

    // // Open ufat file
    // if (ufat_open_file(&uf, &handle->file, &ent) != UFAT_OK) {
    //     free(handle);
    //     free(stream);
    //     return NULL;
    // }

    // // Handle append mode
    // if (append) {
    //     handle->pos = ent.file_size;
    //     // Advance to end of file
    //     if (ufat_file_advance(&handle->file, ent.file_size) != UFAT_OK) {
    //         free(handle);
    //         free(stream);
    //         return NULL;
    //     }
    // }

    // // Configure FILE structure
    // stream->handle = (_PDCLIB_fd_t)handle;
    // stream->status = _PDCLIB_FOPEN;
    // if (read || plus) stream->status |= _PDCLIB_FREAD;
    // if (write || plus) stream->status |= _PDCLIB_FWRITE;

    // // Allocate buffer
    // stream->bufsize = 1024;
    // stream->buffer = malloc(stream->bufsize);
    // if (!stream->buffer) {
    //     free(handle);
    //     free(stream);
    //     return NULL;
    // }
    // stream->status |= _PDCLIB_FBYTE;

    // return stream;

    return NULL;
}

//-----------------------------------------------------------------
// fclose: Close a file and release resources
//-----------------------------------------------------------------
_PDCLIB_PUBLIC int fclose(FILE *stream) {
    // if (!stream || !(stream->status & _PDCLIB_FOPEN)) return EOF;

    // ufat_file_handle *handle = (ufat_file_handle *)stream->handle;

    // // Flush buffer if writing
    // if ((stream->status & _PDCLIB_FWRITE) && stream->bufidx > 0) {
    //     ufat_file_write(&handle->file, stream->buffer, stream->bufidx);
    //     stream->bufidx = 0;
    // }

    // // Cleanup resources
    // free(handle);
    // free(stream->buffer);
    // free(stream);

    return 0;
}

//-----------------------------------------------------------------
// fread: Read data from file into buffer
//-----------------------------------------------------------------
_PDCLIB_PUBLIC size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    // if (!ptr || !stream || !(stream->status & _PDCLIB_FREAD)) return 0;

    // ufat_file_handle *handle = (ufat_file_handle *)stream->handle;
    // size_t total = size * nmemb;
    // size_t read = 0;

    // // Read from buffer
    // if (stream->bufidx < stream->bufend) {
    //     size_t buffered = stream->bufend - stream->bufidx;
    //     size_t to_copy = buffered < total ? buffered : total;
    //     memcpy(ptr, stream->buffer + stream->bufidx, to_copy);
    //     stream->bufidx += to_copy;
    //     read += to_copy;
    //     ptr = (char *)ptr + to_copy;
    // }

    // // Read remaining data directly
    // if (read < total) {
    //     int ret = ufat_file_read(&handle->file, ptr, total - read);
    //     if (ret < 0) {
    //         stream->status |= _PDCLIB_ERROR;
    //         return read / size;
    //     }
    //     read += ret;
    //     handle->pos += ret;
    // }

    // return read / size;

    return 0;
}

//-----------------------------------------------------------------
// fwrite: Write data to file from buffer
//-----------------------------------------------------------------
_PDCLIB_PUBLIC size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream) {
    // if (!ptr || !stream || !(stream->status & _PDCLIB_FWRITE)) return 0;

    // ufat_file_handle *handle = (ufat_file_handle *)stream->handle;
    // size_t total = size * nmemb;
    // size_t written = 0;

    // // Handle append mode
    // if (handle->is_appending) {
    //     handle->pos = handle->file.file_size;
    //     if (ufat_file_advance(&handle->file, handle->pos) != UFAT_OK) {
    //         stream->status |= _PDCLIB_ERROR;
    //         return 0;
    //     }
    //     handle->is_appending = false;
    // }

    // // Buffer management
    // if (stream->bufidx + total > stream->bufsize) {
    //     int ret = ufat_file_write(&handle->file, stream->buffer, stream->bufidx);
    //     if (ret != stream->bufidx) {
    //         stream->status |= _PDCLIB_ERROR;
    //         return 0;
    //     }
    //     handle->pos += stream->bufidx;
    //     stream->bufidx = 0;
    // }

    // // Write to buffer or directly
    // if (total <= stream->bufsize - stream->bufidx) {
    //     memcpy(stream->buffer + stream->bufidx, ptr, total);
    //     stream->bufidx += total;
    //     written = total;
    // } else {
    //     int ret = ufat_file_write(&handle->file, ptr, total);
    //     if (ret != total) {
    //         stream->status |= _PDCLIB_ERROR;
    //         return 0;
    //     }
    //     handle->pos += ret;
    //     written = ret;
    // }

    // return written / size;

    return 0;
}



_PDCLIB_PUBLIC int remove(const char *path) {
    // printf("remove: Attempting to remove: %s\n", path);

    // struct ufat_directory dir;
    // struct ufat_dirent ent;
    // const char* remaining;
    
    // // Start at root directory
    // ufat_open_root(&uf, &dir);

    // // Find the entry
    // int ret = ufat_dir_find_path(&dir, path, &ent, &remaining);
    // if (ret != UFAT_OK) {
    //     printf("remove: Path not found or invalid: %s\n", path);
    //     return -1; // Error: file or path not found
    // }

    // // Check if it's a file or directory
    // if (ent.attributes & UFAT_ATTR_DIRECTORY) {
    //     // It's a directory
    //     if (ent.file_size != 0) { // Checking if the directory is empty (assuming file_size indicates this for directories)
    //         printf("remove: Directory not empty, cannot remove: %s\n", path);
    //         return -1; // Error: directory not empty
    //     }
    //     // For directory removal, we still use ufat_dir_delete since remove_file only handles files
    //     ret = ufat_dir_delete(&uf, &ent);
    //     if (ret != UFAT_OK) {
    //         printf("remove: Failed to delete directory: %s\n", path);
    //         return -1; // Error: failed to delete directory
    //     }
    // } else {
    //     // Use existing remove_file function for files (assuming delete_file is now remove_file)
    //     if (!delete_file(path)) {
    //         return -1; // Deletion failed
    //     }
    // }

    // printf("remove: Successfully removed: %s\n", path);
    return -1; // Success
}
