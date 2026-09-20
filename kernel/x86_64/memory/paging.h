#ifndef PAGING_H
#define PAGING_H

#include <types.h>
#include <memory.h>
#include <globals.h>

#define FLAG_PRESENT (1ULL << 0)
#define FLAG_RW      (1ULL << 1)
#define FLAG_US      (1ULL << 2)

#define FLAGS        (FLAG_PRESENT | FLAG_RW | FLAG_US)
#define ADDR_MASK    0x000ffffffffff000ULL

struct table {
        u64 entry[512];
} __attribute__((aligned(4096)));

extern struct table pml4;

void map_page(void *phys, void *virt);
void map_2mb_page(void *phys, void *virt);
void *find_physaddr(void *virt);
void init_paging(void);

extern void load_pml4(struct table *pml4);

#endif
