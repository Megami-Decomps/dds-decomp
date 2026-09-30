#include "common.h"

extern void sdfCounterTickCountdown(void);

extern void mnuTickMapTimers(void);

extern void func_002C5C70(void);

extern void func_002C3DB0(void);

extern s32 sdfCounterGetDisplayValue(void);

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
    s16 mapTimerPrimary;   /* 0x0C */
    s16 mapTimerSecondary; /* 0x0E */
    s32 y;             /* 0x10 */
    s16 startX;        /* 0x14 */
    s16 startY;        /* 0x16 */
    s16 targetX;       /* 0x18 */
    s16 targetY;       /* 0x1A */
    s16 curX;          /* 0x1C */
    s16 curY;          /* 0x1E */
    s32 frames;        /* 0x20 */
} SdfCounterTimer;

typedef struct {
    s32 index;                     /* 0x00 */
    u8 pad04[0x54];
    struct SdfCounterChannel *next;    /* 0x58 */
    u8 pad5C[0x14];
    SdfCounterDisplay *display;        /* 0x70 */
} SdfCounterChannel;

typedef struct {
    u8 pad00[0xC];
    s32 base;                          /* 0x0C */
    SdfCounterChannel *first;          /* 0x10 */
    SdfCounterChannel *last;           /* 0x14 */
    SdfCounterChannel *selected;       /* 0x18 */
    SdfCounterChannel *channel;        /* 0x1C */
    s32 active;                        /* 0x20 */
    u8 pad24[4];
    s32 posX;                          /* 0x28 */
    u8 pad2C[4];
    SdfCounterTimer *timer;            /* 0x30 */
} SdfCounterRuntime;

extern void func_002CFF98(void *);

extern void mnuDestroyListState(SdfCounterRuntime *);

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

extern void func_002C45C8(SdfCounterRuntime *);

extern void evtSubmitGsRegister47(s32, s32, s32, s32, s32, s32, s32, s32);

extern void func_00108CB8(s32);

extern void func_00108FA0(s32, s32, s32, s32, s32, s32, s32, s32, u32, u32, u32, u32, u32);

typedef struct MapResource {
    u32 image;
    u32 handle;
    u32 descriptor;
    u32 unkC;
} MapResource;

extern MapResource D_00390700;

void sdfInitInnerVectors(void) {
    effObjSetInnerFirstVec(D_003BD264, D_003900A0);
    effObjSetInnerSecondVec(D_003BD264, D_003900B0);
    ((EffObjHeader *)D_003BD264)->vtbl->refresh(D_003BD264);
}

INCLUDE_ASM(const s32, "game/code_002C3868", func_002C38B0);

INCLUDE_ASM(const s32, "game/code_002C3868", func_002C3AC8);

INCLUDE_ASM(const s32, "game/code_002C3868", func_002C3C48);

/* Reset the current selection before installing the requested index. */
void sdfSetSelectedIndex(u32 index) {
    func_002C3C48();
    D_003BD268 = index;
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

/* Cycle through the same bounded selection in the opposite direction. */
void sdfCycleBackward(void) {
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

void func_002C45C8(SdfCounterRuntime *rt) {
    SdfCounterChannel *channel;

    if (rt != NULL) {
        for (channel = rt->first; channel != NULL; channel = channel->next) {
            func_002CFF98(channel->display);
            channel->display = NULL;
        }
        func_002CFF98(rt->timer);
        rt->timer = NULL;
        mnuDestroyListState(rt);
    }
}

void func_002C4630(void) {
    func_002C45C8(D_003BD274);
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

s32 sdfCounterGetDisplayValue(void) {
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

/* The display counter saturates at ten rather than wrapping. */
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

/* Enabling a stopped timer starts it at one; disabling clears it. */
void mnuSetMapTimerFlags(s32 flags) {
    SdfCounterTimer *timer = ((SdfCounterRuntime *)D_003BD274)->timer;
    if ((flags & 1) != 0) {
        if (timer->mapTimerPrimary == 0) {
            timer->mapTimerPrimary = 1;
        }
    } else {
        timer->mapTimerPrimary = 0;
    }
    if ((flags & 2) != 0) {
        if (timer->mapTimerSecondary == 0) {
            timer->mapTimerSecondary = 1;
        }
    } else {
        timer->mapTimerSecondary = 0;
    }
}

/* Active timers repeat every 60 ticks rather than stopping at zero. */
void mnuTickMapTimers(void) {
    SdfCounterTimer *timer = ((SdfCounterRuntime *)D_003BD274)->timer;
    if (timer->mapTimerPrimary > 0) {
        timer->mapTimerPrimary--;
        if (timer->mapTimerPrimary == 0) {
            timer->mapTimerPrimary = 60;
        }
    }
    if (timer->mapTimerSecondary > 0) {
        timer->mapTimerSecondary--;
        if (timer->mapTimerSecondary == 0) {
            timer->mapTimerSecondary = 60;
        }
    }
}

INCLUDE_ASM(const s32, "game/code_002C3868", func_002C57F0);

s32 func_002C5BF8(void) {
    SdfCounterRuntime *rt = (SdfCounterRuntime *)D_003BD274;
    s32 count;
    s32 done;
    if (rt->active == 0) {
        return 0;
    }
    count = rt->selected->index;
    done = count != 0;
    return ((count + rt->base - 1) ^ rt->last->index) != 0 ? (done | 2) : done;
}

void func_002C5C40(s16 x, s16 y) {
    SdfCounterTimer *timer = ((SdfCounterRuntime *)D_003BD274)->timer;

    timer->startX = timer->curX;
    timer->startY = timer->curY;
    timer->targetX = x;
    timer->targetY = y;
    timer->frames = 3;
}

INCLUDE_ASM(const s32, "game/code_002C3868", func_002C5C70);

void func_002C5D30(void) {
}

void func_002C5D38(s32 arg0, s32 x, f32 fade) {
    SdfCounterTimer *timer;
    s32 offset;
    f32 grow;
    f32 shrink;

    grow = fade * 0.5f + (1.0f - fade) * 2.5f;
    offset = ((SdfCounterRuntime *)D_003BD274)->selected->index * (((SdfCounterRuntime *)D_003BD274)->posX >> 3);
    timer = ((SdfCounterRuntime *)D_003BD274)->timer;
    evtSubmitGsRegister47(1, 0, 0x80, 3, 0, 0, 1, 1);
    func_00108CB8(1);
    shrink = (1.0f - grow) * 8.5f;
    func_00108FA0((s32)(shrink + 13.0f), (s32)((f32)(x - offset + timer->y + 7) + shrink), (s32)(grow * 17.0f), (s32)(grow * 17.0f),
                  0xB6, 0x151, 0x11, 0x11,
                  ((u32)(fade * 64.0f) << 24) | 0x808080,
                  ((u32)(fade * 64.0f) << 24) | 0x808080,
                  ((u32)(fade * 64.0f) << 24) | 0x808080,
                  ((u32)(fade * 64.0f) << 24) | 0x808080,
                  D_00390700.image);
    func_00108CB8(0);
}

INCLUDE_SDATA(const s32, "game/code_002C3868", D_003BD268);

INCLUDE_SDATA(const s32, "game/code_002C3868", D_003BD26C);

INCLUDE_SDATA(const s32, "game/code_002C3868", D_003BD270);

INCLUDE_SDATA(const s32, "game/code_002C3868", D_003BD271);

INCLUDE_SDATA(const s32, "game/code_002C3868", D_003BD274);

