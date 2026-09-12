#ifdef __cplusplus
extern "C" {
#endif

#ifndef _KERNEL_BLKDEV_H
#define _KERNEL_BLKDEV_H

#include <stdint.h>
#include <stddef.h>

/* Block device class structure */
typedef struct {
    const char *name;                          // Name of the block device class
    unsigned int major;                        // Major number for the device class
    int (*read_blocks)(unsigned int minor, uint32_t block, unsigned int nblocks, void *buffer);  // Read function pointer
    int (*write_blocks)(unsigned int minor, uint32_t block, unsigned int nblocks, void *buffer); // Write function pointer
    struct blkdev_class *next;                 // Linked list of block device classes
} blkdev_class_t;

/* Block device instance structure */
typedef struct {
    blkdev_class_t *dev_class;                     // Pointer to the block device class
    uint32_t major;                            // Major number
    uint32_t minor;                            // Minor number
    const char *name;                          // Device name
    uint32_t block_size;                       // Block size in bytes
    uint64_t capacity;                         // Device capacity in blocks
    struct blkdev_instance *next;              // Linked list of block device instances
} blkdev_instance_t;

/* Function prototypes */
int register_blkdev_class(unsigned int major, const char *name,
                          int (*read_blocks)(unsigned int minor, uint32_t block, unsigned int nblocks, void *buffer),
                          int (*write_blocks)(unsigned int minor, uint32_t block, unsigned int nblocks, void *buffer));

int register_blkdev_instance(unsigned int major, unsigned int minor, const char *name,
                             uint32_t block_size, uint64_t capacity);

#endif // _KERNEL_BLKDEV_H
#ifdef __cplusplus
}
#endif
