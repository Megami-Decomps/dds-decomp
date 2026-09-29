#include "common.h"

typedef struct PolyTransform {
    u8 pad0[0xC8];
    f32 scaleC8;
    f32 scaleCC;
    f32 scaleD0;
    u8 padD4[8];
    f32 scaleDC;
} PolyTransform;

typedef struct PolyEntryPool {
    u8 pad00[0x10];
    u32 entryCount; /* 0x10 */
    u32 sentinel; /* 0x14 */
    u8 pad18[0x50];
    u32 stateA; /* 0x68 */
    u32 stateB; /* 0x6C */
    u8 pad70[0x88];
    s32 *records; /* 0xF8: entries have five 32-bit words */
} PolyEntryPool;

void func_001655D0(u32 arg0) {
    func_001634A8(*(u32 *)((s32)arg0 + 0xdc));
    func_00328E48(arg0);
}

INCLUDE_ASM(const s32, "effect/polyManager", func_00165600);

void func_00165670(float factor, PolyTransform *transform) {
    transform->scaleCC = transform->scaleCC * factor;
    transform->scaleD0 = transform->scaleD0 * factor;
}

INCLUDE_ASM(const s32, "effect/polyManager", func_00165690);

void func_00165838(s32 arg0) {
    func_00165690();
    func_00163508(*(u32 *)(arg0 + 0xdc));
}

INCLUDE_ASM(const s32, "effect/polyManager", func_00165860);

INCLUDE_ASM(const s32, "effect/polyManager", func_001659B8);

INCLUDE_ASM(const s32, "effect/polyManager", func_00165A78);

INCLUDE_ASM(const s32, "effect/polyManager", func_00165B98);

void func_00165CC0(s32 arg0) {
    func_001634A8(*(u32 *)(arg0 + 0xf0));
    func_003297C8(*(u32 *)(arg0 + 0xf8));
}

INCLUDE_ASM(const s32, "effect/polyManager", func_00165CF0);

INCLUDE_ASM(const s32, "effect/polyManager", func_00165D38);

INCLUDE_ASM(const s32, "effect/polyManager", func_00165E28);

INCLUDE_ASM(const s32, "effect/polyManager", func_00165FC8);

void func_00166190(float factor, PolyTransform *transform) {
    transform->scaleC8 = transform->scaleC8 * factor;
    transform->scaleDC = transform->scaleDC * factor;
    transform->scaleCC = transform->scaleCC * factor;
    transform->scaleD0 = transform->scaleD0 * factor;
}

INCLUDE_ASM(const s32, "effect/polyManager", func_001661C8);

INCLUDE_ASM(const s32, "effect/polyManager", func_00166350);

void func_00166478(s32 arg0) {
    func_001634A8(*(u32 *)(arg0 + 0xe0));
    func_003297C8(*(u32 *)(arg0 + 0xe8));
}

INCLUDE_ASM(const s32, "effect/polyManager", func_001664A8);

INCLUDE_ASM(const s32, "effect/polyManager", func_001664F0);

INCLUDE_ASM(const s32, "effect/polyManager", func_00166590);

void func_001667E8(float factor, PolyTransform *transform) {
    transform->scaleCC = transform->scaleCC * factor;
}

INCLUDE_ASM(const s32, "effect/polyManager", func_001667F8);

INCLUDE_ASM(const s32, "effect/polyManager", func_00166980);

void func_00166AB0(s32 arg0) {
    func_001634A8(*(u32 *)(arg0 + 0xf4));
    func_003297C8(*(u32 *)(arg0 + 0xfc));
}

INCLUDE_ASM(const s32, "effect/polyManager", func_00166AE0);

INCLUDE_ASM(const s32, "effect/polyManager", func_00166B40);

INCLUDE_ASM(const s32, "effect/polyManager", func_00166CB0);

void func_00166EA0(float factor, PolyTransform *transform) {
    transform->scaleCC = transform->scaleCC * factor;
    transform->scaleD0 = transform->scaleD0 * factor;
}

INCLUDE_ASM(const s32, "effect/polyManager", func_00166EC0);

void polyResetEntries(PolyEntryPool *pool) {
    u32 entryCount;
    s32 *record;
    u32 index;

    entryCount = pool->entryCount;
    index = 0;
    pool->sentinel = 0xfffffff;
    pool->stateB = 0;
    pool->stateA = 0;
    record = pool->records;
    if (entryCount != 0) {
        do {
            if (*record != -0xffffff) {
                *record = 0xffffff0;
            }
            index = index + 1;
            record = record + 5;
        } while (index < entryCount);
    }
}
