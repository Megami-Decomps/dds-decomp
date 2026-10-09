#ifndef DDS_NESTED_RESOURCE_H
#define DDS_NESTED_RESOURCE_H

#include "common.h"

typedef struct DdsCountedPayload {
    u32 count;
    void *data;
} DdsCountedPayload;

typedef struct DdsNestedGroup {
    u32 unk_00;
    u16 firstCount;
    u16 secondCount;
    DdsCountedPayload *first;
    DdsCountedPayload *second;
} DdsNestedGroup;

typedef struct DdsNestedHeader {
    u32 unk_00;
    u32 unk_04;
    u32 unk_08;
    u16 unk_0C;
    u16 groupCount;
    DdsNestedGroup *groups;
} DdsNestedHeader;

typedef char DdsNestedResourceLayoutAssert[
    (sizeof(DdsCountedPayload) == 8 &&
     (unsigned long)&((DdsCountedPayload *)0)->count == 0 &&
     (unsigned long)&((DdsCountedPayload *)0)->data == 4 &&
     sizeof(DdsNestedGroup) == 0x10 &&
     (unsigned long)&((DdsNestedGroup *)0)->unk_00 == 0 &&
     (unsigned long)&((DdsNestedGroup *)0)->firstCount == 4 &&
     (unsigned long)&((DdsNestedGroup *)0)->secondCount == 6 &&
     (unsigned long)&((DdsNestedGroup *)0)->first == 8 &&
     (unsigned long)&((DdsNestedGroup *)0)->second == 0xC &&
     sizeof(DdsNestedHeader) == 0x14 &&
     (unsigned long)&((DdsNestedHeader *)0)->unk_00 == 0 &&
     (unsigned long)&((DdsNestedHeader *)0)->unk_04 == 4 &&
     (unsigned long)&((DdsNestedHeader *)0)->unk_08 == 8 &&
     (unsigned long)&((DdsNestedHeader *)0)->unk_0C == 0xC &&
     (unsigned long)&((DdsNestedHeader *)0)->groupCount == 0xE &&
     (unsigned long)&((DdsNestedHeader *)0)->groups == 0x10) ? 1 : -1];

/* Measures one group header and its two counted arrays of eight-byte records. */
s32 dds3MeasureMenuRecord(DdsNestedGroup *group);

#endif /* DDS_NESTED_RESOURCE_H */
