#include "common.h"

extern u32 D_0043895C;

extern void (*D_004389C4)(void);

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

void itfSetFadeMode(FadeEntry *entry, s32 mode, s32 value) {
    switch (mode) {
    case 0:
        if (value >= 0x80) {
            entry->extent = 0;
        }
        break;
    case 1:
        if (value >= 0x80) {
            entry->extent = 0x80;
        }
        break;
    }
    entry->mode = mode;
    entry->step = value;
}

void itfQueueFadeMode(FadeEntry *entry, u32 mode, u32 step, u32 delay) {
    entry->queuedMode = mode + 1;
    entry->queuedStep = step;
    entry->delay = delay;
}

u32 itfGetFadeExtent(FadeEntry *entry) {
    return entry->extent;
}

/* Keep the K&R signature: the matched drawing callbacks omit this argument. */
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

void itfUpdateFade(u32 address) {
    FadeEntry *entry = (FadeEntry *)address;
    if (entry->queuedMode > 0) {
        if (entry->delay == 0) {
            itfSetFadeMode(entry, entry->queuedMode - 1, entry->queuedStep);
            entry->queuedMode = 0;
        } else {
            entry->delay--;
        }
    }
    switch (entry->mode) {
    case 1:
        if (entry->extent < 0x80) {
            entry->extent += entry->step;
        }
        if (entry->extent > 0x80) {
            entry->extent = 0x80;
        }
        break;
    case 0:
        if (entry->extent > 0) {
            entry->extent -= entry->step;
        }
        if (entry->extent < 0) {
            entry->extent = 0;
        }
        break;
    }
}

void func_0031E640(u32 address) {
    s64 active;
    s32 entry;

    active = itfIsFadeActive();
    if (active != 0) {
        entry = (s32)address;
        mnuDrawIndexedFadeGlyph(0, 0, *(u32 *)(entry + 0x14), 7, 0x54);
        mnuDrawIndexedFadeGlyph(0, 0, *(u32 *)(entry + 0x14), 8, 0x54);
        mnuDrawIndexedFadeGlyph(0, 0, *(u32 *)(entry + 0x14), 9, 0x54);
        mnuDrawIndexedFadeGlyph(0, 0, *(u32 *)(entry + 0x14), 10, 0x54);
        itfUpdateFade(address);
        return;
    }
}

INCLUDE_ASM(const s32, "game/code_0031E430", func_0031E6E0);

void func_0031E7C8(FadeEntry *entry, u32 frame) {
    entry->frame = frame;
}

void func_0031E7D0(u32 arg0) {
    s64 temp_v0;
    s32 temp_v1;

    temp_v0 = itfIsFadeActive();
    if (temp_v0 != 0) {
        temp_v1 = (s32)arg0;
        mnuDrawIndexedFadeGlyph(0, 0, *(u32 *)(temp_v1 + 0x14), 3, 0x54);
        mnuDrawIndexedFadeGlyph(0, 0, *(u32 *)(temp_v1 + 0x14), 2, 0x54);
        func_0031E6E0(0xe0, 0x2a8, *(u32 *)(temp_v1 + 0x14), *(u32 *)(temp_v1 + 0x18));
        itfUpdateFade(arg0);
        return;
    }
}

void func_0031E850(FadeEntry *entry, u32 frame) {
    entry->frame = frame;
}

void func_0031E858(u32 arg0) {
    s64 temp_v0;
    s32 temp_v1;

    temp_v0 = itfIsFadeActive();
    if (temp_v0 != 0) {
        temp_v1 = (s32)arg0;
        mnuDrawIndexedFadeGlyph(0, 0, *(u32 *)(temp_v1 + 0x14), 1, 0x54);
        mnuDrawIndexedFadeGlyph(0, 0, *(u32 *)(temp_v1 + 0x14), 0, 0x54);
        func_0031E6E0(0xe0, 0x118, *(u32 *)(temp_v1 + 0x14), *(u32 *)(temp_v1 + 0x18));
        itfUpdateFade(arg0);
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

void mnuDrawFadeSequenceThree(u32 arg0) {
    if (itfIsFadeActive() != 0) {
        mnuDrawIndexedFadeGlyph(0, 0, *(u32 *)(arg0 + 0x14), 0x1a, 0x54);
        mnuDrawIndexedFadeGlyph(0, 0, *(u32 *)(arg0 + 0x14), 0x1b, 0x54);
        {
            s32 frame = *(s32 *)(arg0 + 0x18);
            if (frame < 0) {
                frame = 0;
                *(s32 *)(arg0 + 0x18) = frame;
            }
            if (frame >= 3) {
                *(s32 *)(arg0 + 0x18) = 2;
                frame = 2;
            }
            mnuDrawIndexedFadeGlyph(0, frame * 144, *(u32 *)(arg0 + 0x14), 0x1d, 0x54);
        }
        itfUpdateFade(arg0);
    }
}

void func_0031EE28(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x18) = arg1;
}

void mnuDrawFadeSequenceTwo(u32 arg0) {
    if (itfIsFadeActive() != 0) {
        mnuDrawIndexedFadeGlyph(0, 0, *(u32 *)(arg0 + 0x14), 0x1a, 0x54);
        mnuDrawIndexedFadeGlyph(0, 0, *(u32 *)(arg0 + 0x14), 0x1c, 0x54);
        {
            s32 frame = *(s32 *)(arg0 + 0x18);
            if (frame < 0) {
                frame = 0;
                *(s32 *)(arg0 + 0x18) = frame;
            }
            if (frame >= 2) {
                *(s32 *)(arg0 + 0x18) = 1;
                frame = 1;
            }
            mnuDrawIndexedFadeGlyph(0, frame * 144, *(u32 *)(arg0 + 0x14), 0x1d, 0x54);
        }
        itfUpdateFade(arg0);
    }
}

void func_0031EEE8(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x18) = arg1;
}

void mnuDrawFadeSequenceOffset(u32 arg0) {
    if (itfIsFadeActive() != 0) {
        mnuDrawIndexedFadeGlyph(0, 0, *(u32 *)(arg0 + 0x14), 0x1a, 0x54);
        mnuDrawIndexedFadeGlyph(0, 0, *(u32 *)(arg0 + 0x14), 0x1e, 0x54);
        {
            s32 frame = *(s32 *)(arg0 + 0x18);
            if (frame <= 0) {
                *(s32 *)(arg0 + 0x18) = 1;
                frame = 1;
            }
            if (frame >= 4) {
                *(s32 *)(arg0 + 0x18) = 3;
                frame = 3;
            }
            mnuDrawIndexedFadeGlyph(0, 0, *(u32 *)(arg0 + 0x14), frame + 0x1e, 0x54);
        }
        mnuDrawIndexedFadeGlyph(0, 0, *(u32 *)(arg0 + 0x14), 0x22, 0x54);
        itfUpdateFade(arg0);
    }
}

void func_0031EFB8(u32 arg0) {
    s64 temp_v0;
    s32 temp_v1;

    temp_v0 = itfIsFadeActive();
    if (temp_v0 != 0) {
        temp_v1 = (s32)arg0;
        mnuDrawIndexedFadeGlyph(0, 0, *(u32 *)(temp_v1 + 0x14), 0x1a, 0x54);
        mnuDrawIndexedFadeGlyph(0, 0, *(u32 *)(temp_v1 + 0x14), 0x23, 0x54);
        mnuDrawIndexedFadeGlyph(0, 0, *(u32 *)(temp_v1 + 0x14), 0x24, 0x54);
        itfUpdateFade(arg0);
        return;
    }
}

void func_0031F040(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x18) = arg1;
}

void func_0031F048(u32 arg0) {
    s64 temp_v0;
    s32 temp_v1;

    temp_v0 = itfIsFadeActive();
    if (temp_v0 == 0) {
        return;
    }
    temp_v1 = (s32)arg0;
    mnuDrawIndexedFadeGlyph(0, 0, *(u32 *)(temp_v1 + 0x14), 6, 0x54);
    if (*(s32 *)(temp_v1 + 0x18) != 1) {
        if (*(s32 *)(temp_v1 + 0x18) != 2) goto LAB_0031f0c4;
        mnuDrawIndexedFadeGlyph(0, 0, *(u32 *)(temp_v1 + 0x14), 5, 0x54);
    }
    mnuDrawIndexedFadeGlyph(0, 0, *(u32 *)(temp_v1 + 0x14), 4, 0x54);
LAB_0031f0c4:
    itfUpdateFade(arg0);
}

INCLUDE_SDATA(const s32, "game/code_0031E430", D_0043895C);

