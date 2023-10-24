#include "../include/disk.h"
#include "../include/x86.h"
#include "../include/stdio.h"

#include <stdbool.h>

bool DISK_initialize(DISK* disk, uint8_t driveNumber) {
    uint8_t driveType;
    uint16_t cylinders, sectors, heads;

    if (!x86_disk_getDriveParams(driveNumber, &driveType, &cylinders, &sectors, &heads))
        return false;

    disk->id        = driveNumber;
    disk->cylinders = cylinders;
    disk->heads     = heads;
    disk->sectors   = sectors;

    return true;
}

void DISK_LBA2CHS(DISK* disk, uint32_t lba, uint16_t* cylinderOut, uint16_t* sectorOut, uint16_t* headOut) {
    *sectorOut      = lba % disk->sectors + 1;              // sector   = (LBA % sectors per track + 1)
    *cylinderOut    = (lba / disk->sectors) / disk->heads;  // cylinder = (LBA / sectors per track) / heads
    *headOut        = (lba / disk->sectors) % disk->heads;  // head     = (LBA / sectors per track) % heads
}

bool DISK_readSectors(DISK* disk, uint32_t lba, uint8_t sectors, void* dataOut) {
    uint16_t cylinder, sector, head;

    DISK_LBA2CHS(disk, lba, &cylinder, &sector, &head);

    for (int i = 0; i < 3; i++) {
        if (x86_diskRead(disk->id, cylinder, sector, head, sectors, dataOut))
            return true;

        x86_diskReset(disk->id);
    }

    return false;
}