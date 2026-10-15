#ifndef DDS_POLY_NODE_H
#define DDS_POLY_NODE_H

#include "common.h"
#include "par_cell_api.h"

/* Basic radial-fan state shared by the paired polyManager implementations.
 * The byte spans whose uses are not established remain opaque. */
typedef struct PolyNode {
    f32 origin[4];
    u16 entryCount;
    u8 pad12[2];
    s32 duration;
    u8 pad18[8];
    f32 matrix[16];
    u8 pad60[0x52];
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
typedef char PolyNode_active_offset_must_be_0xB2[
    ((u32)&((PolyNode *)0)->active == 0xB2) ? 1 : -1];
typedef char PolyNode_loop_offset_must_be_0xC0[
    ((u32)&((PolyNode *)0)->loop == 0xC0) ? 1 : -1];
typedef char PolyNode_strip_offset_must_be_0xDC[
    ((u32)&((PolyNode *)0)->strip == 0xDC) ? 1 : -1];
typedef char PolyNode_ages_offset_must_be_0xE0[
    ((u32)&((PolyNode *)0)->ages == 0xE0) ? 1 : -1];

#endif
