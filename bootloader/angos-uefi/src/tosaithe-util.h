#ifndef INCLUDED_TOSAITHE_UTIL_H
#define INCLUDED_TOSAITHE_UTIL_H

#include <uefi.h>
#include <uefi-media-file.h>


#include <stdint.h>
#include <stddef.h>

extern EFI_BOOT_SERVICES *EBS;
extern EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *EFI_con_out;

// Allocate from the "pool". This is, essentially, malloc.
inline void *alloc_pool(unsigned size)
{
    void *allocdBuf;
    EFI_STATUS status = EBS->AllocatePool(EfiLoaderData, size, &allocdBuf);
    if (EFI_ERROR(status)) {
        return nullptr;
    }
    return allocdBuf;
}

// Return memory to the "pool".
inline void free_pool(void *buf)
{
    EBS->FreePool(buf);
}

// Find (or at least guess) the best GOP interface to use
EFI_GRAPHICS_OUTPUT_PROTOCOL *find_GOP();

// Locate a protocol by finding a singular handle supporting it
inline void *locate_protocol(const EFI_GUID &guid)
{
    void *interface_ptr = nullptr;

    // LocateProtocol only available since UEFI 1.10:
    if (EBS->Hdr.Revision >= (0x100 + 10)) {
        EBS->LocateProtocol(&guid, nullptr, &interface_ptr);
        return interface_ptr;
    }

    // Without LocateProtocol, we will try LocateHandle + HandleProtocol.
    EFI_HANDLE located_handle;
    UINTN handle_buf_size = sizeof(located_handle);

    EFI_STATUS status = EBS->LocateHandle(ByProtocol, &guid, nullptr, &handle_buf_size, &located_handle);
    if (EFI_ERROR(status)) {
        if (status != EFI_BUFFER_TOO_SMALL) {
            return nullptr;
        }

        // Our "buffer" was too small, i.e. there is more than one matching handle. Allocate a real buffer
        // on the heap to obtain the full list of handles (even though we only need the first...).
        EFI_HANDLE *heap_buf = (EFI_HANDLE *) alloc_pool(handle_buf_size);
        if (heap_buf == nullptr) {
            return nullptr;
        }
        status = EBS->LocateHandle(ByProtocol, &guid, nullptr, &handle_buf_size, heap_buf);

        // Take the first handle, then free the buffer:
        located_handle = *heap_buf;
        free_pool(heap_buf);
        if (EFI_ERROR(status)) {
            return nullptr;
        }
    }

    // This should succeed.
    EBS->HandleProtocol(located_handle, &guid, &interface_ptr);

    return interface_ptr;
}

// Owned EFI page allocation.
class efi_page_alloc
{
    EFI_PHYSICAL_ADDRESS address_ = 0;
    UINTN pages_ = 0;

public:
    efi_page_alloc() {}

    ~efi_page_alloc()
    {
        if (address_ != 0 && pages_ != 0) {
            EBS->FreePages(address_, pages_);
        }
    }

    efi_page_alloc(const efi_page_alloc &) = delete;
    efi_page_alloc &operator=(const efi_page_alloc &) = delete;

    // allocate pages at the specified address
    bool allocate_nx(EFI_PHYSICAL_ADDRESS address, UINTN num_pages)
    {
        EFI_STATUS status =
            EBS->AllocatePages(
                AllocateAddress,
                EfiLoaderCode,
                num_pages,
                &address
            );

        if (EFI_ERROR(status)) {
            return false;
        }

        address_ = address;
        pages_ = num_pages;
        return true;
    }

    void allocate(EFI_PHYSICAL_ADDRESS address, UINTN num_pages)
    {
        allocate_nx(address, num_pages);
    }

    // allocate pages at any address
    bool allocate_nx(UINTN num_pages)
    {
        EFI_PHYSICAL_ADDRESS address = 0;

        EFI_STATUS status =
            EBS->AllocatePages(
                AllocateAnyPages,
                EfiLoaderCode,
                num_pages,
                &address
            );

        if (EFI_ERROR(status)) {
            return false;
        }

        address_ = address;
        pages_ = num_pages;
        return true;
    }

    void allocate(UINTN num_pages)
    {
        allocate_nx(num_pages);
    }

    // Extend allocation without moving it.
    bool extend_nx(UINTN num_pages)
    {
        if (address_ == 0 || pages_ == 0) {
            return false;
        }

        EFI_PHYSICAL_ADDRESS address =
            address_ + pages_ * 4096u;

        EFI_STATUS status =
            EBS->AllocatePages(
                AllocateAddress,
                EfiLoaderCode,
                num_pages,
                &address
            );

        if (EFI_ERROR(status)) {
            return false;
        }

        pages_ += num_pages;
        return true;
    }

    void rezone(EFI_PHYSICAL_ADDRESS address, UINTN num_pages)
    {
        address_ = address;
        pages_ = num_pages;
    }

    UINTN page_count() const
    {
        return pages_;
    }

    EFI_PHYSICAL_ADDRESS get_ptr() const
    {
        return address_;
    }
};


// // Owning EFI_FILE_PROTOCOL handle.
class efi_file_handle
{
    EFI_FILE_PROTOCOL *handle_ = nullptr;

public:
    efi_file_handle() {}

    explicit efi_file_handle(EFI_FILE_PROTOCOL *handle)
        : handle_(handle)
    {
    }

    ~efi_file_handle()
    {
        reset(nullptr);
    }

    efi_file_handle(const efi_file_handle &) = delete;
    efi_file_handle &operator=(const efi_file_handle &) = delete;

    void reset(EFI_FILE_PROTOCOL *handle)
    {
        if (handle_ != nullptr) {
            handle_->Close(handle_);
        }

        handle_ = handle;
    }

    EFI_FILE_PROTOCOL *get() const
    {
        return handle_;
    }

    EFI_STATUS read(UINTN *read_amount, void *addr)
    {
        return handle_->Read(handle_, read_amount, addr);
    }

    EFI_STATUS seek(UINTN position)
    {
        return handle_->SetPosition(handle_, position);
    }
};


// inline void con_write(const CHAR16 *str)
{
    EFI_con_out->OutputString(EFI_con_out, str);
}

inline void con_write(uint64_t val)
{
    CHAR16 buf[21];

    unsigned pos = 20;
    buf[20] = 0;

    do {
        unsigned digit = val % 10;
        val = val / 10;
        pos--;
        buf[pos] = digit + '0';
    } while (val > 0);

    con_write(buf + pos);
}

inline unsigned strlen(const CHAR16 *str)
{
    unsigned i = 0;
    while (str[i] != 0) {
        i++;
    }
    return i;
}

inline CHAR16 *strdup(CHAR16 *str)
{
    unsigned len = strlen(str);
    CHAR16 *rbuf = (CHAR16 *) alloc_pool(len);
    if (rbuf == nullptr) {
        return nullptr;
    }

    for (unsigned i = 0; i < len; i++) {
        rbuf[i] = str[i];
    }

    return rbuf;
}

inline CHAR16 hexdigit(int val)
{
    if (val < 10) {
        return L'0' + val;
    }
    return L'A' + val - 10;
}

inline void con_write_hex(uint64_t val)
{
    CHAR16 buf[21];

    unsigned pos = 20;
    buf[20] = 0;

    do {
        unsigned digit = val % 16;
        val = val / 16;
        pos--;
        buf[pos] = hexdigit(digit);
    } while (val > 0);

    con_write(buf + pos);
}

template <typename T> void swap(T &a, T &b)
{
    T temp = a;
    a = b;
    b = temp;
}

// Find the file path (if any) device node in the device path and return the offset.
// Return (unsigned)-1 if not found.
inline unsigned find_file_path(const EFI_DEVICE_PATH_PROTOCOL *dp)
{
    typedef unsigned char byte;
    byte *dp_u8_start = (byte *)dp;
    byte *dp_u8 = dp_u8_start;

    uint8_t dpn_type = dp_u8[0];
    while(dpn_type != 0x7F) {
        if (dpn_type == 0x04) {
            uint8_t dpn_subtype = dp_u8[1];
            if (dpn_subtype == 0x04) {
                return dp_u8 - dp_u8_start;
            }
        }

        uint16_t len = dp_u8[2] + (dp_u8[3] << 8);
        dp_u8 += len;
        dpn_type = dp_u8[0];
    }

    return -1;
}

// Find the end-of-device-path node in a device path
inline unsigned find_devpath_end(const EFI_DEVICE_PATH_PROTOCOL *dp)
{
    typedef unsigned char byte;
    byte *dp_u8_start = (byte *)dp;
    byte *dp_u8 = dp_u8_start;

    uint8_t dpn_type = dp_u8[0];
    while(dpn_type != 0x7F) {
        uint16_t len = dp_u8[2] + (dp_u8[3] << 8);
        dp_u8 += len;
        dpn_type = dp_u8[0];
    }

    return dp_u8 - dp_u8_start;
}

// Find the total size of a (singular) device path including end node
inline unsigned find_devpath_size(const EFI_DEVICE_PATH_PROTOCOL *dp)
{
    typedef unsigned char byte;
    byte *dp_u8_start = (byte *)dp;
    byte *dp_u8 = dp_u8_start;

    unsigned end = find_devpath_end(dp);

    uint16_t len = dp_u8[end + 2] + (dp_u8[end + 3] << 8);
    return end + len;
}

// Advance to next device path instance (or return nullptr)
inline EFI_DEVICE_PATH_PROTOCOL *find_next_devpath_instance(const EFI_DEVICE_PATH_PROTOCOL *dp)
{
    typedef unsigned char byte;
    unsigned end_offs = find_devpath_end(dp);
    byte *dp_u8 = (byte *)dp + end_offs;

    uint8_t dpn_subtype = dp_u8[1];
    if (dpn_subtype != 0x1) {
        // Not the start of a new instance, therefore end of device path
        return nullptr;
    }

    uint16_t dpn_end_node_len = dp_u8[2] + (dp_u8[3] << 8);
    return (EFI_DEVICE_PATH_PROTOCOL *)(dp_u8 + dpn_end_node_len);
}

struct open_file_error
{
    enum of_stage {
        NO_FSPROTOCOL_FOR_DEV_PATH,
        CANNOT_OPEN_VOLUME,
        NO_DPTT_PROTOCOL,
        CANNOT_OPEN_FILE
    };

    of_stage reason;
    EFI_STATUS status;
};

EFI_FILE_PROTOCOL *open_file(
    const EFI_DEVICE_PATH_PROTOCOL *dev_path,
    open_file_error *error
);

// Open a file, specified via devpath; throws open_file_exception on error, std::bad_alloc if out
// of memory
EFI_FILE_PROTOCOL *open_file(const EFI_DEVICE_PATH_PROTOCOL *dev_path);

// Get file info. May throw std::bad_alloc.
inline EFI_FILE_INFO *get_file_info(EFI_FILE_PROTOCOL *file)
{
    UINTN bufferSize = 128;
    EFI_FILE_INFO *buffer = (EFI_FILE_INFO *) alloc_pool(bufferSize);
    if (buffer == nullptr) {
        return nullptr;
    }

    EFI_STATUS status = file->GetInfo(file, &EFI_file_info_id, &bufferSize, buffer);
    if (status == EFI_BUFFER_TOO_SMALL) {
        free_pool(buffer);
        // bufferSize has now been updated:
        buffer = (EFI_FILE_INFO *) alloc_pool(bufferSize);
        if (buffer == nullptr) {
            return nullptr;
        }

        status = file->GetInfo(file, &EFI_file_info_id, &bufferSize, buffer);
    }

    if (EFI_ERROR(status)) {
        free_pool(buffer);
        return nullptr;
    }

    return buffer;
}

#endif /* INCLUDED_TOSAITHE_UTIL_H */
