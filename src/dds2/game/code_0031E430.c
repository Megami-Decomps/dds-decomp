#include "common.h"

extern u32 D_0043895C;

extern void (*D_004389C4)(void);

/* Fade width is in half-pixels; the glyph renderer doubles it at draw time. */
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
    s32 queuedMode;
    s32 queuedStep;
    s32 delay;
    s32 extent;
    s32 frame;
    s32 parameter;
    s32 secondaryParameter;
} FadeEntry;
extern const FadeOffset D_0040B088[];
extern s32 func_0031E410(s32 index);

/* Draw the indexed glyph at its per-glyph origin with the current tint. */
s32 mnuDrawIndexedFadeGlyph(s32 x, s32 y, s32 width, s32 index, s32 effect) {
    s32 texture = func_0031E410(index);
    return func_00306CD0(x + D_0040B088[index].x,
                         y + D_0040B088[index].y,
                         0, width * 2, 0, D_0043895C, texture, effect);
}

void itfFadeSetTint(u32 tint) {
    D_0043895C = tint;
}

void itfFadeClearTint(void) {
    D_0043895C = 0;
}

/* Set a fade direction; a completed step snaps directly to its endpoint. */
void itfSetFadeMode(FadeEntry *entry, s32 mode, s32 value) {
    switch (mode) {
    case FADE_MODE_OUT:
        if (value >= FADE_MAX_EXTENT) {
            entry->extent = 0;
        }
        break;
    case FADE_MODE_IN:
        if (value >= FADE_MAX_EXTENT) {
            entry->extent = FADE_MAX_EXTENT;
        }
        break;
    }
    entry->mode = mode;
    entry->step = value;
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

INCLUDE_ASM(const s32, "game/code_0031E430", func_0031E6E0);

void func_0031E7C8(FadeEntry *entry, u32 frame) {
    entry->frame = frame;
}

void func_0031E7D0(FadeEntry *entry) {
    s64 active;

    active = itfIsFadeActive();
    if (active != 0) {
        mnuDrawIndexedFadeGlyph(0, 0, entry->extent, 3, 0x54);
        mnuDrawIndexedFadeGlyph(0, 0, entry->extent, 2, 0x54);
        func_0031E6E0(0xe0, 0x2a8, entry->extent, entry->frame);
        itfUpdateFade(entry);
        return;
    }
}

void func_0031E850(FadeEntry *entry, u32 frame) {
    entry->frame = frame;
}

void func_0031E858(FadeEntry *entry) {
    s64 active;

    active = itfIsFadeActive();
    if (active != 0) {
        mnuDrawIndexedFadeGlyph(0, 0, entry->extent, 1, 0x54);
        mnuDrawIndexedFadeGlyph(0, 0, entry->extent, 0, 0x54);
        func_0031E6E0(0xe0, 0x118, entry->extent, entry->frame);
        itfUpdateFade(entry);
        return;
    }
}

void func_0031E8D8(FadeEntry *entry, u32 parameter) {
    entry->parameter = parameter;
}

void func_0031E8E0(FadeEntry *entry, u32 parameter) {
    entry->secondaryParameter = parameter;
}

void func_0031E8E8(FadeEntry *entry, u32 frame) {
    entry->frame = frame;
}

INCLUDE_ASM(const s32, "game/code_0031E430", func_0031E8F0);

void func_0031ED68(FadeEntry *entry, u32 frame) {
    entry->frame = frame;
}

/* Clamp the frame to three atlas rows and draw the three-part fade. */
void mnuDrawFadeSequenceThree(FadeEntry *entry) {
    if (itfIsFadeActive() != 0) {
        mnuDrawIndexedFadeGlyph(0, 0, entry->extent, 0x1a, 0x54);
        mnuDrawIndexedFadeGlyph(0, 0, entry->extent, 0x1b, 0x54);
        {
            s32 frame = entry->frame;
            if (frame < 0) {
                frame = 0;
                entry->frame = frame;
            }
            if (frame >= 3) {
                entry->frame = 2;
                frame = 2;
            }
            mnuDrawIndexedFadeGlyph(0, frame * 144, entry->extent, 0x1d, 0x54);
        }
        itfUpdateFade(entry);
    }
}

void func_0031EE28(FadeEntry *entry, u32 frame) {
    entry->frame = frame;
}

/* Clamp the frame to two atlas rows and draw the three-part fade. */
void mnuDrawFadeSequenceTwo(FadeEntry *entry) {
    if (itfIsFadeActive() != 0) {
        mnuDrawIndexedFadeGlyph(0, 0, entry->extent, 0x1a, 0x54);
        mnuDrawIndexedFadeGlyph(0, 0, entry->extent, 0x1c, 0x54);
        {
            s32 frame = entry->frame;
            if (frame < 0) {
                frame = 0;
                entry->frame = frame;
            }
            if (frame >= 2) {
                entry->frame = 1;
                frame = 1;
            }
            mnuDrawIndexedFadeGlyph(0, frame * 144, entry->extent, 0x1d, 0x54);
        }
        itfUpdateFade(entry);
    }
}

void func_0031EEE8(FadeEntry *entry, u32 frame) {
    entry->frame = frame;
}

/* Draw the offset atlas sequence, clamping frames to the valid 1..3 range. */
void mnuDrawFadeSequenceOffset(FadeEntry *entry) {
    if (itfIsFadeActive() != 0) {
        mnuDrawIndexedFadeGlyph(0, 0, entry->extent, 0x1a, 0x54);
        mnuDrawIndexedFadeGlyph(0, 0, entry->extent, 0x1e, 0x54);
        {
            s32 frame = entry->frame;
            if (frame <= 0) {
                entry->frame = 1;
                frame = 1;
            }
            if (frame >= 4) {
                entry->frame = 3;
                frame = 3;
            }
            mnuDrawIndexedFadeGlyph(0, 0, entry->extent, frame + 0x1e, 0x54);
        }
        mnuDrawIndexedFadeGlyph(0, 0, entry->extent, 0x22, 0x54);
        itfUpdateFade(entry);
    }
}

void func_0031EFB8(FadeEntry *entry) {
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

void func_0031F040(FadeEntry *entry, u32 frame) {
    entry->frame = frame;
}

void func_0031F048(FadeEntry *entry) {
    s64 active;

    active = itfIsFadeActive();
    if (active == 0) {
        return;
    }
    mnuDrawIndexedFadeGlyph(0, 0, entry->extent, 6, 0x54);
    if (entry->frame != 1) {
        if (entry->frame != 2) goto LAB_0031f0c4;
        mnuDrawIndexedFadeGlyph(0, 0, entry->extent, 5, 0x54);
    }
    mnuDrawIndexedFadeGlyph(0, 0, entry->extent, 4, 0x54);
LAB_0031f0c4:
    itfUpdateFade(entry);
}

INCLUDE_SDATA(const s32, "game/code_0031E430", D_0043895C);

