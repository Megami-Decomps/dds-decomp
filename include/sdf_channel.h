#ifndef SDF_CHANNEL_H
#define SDF_CHANNEL_H

#include "common.h"

/* Each configured segment supplies a destination slot and an opaque word. */
typedef struct SdfChannelEntry {
    void **slot;
    u32 unused;
} SdfChannelEntry;

/* The channel loader scans these entries through the first null slot. */
typedef struct SdfChannel {
    const char *path;
    SdfChannelEntry entries[20];
} SdfChannel;

typedef char SdfChannelEntry_size_must_be_8[(sizeof(SdfChannelEntry) == 8) ? 1 : -1];
typedef char SdfChannelEntry_unused_offset_must_be_4[
    ((u32)&((SdfChannelEntry *)0)->unused == 4) ? 1 : -1];
typedef char SdfChannel_entries_offset_must_be_4[
    ((u32)&((SdfChannel *)0)->entries == 4) ? 1 : -1];
typedef char SdfChannel_size_must_be_0xA4[(sizeof(SdfChannel) == 0xA4) ? 1 : -1];

#endif /* SDF_CHANNEL_H */
