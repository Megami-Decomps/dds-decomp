#include "common.h"

extern u32 itfFadeTint;

extern void (*sdfTickCallback)(void);

/* The glyph renderer receives twice the stored extent; full fade passes 0x100. */
#define FADE_MAX_EXTENT 0x80
#define FADE_MODE_OUT 0
#define FADE_MODE_IN 1

typedef struct FadeOffset {
    s32 unused;
    s32 x;
    s32 y;
} FadeOffset;

typedef struct FadeEntry {
    s32 mode;
    s32 step;
    s32 queuedMode; /* zero = no transition; otherwise stores mode + 1 */
    s32 queuedStep;
    s32 delay;
    s32 extent;
    s32 displayValue; /* decimal number or atlas/glyph selection, depending on callback */
    s32 parameter;
    s32 secondaryParameter;
} FadeEntry;
extern const FadeOffset D_0040B088[];
extern s32 mnuGetIndexedFadeTexture(s32 index);

/* Draw the indexed glyph at its per-glyph origin with the current tint. */
s32 mnuDrawIndexedFadeGlyph(s32 x, s32 y, s32 extent, s32 index, s32 effect) {
    s32 texture = mnuGetIndexedFadeTexture(index);
    return func_00306CD0(x + D_0040B088[index].x,
                         y + D_0040B088[index].y,
                         0, extent * 2, 0, itfFadeTint, texture, effect);
}

void itfFadeSetTint(u32 tint) {
    itfFadeTint = tint;
}

void itfFadeClearTint(void) {
    itfFadeTint = 0;
}

/* Set a fade direction; a completed step snaps directly to its endpoint. */
void itfSetFadeMode(FadeEntry *entry, s32 mode, s32 step) {
    switch (mode) {
    case FADE_MODE_OUT:
        if (step >= FADE_MAX_EXTENT) {
            entry->extent = 0;
        }
        break;
    case FADE_MODE_IN:
        if (step >= FADE_MAX_EXTENT) {
            entry->extent = FADE_MAX_EXTENT;
        }
        break;
    }
    entry->mode = mode;
    entry->step = step;
}

/* Queue a fade transition after the requested number of updates. */
void itfQueueFadeMode(FadeEntry *entry, u32 mode, u32 step, u32 delay) {
    entry->queuedMode = mode + 1;
    entry->queuedStep = step;
    entry->delay = delay;
}

u32 itfGetFadeExtent(FadeEntry *entry) {
    return entry->extent;
}

/* K&R signature retained: matched drawing callbacks omit the argument. */
s32 itfIsFadeActive(entry)
    FadeEntry *entry;
{
    if (entry->extent == 0) {
        if (entry->mode == 0) {
            return 0;
        }
    }
    return 1;
}

/* Process a queued transition and clamp the current fade extent each frame. */
void itfUpdateFade(FadeEntry *entry) {
    if (entry->queuedMode > 0) {
        if (entry->delay == 0) {
            itfSetFadeMode(entry, entry->queuedMode - 1, entry->queuedStep);
            entry->queuedMode = 0;
        } else {
            entry->delay--;
        }
    }
    switch (entry->mode) {
    case FADE_MODE_IN:
        if (entry->extent < FADE_MAX_EXTENT) {
            entry->extent += entry->step;
        }
        if (entry->extent > FADE_MAX_EXTENT) {
            entry->extent = FADE_MAX_EXTENT;
        }
        break;
    case FADE_MODE_OUT:
        if (entry->extent > 0) {
            entry->extent -= entry->step;
        }
        if (entry->extent < 0) {
            entry->extent = 0;
        }
        break;
    }
}

/* Draw the four-part strip only while the fade is visible or in progress. */
void itfDrawFadeGlyphStrip(FadeEntry *entry) {
    s64 active;

    active = itfIsFadeActive();
    if (active != 0) {
        mnuDrawIndexedFadeGlyph(0, 0, entry->extent, 7, 0x54);
        mnuDrawIndexedFadeGlyph(0, 0, entry->extent, 8, 0x54);
        mnuDrawIndexedFadeGlyph(0, 0, entry->extent, 9, 0x54);
        mnuDrawIndexedFadeGlyph(0, 0, entry->extent, 10, 0x54);
        itfUpdateFade(entry);
        return;
    }
}

/* Keep all decimal places visible, dimming zeros before the first nonzero digit. */
void func_0031E6E0(s32 x, s32 y, s32 extent, u32 value) {
    s32 placeValue = 10000000;
    s32 leadingZero = 1;
    s32 i;

    for (i = 0; i < 8; i++) {
        s32 digit = value / placeValue;

        if (digit > 0) {
            leadingZero = 0;
        }
        value -= placeValue * digit;
        placeValue /= 10;
        mnuDrawIndexedFadeGlyph(x, y, leadingZero == 0 ? extent : extent / 2, digit + 11, 0x54);
        x += 0x90;
    }
}

void func_0031E7C8(FadeEntry *entry, u32 displayValue) {
    entry->displayValue = displayValue;
}

void itfDrawLowerFadeGlyphPair(FadeEntry *entry) {
    s64 active;

    active = itfIsFadeActive();
    if (active != 0) {
        mnuDrawIndexedFadeGlyph(0, 0, entry->extent, 3, 0x54);
        mnuDrawIndexedFadeGlyph(0, 0, entry->extent, 2, 0x54);
        func_0031E6E0(0xe0, 0x2a8, entry->extent, entry->displayValue);
        itfUpdateFade(entry);
        return;
    }
}

void func_0031E850(FadeEntry *entry, u32 displayValue) {
    entry->displayValue = displayValue;
}

void itfDrawUpperFadeGlyphPair(FadeEntry *entry) {
    s64 active;

    active = itfIsFadeActive();
    if (active != 0) {
        mnuDrawIndexedFadeGlyph(0, 0, entry->extent, 1, 0x54);
        mnuDrawIndexedFadeGlyph(0, 0, entry->extent, 0, 0x54);
        func_0031E6E0(0xe0, 0x118, entry->extent, entry->displayValue);
        itfUpdateFade(entry);
        return;
    }
}

void itfSetFadeParameter(FadeEntry *entry, u32 parameter) {
    entry->parameter = parameter;
}

void itfSetFadeSecondaryParameter(FadeEntry *entry, u32 parameter) {
    entry->secondaryParameter = parameter;
}

void func_0031E8E8(FadeEntry *entry, u32 displayValue) {
    entry->displayValue = displayValue;
}

INCLUDE_ASM(const s32, "game/code_0031E430", func_0031E8F0);

void func_0031ED68(FadeEntry *entry, u32 displayValue) {
    entry->displayValue = displayValue;
}

/* Clamp the selected atlas row and draw the three-part fade. */
void mnuDrawFadeSequenceThree(FadeEntry *entry) {
    if (itfIsFadeActive() != 0) {
        mnuDrawIndexedFadeGlyph(0, 0, entry->extent, 0x1a, 0x54);
        mnuDrawIndexedFadeGlyph(0, 0, entry->extent, 0x1b, 0x54);
        {
            s32 rowIndex = entry->displayValue;
            if (rowIndex < 0) {
                rowIndex = 0;
                entry->displayValue = rowIndex;
            }
            if (rowIndex >= 3) {
                entry->displayValue = 2;
                rowIndex = 2;
            }
            mnuDrawIndexedFadeGlyph(0, rowIndex * 144, entry->extent, 0x1d, 0x54);
        }
        itfUpdateFade(entry);
    }
}

void func_0031EE28(FadeEntry *entry, u32 displayValue) {
    entry->displayValue = displayValue;
}

/* Clamp the selected atlas row and draw the alternate three-part fade. */
void mnuDrawFadeSequenceTwo(FadeEntry *entry) {
    if (itfIsFadeActive() != 0) {
        mnuDrawIndexedFadeGlyph(0, 0, entry->extent, 0x1a, 0x54);
        mnuDrawIndexedFadeGlyph(0, 0, entry->extent, 0x1c, 0x54);
        {
            s32 rowIndex = entry->displayValue;
            if (rowIndex < 0) {
                rowIndex = 0;
                entry->displayValue = rowIndex;
            }
            if (rowIndex >= 2) {
                entry->displayValue = 1;
                rowIndex = 1;
            }
            mnuDrawIndexedFadeGlyph(0, rowIndex * 144, entry->extent, 0x1d, 0x54);
        }
        itfUpdateFade(entry);
    }
}

void func_0031EEE8(FadeEntry *entry, u32 displayValue) {
    entry->displayValue = displayValue;
}

/* Draw the offset glyph sequence, clamping its selector to the valid 1..3 range. */
void mnuDrawFadeSequenceOffset(FadeEntry *entry) {
    if (itfIsFadeActive() != 0) {
        mnuDrawIndexedFadeGlyph(0, 0, entry->extent, 0x1a, 0x54);
        mnuDrawIndexedFadeGlyph(0, 0, entry->extent, 0x1e, 0x54);
        {
            s32 glyphOffset = entry->displayValue;
            if (glyphOffset <= 0) {
                entry->displayValue = 1;
                glyphOffset = 1;
            }
            if (glyphOffset >= 4) {
                entry->displayValue = 3;
                glyphOffset = 3;
            }
            mnuDrawIndexedFadeGlyph(0, 0, entry->extent, glyphOffset + 0x1e, 0x54);
        }
        mnuDrawIndexedFadeGlyph(0, 0, entry->extent, 0x22, 0x54);
        itfUpdateFade(entry);
    }
}

void itfDrawFadeGlyphTriplet(FadeEntry *entry) {
    s64 active;

    active = itfIsFadeActive();
    if (active != 0) {
        mnuDrawIndexedFadeGlyph(0, 0, entry->extent, 0x1a, 0x54);
        mnuDrawIndexedFadeGlyph(0, 0, entry->extent, 0x23, 0x54);
        mnuDrawIndexedFadeGlyph(0, 0, entry->extent, 0x24, 0x54);
        itfUpdateFade(entry);
        return;
    }
}

void func_0031F040(FadeEntry *entry, u32 displayValue) {
    entry->displayValue = displayValue;
}

/* Select optional glyphs from the display value; every choice advances the fade. */
void itfDrawFadeGlyphForFrame(FadeEntry *entry) {
    s64 active;

    active = itfIsFadeActive();
    if (active == 0) {
        return;
    }
    mnuDrawIndexedFadeGlyph(0, 0, entry->extent, 6, 0x54);
    if (entry->displayValue != 1) {
        if (entry->displayValue != 2) goto update;
        mnuDrawIndexedFadeGlyph(0, 0, entry->extent, 5, 0x54);
    }
    mnuDrawIndexedFadeGlyph(0, 0, entry->extent, 4, 0x54);
update:
    itfUpdateFade(entry);
}

INCLUDE_SDATA(const s32, "game/code_0031E430", itfFadeTint);

