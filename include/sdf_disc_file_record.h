#ifndef SDF_DISC_FILE_RECORD_H
#define SDF_DISC_FILE_RECORD_H

#include "common.h"

/* Shared address returned by the disc path lookup. Only the third word has
 * an established meaning in the reconstructed callers. */
typedef struct SdfDiscFileRecord {
    u32 field0;
    u32 field4;
    s32 size;
} SdfDiscFileRecord;

typedef char SdfDiscFileRecordSizeCheck[
    sizeof(SdfDiscFileRecord) == 0x0C ? 1 : -1];

/* Both lookup entry points return the shared 0x0C-byte record. */
SdfDiscFileRecord *sdfDevFindDiscFileRecordByPath(u32 pathAddress, u8 *sectorOut, u32 flagAddress);
SdfDiscFileRecord *sdfDevFindDiscFileRecordByCdromPath(u32 pathAddress, u8 *sectorOut, u32 flagAddress);

#endif
