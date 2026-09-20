python - <<'PY'
from pathlib import Path
import re
import shutil

memory_c = Path("kernel/memory/memory.c")
memory_h = Path("kernel/include/memory.h")
init_c = Path("kernel/x86_64/init.c")

for p in (memory_c, memory_h, init_c):
    shutil.copy2(p, str(p) + ".before-phys-allocator")

c = memory_c.read_text()

if '#include <tosaithe.h>' not in c:
    c = c.replace(
        '#include <syscall.h>\n',
        '#include <syscall.h>\n#include <tosaithe.h>\n',
        1
    )

new_phys_block = r'''
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

'''

pattern = re.compile(
    r'/\*\s*\n\s*Sets a page as used in the physical memory bitmap.*?'
    r'/\*\s*\n\s*Combines contiguous free spaces',
    re.S
)

replacement = new_phys_block + '\n/*\n * Combines contiguous free spaces'

c2, n = pattern.subn(replacement, c, count=1)

if n != 1:
    raise SystemExit("ERROR: could not locate old physical allocator block")

memory_c.write_text(c2)

h = memory_h.read_text()

insert = '''
void phys_memory_init(const struct tosaithe_loader_data *loader_data);
u64 phys_memory_total_pages(void);
u64 phys_memory_free_pages(void);
'''

if 'void phys_memory_init(' not in h:
    h = h.replace('\n#endif\n', insert + '\n#endif\n', 1)

if '#include <tosaithe.h>' not in h:
    h = h.replace('#include <types.h>\n', '#include <types.h>\n#include <tosaithe.h>\n', 1)

memory_h.write_text(h)

i = init_c.read_text()

needle = '''    terminal_init();

    terminal_write("ANGOS KERNEL STARTED\\n");
'''

replacement_init = '''    terminal_init();

    terminal_write("ANGOS KERNEL STARTED\\n");

    phys_memory_init(loader_data);

    if (phys_memory_total_pages() != 0) {
        void *test_page = phys_page_alloc();

        if (test_page) {
            phys_page_free(test_page);
            terminal_write("PHYSICAL MEMORY OK\\n");
        } else {
            terminal_write("PHYSICAL MEMORY FAILED\\n");
        }
    } else {
        terminal_write("NO USABLE MEMORY\\n");
    }
'''

if needle not in i:
    raise SystemExit("ERROR: init.c insertion point not found")

i = i.replace(needle, replacement_init, 1)

init_c.write_text(i)

print("Physical allocator patch applied successfully.")
PY