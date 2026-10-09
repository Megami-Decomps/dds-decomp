#ifndef EFF_PCP_THUNDER_GROUP_H
#define EFF_PCP_THUNDER_GROUP_H

#include "eff.h"

struct EffEventWork;
struct SoundMixer;

/* One retained fragment-history header follows its point/color buffers. */
typedef struct EffPcpThunderFragmentResources {
    u32 color;                              /* 0x00 */
    s32 surfaceIndex;                       /* 0x04 */
    s32 pointCapacity;                      /* 0x08: history point capacity */
    s32 activePointCount;                   /* 0x0C */
    s32 nextPointWriteIndex;                /* 0x10: first point of the next three-point sample */
    s32 subdivisions;                       /* 0x14: retained input value */
    u128 *points;                           /* 0x18 */
    u32 *colors;                            /* 0x1C */
    SdfAsset *resourceHandle;               /* 0x20 */
    SdfMemBlock *allocation;                /* 0x24 */
    u128 *endPoints;                        /* 0x28 */
    u32 *endColors;                         /* 0x2C */
} EffPcpThunderFragmentResources;

/* Copied 0x50-byte input for a group of thunder-fragment ribbons. */
typedef struct EffPcpThunderGroupParams {
    f32 origin[4];                          /* 0x00 */
    u32 mode;                               /* 0x10: retains native selector values */
    f32 spreadX;                            /* 0x14 */
    f32 spreadZ;                            /* 0x18 */
    f32 height;                             /* 0x1C */
    u32 groupCount;                         /* 0x20 */
    s32 curveFrames;                        /* 0x24 */
    s32 startDelayRange;                    /* 0x28 */
    u32 unk2C;                              /* 0x2C */
    s32 fadeFrames;                         /* 0x30 */
    f32 width;                              /* 0x34 */
    s32 historyLength;                      /* 0x38 */
    s32 subdivisions;                       /* 0x3C */
    u32 palette[4];                         /* 0x40: inner-start, outer-start, inner-end, outer-end */
} EffPcpThunderGroupParams;

/* One 0x80-byte moving ribbon slot plus its event work. */
typedef struct EffPcpThunderGroupSlot {
    f32 position[4];                        /* 0x00: current sampled point */
    s32 age;                                /* 0x10: negative delay, then elapsed frames */
    f32 fadeScale;                          /* 0x14 */
    EffPcpThunderFragmentResources *fragment;/* 0x18 */
    EffSegmentedBezierSlot curve;           /* 0x1C */
    struct EffEventWork *eventNode;          /* 0x7C */
} EffPcpThunderGroupSlot;

/* Allocation header followed by groupCount inline 0x80-byte slots. */
typedef struct EffPcpThunderGroup {
    EffPcpThunderGroupParams params;         /* 0x00 */
    EffPcpThunderGroupSlot *slots;           /* 0x50 */
    u32 tintColor;                           /* 0x54 */
    struct SoundMixer *soundMixer;           /* 0x58 */
    u8 ownsMixerVoices;                      /* 0x5C */
    u8 pad5D[3];                             /* 0x5D */
    SdfMemBlock *allocation;                 /* 0x60 */
} EffPcpThunderGroup;

typedef char EffPcpThunderFragmentResources_size[(sizeof(EffPcpThunderFragmentResources) == 0x30) ? 1 : -1];
typedef char EffPcpThunderFragmentResources_pointCapacity_offset[((u32)&((EffPcpThunderFragmentResources *)0)->pointCapacity == 0x08) ? 1 : -1];
typedef char EffPcpThunderFragmentResources_nextPointWriteIndex_offset[((u32)&((EffPcpThunderFragmentResources *)0)->nextPointWriteIndex == 0x10) ? 1 : -1];
typedef char EffPcpThunderGroupParams_size[(sizeof(EffPcpThunderGroupParams) == 0x50) ? 1 : -1];
typedef char EffPcpThunderGroupSlot_size[(sizeof(EffPcpThunderGroupSlot) == 0x80) ? 1 : -1];
typedef char EffPcpThunderGroup_size[(sizeof(EffPcpThunderGroup) == 0x64) ? 1 : -1];
typedef char EffPcpThunderGroupParams_groupCount_offset[((u32)&((EffPcpThunderGroupParams *)0)->groupCount == 0x20) ? 1 : -1];
typedef char EffPcpThunderGroupParams_historyLength_offset[((u32)&((EffPcpThunderGroupParams *)0)->historyLength == 0x38) ? 1 : -1];
typedef char EffPcpThunderGroupParams_palette_offset[((u32)&((EffPcpThunderGroupParams *)0)->palette == 0x40) ? 1 : -1];
typedef char EffPcpThunderGroup_slots_offset[((u32)&((EffPcpThunderGroup *)0)->slots == 0x50) ? 1 : -1];
typedef char EffPcpThunderGroup_tintColor_offset[((u32)&((EffPcpThunderGroup *)0)->tintColor == 0x54) ? 1 : -1];
typedef char EffPcpThunderGroup_soundMixer_offset[((u32)&((EffPcpThunderGroup *)0)->soundMixer == 0x58) ? 1 : -1];
typedef char EffPcpThunderGroupSlot_eventNode_offset[((u32)&((EffPcpThunderGroupSlot *)0)->eventNode == 0x7C) ? 1 : -1];
typedef char EffPcpThunderGroup_ownsMixerVoices_offset[((u32)&((EffPcpThunderGroup *)0)->ownsMixerVoices == 0x5C) ? 1 : -1];
typedef char EffPcpThunderGroup_allocation_offset[((u32)&((EffPcpThunderGroup *)0)->allocation == 0x60) ? 1 : -1];

#endif
