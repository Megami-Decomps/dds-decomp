#ifndef EVT_WORLD_SOURCE_TRANSFORM_H
#define EVT_WORLD_SOURCE_TRANSFORM_H

#include "eff_transform.h"

/* Borrowed 0x20-byte prefix used by kind-0x11 world-node data. Consumers
 * transfer the position and rotation quadwords; the position's w lane is
 * initialized to 1.0f by its constructor. This view does not describe the
 * backing allocation's extent or lifetime. */
typedef struct EvtWorldSourcePosition {
    f32 x;
    f32 y;
    f32 z;
    f32 w;
} EvtWorldSourcePosition;

typedef struct EvtWorldSourceTransformPrefix {
    EvtWorldSourcePosition position; /* 0x00 */
    f32 rotation[4];                 /* 0x10 */
} EvtWorldSourceTransformPrefix;

typedef char EvtWorldSourcePosition_size_must_be_0x10[(sizeof(EvtWorldSourcePosition) == 0x10) ? 1 : -1];
typedef char EvtWorldSourceTransformPrefix_size_must_be_0x20[(sizeof(EvtWorldSourceTransformPrefix) == 0x20) ? 1 : -1];

EffWorldNode *evtSpawnActionObj11(s32 key, EvtWorldSourceTransformPrefix *source, s32 value);

#endif
