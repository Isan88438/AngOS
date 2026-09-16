#include <uefi.h>
#include <uefi-loadedimage.h>
#include <uefi-devicepath.h>
#include <uefi-media-file.h>

#include "tosaithe-util.h"

static void copy_memory(void *dst, const void *src, UINTN size)
{
    unsigned char *d = (unsigned char *)dst;
    const unsigned char *s = (const unsigned char *)src;

    while (size--) {
        *d++ = *s++;
    }
}

EFI_BOOT_SERVICES *EBS;
EFI_SYSTEM_TABLE *EST;
EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *EFI_con_out;

EFI_STATUS load_tsbp(
    EFI_HANDLE ImageHandle,
    const EFI_DEVICE_PATH_PROTOCOL *exec_path,
    const char *cmdLine,
    UINTN ramdisk,
    UINT64 ramdisk_size
);

static EFI_DEVICE_PATH_PROTOCOL *
resolve_relative_path(EFI_HANDLE image_handle, const CHAR16 *path) {
    EFI_DEVICE_PATH_FROM_TEXT_PROTOCOL *text2dp_proto =
        (EFI_DEVICE_PATH_FROM_TEXT_PROTOCOL *)
        locate_protocol(EFI_device_path_from_text_protocol_guid);

    if (text2dp_proto == nullptr) {
        con_write(L"EFI_DEVICE_PATH_FROM_TEXT protocol unavailable.\r\n");
        return nullptr;
    }

    EFI_DEVICE_PATH_PROTOCOL *path_devpath =
        text2dp_proto->ConvertTextToDevicePath(path);

    if (path_devpath == nullptr) {
        con_write(L"Could not create kernel device path.\r\n");
        return nullptr;
    }

    if (path_devpath->Type != 0x04 ||
        path_devpath->SubType != 0x04) {
        return path_devpath;
    }

    EFI_DEVICE_PATH_PROTOCOL *image_path = nullptr;

    EFI_STATUS status =
        EBS->HandleProtocol(
            image_handle,
            &EFI_loaded_image_device_path_protocol_guid,
            (void **)&image_path
        );

    if (EFI_ERROR(status) || image_path == nullptr) {
        con_write(L"Could not obtain bootloader device path.\r\n");
        free_pool(path_devpath);
        return nullptr;
    }

    unsigned file_path_offset = find_file_path(image_path);

    if (file_path_offset == (unsigned)-1) {
        con_write(L"Bootloader device path has no filesystem path.\r\n");
        free_pool(path_devpath);
        return nullptr;
    }

    unsigned kernel_path_size = find_devpath_size(path_devpath);

    EFI_DEVICE_PATH_PROTOCOL *new_path =
        (EFI_DEVICE_PATH_PROTOCOL *)
        alloc_pool(file_path_offset + kernel_path_size);

    if (new_path == nullptr) {
        con_write(L"Out of memory creating kernel path.\r\n");
        free_pool(path_devpath);
        return nullptr;
    }

    memcpy(new_path, image_path, file_path_offset);

    memcpy(
        (char *)new_path + file_path_offset,
        path_devpath,
        kernel_path_size
    );

    free_pool(path_devpath);

    return new_path;
}

extern "C"
EFI_STATUS
EFIAPI
EfiMain(
    EFI_HANDLE ImageHandle,
    EFI_SYSTEM_TABLE *SystemTable
) {
    EST = SystemTable;
    EBS = SystemTable->BootServices;
    EFI_con_out = SystemTable->ConOut;

    con_write(L"AngOS bootloader starting...\r\n");

    EFI_DEVICE_PATH_PROTOCOL *kernel_path =
        resolve_relative_path(
            ImageHandle,
            L"\\AngOS.elf"
        );

    if (kernel_path == nullptr) {
        con_write(L"Could not find \\AngOS.elf\r\n");
        return EFI_NOT_FOUND;
    }

    con_write(L"Loading AngOS kernel...\r\n");

    EFI_STATUS status =
        load_tsbp(
            ImageHandle,
            kernel_path,
            "",
            0,
            0
        );

    free_pool(kernel_path);

    con_write(L"Kernel returned unexpectedly.\r\n");

    return status;
}
