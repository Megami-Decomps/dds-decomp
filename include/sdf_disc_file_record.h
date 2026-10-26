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

/* File reads distinguish an address in the preloaded image from a CD stream. */
typedef u8 SdfDiscReadMode;
enum {
    SDF_DISC_READ_MODE_INDEXED_IMAGE = 0,
    SDF_DISC_READ_MODE_CDROM_STREAM = 1
};

/* Both lookups return the shared 0x0C-byte record. On success they write the
 * 32-bit read location (image address or CD sector); readModeOut is optional. */
SdfDiscFileRecord *sdfCdImageSearchFile(
    const char *path, u32 *readLocationOut, SdfDiscReadMode *readModeOut);
SdfDiscFileRecord *sdfDevFindDiscFileRecordByCdromPath(
    const char *path, u32 *readLocationOut, SdfDiscReadMode *readModeOut);

#endif
