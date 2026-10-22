#include "sdk_ps2.h"
#include "common.h"
#include "sdf_resource.h"
#include "mnu_shooting.h"
#include "sdf.h"


enum { SLOT_IN_USE = 1 };

extern void evtPrintDeveloperConsoleMessage(const char *format, ...);

/* Sprite-number work pool: count 0x14-byte items plus a 0xC-byte header. */
MnuSpriteNumberPool *itfCreateSpriteWorkPool(u32 count) {
    u32 size = count * 0x14 + 0xC;
    sdfMemHandle_t *allocation = sdfMemAllocDirect(size);
    MnuSpriteNumberPool *pool = (MnuSpriteNumberPool *)sdfMemGetPtr(allocation);
    memset(pool, 0, size);
    pool->allocation = allocation;
    pool->count = count;
    pool->slots = (MnuSpriteNumberSlot *)(pool + 1);
    evtPrintDeveloperConsoleMessage("Sprite Num Work Crate Size[%d]\n", size);
    return pool;
}

/* Release the allocation containing the sprite-number pool header and slots. */
void itfReleaseSpriteWorkPool(MnuSpriteNumberPool *pool) {
    sdfMemFreeDirect(pool->allocation);
}

/* Reserve the first unclaimed sprite-number slot. */
MnuSpriteNumberSlot *itfClaimFreeWideSlot(MnuSpriteNumberPool *pool) {
    MnuSpriteNumberSlot *entry;
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

MnuSpriteNumberSlot *itfClaimWideSlotWithTaggedPayload(s32 screenX, s32 screenY,
        s32 value, s8 tag, MnuSpriteNumberPool *pool) {
    MnuSpriteNumberSlot *slot = itfClaimFreeWideSlot(pool);
    u32 bits = (tag & 0xFF) << 1;

    if (slot != NULL) {
        slot->screenX = screenX;
        slot->screenY = screenY;
        slot->value = value;
        slot->ageFrames = 0;
        slot->flags = (slot->flags & ~0x1FE) | bits;
    }
    return slot;
}

/* Return a number-popup slot to the pool without disturbing its tag. */
void itfReleaseWideSlot(MnuSpriteNumberSlot *slot) {
    slot->flags = slot->flags & ~SLOT_IN_USE;
}

extern void itfDrawCenteredIndexedImage(s32 x, s32 y, s32 z, s32 alpha, s32 flags, s32 index, s32 context);

/* Draw decimal digits and optional tags with fade/rise; bit 0 pauses aging. */
void itfUpdateAndDrawSpriteNumbers(MnuSpriteNumberPool *pool, u32 flags) {
    MnuSpriteNumberSlot *slot = pool->slots;
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
                slot->ageFrames++;
            }
            if (slot->ageFrames < 20) {
                opacity = (f32)slot->ageFrames / 20.0f;
            } else if (slot->ageFrames < 70) {
                opacity = 1.0f;
            } else {
                opacity = (f32)(90 - slot->ageFrames) / 20.0f;
            }
            if (slot->ageFrames < 20) {
                rise = (f32)slot->ageFrames / 20.0f;
            } else {
                rise = 1.0f;
            }
            memset(text, 0, sizeof(text));
            length = 0;
            sprintf(text, "%d", slot->value);
            x = slot->screenX;
            y = slot->screenY;
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
                    itfDrawCenteredIndexedImage(x + offset, (s32)((f32)(y + 32) - rise * 16.0f), 0,
                        (s32)(opacity * 128.0f), 0, text[digit] - 7, 0x53);
                } else {
                    itfDrawCenteredIndexedImage(x + offset, (s32)((f32)(y + 32) - rise * 16.0f), 0,
                        (s32)(opacity * 128.0f), 0, text[digit] + 3, 0x53);
                }
            }
            if (((slot->flags >> 1) & 0xFF) != 0) {
                offset += 13;
                switch ((slot->flags >> 1) & 0xFF) {
                case 1:
                    itfDrawCenteredIndexedImage(x + offset, (s32)((f32)(y + 32) - rise * 16.0f), 0,
                        (s32)(opacity * 128.0f), 0, 61, 0x53);
                    break;
                case 2:
                    itfDrawCenteredIndexedImage(x + offset, (s32)((f32)(y + 32) - rise * 16.0f), 0,
                        (s32)(opacity * 128.0f), 0, 62, 0x53);
                    break;
                case 4:
                    itfDrawCenteredIndexedImage(x + offset, (s32)((f32)(y + 32) - rise * 16.0f), 0,
                        (s32)(opacity * 128.0f), 0, 63, 0x53);
                    break;
                case 8:
                    itfDrawCenteredIndexedImage(x + offset, (s32)((f32)(y + 32) - rise * 16.0f), 0,
                        (s32)(opacity * 128.0f), 0, 64, 0x53);
                    break;
                case 20:
                    itfDrawCenteredIndexedImage(x + offset, (s32)((f32)(y + 32) - rise * 16.0f), 0,
                        (s32)(opacity * 128.0f), 0, 75, 0x53);
                    break;
                }
            }
            if (slot->ageFrames >= 91) {
                itfReleaseWideSlot(slot);
            }
        }
    }
}

/* Sprite-hit-effect work pool: count 0x10-byte items plus a 0xC-byte header. */
MnuHitEffectPool *itfCreateSpriteHitEffectWorkPool(u32 count) {
    u32 size = count * 0x10 + 0xC;
    sdfMemHandle_t *allocation = sdfMemAllocDirect(size);
    MnuHitEffectPool *pool = (MnuHitEffectPool *)sdfMemGetPtr(allocation);

    memset(pool, 0, size);
    pool->allocation = allocation;
    pool->count = count;
    pool->slots = (MnuHitEffectSlot *)(pool + 1);
    evtPrintDeveloperConsoleMessage("Sprite HitEffect Work Crate Size[%d]\n", size);
    return pool;
}

/* Release the allocation containing the hit-effect pool header and records. */
void itfReleaseSpriteHitEffectWorkPool(MnuHitEffectPool *pool) {
    sdfMemFreeDirect(pool->allocation);
}

/* Reserve the first unclaimed hit-effect slot. */
MnuHitEffectSlot *itfClaimFreeCompactSlot(MnuHitEffectPool *pool) {
    MnuHitEffectSlot *entry;
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

/* Claim a hit-effect slot at the supplied screen point. */
MnuHitEffectSlot *itfClaimCompactSlotWithPayload(s32 screenX, s32 screenY,
        MnuHitEffectPool *pool) {
    MnuHitEffectSlot *slot = itfClaimFreeCompactSlot(pool);

    if (slot != NULL) {
        slot->screenX = screenX;
        slot->screenY = screenY;
        slot->ageFrames = 0;
    }
    return slot;
}

/* Return a hit-effect slot to the pool. */
void itfReleaseCompactSlot(MnuHitEffectSlot *slot) {
    slot->flags = slot->flags & ~SLOT_IN_USE;
}

extern void sdfSubmitGsTestOneRegisterPacket(u64 data, u32 kind);
extern void uiDrawUniformColorRect(u32 x, u32 y, u32 z, u32 width, u32 height, u32 color, u32 surfaceIndex);
extern void itfDrawUniformlyScaledIndexedImage(s32 x, s32 y, s32 z, s32 alpha, s32 flags, s32 index, s32 context, f32 scale);

void itfUpdateAndDrawSpriteHitEffects(MnuHitEffectPool *pool, u32 flags) {
    MnuHitEffectSlot *slot;
    s32 i;
    f32 phase;

    sdfSubmitGsTestOneRegisterPacket(0x30000, 0x54);
    uiDrawUniformColorRect(0, 0, 0, 0x2000, 0xE00, 0, 0x54);
    sdfSubmitGsTestOneRegisterPacket(0x5100D, 0x54);
    slot = pool->slots;
    for (i = 0; i < pool->count; i++, slot++) {
        if ((slot->flags & SLOT_IN_USE) != 0) {
            if ((flags & 1) == 0) {
                slot->ageFrames++;
            }
            phase = (f32)slot->ageFrames / 10.0f;
            itfDrawUniformlyScaledIndexedImage(slot->screenX, slot->screenY, 0,
                (s32)((1.0f - phase) * 128.0f * 0.8f), 0, 8, 0x54, phase * 2.0f + 0.5f);
            if (slot->ageFrames >= 11) {
                itfReleaseCompactSlot(slot);
            }
        }
    }
}


void itfApplyWorkTintAndClearBuffers(u8 *work) {
    MnuShootingWork *state = (MnuShootingWork *)work;

    itfFadeSetTint(state->fadeResources.fadeSprites);
    memset(&state->strip, 0, sizeof(state->strip));
    memset(&state->lowerNumber, 0, sizeof(state->lowerNumber));
    memset(&state->upperNumber, 0, sizeof(state->upperNumber));
    memset(&state->gauge, 0, sizeof(state->gauge));
    memset(&state->pauseFade, 0, sizeof(state->pauseFade));
    memset(&state->roundFade, 0, sizeof(state->roundFade));
    memset(&state->scoreFade, 0, sizeof(state->scoreFade));
    memset(&state->frame, 0, sizeof(state->frame));
}

extern void itfFadeClearTint(void);

void itfClearTintAndWorkBuffers(u8 *work) {
    MnuShootingWork *state = (MnuShootingWork *)work;

    itfFadeClearTint();
    memset(&state->strip, 0, sizeof(state->strip));
    memset(&state->lowerNumber, 0, sizeof(state->lowerNumber));
    memset(&state->upperNumber, 0, sizeof(state->upperNumber));
    memset(&state->gauge, 0, sizeof(state->gauge));
    memset(&state->pauseFade, 0, sizeof(state->pauseFade));
    memset(&state->roundFade, 0, sizeof(state->roundFade));
    memset(&state->scoreFade, 0, sizeof(state->scoreFade));
    memset(&state->frame, 0, sizeof(state->frame));
}


/* Fade blocks have a common state prefix and callback-specific trailing data. */
void itfDrawFullExtentWorkPanels(MnuShootingWork *work) {
    itfSetFadeMode(&work->strip, 1, 0x80);
    itfDrawFadeGlyphStrip(&work->strip);
    itfSetFadeMode(&work->lowerNumber.fade, 1, 0x80);
    mnuDrawNumberFadeWith(itfDrawLowerFadeGlyphPair, &work->lowerNumber);
    itfSetLowerFadeNumberDisplayValue(&work->lowerNumber, work->currentScore);
    itfSetFadeMode(&work->upperNumber.fade, 1, 0x80);
    mnuDrawNumberFadeWith(itfDrawUpperFadeGlyphPair, &work->upperNumber);
    itfSetUpperFadeNumberDisplayValue(&work->upperNumber, work->peakScore);
    itfSetFadeMode(&work->gauge.fade, 1, 0x80);
    itfSetFadeParameter(&work->gauge, 100);
    itfSetFadeSecondaryParameter(&work->gauge, work->progress);
    if (work->completed > 0) {
        itfSetFadeGaugeDisplayValue(&work->gauge, 1);
    } else {
        itfSetFadeGaugeDisplayValue(&work->gauge, 0);
    }
    func_0031E8F0(&work->gauge);
    itfSetFadeMode(&work->frame.fade, 1, 0x80);
    itfSetFrameFadeDisplayValue(&work->frame, work->frameDisplayValue);
    itfDrawFadeGlyphForFrame(&work->frame);
}

/* Per-glyph texture byte, one 12-byte entry per glyph. */


u8 mnuGetIndexedFadeTexture(s32 glyphIndex) {
    return D_0040B088[glyphIndex].glyph;
}
