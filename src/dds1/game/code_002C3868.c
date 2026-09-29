#include "common.h"

extern void sdfCounterTickCountdown(void);

extern void mnuTickMapTimers(void);

extern void func_002C5C70(void);

extern void func_002C3DB0(void);

extern s32 func_002C4A10(void);

typedef struct {
    u32 *word;         /* 0x00 */
    u8 pad04[4];
    s16 value;         /* 0x08 */
} SdfCounterDisplay;

typedef struct {
    s32 value;         /* 0x00 */
    s16 countdown;     /* 0x04 */
    s16 mode;          /* 0x06 */
    s16 pad08[2];
    s16 mapTimerFirst; /* 0x0C */
    s16 mapTimerSecond; /* 0x0E */
} SdfCounterTimer;

typedef struct {
    u8 pad00[0x70];
    SdfCounterDisplay *display; /* 0x70 */
} SdfCounterChannel;

typedef struct {
    u8 pad00[0x1C];
    SdfCounterChannel *channel; /* 0x1C */
    u8 pad20[0x10];
    SdfCounterTimer *timer;     /* 0x30 */
} SdfCounterRuntime;

extern s32 D_003BD274;

extern u32 D_003BD268;

extern s8 D_003BD270;

extern s32 D_003BD26C;

extern void func_002C3C48(void);

extern s32 D_003BD264;

extern u8 D_003900A0[];

extern u8 D_003900B0[];

extern void effObjSetInnerFirstVec(s32, void *);

extern void effObjSetInnerSecondVec(s32, void *);

typedef struct EffObjVtbl {
    u8 pad00[8];
    void (*refresh)(s32);       /* 0x08 */
} EffObjVtbl;

typedef struct EffObjHeader {
    u8 pad00[0x10];
    EffObjVtbl *vtbl;           /* 0x10 */
} EffObjHeader;

extern s32 func_002C45C8();

void sdfInitInnerVectors(void) {
    effObjSetInnerFirstVec(D_003BD264, D_003900A0);
    effObjSetInnerSecondVec(D_003BD264, D_003900B0);
    ((EffObjHeader *)D_003BD264)->vtbl->refresh(D_003BD264);
}

INCLUDE_ASM(const s32, "game/code_002C3868", func_002C38B0);

INCLUDE_ASM(const s32, "game/code_002C3868", func_002C3AC8);

INCLUDE_ASM(const s32, "game/code_002C3868", func_002C3C48);

void func_002C3CD0(u32 arg0) {
    func_002C3C48();
    D_003BD268 = arg0;
}

void sdfCycleForward(void) {
    if ((s32)D_003BD268 < D_003BD26C - 1) {
        func_002C3C48();
        D_003BD268 = D_003BD268 + 1;
    } else {
        func_002C3C48();
        D_003BD268 = 0;
    }
}

void func_002C3D48(void) {
    if (D_003BD268 != 0) {
        func_002C3C48();
        D_003BD268 = D_003BD268 - 1;
    } else {
        func_002C3C48();
        D_003BD268 = D_003BD26C - 1;
    }
}

s8 func_002C3D88(void) {
    return D_003BD270;
}

void func_002C3D90(void) {
    func_002C3DB0();
}

INCLUDE_ASM(const s32, "game/code_002C3868", func_002C3DB0);

INCLUDE_ASM(const s32, "game/code_002C3868", func_002C3F78);

INCLUDE_ASM(const s32, "game/code_002C3868", func_002C4160);

INCLUDE_ASM(const s32, "game/code_002C3868", func_002C42F0);

INCLUDE_ASM(const s32, "game/code_002C3868", func_002C44D0);

INCLUDE_ASM(const s32, "game/code_002C3868", func_002C45C8);

s64 func_002C4630(void) {
    return func_002C45C8(D_003BD274);
}

void func_002C4650(void) {
    sdfCounterTickCountdown();
    mnuTickMapTimers();
    func_002C5C70();
}

INCLUDE_ASM(const s32, "game/code_002C3868", func_002C4680);

INCLUDE_ASM(const s32, "game/code_002C3868", func_002C4850);

u32 sdfCounterGetDisplayWordPointer(void) {
    return (u32)((SdfCounterRuntime *)D_003BD274)->channel->display->word;
}

s32 func_002C4A10(void) {
    return ((SdfCounterRuntime *)D_003BD274)->channel->display->value;
}

float sdfCounterGetScaledValue(void) {
    SdfCounterTimer *timer;

    timer = ((SdfCounterRuntime *)D_003BD274)->timer;
    return (float)timer->value / 10.0f;
}

INCLUDE_ASM(const s32, "game/code_002C3868", func_002C4A58);

INCLUDE_ASM(const s32, "game/code_002C3868", func_002C4C88);

INCLUDE_ASM(const s32, "game/code_002C3868", func_002C5338);

void sdfCounterIncrease(void) {
    s32 currentValue;

    currentValue = ((SdfCounterRuntime *)D_003BD274)->timer->value;
    if (currentValue < 10) {
        ((SdfCounterRuntime *)D_003BD274)->timer->value = currentValue + 1;
    }
}

void sdfCounterDecrease(void) {
    s32 currentValue;

    currentValue = ((SdfCounterRuntime *)D_003BD274)->timer->value;
    if (currentValue != 0) {
        ((SdfCounterRuntime *)D_003BD274)->timer->value = currentValue - 1;
    }
}

void sdfCounterSetMode(s32 mode) {
    SdfCounterTimer *timer;

    timer = ((SdfCounterRuntime *)D_003BD274)->timer;
    timer->mode = mode;
    timer->countdown = 8;
}

void sdfCounterTickCountdown(void) {
    SdfCounterTimer *timer;

    timer = ((SdfCounterRuntime *)D_003BD274)->timer;
    if (0 < timer->countdown) {
        timer->countdown = timer->countdown - 1;
    }
}

void mnuSetMapTimerFlags(s32 flags) {
    SdfCounterTimer *timer = ((SdfCounterRuntime *)D_003BD274)->timer;
    if ((flags & 1) != 0) {
        if (timer->mapTimerFirst == 0) {
            timer->mapTimerFirst = 1;
        }
    } else {
        timer->mapTimerFirst = 0;
    }
    if ((flags & 2) != 0) {
        if (timer->mapTimerSecond == 0) {
            timer->mapTimerSecond = 1;
        }
    } else {
        timer->mapTimerSecond = 0;
    }
}

void mnuTickMapTimers(void) {
    SdfCounterTimer *timer = ((SdfCounterRuntime *)D_003BD274)->timer;
    if (timer->mapTimerFirst > 0) {
        timer->mapTimerFirst--;
        if (timer->mapTimerFirst == 0) {
            timer->mapTimerFirst = 60;
        }
    }
    if (timer->mapTimerSecond > 0) {
        timer->mapTimerSecond--;
        if (timer->mapTimerSecond == 0) {
            timer->mapTimerSecond = 60;
        }
    }
}

INCLUDE_ASM(const s32, "game/code_002C3868", func_002C57F0);

INCLUDE_ASM(const s32, "game/code_002C3868", func_002C5BF8);

INCLUDE_ASM(const s32, "game/code_002C3868", func_002C5C40);

INCLUDE_ASM(const s32, "game/code_002C3868", func_002C5C70);

void func_002C5D30(void) {
}

INCLUDE_ASM(const s32, "game/code_002C3868", func_002C5D38);

INCLUDE_SDATA(const s32, "game/code_002C3868", D_003BD268);

INCLUDE_SDATA(const s32, "game/code_002C3868", D_003BD26C);

INCLUDE_SDATA(const s32, "game/code_002C3868", D_003BD270);

INCLUDE_SDATA(const s32, "game/code_002C3868", D_003BD271);

INCLUDE_SDATA(const s32, "game/code_002C3868", D_003BD274);

