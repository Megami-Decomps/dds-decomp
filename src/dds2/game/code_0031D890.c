#include "common.h"
#include "sdf.h"

void sdfReleaseResourceAllocation(SdfMemBlock *allocation);

enum { SLOT_IN_USE = 1 };

typedef struct WideSlot {
    u32 flags;
    s32 x;
    s32 y;
    s32 value;
    s32 age;
} WideSlot;

typedef struct CompactSlot {
    u32 flags;
    u32 firstPayload;
    u32 secondPayload;
    s32 age;
} CompactSlot;

typedef struct WideSlotPool {
    SdfMemBlock *allocation;
    WideSlot *slots;
    s32 count;
} WideSlotPool;

typedef struct CompactSlotPool {
    SdfMemBlock *allocation;
    CompactSlot *slots;
    s32 count;
} CompactSlotPool;

extern SdfMemBlock *sdfAllocGeneralBlock(s32 size);
extern u32 sdfMemoryGetBlockAddress(SdfMemBlock *allocation);
extern void evtPrintDeveloperConsoleMessage(const char *format, ...);
extern char D_0042DBA0[];

typedef char WideSlotLayoutAssert[
    (sizeof(WideSlot) == 0x14 &&
     (unsigned long)&((WideSlot *)0)->x == 4 &&
     (unsigned long)&((WideSlot *)0)->y == 8 &&
     (unsigned long)&((WideSlot *)0)->value == 0xC &&
     (unsigned long)&((WideSlot *)0)->age == 0x10) ? 1 : -1];
typedef char CompactSlotLayoutAssert[
    (sizeof(CompactSlot) == 0x10 &&
     (unsigned long)&((CompactSlot *)0)->age == 0xC) ? 1 : -1];
typedef char PoolLayoutAssert[
    (sizeof(WideSlotPool) == 0xC &&
     sizeof(CompactSlotPool) == 0xC &&
     (unsigned long)&((WideSlotPool *)0)->slots == 4 &&
     (unsigned long)&((WideSlotPool *)0)->count == 8 &&
     (unsigned long)&((CompactSlotPool *)0)->slots == 4 &&
     (unsigned long)&((CompactSlotPool *)0)->count == 8) ? 1 : -1];

/* Sprite-number work pool: count 0x14-byte items plus a 0xC-byte header. */
WideSlotPool *itfCreateSpriteWorkPool(u32 count) {
    u32 size = count * 0x14 + 0xC;
    SdfMemBlock *allocation = sdfAllocGeneralBlock(size);
    WideSlotPool *pool = (WideSlotPool *)sdfMemoryGetBlockAddress(allocation);
    memset(pool, 0, size);
    pool->allocation = allocation;
    pool->count = count;
    pool->slots = (WideSlot *)(pool + 1);
    evtPrintDeveloperConsoleMessage("Sprite Num Work Crate Size[%d]\n", size);
    return pool;
}

void func_0031D928(WideSlotPool *pool) {
    sdfReleaseResourceAllocation(pool->allocation);
}

/* Reserve the first unclaimed 0x14-byte slot in the wide pool. */
WideSlot *itfClaimFreeWideSlot(WideSlotPool *pool) {
    WideSlot *entry;
    s32 index;

    index = 0;
    entry = pool->slots;
    if (0 < pool->count) {
        do {
            if ((entry->flags & SLOT_IN_USE) == 0) {
                entry->flags = entry->flags | SLOT_IN_USE;
                return entry;
            }
            index = index + 1;
            entry = entry + 1;
        } while (index < pool->count);
    }
    return NULL;
}

WideSlot *itfClaimWideSlotWithTaggedPayload(u32 a, u32 b, u32 c, s8 tag, WideSlotPool *pool) {
    WideSlot *slot = itfClaimFreeWideSlot(pool);
    u32 bits = (tag & 0xFF) << 1;

    if (slot != NULL) {
        slot->x = a;
        slot->y = b;
        slot->value = c;
        slot->age = 0;
        slot->flags = (slot->flags & ~0x1FE) | bits;
    }
    return slot;
}

/* Return a wide slot to the pool without disturbing its other flags. */
void itfReleaseWideSlot(WideSlot *slot) {
    slot->flags = slot->flags & ~SLOT_IN_USE;
}

extern void func_0031CE60(s32 x, s32 y, s32 z, s32 alpha, s32 flags, s32 index, s32 context);
extern s32 func_0035C860(char *destination, const char *format, ...);

void func_0031DA38(WideSlotPool *pool, u32 flags) {
    WideSlot *slot = pool->slots;
    s32 slotIndex;
    char text[32];
    s8 length;
    s32 width;
    s32 offset;
    s32 x;
    s32 y;
    s32 digit;
    f32 opacity;
    f32 rise;

    for (slotIndex = 0; slotIndex < pool->count; slotIndex++, slot++) {
        if ((slot->flags & SLOT_IN_USE) != 0) {
            if ((flags & 1) == 0) {
                slot->age++;
            }
            if (slot->age < 20) {
                opacity = (f32)slot->age / 20.0f;
            } else if (slot->age < 70) {
                opacity = 1.0f;
            } else {
                opacity = (f32)(90 - slot->age) / 20.0f;
            }
            if (slot->age < 20) {
                rise = (f32)slot->age / 20.0f;
            } else {
                rise = 1.0f;
            }
            memset(text, 0, sizeof(text));
            length = 0;
            func_0035C860(text, "%d", slot->value);
            x = slot->x;
            y = slot->y;
            width = 0;
            while (text[length] != 0) {
                length++;
                width += 10;
            }
            if (((slot->flags >> 1) & 0xFF) != 0) {
                width += 41;
            }
            offset = (s32)((f32)-width * 0.5f);
            for (digit = 0; digit < length; digit++, offset += 10) {
                if (((slot->flags >> 1) & 0xFF) == 20) {
                    func_0031CE60(x + offset, (s32)((f32)(y + 32) - rise * 16.0f), 0,
                        (s32)(opacity * 128.0f), 0, text[digit] - 7, 0x53);
                } else {
                    func_0031CE60(x + offset, (s32)((f32)(y + 32) - rise * 16.0f), 0,
                        (s32)(opacity * 128.0f), 0, text[digit] + 3, 0x53);
                }
            }
            if (((slot->flags >> 1) & 0xFF) != 0) {
                offset += 13;
                switch ((slot->flags >> 1) & 0xFF) {
                case 1:
                    func_0031CE60(x + offset, (s32)((f32)(y + 32) - rise * 16.0f), 0,
                        (s32)(opacity * 128.0f), 0, 61, 0x53);
                    break;
                case 2:
                    func_0031CE60(x + offset, (s32)((f32)(y + 32) - rise * 16.0f), 0,
                        (s32)(opacity * 128.0f), 0, 62, 0x53);
                    break;
                case 4:
                    func_0031CE60(x + offset, (s32)((f32)(y + 32) - rise * 16.0f), 0,
                        (s32)(opacity * 128.0f), 0, 63, 0x53);
                    break;
                case 8:
                    func_0031CE60(x + offset, (s32)((f32)(y + 32) - rise * 16.0f), 0,
                        (s32)(opacity * 128.0f), 0, 64, 0x53);
                    break;
                case 20:
                    func_0031CE60(x + offset, (s32)((f32)(y + 32) - rise * 16.0f), 0,
                        (s32)(opacity * 128.0f), 0, 75, 0x53);
                    break;
                }
            }
            if (slot->age >= 91) {
                itfReleaseWideSlot(slot);
            }
        }
    }
}

/* Sprite-hit-effect work pool: count 0x10-byte items plus a 0xC-byte header. */
CompactSlotPool *func_0031DEB8(u32 count) {
    u32 size = count * 0x10 + 0xC;
    SdfMemBlock *allocation = sdfAllocGeneralBlock(size);
    CompactSlotPool *pool = (CompactSlotPool *)sdfMemoryGetBlockAddress(allocation);

    memset(pool, 0, size);
    pool->allocation = allocation;
    pool->count = count;
    pool->slots = (CompactSlot *)(pool + 1);
    evtPrintDeveloperConsoleMessage(D_0042DBA0, size);
    return pool;
}

void func_0031DF48(CompactSlotPool *pool) {
    sdfReleaseResourceAllocation(pool->allocation);
}

/* Reserve the first unclaimed 0x10-byte slot in the compact pool. */
CompactSlot *itfClaimFreeCompactSlot(CompactSlotPool *pool) {
    CompactSlot *entry;
    s32 index;

    index = 0;
    entry = pool->slots;
    if (0 < pool->count) {
        do {
            if ((entry->flags & SLOT_IN_USE) == 0) {
                entry->flags = entry->flags | SLOT_IN_USE;
                return entry;
            }
            index = index + 1;
            entry = entry + 1;
        } while (index < pool->count);
    }
    return NULL;
}

/* Claim a compact slot and fill its two payload words; returns the slot (NULL if the pool is full). */
CompactSlot *itfClaimCompactSlotWithPayload(u32 first, u32 second, CompactSlotPool *pool) {
    CompactSlot *slot = itfClaimFreeCompactSlot(pool);

    if (slot != NULL) {
        slot->firstPayload = first;
        slot->secondPayload = second;
        slot->age = 0;
    }
    return slot;
}

/* Return a compact slot to the pool without disturbing its other flags. */
void itfReleaseCompactSlot(CompactSlot *slot) {
    slot->flags = slot->flags & ~SLOT_IN_USE;
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
