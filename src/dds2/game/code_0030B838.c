#include "common.h"
#include "fpu.h"

extern s32 D_004388B4;

typedef struct SdfQuad {
    u32 word[4];
} SdfQuad;

extern SdfQuad D_00400970;

extern SdfQuad D_00400980;

extern void effObjSetInnerFirstVec(s32, void *);

extern void effObjSetInnerSecondVec(s32, void *);

extern s32 func_0030DE08(f32, f32);

extern s32 func_0030C5D8();

extern u64 frFontMeasureLines(u64);

extern u64 func_0019F448(u64, u64, u64, u64, u64, u64);

extern u32 D_004388B8;

extern s32 D_004388C4;

typedef struct {
    u32 *word;         /* 0x00 */
    u8 pad04[4];
    s16 value;         /* 0x08 */
} SdfCounterDisplay;

typedef struct {
    s32 value;            /* 0x00 */
    s16 countdown;        /* 0x04 */
    s16 mode;             /* 0x06 */
    u8 pad08[4];
    s16 mapTimerPrimary;  /* 0x0C */
    s16 mapTimerSecondary; /* 0x0E */
} SdfCounterTimer;

typedef struct SdfCounterChannel {
    u8 pad00[0x58];
    struct SdfCounterChannel *next; /* 0x58 */
    u8 pad5C[0x14];
    SdfCounterDisplay *display;     /* 0x70 */
} SdfCounterChannel;

typedef struct {
    u8 pad00[0x10];
    SdfCounterChannel *first;       /* 0x10 */
    u8 pad14[8];
    SdfCounterChannel *channel;     /* 0x1C */
    u8 pad20[0x10];
    SdfCounterTimer *timer;         /* 0x30 */
} SdfCounterRuntime;

extern u32 D_00439098;

extern s32 D_0043909C;

extern u32 D_004390A0;

extern s32 func_0030C9B8(void);

extern s32 D_004388BC;

extern void func_0030BBA8(void);

extern s8 D_004388C0;

extern void sdfCounterTickCountdown(void);

extern void mnuTickMapTimers(void);

extern s32 func_0030D438(void);

extern s32 func_0030BD10();

typedef struct MapResource {
    u32 image;
    u32 handle;
    u32 descriptor;
    u32 unkC;
} MapResource;

extern MapResource D_00401280[10];

extern MapResource D_00401260;

extern MapResource D_00401270;

extern u32 fldReleaseMapResource(s32 *);

typedef struct EffObjVtbl {
    u8 pad00[8];
    void (*refresh)(s32);       /* 0x08 */
} EffObjVtbl;

typedef struct EffObjHeader {
    u8 pad00[0x10];
    EffObjVtbl *vtbl;           /* 0x10 */
} EffObjHeader;

extern u8 D_00400BB0[];

typedef struct {
    u8 pad00[0x0C];
    s32 selectedCount; /* 0x0C */
    u8 pad10[0x10];
    s32 maxCount;      /* 0x20 */
} MapSelection;

extern u32 D_0045C7C0[];

void sdfInitInnerVectors(void) {
    effObjSetInnerFirstVec(D_004388B4, &D_00400970);
    effObjSetInnerSecondVec(D_004388B4, &D_00400980);
    ((EffObjHeader *)D_004388B4)->vtbl->refresh(D_004388B4);
}

INCLUDE_ASM(const s32, "game/code_0030B838", func_0030B880);

INCLUDE_ASM(const s32, "game/code_0030B838", func_0030BA98);

INCLUDE_ASM(const s32, "game/code_0030B838", func_0030BBA8);

void func_0030BC30(u32 arg0) {
    func_0030BBA8();
    D_004388B8 = arg0;
}

void sdfCycleForward(void) {
    if ((s32)D_004388B8 < D_004388BC - 1) {
        func_0030BBA8();
        D_004388B8 = D_004388B8 + 1;
    } else {
        func_0030BBA8();
        D_004388B8 = 0;
    }
}

void func_0030BCA8(void) {
    if (D_004388B8 != 0) {
        func_0030BBA8();
        D_004388B8 = D_004388B8 - 1;
    } else {
        func_0030BBA8();
        D_004388B8 = D_004388BC - 1;
    }
}

s8 func_0030BCE8(void) {
    return D_004388C0;
}

s64 func_0030BCF0(void) {
    return func_0030BD10();
}

INCLUDE_ASM(const s32, "game/code_0030B838", func_0030BD10);

INCLUDE_ASM(const s32, "game/code_0030B838", func_0030BED8);

INCLUDE_ASM(const s32, "game/code_0030B838", func_0030C0C0);

INCLUDE_ASM(const s32, "game/code_0030B838", func_0030C250);

INCLUDE_ASM(const s32, "game/code_0030B838", func_0030C378);

u8 *func_0030C568(s32 scene) {
    if (scene == 4) {
        if (mdlFlagTest(0x13)) {
            scene = 10;
        }
        if (mdlFlagTest(0x29)) {
            scene = 11;
        }
    }
    return D_00400BB0 + scene * 52 - 0x34;
}

INCLUDE_ASM(const s32, "game/code_0030B838", func_0030C5D8);

s64 func_0030C640(void) {
    return func_0030C5D8(D_004388C4);
}

s64 func_0030C660(void) {
    sdfCounterTickCountdown();
    mnuTickMapTimers();
    return func_0030D438();
}

INCLUDE_ASM(const s32, "game/code_0030B838", func_0030C690);

INCLUDE_ASM(const s32, "game/code_0030B838", func_0030C8E8);

u32 sdfCounterGetDisplayWordPointer(void) {
    return (u32)((SdfCounterRuntime *)D_004388C4)->channel->display->word;
}

s32 func_0030C9B8(void) {
    return ((SdfCounterRuntime *)D_004388C4)->channel->display->value;
}

s16 func_0030C9D0(s32 remaining) {
    SdfCounterChannel *task = ((SdfCounterRuntime *)D_004388C4)->first;
    if (remaining > 0) {
        do {
            remaining--;
            task = task->next;
        } while (remaining != 0);
    }
    return task->display->value;
}

float sdfCounterGetScaledValue(void) {
    SdfCounterTimer *timer;

    timer = ((SdfCounterRuntime *)D_004388C4)->timer;
    return (float)timer->value / 10.0f;
}

INCLUDE_ASM(const s32, "game/code_0030B838", func_0030CA38);

INCLUDE_ASM(const s32, "game/code_0030B838", func_0030CC68);

INCLUDE_ASM(const s32, "game/code_0030B838", func_0030CEF0);

void sdfCounterIncrease(void) {
    s32 currentValue;

    currentValue = ((SdfCounterRuntime *)D_004388C4)->timer->value;
    if (currentValue < 10) {
        ((SdfCounterRuntime *)D_004388C4)->timer->value = currentValue + 1;
    }
}

void sdfCounterDecrease(void) {
    s32 currentValue;

    currentValue = ((SdfCounterRuntime *)D_004388C4)->timer->value;
    if (currentValue != 0) {
        ((SdfCounterRuntime *)D_004388C4)->timer->value = currentValue - 1;
    }
}

void sdfCounterSetMode(s32 mode) {
    SdfCounterTimer *timer;

    timer = ((SdfCounterRuntime *)D_004388C4)->timer;
    timer->mode = mode;
    timer->countdown = 8;
}

void sdfCounterTickCountdown(void) {
    SdfCounterTimer *timer;

    timer = ((SdfCounterRuntime *)D_004388C4)->timer;
    if (0 < timer->countdown) {
        timer->countdown = timer->countdown - 1;
    }
}

void mnuSetMapTimerFlags(s32 flags) {
    SdfCounterTimer *timers = ((SdfCounterRuntime *)D_004388C4)->timer;
    if ((flags & 1) != 0) {
        if (timers->mapTimerPrimary == 0) {
            timers->mapTimerPrimary = 1;
        }
    } else {
        timers->mapTimerPrimary = 0;
    }
    if ((flags & 2) != 0) {
        if (timers->mapTimerSecondary == 0) {
            timers->mapTimerSecondary = 1;
        }
    } else {
        timers->mapTimerSecondary = 0;
    }
}

void mnuTickMapTimers(void) {
    SdfCounterTimer *timers = ((SdfCounterRuntime *)D_004388C4)->timer;
    if (timers->mapTimerPrimary > 0) {
        timers->mapTimerPrimary--;
        if (timers->mapTimerPrimary == 0) {
            timers->mapTimerPrimary = 60;
        }
    }
    if (timers->mapTimerSecondary > 0) {
        timers->mapTimerSecondary--;
        if (timers->mapTimerSecondary == 0) {
            timers->mapTimerSecondary = 60;
        }
    }
}

INCLUDE_ASM(const s32, "game/code_0030B838", func_0030D3A8);

INCLUDE_ASM(const s32, "game/code_0030B838", func_0030D3C0);

INCLUDE_ASM(const s32, "game/code_0030B838", func_0030D408);

INCLUDE_ASM(const s32, "game/code_0030B838", func_0030D438);

void func_0030D4F8(void) {
}

INCLUDE_ASM(const s32, "game/code_0030B838", func_0030D500);

u64 func_0030D780(u64 arg0) {
    u64 temp_v0;
    u64 temp_v1;

    temp_v0 = func_0019F448(0, 0, 0, 0, arg0, 0);
    temp_v1 = frFontMeasureLines(temp_v0);
    func_0019C5B0(temp_v0);
    return temp_v1;
}

s32 fldCountMaskBitsBeforeOrdinal(s32 mask, s32 ordinal) {
    s32 bitIndex = 0;
    s32 count = 0;
    s32 nextIndex;

    do {
        nextIndex = bitIndex + 1;
        if (ordinal == nextIndex) {
            break;
        }
        count += (mask >> bitIndex) & 1;
        bitIndex = nextIndex;
    } while (bitIndex < 0x1F);
    return count;
}

INCLUDE_ASM(const s32, "game/code_0030B838", func_0030D818);

void fldSetMapSelectedCount(MapSelection *selection, s32 count) {
    if ((count <= selection->maxCount) && (count != 0)) {
        selection->selectedCount = count;
    }
}

void fldIncreaseMapSelectedCount(MapSelection *selection) {
    if (selection->selectedCount < 10) {
        selection->selectedCount = selection->selectedCount + 1;
    }
}

void fldDecreaseMapSelectedCount(MapSelection *selection) {
    if (1 < selection->selectedCount) {
        selection->selectedCount = selection->selectedCount - 1;
    }
}

u32 func_0030D900(s32 context) {
    s32 task = *(s32 *)(context + 0x10);
    u32 mask = 0;
    do {
        mask |= 1 << (*(s16 *)(*(s32 *)(task + 0x70) + 8) - 1);
        task = *(s32 *)(task + 0x58);
    } while (task != 0);
    return mask;
}

INCLUDE_ASM(const s32, "game/code_0030B838", func_0030D938);

INCLUDE_ASM(const s32, "game/code_0030B838", func_0030DAA0);

void func_0030DAE8(void) {
    s32 remaining = 24;
    u32 *slot = D_0045C7C0;
    do {
        if (*slot != 0) {
            func_003054E8(*slot);
        }
        *slot++ = 0;
    } while (--remaining >= 0);
}

INCLUDE_ASM(const s32, "game/code_0030B838", func_0030DB40);

INCLUDE_ASM(const s32, "game/code_0030B838", func_0030DBF0);

INCLUDE_ASM(const s32, "game/code_0030B838", func_0030DE08);

s64 func_0030E010(f32 x) {
    return func_0030DE08(x, x);
}

INCLUDE_ASM(const s32, "game/code_0030B838", func_0030E030);

s32 fldReleaseLocalMapResources(void) {
    s32 i = 9;
    MapResource *item = D_00401280;
    do {
        fldReleaseMapResource((s32 *)item);
        item++;
        --i;
    } while (i >= 0);
    fldReleaseMapResource((s32 *)&D_00401260);
    fldReleaseMapResource((s32 *)&D_00401270);
    return 1;
}

void func_0030E130(void) {
    s32 temp_v0;

    D_00439098 = 0;
    temp_v0 = func_0030C9B8();
    D_0043909C = temp_v0 - 1;
    D_004390A0 = 0x3c;
}

void func_0030E160(void) {
    if ((s32)D_00439098 < 0x3C) {
        D_00439098++;
    }
}

void func_0030E180(void) {
    if ((s32)D_00439098 > 0) {
        D_00439098 -= 2;
    } else {
        D_00439098 = 0;
    }
}

INCLUDE_RODATA(const s32, "game/code_0030B838", D_0042D418);

