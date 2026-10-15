#ifndef PAR_KIND_API_H
#define PAR_KIND_API_H

#include "common.h"

struct ParKindState;

/* Halfword tag at ParKindState +0 selects one of its +8/+0x10/+0x14 views. */
enum ParKindStateKind {
    PAR_KIND_STATE_HISTORY = 1,
    PAR_KIND_STATE_BILLBOARD_CROSS_STRIP = 2,
    PAR_KIND_STATE_BILLBOARD_CROSS_TRIANGLE = 3,
    PAR_KIND_STATE_TRACK_POLYGON_CROSS_AXES = 4
};

/* Resource-backed objects clone their billboard when instantiated. */
enum { PAR_BILLBOARD_CLONE_FROM_RESOURCE = -1 };

void parUpdateSharedScaleAndDelta(struct ParKindState *state);
void parDispatchKindInit(struct ParKindState *state, s32 index);
void parDispatchKindUpdate(struct ParKindState *state, s32 index, u32 color, f32 speed);

#endif
