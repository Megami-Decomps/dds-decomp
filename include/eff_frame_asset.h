#ifndef EFF_FRAME_ASSET_H
#define EFF_FRAME_ASSET_H

#include "common.h"

/* Shared billboard/frame resource view. The generic EffFrameState owner can
 * point at several resource kinds; only cast its asset slot to this record in
 * frame callbacks. The +0x18 pointer is preserved as an opaque pointer because
 * the paired units disagree on its signed element type and do not consume it. */
typedef struct EffFrameAsset {
    u8 pad00[8];
    u32 frameCount;          /* 0x08 */
    u8 pad0C[0x0C];
    void *animationFrames;   /* 0x18 */
    void *frameStorage;      /* 0x1C */
    f32 *transformRows;      /* 0x20 */
    u32 pad24;
    u32 *colorRows;          /* 0x28 */
} EffFrameAsset;

typedef char EffFrameAssetSizeCheck[
    (sizeof(EffFrameAsset) == 0x2C) ? 1 : -1];
typedef char EffFrameAssetFrameCountOffsetCheck[
    ((u32)&((EffFrameAsset *)0)->frameCount == 0x08) ? 1 : -1];
typedef char EffFrameAssetAnimationFramesOffsetCheck[
    ((u32)&((EffFrameAsset *)0)->animationFrames == 0x18) ? 1 : -1];
typedef char EffFrameAssetFrameStorageOffsetCheck[
    ((u32)&((EffFrameAsset *)0)->frameStorage == 0x1C) ? 1 : -1];
typedef char EffFrameAssetColorRowsOffsetCheck[
    ((u32)&((EffFrameAsset *)0)->colorRows == 0x28) ? 1 : -1];

#endif /* EFF_FRAME_ASSET_H */
