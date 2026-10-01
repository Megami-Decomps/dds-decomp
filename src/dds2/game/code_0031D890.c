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

u32 *itfClaimWideSlotWithTaggedPayload(u32 a, u32 b, u32 c, s8 tag, WideSlotPool *pool) {
    u32 *slot = itfClaimFreeWideSlot(pool);
    u32 bits = (tag & 0xFF) << 1;

    if (slot != NULL) {
        slot[1] = a;
        slot[2] = b;
        slot[3] = c;
        slot[4] = 0;
        slot[0] = (slot[0] & ~0x1FE) | bits;
    }
    return slot;
}

/* Return a wide slot to the pool without disturbing its other flags. */
void itfReleaseWideSlot(u32 *flags) {
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

/* Claim a compact slot and fill its two payload words; returns the slot (NULL if the pool is full). */
u32 *itfClaimCompactSlotWithPayload(u32 first, u32 second, CompactSlotPool *pool) {
    u32 *slot = itfClaimFreeCompactSlot(pool);

    if (slot != NULL) {
        slot[1] = first;
        slot[2] = second;
        slot[3] = 0;
    }
    return slot;
}

/* Return a compact slot to the pool without disturbing its other flags. */
void itfReleaseCompactSlot(u32 *flags) {
    *flags = *flags & ~SLOT_IN_USE;
}

INCLUDE_ASM(const s32, "game/code_0031D890", func_0031E020);

extern void itfFadeSetTint(u32 tint);

void func_0031E198(u8 *work) {
    itfFadeSetTint(*(u32 *)(work + 0x34));
    memset(work + 0xB4, 0, 0x18);
    memset(work + 0xCC, 0, 0x1C);
    memset(work + 0xE8, 0, 0x1C);
    memset(work + 0x104, 0, 0x48);
    memset(work + 0x14C, 0, 0x1C);
    memset(work + 0x168, 0, 0x1C);
    memset(work + 0x184, 0, 0x18);
    memset(work + 0x1B8, 0, 0x1C);
}

extern void itfFadeClearTint(void);

void func_0031E240(u8 *work) {
    itfFadeClearTint();
    memset(work + 0xB4, 0, 0x18);
    memset(work + 0xCC, 0, 0x1C);
    memset(work + 0xE8, 0, 0x1C);
    memset(work + 0x104, 0, 0x48);
    memset(work + 0x14C, 0, 0x1C);
    memset(work + 0x168, 0, 0x1C);
    memset(work + 0x184, 0, 0x18);
    memset(work + 0x1B8, 0, 0x1C);
}

INCLUDE_ASM(const s32, "game/code_0031D890", func_0031E2E8);

/* Per-glyph texture byte, one 12-byte entry per glyph. */
extern u8 D_0040B088[];

u8 mnuGetIndexedFadeTexture(s32 index) {
    return D_0040B088[index * 12];
}

INCLUDE_SDATA(const s32, "game/code_0031D890", D_00438958);

