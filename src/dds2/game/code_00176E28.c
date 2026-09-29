#include "common.h"

extern u64 effParamTableGetBlock(u64, u64);

typedef struct EffVectorPart {
    u8 pad0[8];
    f32 accumulated;
    u8 padC[4];
} EffVectorPart;

typedef struct EffVectorWork {
    u8 pad0[0x50];
    f32 increment;
    u8 pad54[4];
    EffVectorPart *parts;
} EffVectorWork;

typedef struct EffRecordPool {
    u8 pad00[0x50];
    u32 settingA;  /* 0x50 */
    u32 settingB;  /* 0x54 */
    u8 pad58[8];
    s32 recordBase;    /* 0x60: address of stride-dependent records */
    s32 auxRecordBase; /* 0x64: address of stride-dependent auxiliary records */
    u32 resource;   /* 0x68: released by func_00333918 */
    u32 buffer;     /* 0x6C: freed by func_003297C8 */
} EffRecordPool;

INCLUDE_ASM(const s32, "game/code_00176E28", func_00176E28);

void func_00177078(u64 arg0) {
    u64 temp_v0;

    temp_v0 = effParamTableGetBlock(arg0, 0);
    func_00176E28(temp_v0);
}

void func_00177098(void) {
    func_00176E28();
}

void func_001770B0(s32 arg0) {
    func_001781F8(*(u32 *)(arg0 + 0x7c));
    func_003297C8(*(u32 *)(arg0 + 0x78));
}

INCLUDE_ASM(const s32, "game/code_00176E28", func_001770E0);

void func_001770F0(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x60) = arg1;
}

void func_001770F8(u8 *work, f32 value) {
    *(f32 *)(work + 0x64) = value;
}

INCLUDE_ASM(const s32, "game/code_00176E28", func_00177100);

INCLUDE_ASM(const s32, "game/code_00176E28", func_00177130);

INCLUDE_ASM(const s32, "game/code_00176E28", func_00177220);

void effAdvanceVectorRecord(EffVectorWork *work, s32 index) {
    EffVectorPart *part;

    part = &work->parts[index];
    part->accumulated = part->accumulated + work->increment;
}

INCLUDE_ASM(const s32, "game/code_00176E28", func_00177408);

INCLUDE_ASM(const s32, "game/code_00176E28", func_00177760);

void func_00177880(EffRecordPool *pool) {
    func_00333918(pool->resource);
    func_003297C8(pool->buffer);
}

INCLUDE_ASM(const s32, "game/code_00176E28", func_001778B0);

s32 func_00177B60(EffRecordPool *pool, s32 index) {
    return pool->recordBase + index * 0x50;
}

s32 func_00177B78(EffRecordPool *pool, s32 index) {
    return pool->auxRecordBase + index * 0x14;
}

void func_00177B90(EffRecordPool *pool, u32 value) {
    pool->settingA = value;
}

void func_00177B98(EffRecordPool *pool, u32 value) {
    pool->settingB = value;
}

void func_00177BA0(u8 *work, f32 value) {
    *(f32 *)(work + 0x5C) = value;
}

INCLUDE_ASM(const s32, "game/code_00176E28", func_00177BA8);

void func_00177CA0(EffRecordPool *pool) {
    func_00333918(pool->resource);
    func_003297C8(pool->buffer);
}

INCLUDE_ASM(const s32, "game/code_00176E28", func_00177CD0);

s32 func_00177E78(EffRecordPool *pool, s32 index) {
    return pool->recordBase + index * 0x30;
}

s32 func_00177E90(EffRecordPool *pool, s32 index) {
    return pool->auxRecordBase + index * 0xc;
}

INCLUDE_ASM(const s32, "game/code_00176E28", func_00177EA8);

void func_00177FA8(EffRecordPool *pool) {
    func_00333918(pool->resource);
    func_003297C8(pool->buffer);
}

INCLUDE_ASM(const s32, "game/code_00176E28", func_00177FD8);

s32 func_00178190(EffRecordPool *pool, s32 index) {
    return pool->recordBase + index * 0x40;
}

s32 func_001781A0(EffRecordPool *pool, s32 index) {
    return pool->auxRecordBase + index * 0x10;
}

INCLUDE_ASM(const s32, "game/code_00176E28", func_001781B0);

INCLUDE_ASM(const s32, "game/code_00176E28", func_001781F8);

INCLUDE_ASM(const s32, "game/code_00176E28", func_00178210);

s32 func_001784B0(EffRecordPool *pool, s32 index) {
    return pool->recordBase + index * 0x50;
}

s32 func_001784C8(EffRecordPool *pool, s32 index) {
    return pool->auxRecordBase + index * 0x14;
}

void func_001784E0(EffRecordPool *pool, u32 value) {
    pool->settingA = value;
}

void func_001784E8(EffRecordPool *pool, u32 value) {
    pool->settingB = value;
}

void func_001784F0(u8 *work, f32 value) {
    *(f32 *)(work + 0x5C) = value;
}

INCLUDE_ASM(const s32, "game/code_00176E28", func_001784F8);
