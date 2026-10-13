#ifndef EFF_PCP_SPAN_H
#define EFF_PCP_SPAN_H

#include "common.h"
#include "eff_node_descriptor.h"

struct EffNode;

/* Serialized inputs copied in full into the orbiting aim-node work owner. */
typedef struct EffPCPSpanParams {
    f32 anchor[4];
    u32 holdFrames;
    u32 activeFrames;
    s32 fadeFrames;
    f32 sweepDegrees;
    f32 radius;
} EffPCPSpanParams;

/* Constructor allocation is 0x78 bytes and initializes this complete owner. */
typedef struct EffPCPSpanWork {
    f32 matrix[16];
    EffPCPSpanParams params;
    u32 color;
    u32 frame;
    f32 angle;
    f32 spin;
    struct EffNode *optionalHandle;
} EffPCPSpanWork;

typedef char EffPCPSpanParams_size_must_be_0x24[
    (sizeof(EffPCPSpanParams) == 0x24) ? 1 : -1];
typedef char EffPCPSpanParams_holdFrames_offset_must_be_0x10[
    ((u32)&((EffPCPSpanParams *)0)->holdFrames == 0x10) ? 1 : -1];
typedef char EffPCPSpanParams_fadeFrames_offset_must_be_0x18[
    ((u32)&((EffPCPSpanParams *)0)->fadeFrames == 0x18) ? 1 : -1];
typedef char EffPCPSpanParams_radius_offset_must_be_0x20[
    ((u32)&((EffPCPSpanParams *)0)->radius == 0x20) ? 1 : -1];
typedef char EffPCPSpanWork_size_must_be_0x78[
    (sizeof(EffPCPSpanWork) == 0x78) ? 1 : -1];
typedef char EffPCPSpanWork_params_offset_must_be_0x40[
    ((u32)&((EffPCPSpanWork *)0)->params == 0x40) ? 1 : -1];
typedef char EffPCPSpanWork_color_offset_must_be_0x64[
    ((u32)&((EffPCPSpanWork *)0)->color == 0x64) ? 1 : -1];
typedef char EffPCPSpanWork_optionalHandle_offset_must_be_0x74[
    ((u32)&((EffPCPSpanWork *)0)->optionalHandle == 0x74) ? 1 : -1];

EffPCPSpanWork *effPcpSpanCreate(void *params, EffNodeDescriptor *handleParams);

#endif /* EFF_PCP_SPAN_H */
