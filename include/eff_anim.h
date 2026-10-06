#ifndef EFF_ANIM_H
#define EFF_ANIM_H

#include "common.h"

/* Animation segments contribute length frames plus one boundary frame. */
typedef struct EffAnimSegment {
    u32 length;     // 0x00
    u8 pad_04[0xC];
} EffAnimSegment;

typedef struct EffAnimSet {
    u8 pad_00[4];
    u32 count;                 // 0x04
    u32 flags;                 // 0x08: 1 loop, 4 double Y scale
    u8 pad_0C[4];
    EffAnimSegment *segments;  // 0x10
    void **handles;            // 0x14
    u32 length;                // 0x18
    u32 refCount;              // 0x1C
    s32 unk20;                 // 0x20: initialized to -1 by the container constructor
    u32 allocation;            // 0x24: retained block released with the final reference
} EffAnimSet;

typedef char EffAnimSet_size_must_be_0x28[
    (sizeof(EffAnimSet) == 0x28) ? 1 : -1];

typedef struct EffAnimSample {
    f32 scaleX;     // 0x00
    f32 scaleY;     // 0x04
    f32 angle;      // 0x08
    s32 segment;    // 0x0C
} EffAnimSample;

void effSampleAnimSet(EffAnimSet *set, u32 frame, EffAnimSample *out);
struct SdfTex;
struct SdfTex *effAssignSampledSegmentReference(EffAnimSet *set, void *target, const EffAnimSample *sample);

#endif /* EFF_ANIM_H */
