#ifndef EFF_ANIM_H
#define EFF_ANIM_H

#include "eff_expanded_list.h"

typedef struct EffAnimSample {
    f32 scaleX;     // 0x00
    f32 scaleY;     // 0x04
    f32 angle;      // 0x08
    s32 segment;    // 0x0C
} EffAnimSample;

void effSampleAnimSet(EffExpandedList *set, u32 frame, EffAnimSample *out);
struct SdfTex;
struct SdfTex *effAssignSampledSegmentReference(EffExpandedList *set, void *target, const EffAnimSample *sample);

#endif /* EFF_ANIM_H */
