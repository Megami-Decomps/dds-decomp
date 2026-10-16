#ifndef DDS_POLY_NODE_H
#define DDS_POLY_NODE_H

#include "common.h"
#include "par_cell_api.h"

/* Basic radial-fan state shared by the paired polyManager implementations.
 * Its fixed header uses the same serialized field positions as PolyRingHead;
 * the clone path writes headerSize at +0x18 and the consumers use the fields
 * from +0xC0 onward. */
typedef struct PolyNode {
    f32 origin[4];
    u16 entryCount;
    u8 pad12[2];
    s32 duration;
    s32 headerSize;
    u8 pad1C[4];
    f32 matrix[16];
    u32 color;
    u8 templateLoop;
    u8 pad65;
    u16 drawBucket;
    u32 startColorRampFrames;
    u32 endColorRampFrames;
    f32 matrixCopy[16];
    u8 padB0[2];
    u16 active;
    u8 padB4[0xC];
    u8 loop;
    u8 padC1[3];
    s32 spawnDelayStep;
    u16 segments;
    u8 padCA[2];
    f32 radius;
    f32 pairDisplacement;
    u8 padD4[8];
    ParSystem *strip;
    s32 *ages;
} PolyNode;

typedef char PolyNode_size_must_be_0xE4[(sizeof(PolyNode) == 0xE4) ? 1 : -1];
typedef char PolyNode_matrix_offset_must_be_0x20[
    ((u32)&((PolyNode *)0)->matrix == 0x20) ? 1 : -1];
typedef char PolyNode_headerSize_offset_must_be_0x18[
    ((u32)&((PolyNode *)0)->headerSize == 0x18) ? 1 : -1];
typedef char PolyNode_color_offset_must_be_0x60[
    ((u32)&((PolyNode *)0)->color == 0x60) ? 1 : -1];
typedef char PolyNode_templateLoop_offset_must_be_0x64[
    ((u32)&((PolyNode *)0)->templateLoop == 0x64) ? 1 : -1];
typedef char PolyNode_drawBucket_offset_must_be_0x66[
    ((u32)&((PolyNode *)0)->drawBucket == 0x66) ? 1 : -1];
typedef char PolyNode_startColorRampFrames_offset_must_be_0x68[
    ((u32)&((PolyNode *)0)->startColorRampFrames == 0x68) ? 1 : -1];
typedef char PolyNode_endColorRampFrames_offset_must_be_0x6C[
    ((u32)&((PolyNode *)0)->endColorRampFrames == 0x6C) ? 1 : -1];
typedef char PolyNode_matrixCopy_offset_must_be_0x70[
    ((u32)&((PolyNode *)0)->matrixCopy == 0x70) ? 1 : -1];
typedef char PolyNode_active_offset_must_be_0xB2[
    ((u32)&((PolyNode *)0)->active == 0xB2) ? 1 : -1];
typedef char PolyNode_loop_offset_must_be_0xC0[
    ((u32)&((PolyNode *)0)->loop == 0xC0) ? 1 : -1];
typedef char PolyNode_strip_offset_must_be_0xDC[
    ((u32)&((PolyNode *)0)->strip == 0xDC) ? 1 : -1];
typedef char PolyNode_ages_offset_must_be_0xE0[
    ((u32)&((PolyNode *)0)->ages == 0xE0) ? 1 : -1];

#endif
