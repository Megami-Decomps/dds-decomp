#ifndef PAR_KIND_API_H
#define PAR_KIND_API_H

#include "common.h"

struct ParKindState;

void parUpdateSharedScaleAndDelta(struct ParKindState *state);
void parDispatchKindInit(struct ParKindState *state, s32 index);
void parDispatchKindUpdate(struct ParKindState *state, s32 index, u32 color, f32 speed);

#endif
