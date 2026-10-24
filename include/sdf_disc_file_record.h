#ifndef SDF_DISC_FILE_RECORD_H
#define SDF_DISC_FILE_RECORD_H

#include "common.h"

/* Entry in the relative-offset disc index. Directories use a negative size
 * whose magnitude is their child count; files use a nonnegative byte size. */
typedef struct SdfDiscFileRecord {
    u32 nameOffset; /* 0x00: string offset from the loaded index base */
    u32 dataOffset; /* 0x04: child records for directories, file data for files */
    s32 sizeOrNegativeChildCount; /* 0x08: file bytes or negative directory entry count */
} SdfDiscFileRecord;

typedef char SdfDiscFileRecordSizeCheck[
    sizeof(SdfDiscFileRecord) == 0x0C ? 1 : -1];

/* Both lookup entry points return the shared 0x0C-byte record. */
SdfDiscFileRecord *sdfCdImageSearchFile(u32 pathAddress, u8 *sectorOut, u32 flagAddress);
SdfDiscFileRecord *sdfDevFindDiscFileRecordByCdromPath(u32 pathAddress, u8 *sectorOut, u32 flagAddress);

#endif
