#ifndef ANGOS_TOSAITHE_H
#define ANGOS_TOSAITHE_H

#include <types.h>

typedef struct {
	u32 signature;
	u32 version;
	u32 min_reqd_version;
	u32 flags;
	uintptr_t stack_ptr;
} tosaithe_entry_header;

typedef struct {
	uintptr_t base;
	uintptr_t length;
	u32 type;
	u32 flags;
} tsbp_mmap_entry;

typedef struct {
	uintptr_t base_phys;
	uintptr_t base_virt;
	uintptr_t length;
	u32 flags;
} tsbp_kernel_mapping;

typedef struct {
	u32 signature;
	u32 version;
	u32 flags;

	const char *cmdline;

	tsbp_mmap_entry *memmap;
	u32 memmap_entries;

	tsbp_kernel_mapping *kern_map;
	u32 kern_map_entries;

	void *ramdisk;
	u64 ramdisk_size;

	void *acpi_rdsp;
	void *smbios3_entry;

	void *efi_memmap;
	u32 efi_memmap_descr_size;
	u32 efi_memmap_size;
	void *efi_system_table;

	void *framebuffer_addr;
	uintptr_t framebuffer_size;
	u16 framebuffer_width;
	u16 framebuffer_height;
	u16 framebuffer_pitch;
	u16 framebuffer_bpp;

	u8 red_mask_size;
	u8 red_mask_shift;
	u8 green_mask_size;
	u8 green_mask_shift;
	u8 blue_mask_size;
	u8 blue_mask_shift;
} tosaithe_loader_data;

#define TOSAITHE_SIGNATURE 0x50425354u
#define TOSAITHE_LOADER_SIGNATURE 0x444C5354u

#define TOSAITHE_HDR_REQ_FRAMEBUFFER 1u

#define TSBP_MMAP_USABLE                 0
#define TSBP_MMAP_RESERVED               1
#define TSBP_MMAP_ACPI_RECLAIMABLE       2
#define TSBP_MMAP_ACPI_NVS               3
#define TSBP_MMAP_UEFI_RUNTIME_CODE      4
#define TSBP_MMAP_UEFI_RUNTIME_DATA      5
#define TSBP_MMAP_BAD_MEMORY             6
#define TSBP_MMAP_PERSISTENT_MEMORY      7
#define TSBP_MMAP_BOOTLOADER_RECLAIMABLE 0x1000
#define TSBP_MMAP_KERNEL                  0x1001
#define TSBP_MMAP_RAMDISK                 0x1002
#define TSBP_MMAP_FRAMEBUFFER             0x1003

#define TSBP_MMAP_CACHE_MASK 0x7
#define TSBP_MMAP_CACHE_WB   0x0
#define TSBP_MMAP_CACHE_WT   0x1
#define TSBP_MMAP_CACHE_UC   0x2
#define TSBP_MMAP_CACHE_WP   0x4
#define TSBP_MMAP_CACHE_WC   0x5
#define TSBP_MMAP_UEFI_RUNTIME 0x10

#define TSBP_KERNEL_EXEC  0x1
#define TSBP_KERNEL_WRITE 0x2
#define TSBP_KERNEL_READ  0x4

#endif