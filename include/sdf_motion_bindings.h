#ifndef SDF_MOTION_BINDINGS_H
#define SDF_MOTION_BINDINGS_H

#include "sdf_draw.h"

/* Common dispatch/source prefix used by motion-owned bindings. */
typedef struct SdfMotionBindingHead {
    void *dispatch;
    void *source;
} SdfMotionBindingHead;

typedef struct SdfMotionKeyInterval {
    f32 *firstKey;
    f32 *secondKey;
    f32 weight;
} SdfMotionKeyInterval;

/* Key-frame words are followed by a variable-length key payload. */
typedef struct SdfMotionKeyTrack {
    u16 unk00;
    u16 unk02;
    u16 keyCount;
    u16 keyStride;
    u16 keyFrames[1];
} SdfMotionKeyTrack;

typedef struct SdfMotionKeyBinding {
    void *dispatch;
    Motion *motion;
    SdfMotionKeyTrack *track;
} SdfMotionKeyBinding;

typedef struct SdfMotionDrawBinding {
    SdfMotionKeyBinding keys;
    SdfDrawNode *node;
    f32 capturedVector[4];
} SdfMotionDrawBinding;

/* The key-flag callback observes the draw node's flag and returns its inverse. */
typedef struct SdfMotionKeyFlagBinding {
    SdfMotionKeyBinding keys;
    SdfDrawNode *node;
    s8 keyFlagClear;
    u8 pad11[3];
} SdfMotionKeyFlagBinding;

enum {
    SDF_MOTION_KEY_SAMPLE_BYTE_ZERO = 0x0010
};

typedef char SdfMotionBindingHead_size[(sizeof(SdfMotionBindingHead) == 0x08) ? 1 : -1];
typedef char SdfMotionKeyInterval_size[(sizeof(SdfMotionKeyInterval) == 0x0C) ? 1 : -1];
typedef char SdfMotionKeyBinding_size[(sizeof(SdfMotionKeyBinding) == 0x0C) ? 1 : -1];
typedef char SdfMotionDrawBinding_size[(sizeof(SdfMotionDrawBinding) == 0x20) ? 1 : -1];
typedef char SdfMotionKeyFlagBinding_size[(sizeof(SdfMotionKeyFlagBinding) == 0x14) ? 1 : -1];

typedef char SdfMotionKeyBinding_track_at_08[
    ((u32)&((SdfMotionKeyBinding *)0)->track == 0x08) ? 1 : -1];
typedef char SdfMotionDrawBinding_node_at_0C[
    ((u32)&((SdfMotionDrawBinding *)0)->node == 0x0C) ? 1 : -1];
typedef char SdfMotionKeyFlagBinding_node_at_0C[
    ((u32)&((SdfMotionKeyFlagBinding *)0)->node == 0x0C) ? 1 : -1];
typedef char SdfMotionKeyFlagBinding_result_at_10[
    ((u32)&((SdfMotionKeyFlagBinding *)0)->keyFlagClear == 0x10) ? 1 : -1];

#endif
