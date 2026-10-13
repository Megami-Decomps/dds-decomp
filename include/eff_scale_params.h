#ifndef EFF_SCALE_PARAMS_H
#define EFF_SCALE_PARAMS_H

#include "common.h"

typedef struct ScaleEntry {
    f32 unk0;
    f32 value;
} ScaleEntry;

/* Access view through +0xF4; intervening storage and any allocation tail stay opaque. */
typedef struct ScaleSet {
    u8 pad0[0x64];
    f32 unk64;
    f32 unk68;
    u8 pad6C[4];
    ScaleEntry entries[3];
    u8 pad88[0x40];
    f32 unkC8;
    f32 unkCC;
    f32 unkD0;
    f32 unkD4;
    f32 unkD8;
    f32 unkDC;
    f32 unkE0;
    f32 unkE4;
    u8 padE8[8];
    f32 unkF0;
} ScaleSet;

typedef struct ScaleOwner {
    u8 pad0[0x20];
    ScaleSet *dst;
    ScaleSet *src;
} ScaleOwner;

typedef char ScaleEntry_size_must_be_0x08[(sizeof(ScaleEntry) == 0x08) ? 1 : -1];
typedef char ScaleEntry_value_offset_must_be_0x04[
    ((u32)&((ScaleEntry *)0)->value == 0x04) ? 1 : -1];
typedef char ScaleSet_entries_offset_must_be_0x70[
    ((u32)&((ScaleSet *)0)->entries == 0x70) ? 1 : -1];
typedef char ScaleSet_unkF0_offset_must_be_0xF0[
    ((u32)&((ScaleSet *)0)->unkF0 == 0xF0) ? 1 : -1];
typedef char ScaleOwner_dst_offset_must_be_0x20[
    ((u32)&((ScaleOwner *)0)->dst == 0x20) ? 1 : -1];
typedef char ScaleOwner_src_offset_must_be_0x24[
    ((u32)&((ScaleOwner *)0)->src == 0x24) ? 1 : -1];

#endif /* EFF_SCALE_PARAMS_H */
