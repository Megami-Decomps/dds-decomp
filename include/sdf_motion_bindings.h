#ifndef SDF_MOTION_BINDINGS_H
#define SDF_MOTION_BINDINGS_H

#include "sdf.h"
#include "sdf_draw.h"

/* Common dispatch/source prefix used by motion-owned bindings. */
typedef struct SdfMotionBindingHead {
    void *dispatch;
    void *source;
} SdfMotionBindingHead;

void sdfSetMotionPointerPair(SdfMotionBindingHead *binding, void *source,
                             void *dispatch);

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

void sdfFindMotionKeyInterval(SdfMotionKeyBinding *binding,
                              SdfMotionKeyInterval *interval, f32 frame);

void sdfMotionBindKeyTrack(SdfMotionKeyBinding *binding, SdfMotionKeyTrack *track);

/* Indexed asset bindings share the key prefix and retain the selected asset. */
typedef struct SdfMotionIndexedBinding {
    SdfMotionKeyBinding keys;
    SdfAsset *target;
} SdfMotionIndexedBinding;

/* Slot weights update a model entry and retain its complete previous pair. */
typedef struct SdfMotionSlotPairBinding {
    SdfMotionKeyBinding keys;
    SdfSlotEntry *current;
    SdfSlotEntry previous;
} SdfMotionSlotPairBinding;

/* Color/word and scalar bindings capture one target word at +0x10. */
typedef struct SdfMotionIndexedValueBinding {
    SdfMotionKeyBinding keys;
    SdfAsset *target;
    union {
        u32 capturedWord;
        f32 capturedFloat;
    };
} SdfMotionIndexedValueBinding;

typedef struct __attribute__((packed)) SdfMotionTextParamPrefix {
    s64 words[2];
} SdfMotionTextParamPrefix;

typedef struct SdfMotionTextParamSnapshot {
    SdfMotionTextParamPrefix prefix;
    s32 finalWord;
} SdfMotionTextParamSnapshot;

typedef union SdfMotionTextCapture {
    f32 capturedValues[5];
    SdfMotionTextParamSnapshot snapshot;
    u8 unusedDirectTail[0x14];
} SdfMotionTextCapture;

/* Text blends snapshot 0x14 bytes; direct-key bindings leave this tail unused. */
typedef struct SdfMotionIndexedTextBinding {
    SdfMotionKeyBinding keys;
    SdfAsset *target;
    SdfMotionTextCapture capture;
} SdfMotionIndexedTextBinding;

/* Shared 0x10-byte draw-target prefix; vector and key-flag tails differ. */
typedef struct SdfMotionDrawTargetBinding {
    SdfMotionKeyBinding keys;
    SdfDrawNode *node;
} SdfMotionDrawTargetBinding;

void sdfMotionBindDrawNode(SdfMotionDrawTargetBinding *binding, Motion *motion,
                          void *dispatch, s32 nodeIndex);

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
typedef char SdfMotionSlotPairBinding_size[
    (sizeof(SdfMotionSlotPairBinding) == 0x20) ? 1 : -1];
typedef char SdfMotionSlotPairBinding_current_at_0C[
    ((u32)&((SdfMotionSlotPairBinding *)0)->current == 0x0C) ? 1 : -1];
typedef char SdfMotionSlotPairBinding_previous_at_10[
    ((u32)&((SdfMotionSlotPairBinding *)0)->previous == 0x10) ? 1 : -1];

typedef char SdfMotionIndexedBinding_size[(sizeof(SdfMotionIndexedBinding) == 0x10) ? 1 : -1];
typedef char SdfMotionIndexedValueBinding_size[
    (sizeof(SdfMotionIndexedValueBinding) == 0x14) ? 1 : -1];
typedef char SdfMotionTextParamSnapshot_size[
    (sizeof(SdfMotionTextParamSnapshot) == 0x14) ? 1 : -1];
typedef char SdfMotionIndexedTextBinding_size[
    (sizeof(SdfMotionIndexedTextBinding) == 0x24) ? 1 : -1];
typedef char SdfMotionDrawTargetBinding_size[
    (sizeof(SdfMotionDrawTargetBinding) == 0x10) ? 1 : -1];
typedef char SdfMotionDrawBinding_size[(sizeof(SdfMotionDrawBinding) == 0x20) ? 1 : -1];
typedef char SdfMotionKeyFlagBinding_size[(sizeof(SdfMotionKeyFlagBinding) == 0x14) ? 1 : -1];

typedef char SdfMotionKeyBinding_track_at_08[
    ((u32)&((SdfMotionKeyBinding *)0)->track == 0x08) ? 1 : -1];
typedef char SdfMotionIndexedBinding_target_at_0C[
    ((u32)&((SdfMotionIndexedBinding *)0)->target == 0x0C) ? 1 : -1];
typedef char SdfMotionIndexedValueBinding_target_at_0C[
    ((u32)&((SdfMotionIndexedValueBinding *)0)->target == 0x0C) ? 1 : -1];
typedef char SdfMotionIndexedTextBinding_target_at_0C[
    ((u32)&((SdfMotionIndexedTextBinding *)0)->target == 0x0C) ? 1 : -1];
typedef char SdfMotionIndexedValueBinding_capture_at_10[
    ((u32)&((SdfMotionIndexedValueBinding *)0)->capturedWord == 0x10) ? 1 : -1];
typedef char SdfMotionIndexedTextBinding_capture_at_10[
    ((u32)&((SdfMotionIndexedTextBinding *)0)->capture == 0x10) ? 1 : -1];
typedef char SdfMotionDrawTargetBinding_node_at_0C[
    ((u32)&((SdfMotionDrawTargetBinding *)0)->node == 0x0C) ? 1 : -1];
typedef char SdfMotionDrawBinding_node_at_0C[
    ((u32)&((SdfMotionDrawBinding *)0)->node == 0x0C) ? 1 : -1];
typedef char SdfMotionKeyFlagBinding_node_at_0C[
    ((u32)&((SdfMotionKeyFlagBinding *)0)->node == 0x0C) ? 1 : -1];
typedef char SdfMotionKeyFlagBinding_result_at_10[
    ((u32)&((SdfMotionKeyFlagBinding *)0)->keyFlagClear == 0x10) ? 1 : -1];

#endif
