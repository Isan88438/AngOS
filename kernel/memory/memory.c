#include <types.h>
#include <memory.h>
#include <string.h>
#include <globals.h>
#include "memory.h"
#include <syscall.h>
#include <tosaithe.h>
#ifndef CEILING
#define CEILING(a, b) (((a) + (b) - 1) / (b))
#endif

/*
	Allocate virtual memory from a heap
*/
void *malloc_heap(size_t size, int heap) {
	/* Error checking */
	if (heap >= HEAPS || size == 0)
		return 0;

	/* Look through freespace for large enough space */
	int space;
	for (space = 0; space < FREE_SPACES; ++space)
		if (freespace[heap][space].size >= size)
			goto exit1;
	return 0;
exit1:;

	/* Save address (this address will be returned) */
	void *const addr = freespace[heap][space].addr;

	/* Shrink the freespace size */
	freespace[heap][space].addr += size;
	freespace[heap][space].size -= size;

	/* Mark allocation in table */
	int alloc;
	for (alloc = 0; alloc < ALLOCATIONS; ++alloc)
		if (allocations[heap][alloc].size == 0)
			goto exit2;
	return 0;
	exit2:

	allocations[heap][alloc].addr = addr;
	allocations[heap][alloc].size = size;

	/* Map pages to that part if no pages are mapped there already */
	size_t pages = CEILING((size_t)(addr + size), PAGE_SIZE) - (size_t)(addr) / PAGE_SIZE;
	for (size_t i = 0; i < pages; ++i) {
		if (!find_physaddr(addr + i * PAGE_SIZE))
			map_page(phys_page_alloc(), addr + i * PAGE_SIZE);
	}
	return addr;
}

/*
	Allocate memory from the kernel heap
*/
void *malloc(size_t size) {
	return malloc_heap(size, 0);
}

/*
	Frees memory allocated from malloc
*/
void free(void *addr) {
	/* Heap/bounds checker */
	int heap;
	if (addr >= kernel_heap && addr < kernel_stack)
		heap = 0;
	else if (addr >= userspace_heap && addr < userspace_stack)
		heap = 1;
	else
		return;

	/* Find the allocation in the allocs table to find the size */
	int alloc;
	size_t size;
	for (alloc = 0; alloc < ALLOCATIONS; ++alloc)
		if (addr == allocations[heap][alloc].addr)
			goto exit1;
	return;
	exit1:
	size = allocations[heap][alloc].size;
	allocations[heap][alloc].addr = 0;
	allocations[heap][alloc].size = 0;
	bool returned = false;
	for (int i = 0; i < FREE_SPACES; ++i) {
		/* Memory is behind freespace */
		if (addr + size == freespace[heap][i].addr) {
			freespace[heap][i].addr -= size;
			freespace[heap][i].size += size;
			returned = true;
			break;
		}
		/* Memory is after freespace */
		if (addr == freespace[heap][i].addr + freespace[heap][i].size) {
			freespace[heap][i].size += size;
			returned = true;
			break;
		}
	}
	if (!returned) {
		for (int i = 0; i < FREE_SPACES; ++i) {
			if (freespace[heap][i].size == 0) {
				freespace[heap][i].addr = addr;
				freespace[heap][i].size = size;
				goto exit2;
			}
		}
		return;
	}
	exit2:;

	/* Free pages (if there is no other memory in the page) */
	size_t pages = CEILING((size_t) addr + size, PAGE_SIZE) - (size_t) addr / PAGE_SIZE;
	bool sused = false, eused = false;
	for (int i = 0; i < ALLOCATIONS; ++i) {
		if (((size_t) addr / PAGE_SIZE) * PAGE_SIZE ==
			(((size_t) allocations[heap][i].addr + allocations[heap][i].size - 1) / PAGE_SIZE) * PAGE_SIZE)
			sused = true;
		if ((((size_t) addr + size - 1) / PAGE_SIZE) * PAGE_SIZE ==
			((size_t) allocations[heap][i].addr / PAGE_SIZE) * PAGE_SIZE)
			eused = true;
	}
	if (!sused) phys_page_free(find_physaddr(addr));
	if (!eused) phys_page_free(find_physaddr(addr + size - 1));
	for (size_t i = 1; i < pages - 1; ++i)
		phys_page_free(find_physaddr(addr + i * PAGE_SIZE));
}


#define MAX_PHYS_REGIONS 512

typedef struct {
        u64 base;
        u64 pages;
} phys_region_t;

static phys_region_t managed_regions[MAX_PHYS_REGIONS];
static phys_region_t free_regions[MAX_PHYS_REGIONS];

static size_t managed_region_count;
static size_t free_region_count;

static u64 phys_total_pages;
static u64 phys_free_pages_count;

static void phys_sort_and_merge(phys_region_t *regions, size_t *count) {
        for (size_t i = 1; i < *count; ++i) {
                phys_region_t value = regions[i];
                size_t j = i;

                while (j > 0 && regions[j - 1].base > value.base) {
                        regions[j] = regions[j - 1];
                        --j;
                }

                regions[j] = value;
        }

        size_t out = 0;

        for (size_t i = 0; i < *count; ++i) {
                if (regions[i].pages == 0)
                        continue;

                if (out != 0) {
                        u64 end =
                                regions[out - 1].base +
                                regions[out - 1].pages * PAGE_SIZE;

                        if (end == regions[i].base) {
                                regions[out - 1].pages += regions[i].pages;
                                continue;
                        }
                }

                regions[out++] = regions[i];
        }

        *count = out;
}

static bool phys_managed(u64 addr) {
        for (size_t i = 0; i < managed_region_count; ++i) {
                u64 begin = managed_regions[i].base;
                u64 end =
                        begin +
                        managed_regions[i].pages * PAGE_SIZE;

                if (addr >= begin && addr < end)
                        return true;
        }

        return false;
}

static bool phys_free_already(u64 addr) {
        for (size_t i = 0; i < free_region_count; ++i) {
                u64 begin = free_regions[i].base;
                u64 end =
                        begin +
                        free_regions[i].pages * PAGE_SIZE;

                if (addr >= begin && addr < end)
                        return true;
        }

        return false;
}

static bool phys_insert_free_page(u64 addr) {
        size_t pos = 0;

        while (pos < free_region_count &&
               free_regions[pos].base < addr)
                ++pos;

        if (pos > 0) {
                phys_region_t *prev = &free_regions[pos - 1];
                u64 prev_end =
                        prev->base + prev->pages * PAGE_SIZE;

                if (prev_end == addr) {
                        ++prev->pages;

                        if (pos < free_region_count &&
                            addr + PAGE_SIZE == free_regions[pos].base) {
                                prev->pages += free_regions[pos].pages;

                                for (size_t i = pos;
                                     i + 1 < free_region_count;
                                     ++i) {
                                        free_regions[i] = free_regions[i + 1];
                                }

                                --free_region_count;
                        }

                        return true;
                }
        }

        if (pos < free_region_count &&
            addr + PAGE_SIZE == free_regions[pos].base) {
                free_regions[pos].base = addr;
                ++free_regions[pos].pages;
                return true;
        }

        if (free_region_count >= MAX_PHYS_REGIONS)
                return false;

        for (size_t i = free_region_count; i > pos; --i)
                free_regions[i] = free_regions[i - 1];

        free_regions[pos].base = addr;
        free_regions[pos].pages = 1;
        ++free_region_count;

        return true;
}

void phys_memory_init(const tosaithe_loader_data *loader_data) {
        managed_region_count = 0;
        free_region_count = 0;
        phys_total_pages = 0;
        phys_free_pages_count = 0;

        if (!loader_data || !loader_data->memmap)
                return;

        for (u32 i = 0; i < loader_data->memmap_entries; ++i) {
                const tsbp_mmap_entry *entry = &loader_data->memmap[i];

                if (entry->type != 0)
                        continue;

                u64 begin = (u64)entry->base;
                u64 end = begin + (u64)entry->length;

                begin = (begin + PAGE_SIZE - 1) & ~(u64)(PAGE_SIZE - 1);
                end &= ~(u64)(PAGE_SIZE - 1);

                if (end <= begin)
                        continue;

                if (begin == 0) {
                        if (end <= PAGE_SIZE)
                                continue;

                        begin = PAGE_SIZE;
                }

                u64 pages = (end - begin) / PAGE_SIZE;

                if (managed_region_count >= MAX_PHYS_REGIONS)
                        break;

                managed_regions[managed_region_count].base = begin;
                managed_regions[managed_region_count].pages = pages;
                ++managed_region_count;
        }

        phys_sort_and_merge(managed_regions, &managed_region_count);

        for (size_t i = 0; i < managed_region_count; ++i) {
                free_regions[free_region_count++] = managed_regions[i];
                phys_total_pages += managed_regions[i].pages;
        }

        free_region_count = managed_region_count;
        phys_free_pages_count = phys_total_pages;
}

void *phys_page_alloc(void) {
        for (size_t i = 0; i < free_region_count; ++i) {
                phys_region_t *region = &free_regions[i];

                if (region->pages == 0)
                        continue;

                u64 addr = region->base;

                region->base += PAGE_SIZE;
                --region->pages;

                if (region->pages == 0) {
                        for (size_t j = i;
                             j + 1 < free_region_count;
                             ++j) {
                                free_regions[j] = free_regions[j + 1];
                        }

                        --free_region_count;
                }

                --phys_free_pages_count;
                return (void *)addr;
        }

        return 0;
}

void phys_page_free(void *addr) {
        u64 page = (u64)addr;

        if (page == 0 || (page & (PAGE_SIZE - 1)) != 0)
                return;

        if (!phys_managed(page))
                return;

        if (phys_free_already(page))
                return;

        if (!phys_insert_free_page(page))
                return;

        ++phys_free_pages_count;
}

u64 phys_memory_total_pages(void) {
        return phys_total_pages;
}

u64 phys_memory_free_pages(void) {
        return phys_free_pages_count;
}


/*
 * Combines contiguous free spaces into one entry
*/
void refresh_freespace() {
	for (int i = 0; i < HEAPS; ++i) {
		for (int j = 0; j < FREE_SPACES; ++j) {
			void *end_addr = freespace[i][j].addr + freespace[i][j].size;
			for (int k = 0; k < FREE_SPACES; ++k) {
				if (freespace[i][k].addr == end_addr) {
					/* Combine the sizes */
					freespace[i][j].size += freespace[i][k].size;

					/* Delete old entry */
					freespace[i][k].addr = 0;
					freespace[i][k].size = 0;
				}
			}
		}
	}
}

void free_pages(const void *virt, size_t pages) {
	unsigned int pml4_idx;
	unsigned int pdpt_idx;
	unsigned int pd_idx;
	unsigned int pt_idx;

	while (pages) {
		pml4_idx =	((u64) virt >> 39) & 0x1FF;
		pdpt_idx =	((u64) virt >> 30) & 0x1FF;
		pd_idx =	((u64) virt >> 21) & 0x1FF;
		pt_idx =	((u64) virt >> 12) & 0x1FF;

		struct table *pdpt = ((void *) pml4.entry[pml4_idx]);
		struct table *pd = ((void *) pdpt->entry[pdpt_idx]);
		struct table *pt = ((void *) pd->entry[pd_idx]);

		pt->entry[pt_idx] &= ~(1 << 0);

		virt += PAGE_SIZE;
		--pages;
	}
}

void init_malloc() {
	freespace[0][0].addr = kernel_heap;
	freespace[0][0].size = kernel_stack - kernel_heap;
	freespace[1][0].addr = userspace_heap;
	freespace[1][0].size = userspace_stack - userspace_heap;
}

void init_paging();

void init_memory() {
	init_paging();
	init_malloc();
}
