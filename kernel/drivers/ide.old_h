#pragma once
#include <drivers/fat_types.h>
#include <stdbool.h>
#include <stdint.h>

typedef enum {
	DISK_HD_ATA
} disk_e;

typedef struct {
    unsigned int minor;
    bool initialized;
} hd_ata;

class IDE {
public:
    IDE(disk_e disk = DISK_HD_ATA);
    
    u8 initialize();
    u8 status();
    u8 read(u8* buffer, u32 lba, u32 count);
    u8 write(const u8* buffer, u32 lba, u32 count);

    disk_e disk();

private:
    disk_e mDisk;
    hd_ata mHdSlot;
};