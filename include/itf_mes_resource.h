#ifndef ITF_MES_RESOURCE_H
#define ITF_MES_RESOURCE_H

#include "common.h"
#include "itf_mes_entry_items_result.h"

/* Message tables contain relocated encoded-text addresses. */
typedef struct ItfMesTable {
    u8 unk0[0x18];
    s16 count;
    s16 bitCount;
    u32 items[1];
} ItfMesTable;

typedef struct ItfMesEntry {
    /* Relocated table address, or NONE / BRANCH sentinel. */
    ItfMesEntryItemsResult itemList;
    ItfMesTable *table;
} ItfMesEntry;
typedef char ItfMesEntry_size_must_be_0x8[
    sizeof(ItfMesEntry) == 0x8 ? 1 : -1];
typedef char ItfMesEntry_table_offset_must_be_0x4[
    ((u32)&((ItfMesEntry *)0)->table == 0x4) ? 1 : -1];

/* MSG1 appends a font-entry pointer after the trailing message entry. */
typedef struct ItfMesTrailingFontEntry {
    ItfMesEntry entry;
    u8 *temporaryFontEntry;
} ItfMesTrailingFontEntry;
typedef char ItfMesTrailingFontEntry_size_must_be_0xC[
    sizeof(ItfMesTrailingFontEntry) == 0xC ? 1 : -1];
typedef char ItfMesTrailingFontEntry_font_offset_must_be_0x8[
    ((u32)&((ItfMesTrailingFontEntry *)0)->temporaryFontEntry == 0x8) ? 1 : -1];

typedef struct ItfMesSub {
    u8 unk0[8];
    u32 magic;
    u8 unkC[4];
    s32 fixupTableOffset;
    s32 fixupTableBytes;
    u32 entryCount;
    u8 relocated;
    u8 unk1D[3];
    ItfMesEntry entries[1];
} ItfMesSub;

#endif
