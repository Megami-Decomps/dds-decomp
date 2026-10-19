#include "common.h"
#include "eff.h"
#include "mnu_fade.h"


extern void (*sdfTickCallback)(void);

/* The glyph renderer receives twice the stored extent; full fade passes 0x100. */
#define FADE_MAX_EXTENT 0x80
#define FADE_MODE_OUT 0
#define FADE_MODE_IN 1


extern void effDrawTextureSlotWithBlendedCornerColors(s32, s32, s32, u32, s32, EffectSlotSet *, s32, s32);

/* Draw the indexed glyph at its per-glyph origin with the current tint. */
void mnuDrawIndexedFadeGlyph(s32 x, s32 y, s32 extent, s32 glyphIndex, s32 effect) {
    s32 texture = mnuGetIndexedFadeTexture(glyphIndex);
    effDrawTextureSlotWithBlendedCornerColors(x + D_0040B088[glyphIndex].x,
                         y + D_0040B088[glyphIndex].y,
                         0, extent * 2, 0, itfFadeTint, texture, effect);
}

void itfFadeSetTint(EffectSlotSet *tint) {
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

s32 itfIsFadeActive(FadeEntry *entry)
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
    s32 active;

    active = itfIsFadeActive(entry);
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
void itfDrawEightDigitFadeValue(s32 x, s32 y, s32 extent, u32 value) {
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

/* Set the value consumed by the lower eight-digit fade display. */
void itfSetLowerFadeNumberDisplayValue(FadeNumber *entry, u32 displayValue) {
    entry->displayValue = displayValue;
}

void itfDrawLowerFadeGlyphPair(FadeNumber *entry) {
    s32 active;

    active = itfIsFadeActive(&entry->fade);
    if (active != 0) {
        mnuDrawIndexedFadeGlyph(0, 0, entry->fade.extent, 3, 0x54);
        mnuDrawIndexedFadeGlyph(0, 0, entry->fade.extent, 2, 0x54);
        itfDrawEightDigitFadeValue(0xe0, 0x2a8, entry->fade.extent, entry->displayValue);
        itfUpdateFade(&entry->fade);
        return;
    }
}

/* Set the value consumed by the upper eight-digit fade display. */
void itfSetUpperFadeNumberDisplayValue(FadeNumber *entry, u32 displayValue) {
    entry->displayValue = displayValue;
}

void itfDrawUpperFadeGlyphPair(FadeNumber *entry) {
    s32 active;

    active = itfIsFadeActive(&entry->fade);
    if (active != 0) {
        mnuDrawIndexedFadeGlyph(0, 0, entry->fade.extent, 1, 0x54);
        mnuDrawIndexedFadeGlyph(0, 0, entry->fade.extent, 0, 0x54);
        itfDrawEightDigitFadeValue(0xe0, 0x118, entry->fade.extent, entry->displayValue);
        itfUpdateFade(&entry->fade);
        return;
    }
}

void itfSetFadeParameter(FadeGauge *entry, u32 parameter) {
    entry->parameter = parameter;
}

void itfSetFadeSecondaryParameter(FadeGauge *entry, u32 parameter) {
    entry->secondaryParameter = parameter;
}

void itfSetFadeGaugeDisplayValue(FadeGauge *entry, u32 displayValue) {
    entry->displayValue = displayValue;
}

/* Draw the shooting progress segments, decay overlay, and completion pulse. */
void func_0031E8F0(FadeGauge *entry) {
    s32 segmentSize, remainder, rowCount;
    s32 texture, opacity, row, x;
    s32 cropWidth, rightCrop;
    BdWork *work;

    if (itfIsFadeActive(&entry->fade) == 0) return;
    mnuDrawIndexedFadeGlyph(0, 0, entry->fade.extent, 0x15, 0x54);
    if (entry->displayValue == 0) {
        entry->phase = 0;
    } else {
        entry->phase += 32;
        if (entry->phase > 512) entry->phase = 512;
    }
    segmentSize = entry->parameter >> 2;
    rowCount = entry->secondaryParameter / segmentSize;
    /* Keep the trailing partial segment even at an exact boundary. */
    rowCount++;
    remainder = entry->secondaryParameter % segmentSize;
    texture = mnuGetIndexedFadeTexture(0x16);
    opacity = 128;
    if (entry->phase > 256) opacity = (512 - entry->phase) / 2;
    opacity = opacity * entry->fade.extent / 128;
    x = 0;
    for (row = 0; row < rowCount; row++) {
        if (row < rowCount - 1) {
            BdWork *rowWork = (BdWork *)(texture * sizeof(BdWork) +
                                       (u32)itfFadeTint->workEntries);
            rowWork->parameters[0] = 0;
            rowWork->parameters[2] = 0;
            rowWork->geometry.bounds[2] = rowWork->sourceWidth * 16;
            mnuDrawIndexedFadeGlyph(x, 0, opacity, 0x16, 0x54);
            entry->filled[row] = 1;
        } else {
            BdWork *partialWork;

            cropWidth = remainder * 40 / segmentSize;
            partialWork = (BdWork *)(texture * sizeof(BdWork) +
                                    (u32)itfFadeTint->workEntries);
            partialWork->parameters[0] = 1;
            rightCrop = 40 - cropWidth;
            partialWork->geometry.bounds[2] = cropWidth * 16;
            partialWork->parameters[2] = -1 - rightCrop;
            mnuDrawIndexedFadeGlyph(x + 16, 0, opacity, 0x16, 0x54);
        }
        x += 0x300;
    }
    if (entry->displayValue == 0) {
        for (row = rowCount - 1; row < 4; row++) entry->filled[row] = 0;
    }
    x = 0;
    for (row = 0; row < 4; row++) {
        mnuDrawIndexedFadeGlyph(x, 0,
            entry->decay[row] * entry->fade.extent / 128, 0x17, 0x54);
        x += 0x300;
        if (entry->filled[row] != 0) {
            if (entry->decay[row] < 128) entry->decay[row] += 16;
            if (entry->decay[row] > 128) entry->decay[row] = 128;
        } else {
            if (entry->decay[row] > 0) entry->decay[row] -= 16;
            if (entry->decay[row] < 0) entry->decay[row] = 0;
        }
    }
    if (entry->phase < 256) opacity = entry->phase / 2;
    else opacity = (512 - entry->phase) / 2;
    mnuDrawIndexedFadeGlyph(0, 0, opacity * entry->fade.extent / 128, 0x18, 0x54);
    segmentSize = entry->parameter;
    remainder = entry->secondaryParameter % segmentSize;
    texture = mnuGetIndexedFadeTexture(0x19);
    opacity = 0;
    if (entry->phase > 256) opacity = entry->phase / 2 - 128;
    opacity = opacity * entry->fade.extent / 128;
    cropWidth = remainder * 195 / segmentSize;
    work = (BdWork *)(texture * sizeof(BdWork) +
                     (u32)itfFadeTint->workEntries);
    work->parameters[0] = 5;
    rightCrop = 195 - cropWidth;
    work->geometry.bounds[2] = cropWidth * 16;
    work->parameters[2] = -6 - rightCrop;
    mnuDrawIndexedFadeGlyph(80, 0, opacity, 0x19, 0x54);
    itfUpdateFade(&entry->fade);
}

void mnuSetPauseFadeChoice(FadeNumber *entry, u32 displayValue) {
    entry->displayValue = displayValue;
}

/* Clamp the selected atlas row and draw the three-part fade. */
void mnuDrawFadeSequenceThree(FadeNumber *entry) {
    if (itfIsFadeActive(&entry->fade) != 0) {
        mnuDrawIndexedFadeGlyph(0, 0, entry->fade.extent, 0x1a, 0x54);
        mnuDrawIndexedFadeGlyph(0, 0, entry->fade.extent, 0x1b, 0x54);
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
            mnuDrawIndexedFadeGlyph(0, rowIndex * 144, entry->fade.extent, 0x1d, 0x54);
        }
        itfUpdateFade(&entry->fade);
    }
}

/* Set the two-choice atlas row; the drawing function performs the clamp. */
void itfSetTwoChoiceFadeSelection(FadeNumber *entry, u32 displayValue) {
    entry->displayValue = displayValue;
}

/* Clamp the selected atlas row and draw the alternate three-part fade. */
void mnuDrawFadeSequenceTwo(FadeNumber *entry) {
    if (itfIsFadeActive(&entry->fade) != 0) {
        mnuDrawIndexedFadeGlyph(0, 0, entry->fade.extent, 0x1a, 0x54);
        mnuDrawIndexedFadeGlyph(0, 0, entry->fade.extent, 0x1c, 0x54);
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
            mnuDrawIndexedFadeGlyph(0, rowIndex * 144, entry->fade.extent, 0x1d, 0x54);
        }
        itfUpdateFade(&entry->fade);
    }
}

/* Set the round display's glyph selector; its drawer clamps to 1..3. */
void itfSetRoundFadeDisplayValue(FadeNumber *entry, u32 displayValue) {
    entry->displayValue = displayValue;
}

/* Draw the offset glyph sequence, clamping its selector to the valid 1..3 range. */
void mnuDrawFadeSequenceOffset(FadeNumber *entry) {
    if (itfIsFadeActive(&entry->fade) != 0) {
        mnuDrawIndexedFadeGlyph(0, 0, entry->fade.extent, 0x1a, 0x54);
        mnuDrawIndexedFadeGlyph(0, 0, entry->fade.extent, 0x1e, 0x54);
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
            mnuDrawIndexedFadeGlyph(0, 0, entry->fade.extent, glyphOffset + 0x1e, 0x54);
        }
        mnuDrawIndexedFadeGlyph(0, 0, entry->fade.extent, 0x22, 0x54);
        itfUpdateFade(&entry->fade);
    }
}

void itfDrawFadeGlyphTriplet(FadeEntry *entry) {
    s32 active;

    active = itfIsFadeActive(entry);
    if (active != 0) {
        mnuDrawIndexedFadeGlyph(0, 0, entry->extent, 0x1a, 0x54);
        mnuDrawIndexedFadeGlyph(0, 0, entry->extent, 0x23, 0x54);
        mnuDrawIndexedFadeGlyph(0, 0, entry->extent, 0x24, 0x54);
        itfUpdateFade(entry);
        return;
    }
}

/* Set the selector for optional glyphs in the frame fade display. */
void itfSetFrameFadeDisplayValue(FadeNumber *entry, u32 displayValue) {
    entry->displayValue = displayValue;
}

/* Select optional glyphs from the display value; every choice advances the fade. */
void itfDrawFadeGlyphForFrame(FadeNumber *entry) {
    s32 active;

    active = itfIsFadeActive(&entry->fade);
    if (active == 0) {
        return;
    }
    mnuDrawIndexedFadeGlyph(0, 0, entry->fade.extent, 6, 0x54);
    switch (entry->displayValue) {
    case 2:
        mnuDrawIndexedFadeGlyph(0, 0, entry->fade.extent, 5, 0x54);
        /* fall through */
    case 1:
        mnuDrawIndexedFadeGlyph(0, 0, entry->fade.extent, 4, 0x54);
        break;
    default:
        break;
    }
    itfUpdateFade(&entry->fade);
}

INCLUDE_SDATA(const s32, "game/code_0031E430", itfFadeTint);

