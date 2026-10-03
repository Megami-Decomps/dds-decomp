#include "common.h"

void sdfReleaseResourceAllocation(u32 sprite);

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

extern u32 sdfAllocGeneralBlock(s32 size);
extern void *sdfMemoryGetBlockAddress(u32 handle);
extern void evtPrintDeveloperConsoleMessage(char *text, s32 value);
extern char D_0042DBA0[];

typedef struct SpriteWorkPool {
    u32 handle;
    u8 *items;
    s32 count;
} SpriteWorkPool;

/* Sprite-number work pool: count 0x14-byte items plus a 0xC-byte header. */
u32 itfCreateSpriteWorkPool(u32 count) {
    u32 size = count * 0x14 + 0xC;
    u32 handle = sdfAllocGeneralBlock(size);
    SpriteWorkPool *pool = (SpriteWorkPool *)sdfMemoryGetBlockAddress(handle);
    memset(pool, 0, size);
    pool->handle = handle;
    pool->count = count;
    pool->items = (u8 *)pool + 0xC;
    evtPrintDeveloperConsoleMessage("Sprite Num Work Crate Size[%d]\n", size);
    return (u32)pool;
}

void func_0031D928(u32 *sprite) {
    sdfReleaseResourceAllocation(*sprite);
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

/* Sprite-hit-effect work pool: count 0x10-byte items plus a 0xC-byte header. */
u32 func_0031DEB8(u32 count) {
    u32 size = count * 0x10 + 0xC;
    u32 handle = sdfAllocGeneralBlock(size);
    SpriteWorkPool *pool = (SpriteWorkPool *)sdfMemoryGetBlockAddress(handle);

    memset(pool, 0, size);
    pool->handle = handle;
    pool->count = count;
    pool->items = (u8 *)pool + 0xC;
    evtPrintDeveloperConsoleMessage(D_0042DBA0, size);
    return (u32)pool;
}

void func_0031DF48(u32 *sprite) {
    sdfReleaseResourceAllocation(*sprite);
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

typedef struct FadeEntry FadeEntry;

typedef struct ItfFadeWork {
    u8 unk00[0x34];
    u32 tint;
    u8 unk38[0x3C];
    u32 lowerValue;
    u32 upperValue;
    u32 unk7C;
    u32 gaugeValue;
    u32 unk84;
    s16 unk88;
    u8 unk8A[0x26];
    u32 frameValue;
    u8 strip[0x18];
    u8 lowerNumber[0x1C];
    u8 upperNumber[0x1C];
    u8 gauge[0x48];
    u8 unk14C[0x1C];
    u8 unk168[0x1C];
    u8 unk184[0x18];
    u8 unk19C[0x1C];
    u8 frame[0x1C];
} ItfFadeWork;

extern void itfFadeSetTint(u32 tint);

void itfApplyWorkTintAndClearBuffers(u8 *work) {
    ItfFadeWork *state = (ItfFadeWork *)work;

    itfFadeSetTint(state->tint);
    memset(state->strip, 0, sizeof(state->strip));
    memset(state->lowerNumber, 0, sizeof(state->lowerNumber));
    memset(state->upperNumber, 0, sizeof(state->upperNumber));
    memset(state->gauge, 0, sizeof(state->gauge));
    memset(state->unk14C, 0, sizeof(state->unk14C));
    memset(state->unk168, 0, sizeof(state->unk168));
    memset(state->unk184, 0, sizeof(state->unk184));
    memset(state->frame, 0, sizeof(state->frame));
}

extern void itfFadeClearTint(void);

void itfClearTintAndWorkBuffers(u8 *work) {
    ItfFadeWork *state = (ItfFadeWork *)work;

    itfFadeClearTint();
    memset(state->strip, 0, sizeof(state->strip));
    memset(state->lowerNumber, 0, sizeof(state->lowerNumber));
    memset(state->upperNumber, 0, sizeof(state->upperNumber));
    memset(state->gauge, 0, sizeof(state->gauge));
    memset(state->unk14C, 0, sizeof(state->unk14C));
    memset(state->unk168, 0, sizeof(state->unk168));
    memset(state->unk184, 0, sizeof(state->unk184));
    memset(state->frame, 0, sizeof(state->frame));
}

extern void itfSetFadeMode(FadeEntry *entry, s32 mode, s32 step);
extern void itfDrawFadeGlyphStrip(FadeEntry *entry);
extern void itfDrawLowerFadeGlyphPair(FadeEntry *entry);
extern void func_0031E7C8(FadeEntry *entry, u32 displayValue);
extern void itfDrawUpperFadeGlyphPair(FadeEntry *entry);
extern void func_0031E850(FadeEntry *entry, u32 displayValue);
extern void itfSetFadeParameter(FadeEntry *entry, u32 parameter);
extern void itfSetFadeSecondaryParameter(FadeEntry *entry, u32 parameter);
extern void func_0031E8E8(FadeEntry *entry, u32 displayValue);
extern void func_0031E8F0(FadeEntry *entry);
extern void func_0031F040(FadeEntry *entry, u32 displayValue);
extern void itfDrawFadeGlyphForFrame(FadeEntry *entry);

/* Fade blocks have a common state prefix and callback-specific trailing data. */
void itfDrawFullExtentWorkPanels(ItfFadeWork *work) {
    itfSetFadeMode((FadeEntry *)work->strip, 1, 0x80);
    itfDrawFadeGlyphStrip((FadeEntry *)work->strip);
    itfSetFadeMode((FadeEntry *)work->lowerNumber, 1, 0x80);
    itfDrawLowerFadeGlyphPair((FadeEntry *)work->lowerNumber);
    func_0031E7C8((FadeEntry *)work->lowerNumber, work->lowerValue);
    itfSetFadeMode((FadeEntry *)work->upperNumber, 1, 0x80);
    itfDrawUpperFadeGlyphPair((FadeEntry *)work->upperNumber);
    func_0031E850((FadeEntry *)work->upperNumber, work->upperValue);
    itfSetFadeMode((FadeEntry *)work->gauge, 1, 0x80);
    itfSetFadeParameter((FadeEntry *)work->gauge, 100);
    itfSetFadeSecondaryParameter((FadeEntry *)work->gauge, work->gaugeValue);
    if (work->unk88 > 0) {
        func_0031E8E8((FadeEntry *)work->gauge, 1);
    } else {
        func_0031E8E8((FadeEntry *)work->gauge, 0);
    }
    func_0031E8F0((FadeEntry *)work->gauge);
    itfSetFadeMode((FadeEntry *)work->frame, 1, 0x80);
    func_0031F040((FadeEntry *)work->frame, work->frameValue);
    itfDrawFadeGlyphForFrame((FadeEntry *)work->frame);
}

/* Per-glyph texture byte, one 12-byte entry per glyph. */
extern u8 D_0040B088[];

u8 mnuGetIndexedFadeTexture(s32 index) {
    return D_0040B088[index * 12];
}

INCLUDE_RODATA(const s32, "game/code_0031D890", D_0042DBA0);

INCLUDE_SDATA(const s32, "game/code_0031D890", D_00438958);

