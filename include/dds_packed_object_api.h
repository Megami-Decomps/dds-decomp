#ifndef DDS_PACKED_OBJECT_API_H
#define DDS_PACKED_OBJECT_API_H

#include "mnu_callback_list.h"

/* Packed offsets use the segment index in bits 16-31 and byte offset in 0-15. */
typedef u32 DdsPackedOffset;

/* Owner of packed segments and the two named-reference lists. */
typedef struct DdsPackedObject {
    DdsPackedOffset maxPackedOffset;
    MnuCallbackList *segments;
    MnuCallbackList *namedReferences;
    MnuCallbackList *pendingReferences;
} DdsPackedObject;

typedef char DdsPackedObject_size_must_be_0x10[(sizeof(DdsPackedObject) == 0x10) ? 1 : -1];
typedef char DdsPackedObject_segments_offset_must_be_4[
    ((u32)&((DdsPackedObject *)0)->segments == 4) ? 1 : -1];
typedef char DdsPackedObject_namedReferences_offset_must_be_8[
    ((u32)&((DdsPackedObject *)0)->namedReferences == 8) ? 1 : -1];
typedef char DdsPackedObject_pendingReferences_offset_must_be_C[
    ((u32)&((DdsPackedObject *)0)->pendingReferences == 0xC) ? 1 : -1];

DdsPackedObject *dds3PackWordArrayRecords(u32 **source);
DdsPackedObject *dds3PackNestedResourceRecords(MnuCallbackList **source);
DdsPackedObject *dds3PackSpriteRecords(u32 **source);
DdsPackedObject *dds3PackExtendedSpriteRecords(u32 **source);
s32 dds3ApplyNamedRelocations(DdsPackedObject *object);
DdsPackedObject *dds3WritePendingNamedReferenceValues(DdsPackedObject *object);

#endif
