#ifndef EFF_PCP_GROUP_SET_H
#define EFF_PCP_GROUP_SET_H

#include "eff_param.h"
#include "eff_thunder_fragment.h"

struct SdfMemBlock;

/* Serialized 0x164-byte group-set input copied into each live owner. */
typedef struct EffPCPGroupHead {
    f32 origin[4];
    u8 pad10[0x40];
    u8 unk50; /* 0x50 */
    u8 pad51[3];
    s32 framesPerEntry; /* 0x54: interpolation duration */
    u32 count; /* 0x58: handles per group */
    s32 delaySpread; /* 0x5C: random initial delay */
    s32 fadeIn; /* 0x60 */
    s32 fadeOut; /* 0x64 */
    f32 startPosition; /* 0x68: interpolated position at frame zero */
    f32 endPosition; /* 0x6C: interpolated position at final frame */
    f32 startJitter; /* 0x70: fractional random variation */
    f32 endJitter; /* 0x74: fractional random variation */
    f32 unk78;
    f32 unk7C;
    f32 unk80;
    f32 unk84;
    f32 unk88;
    u8 activeGroups[4];
    EffThunderFragmentParams spawnParams;
    u8 padE4[0x80];
} EffPCPGroupHead;

/* One 0x18-byte per-group fragment entry in the owner's trailing allocation. */
typedef struct EffPCPGroupEntry {
    EffThunderFragmentWork *handle; /* 0x00 */
    s32 frame; /* 0x04: negative until the start delay expires */
    f32 position; /* 0x08: initial interpolated position */
    f32 positionStep; /* 0x0C: change per frame */
    f32 angle; /* 0x10: evenly spaced angle in radians */
    f32 unk14;
} EffPCPGroupEntry;

/* The general allocation contains this 0x180-byte prefix followed by
 * count entries. The four duplicate-handle groups use a separate allocation. */
typedef struct EffPCPGroupSet {
    EffPCPGroupHead head;
    EffPCPGroupEntry *entries; /* 0x164: entries at the end of the main allocation */
    f32 scale; /* 0x168: multiplies start/end positions */
    f32 unk16C;
    u32 color; /* 0x170 */
    EffParamWork **duplicates; /* 0x174: four groups of parameter work */
    struct SdfMemBlock *duplicateHandle; /* 0x178 */
    struct SdfMemBlock *workHandle; /* 0x17C */
} EffPCPGroupSet;

typedef char EffPCPGroupHead_size_must_be_0x164[
    (sizeof(EffPCPGroupHead) == 0x164) ? 1 : -1];
typedef char EffPCPGroupHead_count_offset_must_be_0x58[
    ((u32)&((EffPCPGroupHead *)0)->count == 0x58) ? 1 : -1];
typedef char EffPCPGroupHead_activeGroups_offset_must_be_0x8C[
    ((u32)&((EffPCPGroupHead *)0)->activeGroups == 0x8C) ? 1 : -1];
typedef char EffPCPGroupHead_spawnParams_offset_must_be_0x90[
    ((u32)&((EffPCPGroupHead *)0)->spawnParams == 0x90) ? 1 : -1];
typedef char EffPCPGroupEntry_size_must_be_0x18[
    (sizeof(EffPCPGroupEntry) == 0x18) ? 1 : -1];
typedef char EffPCPGroupEntry_frame_offset_must_be_4[
    ((u32)&((EffPCPGroupEntry *)0)->frame == 4) ? 1 : -1];
typedef char EffPCPGroupEntry_position_offset_must_be_8[
    ((u32)&((EffPCPGroupEntry *)0)->position == 8) ? 1 : -1];
typedef char EffPCPGroupEntry_positionStep_offset_must_be_C[
    ((u32)&((EffPCPGroupEntry *)0)->positionStep == 0xC) ? 1 : -1];
typedef char EffPCPGroupEntry_angle_offset_must_be_10[
    ((u32)&((EffPCPGroupEntry *)0)->angle == 0x10) ? 1 : -1];
typedef char EffPCPGroupSet_size_must_be_0x180[
    (sizeof(EffPCPGroupSet) == 0x180) ? 1 : -1];
typedef char EffPCPGroupSet_entries_offset_must_be_0x164[
    ((u32)&((EffPCPGroupSet *)0)->entries == 0x164) ? 1 : -1];
typedef char EffPCPGroupSet_scale_offset_must_be_0x168[
    ((u32)&((EffPCPGroupSet *)0)->scale == 0x168) ? 1 : -1];
typedef char EffPCPGroupSet_color_offset_must_be_0x170[
    ((u32)&((EffPCPGroupSet *)0)->color == 0x170) ? 1 : -1];
typedef char EffPCPGroupSet_duplicates_offset_must_be_0x174[
    ((u32)&((EffPCPGroupSet *)0)->duplicates == 0x174) ? 1 : -1];
typedef char EffPCPGroupSet_duplicateHandle_offset_must_be_0x178[
    ((u32)&((EffPCPGroupSet *)0)->duplicateHandle == 0x178) ? 1 : -1];
typedef char EffPCPGroupSet_workHandle_offset_must_be_0x17C[
    ((u32)&((EffPCPGroupSet *)0)->workHandle == 0x17C) ? 1 : -1];

#endif /* EFF_PCP_GROUP_SET_H */
