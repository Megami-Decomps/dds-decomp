#ifndef EFF_MODEL_RESOURCE_H
#define EFF_MODEL_RESOURCE_H

#include "common.h"
#include "mdl.h"

/* An effect's model resource: its placement, color, model context and span configuration. */
typedef struct EffModelResource {
    f32 position[4];
    f32 orientation[4]; /* Quaternion. */
    f32 scale;
    u32 color;
    s32 updateCount;
    s32 kind;
    MdlCtx *model;
    u32 attributes;
    struct EffSpanTable *childResource;
    struct EffSpanConfig *source;
} EffModelResource;

#endif
