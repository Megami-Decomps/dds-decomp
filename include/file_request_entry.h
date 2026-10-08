#ifndef FILE_REQUEST_ENTRY_H
#define FILE_REQUEST_ENTRY_H

#include "common.h"

/* Request-indexed memory-card state, one 0x64-byte entry per request. */
typedef struct FileReqEntry {
    u32 unk0;      /* 0x00 */
    u32 unk4;      /* 0x04 */
    u32 sizeKiB;   /* 0x08: fileReqGetSize converts this to bytes */
    u32 unkC;      /* 0x0C */
    u8 unk10;      /* 0x10 */
    u8 status;     /* 0x11: inspected by memory-card request polling */
    u8 slotMetadataDirty; /* 0x12 */
    s8 selectedSlot; /* 0x13 */
    u32 slotFlags[20]; /* 0x14: includes the twenty per-request slot words */
} FileReqEntry;

typedef char FileReqEntrySizeCheck[
    (sizeof(FileReqEntry) == 0x64) ? 1 : -1];
typedef char FileReqEntrySlotFlagsOffsetCheck[
    ((u32)&((FileReqEntry *)0)->slotFlags == 0x14) ? 1 : -1];

/* The flat flag view aliases entry slotFlags at +0x14; the index remains a
 * request/slot word offset, and the request-table extent is not specified. */
extern FileReqEntry fileRequestEntries[];
extern u32 fileRequestSlotFlags[];

#endif
