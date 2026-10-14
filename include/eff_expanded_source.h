#ifndef EFF_EXPANDED_SOURCE_H
#define EFF_EXPANDED_SOURCE_H

#include "eff_expanded_list.h"

/* Serialized texture-animation list header, count entry records, then payloads.
 * flags is copied to EffExpandedList and tested by animation sampling. */
typedef struct EffExpandedSource {
    u8 pad00[4];
    u32 count;
    u32 flags;
    u8 pad0C[4];
    EffExpandedEntry entries[1];
} EffExpandedSource;

typedef char EffExpandedSource_count_offset_check[
    (u32)&((EffExpandedSource *)0)->count == 0x04 ? 1 : -1];
typedef char EffExpandedSource_flags_offset_check[
    (u32)&((EffExpandedSource *)0)->flags == 0x08 ? 1 : -1];
typedef char EffExpandedSource_entries_offset_check[
    (u32)&((EffExpandedSource *)0)->entries == 0x10 ? 1 : -1];
typedef char EffExpandedSource_size_check[
    sizeof(EffExpandedSource) == 0x20 ? 1 : -1];

#endif
