#ifndef BTL_ACTOR_TRANSFORM_PARAMETERS_H
#define BTL_ACTOR_TRANSFORM_PARAMETERS_H

#include "common.h"

/* Shared-or-indexed table selected by btlSelectSharedOrIndexedTransformParameters. */
typedef struct BtlActorTransformParameters {
    f32 position[3]; /* Applied to the actor body offset when the override is enabled. */
    f32 overrideEnabled;
    f32 reach;
    f32 height;
} BtlActorTransformParameters;

typedef char BtlActorTransformParameters_size_must_be_0x18[
    (sizeof(BtlActorTransformParameters) == 0x18) ? 1 : -1];
typedef char BtlActorTransformParameters_override_offset_must_be_0x0C[
    ((u32)&((BtlActorTransformParameters *)0)->overrideEnabled == 0x0C) ? 1 : -1];
typedef char BtlActorTransformParameters_reach_offset_must_be_0x10[
    ((u32)&((BtlActorTransformParameters *)0)->reach == 0x10) ? 1 : -1];
typedef char BtlActorTransformParameters_height_offset_must_be_0x14[
    ((u32)&((BtlActorTransformParameters *)0)->height == 0x14) ? 1 : -1];

#endif /* BTL_ACTOR_TRANSFORM_PARAMETERS_H */
