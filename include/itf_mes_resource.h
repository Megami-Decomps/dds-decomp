#ifndef ITF_MES_RESOURCE_H
#define ITF_MES_RESOURCE_H

#include "common.h"

/* Fixed relocation-header view; the resource's full trailing extent is external. */
typedef struct ItfMesRelocHeader {
    u8 unknown00[8];
    u32 magic;
    u8 unknown0C[4];
    s32 fixupTableOffset;
    s32 fixupTableBytes;
    u8 unknown18[4];
    u8 relocated;
    u8 unknown1D[3];
    u8 payload[1];
} ItfMesRelocHeader;

/* Message tables contain relocated encoded-text addresses. */
typedef struct ItfMesTable {
    u8 unk0[0x18];
    s16 count;
    s16 bitCount;
    u32 items[1];
} ItfMesTable;

typedef struct ItfMesEntry {
    /* Relocated item-address table; address 1 marks a branch entry. */
    u32 *itemList;
    ItfMesTable *table;
} ItfMesEntry;
typedef char ItfMesEntry_size_must_be_0x8[
    sizeof(ItfMesEntry) == 0x8 ? 1 : -1];
typedef char ItfMesEntry_table_offset_must_be_0x4[
    ((u32)&((ItfMesEntry *)0)->table == 0x4) ? 1 : -1];

typedef struct ItfMesSub {
    u8 unk0[8];
    u32 magic;
    u8 unkC[0xC];
    u32 entryCount;
    u8 unk1C[4];
    ItfMesEntry entries[1];
} ItfMesSub;

#endif
