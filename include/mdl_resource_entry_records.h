#ifndef MDL_RESOURCE_ENTRY_RECORDS_H
#define MDL_RESOURCE_ENTRY_RECORDS_H

#include "common.h"

/* Specialized viewer-record views; unconsumed prefix bytes stay opaque. */
typedef struct MdlPartRec {
    u8 opaque00[4];
    u32 size;          /* 0x04: checked against the optional anchor-scale extent */
    s32 firstId;       /* 0x08 */
    u16 count;         /* 0x0C */
    u16 partIndex;     /* 0x0E */
    f32 anchorScale;   /* 0x10: read when size >= 0x11 */
} MdlPartRec;

typedef char MdlPartRec_size[(sizeof(MdlPartRec) == 0x14) ? 1 : -1];
typedef char MdlPartRec_size_offset[((u32)&((MdlPartRec *)0)->size == 0x04) ? 1 : -1];
typedef char MdlPartRec_first_id_offset[((u32)&((MdlPartRec *)0)->firstId == 0x08) ? 1 : -1];
typedef char MdlPartRec_count_offset[((u32)&((MdlPartRec *)0)->count == 0x0C) ? 1 : -1];
typedef char MdlPartRec_part_index_offset[((u32)&((MdlPartRec *)0)->partIndex == 0x0E) ? 1 : -1];
typedef char MdlPartRec_anchor_scale_offset[((u32)&((MdlPartRec *)0)->anchorScale == 0x10) ? 1 : -1];

typedef struct MdlEntryRec {
    u8 opaque00[8];
    s32 dataId;        /* 0x08: resource-key lookup */
    s32 minimumTime;   /* 0x0C: deferred initialization threshold */
    u8 flagB;          /* 0x10 */
    u8 flagA;          /* 0x11 */
    u16 index;         /* 0x12: part-slot index */
} MdlEntryRec;

typedef char MdlEntryRec_size[(sizeof(MdlEntryRec) == 0x14) ? 1 : -1];
typedef char MdlEntryRec_data_id_offset[((u32)&((MdlEntryRec *)0)->dataId == 0x08) ? 1 : -1];
typedef char MdlEntryRec_minimum_time_offset[((u32)&((MdlEntryRec *)0)->minimumTime == 0x0C) ? 1 : -1];
typedef char MdlEntryRec_flag_b_offset[((u32)&((MdlEntryRec *)0)->flagB == 0x10) ? 1 : -1];
typedef char MdlEntryRec_flag_a_offset[((u32)&((MdlEntryRec *)0)->flagA == 0x11) ? 1 : -1];
typedef char MdlEntryRec_index_offset[((u32)&((MdlEntryRec *)0)->index == 0x12) ? 1 : -1];

#endif /* MDL_RESOURCE_ENTRY_RECORDS_H */
