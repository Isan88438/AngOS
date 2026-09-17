#include "tosaithe-util.h"

#include <uefi-devicepath.h>
#include <uefi-media-file.h>

extern EFI_SYSTEM_TABLE *EST;

EFI_FILE_PROTOCOL *open_file(
    const EFI_DEVICE_PATH_PROTOCOL *dev_path,
    open_file_error *error)
{
    if (error != nullptr) {
        error->reason = open_file_error::NO_FSPROTOCOL_FOR_DEV_PATH;
        error->status = EFI_SUCCESS;
    }

    EFI_DEVICE_PATH_PROTOCOL *file_devpath =
        const_cast<EFI_DEVICE_PATH_PROTOCOL *>(dev_path);

    EFI_HANDLE load_dev_hndl;
    EFI_STATUS status =
        EBS->LocateDevicePath(
            &EFI_simple_file_system_protocol_guid,
            &file_devpath,
            &load_dev_hndl);

    if (EFI_ERROR(status)) {
        if (error != nullptr) {
            error->reason = open_file_error::NO_FSPROTOCOL_FOR_DEV_PATH;
            error->status = status;
        }
        return nullptr;
    }

    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *sfs_protocol = nullptr;

    status =
        EBS->HandleProtocol(
            load_dev_hndl,
            &EFI_simple_file_system_protocol_guid,
            (void **)&sfs_protocol);

    if (EFI_ERROR(status) || sfs_protocol == nullptr) {
        if (error != nullptr) {
            error->reason = open_file_error::NO_FSPROTOCOL_FOR_DEV_PATH;
            error->status = status;
        }
        return nullptr;
    }

    EFI_FILE_PROTOCOL *fs_root = nullptr;

    status = sfs_protocol->OpenVolume(sfs_protocol, &fs_root);

    if (EFI_ERROR(status) || fs_root == nullptr) {
        if (error != nullptr) {
            error->reason = open_file_error::CANNOT_OPEN_VOLUME;
            error->status = status;
        }
        return nullptr;
    }

    EFI_DEVICE_PATH_TO_TEXT_PROTOCOL *dp2text_proto =
        (EFI_DEVICE_PATH_TO_TEXT_PROTOCOL *)
        locate_protocol(EFI_device_path_to_text_protocol_guid);

    if (dp2text_proto == nullptr) {
        fs_root->Close(fs_root);

        if (error != nullptr) {
            error->reason = open_file_error::NO_DPTT_PROTOCOL;
            error->status = EFI_UNSUPPORTED;
        }

        return nullptr;
    }

    CHAR16 *file_path =
        dp2text_proto->ConvertDevicePathToText(
            file_devpath,
            false,
            false);

    if (file_path == nullptr) {
        fs_root->Close(fs_root);

        if (error != nullptr) {
            error->reason = open_file_error::CANNOT_OPEN_FILE;
            error->status = EFI_OUT_OF_RESOURCES;
        }

        return nullptr;
    }

    EFI_FILE_PROTOCOL *file_to_load = nullptr;

    status =
        fs_root->Open(
            fs_root,
            &file_to_load,
            file_path,
            EFI_FILE_MODE_READ,
            0);

    free_pool(file_path);
    fs_root->Close(fs_root);

    if (EFI_ERROR(status) || file_to_load == nullptr) {
        if (error != nullptr) {
            error->reason = open_file_error::CANNOT_OPEN_FILE;
            error->status = status;
        }
        return nullptr;
    }

    return file_to_load;
}

EFI_GRAPHICS_OUTPUT_PROTOCOL *find_GOP()
{
    // First try looking up via ConOut handle:
    {
        void *gop_proto = nullptr;
        EFI_STATUS status = EBS->HandleProtocol(&EST->ConsoleOutHandle,
                &EFI_graphics_output_protocol_guid, &gop_proto);
        if (status == EFI_SUCCESS) {
            return (EFI_GRAPHICS_OUTPUT_PROTOCOL *)gop_proto;
        }
    }

    UINTN conout_var_size = 0;
    EFI_STATUS status = EST->RuntimeServices->GetVariable(L"ConOutDev", &EFI_global_variable_guid,
            nullptr, &conout_var_size, nullptr);
    if (EFI_ERROR(status) && status != EFI_BUFFER_TOO_SMALL)
        goto fallback_locate;

    {
        void *conout_var_val = alloc_pool(conout_var_size);
        if (conout_var_val == nullptr)
            goto fallback_locate;

        status = EST->RuntimeServices->GetVariable(L"ConOutDev", &EFI_global_variable_guid, nullptr,
                &conout_var_size, conout_var_val);
        if (EFI_ERROR(status)) {
            free_pool(conout_var_val);
            goto fallback_locate;
        }

        EFI_HANDLE gop_hndl = nullptr;
        EFI_DEVICE_PATH_PROTOCOL *con_out_devpath =
            (EFI_DEVICE_PATH_PROTOCOL *)conout_var_val;

        while (true) {
            status = EBS->LocateDevicePath(&EFI_graphics_output_protocol_guid, &con_out_devpath, &gop_hndl);
            if (status == EFI_SUCCESS) {
                void *gop_proto = nullptr;
                EBS->HandleProtocol(gop_hndl, &EFI_graphics_output_protocol_guid, &gop_proto);
                // (^^ Shouldn't fail, but if it does, we'll return null anyway)
                free_pool(conout_var_val);
                return (EFI_GRAPHICS_OUTPUT_PROTOCOL *)gop_proto;
            }

            con_out_devpath = find_next_devpath_instance(con_out_devpath);
        }
    }

    fallback_locate:

    // fall back to locate_protocol:
    return (EFI_GRAPHICS_OUTPUT_PROTOCOL *) locate_protocol(EFI_graphics_output_protocol_guid);;
}
