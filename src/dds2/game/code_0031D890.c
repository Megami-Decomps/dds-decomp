#include "common.h"

void func_003297C8(u32 sprite);

enum { SLOT_IN_USE = 1 };

typedef struct WideSlot {
    u32 flags;
    u8 pad04[0x10];
} WideSlot;

typedef struct CompactSlot {
    u32 flags;
    u8 pad04[0xC];
} CompactSlot;

typedef struct WideSlotPool {
    u32 pad00;
    WideSlot *slots;
    s32 count;
} WideSlotPool;

typedef struct CompactSlotPool {
    u32 pad00;
    CompactSlot *slots;
    s32 count;
} CompactSlotPool;

INCLUDE_ASM(const s32, "game/code_0031D890", func_0031D890);

void func_0031D928(u32 *sprite) {
    func_003297C8(*sprite);
}

/* Reserve the first unclaimed 0x14-byte slot in the wide pool. */
u32 *itfClaimFreeWideSlot(WideSlotPool *pool) {
    WideSlot *entry;
    s32 index;

    index = 0;
    entry = pool->slots;
    if (0 < pool->count) {
        do {
            if ((entry->flags & SLOT_IN_USE) == 0) {
                entry->flags = entry->flags | SLOT_IN_USE;
                return (u32 *)entry;
            }
            index = index + 1;
            entry = entry + 1;
        } while (index < pool->count);
    }
    return (u32 *)0x0;
}

INCLUDE_ASM(const s32, "game/code_0031D890", func_0031D998);

/* Return a wide slot to the pool without disturbing its other flags. */
void func_0031DA20(u32 *flags) {
    *flags = *flags & ~SLOT_IN_USE;
}

INCLUDE_ASM(const s32, "game/code_0031D890", func_0031DA38);

INCLUDE_ASM(const s32, "game/code_0031D890", func_0031DEB8);

void func_0031DF48(u32 *sprite) {
    func_003297C8(*sprite);
}

/* Reserve the first unclaimed 0x10-byte slot in the compact pool. */
u32 *itfClaimFreeCompactSlot(CompactSlotPool *pool) {
    CompactSlot *entry;
    s32 index;

    index = 0;
    entry = pool->slots;
    if (0 < pool->count) {
        do {
            if ((entry->flags & SLOT_IN_USE) == 0) {
                entry->flags = entry->flags | SLOT_IN_USE;
                return (u32 *)entry;
            }
            index = index + 1;
            entry = entry + 1;
        } while (index < pool->count);
    }
    return (u32 *)0x0;
}

INCLUDE_ASM(const s32, "game/code_0031D890", func_0031DFB8);

/* Return a compact slot to the pool without disturbing its other flags. */
void func_0031E008(u32 *flags) {
    *flags = *flags & ~SLOT_IN_USE;
}

INCLUDE_ASM(const s32, "game/code_0031D890", func_0031E020);

INCLUDE_ASM(const s32, "game/code_0031D890", func_0031E198);

INCLUDE_ASM(const s32, "game/code_0031D890", func_0031E240);

INCLUDE_ASM(const s32, "game/code_0031D890", func_0031E2E8);

INCLUDE_ASM(const s32, "game/code_0031D890", func_0031E410);

INCLUDE_SDATA(const s32, "game/code_0031D890", D_00438958);

