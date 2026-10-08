#ifndef EFF_EXPANDED_LIST_H
#define EFF_EXPANDED_LIST_H

#include "common.h"

struct RefObj;
struct SdfMemBlock;

/* Each serialized entry expands to itself and additionalCount children.
 * payloadSize advances through the entry payloads during construction. */
typedef struct EffExpandedEntry {
    s32 additionalCount;
    u32 payloadSize;
    u8 unk08[8];
} EffExpandedEntry;

/* Kind-7 wrapper owns one allocation and retains a RefObj per entry. */
typedef struct EffExpandedList {
    u8 unk00[4];
    u32 count;
    u8 unk08[8];
    EffExpandedEntry *entries;
    struct RefObj **handles;
    s32 totalEntryCount;
    u32 refCount;
    s32 unk20;
    struct SdfMemBlock *allocation;
} EffExpandedList;

typedef char EffExpandedEntrySizeCheck[(sizeof(EffExpandedEntry) == 0x10) ? 1 : -1];
typedef char EffExpandedListSizeCheck[(sizeof(EffExpandedList) == 0x28) ? 1 : -1];
typedef char EffExpandedAllocationOffsetCheck[((u32)&((EffExpandedList *)0)->allocation == 0x24) ? 1 : -1];

s32 effCountExpandedEntries(EffExpandedList *list);
EffExpandedList *effReferenceObjectRetain(EffExpandedList *obj);
void effReleaseReferenceHolder(EffExpandedList *holder);

#endif
