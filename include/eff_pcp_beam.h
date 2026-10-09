#ifndef EFF_PCP_BEAM_H
#define EFF_PCP_BEAM_H

#include "common.h"

struct SdfAsset;
struct SdfMemBlock;

/* One beam draw node owns its asset and vertex allocation separately. */
typedef struct EffPCPBeamNode {
    f32 matrix[4][4];
    f32 localMatrix[4][4];
    u128 position;
    f32 scale;
    u32 drawKind; /* Indexes the node draw-dispatch table. */
    u32 color;
    u32 vertexCount;
    f32 *points;
    u32 *colors;
    struct SdfAsset *assetHandle;
    struct SdfMemBlock *allocationHandle;
} EffPCPBeamNode;

/* The parameter prefix copied into the standard beam work. */
typedef struct EffPCPBeamParams {
    f32 position[4];
    s32 fadeInFrames;
    s32 fadeOutFrames;
    s32 holdFrames;
    s32 vertexGrowthFrames;
    u8 pad20[4];
    s32 radiusGrowthFrames;
    f32 unk28;
    f32 endRadius;
    u32 segments;
    u32 drawKind;
    f32 firstWidth;
    u32 firstColor;
    f32 middleWidth;
    u32 middleColor;
    f32 lastWidth;
    u32 lastColor;
} EffPCPBeamParams;

typedef struct EffPCPBeamWork {
    EffPCPBeamParams params;
    s32 frame;
    u32 color;
    u32 vertexCount;
    EffPCPBeamNode *node;
} EffPCPBeamWork;

/* The copied 0x5C input head for the linked/ring beam variant. */
typedef struct EffPCPBeamLargeHead {
    f32 pos[4];
    f32 degreesA;
    f32 degreesB;
    f32 stepDegrees;
    f32 rotationDecay;
    u8 mode;
    u8 pad21[3];
    s32 duration;
    s32 fadeIn;
    s32 fadeOut;
    f32 initialRadius;
    f32 initialRadiusStep;
    f32 radiusDecay;
    u32 segments;
    u32 drawKind;
    f32 radiusStepA;
    u32 firstColor;
    f32 radiusStepB;
    u32 middleColor;
    f32 radiusStepC;
    u32 lastColor;
} EffPCPBeamLargeHead;

typedef struct EffPCPBeamLargeWork {
    EffPCPBeamLargeHead head;
    s32 frame;
    u32 color;
    f32 rotationA;
    f32 rotationB;
    f32 rotationStep;
    u8 pad70[4];
    f32 radius;
    f32 radiusStep;
    EffPCPBeamNode *node;
} EffPCPBeamLargeWork;

void effPcpBuildConcentricRingPoints(EffPCPBeamLargeWork *work, f32 radius);

typedef char EffPCPBeamNode_size_must_be_0xB0[
    (sizeof(EffPCPBeamNode) == 0xB0) ? 1 : -1];
typedef char EffPCPBeamNode_asset_offset_must_be_0xA8[
    ((u32)&((EffPCPBeamNode *)0)->assetHandle == 0xA8) ? 1 : -1];
typedef char EffPCPBeamNode_allocation_offset_must_be_0xAC[
    ((u32)&((EffPCPBeamNode *)0)->allocationHandle == 0xAC) ? 1 : -1];
typedef char EffPCPBeamParams_size_must_be_0x50[
    (sizeof(EffPCPBeamParams) == 0x50) ? 1 : -1];
typedef char EffPCPBeamParams_segments_offset_must_be_0x30[
    ((u32)&((EffPCPBeamParams *)0)->segments == 0x30) ? 1 : -1];
typedef char EffPCPBeamWork_size_must_be_0x60[
    (sizeof(EffPCPBeamWork) == 0x60) ? 1 : -1];
typedef char EffPCPBeamWork_node_offset_must_be_0x5C[
    ((u32)&((EffPCPBeamWork *)0)->node == 0x5C) ? 1 : -1];
typedef char EffPCPBeamLargeHead_size_must_be_0x5C[
    (sizeof(EffPCPBeamLargeHead) == 0x5C) ? 1 : -1];
typedef char EffPCPBeamLargeHead_lastColor_offset_must_be_0x58[
    ((u32)&((EffPCPBeamLargeHead *)0)->lastColor == 0x58) ? 1 : -1];
typedef char EffPCPBeamLargeWork_size_must_be_0x80[
    (sizeof(EffPCPBeamLargeWork) == 0x80) ? 1 : -1];
typedef char EffPCPBeamLargeWork_node_offset_must_be_0x7C[
    ((u32)&((EffPCPBeamLargeWork *)0)->node == 0x7C) ? 1 : -1];

#endif
