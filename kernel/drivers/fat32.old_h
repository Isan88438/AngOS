#ifndef FAT32_H
#define FAT32_H

#include "fat_types.h"
#include "ide.h"

class Fat32 {
public:
	Fat32(IDE& ide);

	void mount();

private:
	u8 disk_mount(IDE& disk);
	u8 disk_eject(disk_e disk);

	IDE& mIde;
};


#endif
