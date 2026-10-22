#ifndef EFF_TEXTURE_TABLE_H
#define EFF_TEXTURE_TABLE_H

struct sdfTexHandle_t;

/* Model asset-data view shared by the DDS1 and DDS2 texture upload paths. */
typedef struct EffTexTable {
    u8 opaquePrefix[0x0C];
    struct sdfTexHandle_t **entries;
} EffTexTable;

typedef char EffTexTable_entries_offset_must_be_0x0C[
    ((u32)&((EffTexTable *)0)->entries == 0x0C) ? 1 : -1];

#endif
