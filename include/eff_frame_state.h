#ifndef DDS_EFF_FRAME_STATE_H
#define DDS_EFF_FRAME_STATE_H

#include "common.h"

struct SdfMemBlock;

/* Shared 0xC per-instance prefix: entries, asset view and allocation handle. */
typedef struct EffFrameState {
    u8 *entries;
    u8 *asset;
    struct SdfMemBlock *allocation;
} EffFrameState;

typedef char EffFrameState_size_check[
    (sizeof(EffFrameState) == 0xC) ? 1 : -1];
typedef char EffFrameState_entries_offset_check[
    ((u32)&((EffFrameState *)0)->entries == 0x00) ? 1 : -1];
typedef char EffFrameState_asset_offset_check[
    ((u32)&((EffFrameState *)0)->asset == 0x04) ? 1 : -1];
typedef char EffFrameState_allocation_offset_check[
    ((u32)&((EffFrameState *)0)->allocation == 0x08) ? 1 : -1];

#endif
