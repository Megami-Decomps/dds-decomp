#include "common.h"
#include "gs_packet.h"
#include "sdf.h"

extern void sdfCounterTickCountdown(void);

extern void mnuTickMapTimers(void);

extern void sdfCounterTickPositionTransition(void);

extern void func_002C3DB0(void);

extern s32 sdfCounterGetDisplayValue(void);

typedef struct {
    u32 *word;         /* 0x00 */
    u8 *info;          /* 0x04 */
    s16 value;         /* 0x08 */
    u8 pad0A[4];
    s16 flag;          /* 0x0E */
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

struct SdfCounterRuntime;
typedef void (*SdfCounterDrawFn)();

typedef struct SdfCounterRuntime {
    u8 pad00[0xC];
    s32 base;                          /* 0x0C */
    SdfCounterChannel *first;          /* 0x10 */
    SdfCounterChannel *last;           /* 0x14 */
    SdfCounterChannel *selected;       /* 0x18 */
    SdfCounterChannel *channel;        /* 0x1C */
    s32 active;                        /* 0x20 */
    u8 pad24[4];
    s32 posX;                          /* 0x28 */
    SdfCounterDrawFn draw;             /* 0x2C */
    SdfCounterTimer *timer;            /* 0x30 */
} SdfCounterRuntime;

extern void sdfReleaseChipBlock(void *);

extern void mnuDestroyListState(SdfCounterRuntime *);

extern s32 sdfActiveCounterRuntime;

extern u32 sdfSelectedCounterIndex;

extern s8 D_003BD270;

extern s32 sdfCounterSelectionCount;

extern void sdfLatchBaseVectorsForSelection(void);

extern s32 fldLocalMapCameraObject;

typedef struct {
    f32 v[4];
} SdfCameraVector;

extern SdfCameraVector fldLocalMapFirstCameraVector;
extern SdfCameraVector fldLocalMapSecondCameraVector;
extern SdfCameraVector D_003900C0;
extern SdfCameraVector D_003900D0;
extern f32 D_00390010[][4];
extern f32 D_00390050[][4];
extern s8 D_003BD271;
extern void sdfQuatSlerp(f32 *, f32 *, f32 *, f32);
extern void sdfQuaternionNormalize(f32 *);

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

extern void sdfCounterDestroyRuntime(SdfCounterRuntime *);

extern void evtSubmitGsRegister47(s32, s32, s32, s32, s32, s32, s32, s32);

extern void func_00108CB8(s32);

extern void func_00108FA0(s32, s32, s32, s32, s32, s32, s32, s32, u32, u32, u32, u32, u32);

extern void func_001093B8(s32, s32, s32, s32, s32, s32, s32, s32);

extern s32 func_002C5FB8(u32);

extern void evtPrepareSizedDrawResource();

typedef struct MapResource {
    u32 image;
    u32 handle;
    u32 descriptor;
    u32 unkC;
} MapResource;

extern MapResource fldLocalMapTextureResource;

void sdfInitInnerVectors(void) {
    effObjSetInnerFirstVec(fldLocalMapCameraObject, &fldLocalMapFirstCameraVector);
    effObjSetInnerSecondVec(fldLocalMapCameraObject, &fldLocalMapSecondCameraVector);
    ((EffObjHeader *)fldLocalMapCameraObject)->vtbl->refresh(fldLocalMapCameraObject);
}

INCLUDE_ASM(const s32, "game/code_002C3868", func_002C38B0);

INCLUDE_ASM(const s32, "game/code_002C3868", func_002C3AC8);

extern void sdfCommitPendingVectorAndMarkChanged();

/* Latch the base vectors into the pending pair and flag the change. */
void sdfLatchBaseVectorsForSelection(void) {
    D_003BD270 = 1;
    D_003900C0 = fldLocalMapFirstCameraVector;
    D_003900D0 = fldLocalMapSecondCameraVector;
    sdfCommitPendingVectorAndMarkChanged();
}

/* Reset the current selection before installing the requested index. */
void sdfSetSelectedIndex(u32 index) {
    sdfLatchBaseVectorsForSelection();
    sdfSelectedCounterIndex = index;
}

void sdfCycleForward(void) {
    if ((s32)sdfSelectedCounterIndex < sdfCounterSelectionCount - 1) {
        sdfLatchBaseVectorsForSelection();
        sdfSelectedCounterIndex = sdfSelectedCounterIndex + 1;
    } else {
        sdfLatchBaseVectorsForSelection();
        sdfSelectedCounterIndex = 0;
    }
}

/* Cycle through the same bounded selection in the opposite direction. */
void sdfCycleBackward(void) {
    if (sdfSelectedCounterIndex != 0) {
        sdfLatchBaseVectorsForSelection();
        sdfSelectedCounterIndex = sdfSelectedCounterIndex - 1;
    } else {
        sdfLatchBaseVectorsForSelection();
        sdfSelectedCounterIndex = sdfCounterSelectionCount - 1;
    }
}

s8 func_002C3D88(void) {
    return D_003BD270;
}

void func_002C3D90(void) {
    func_002C3DB0();
}

INCLUDE_ASM(const s32, "game/code_002C3868", func_002C3DB0);

s32 func_002C3F78(void) {
    f32 t;
    s32 index = D_003BD271;

    if (D_003BD270 != 0) {
        t = (45 - D_003BD270) / 45.0f;
        t = 1.0f - t * t;
        if (t > 1.0f) t = 1.0f;
        fldLocalMapFirstCameraVector.v[0] = D_003900C0.v[0] * (1.0f - t) + D_00390010[index][0] * t;
        fldLocalMapFirstCameraVector.v[1] = D_003900C0.v[1] * (1.0f - t) + D_00390010[index][1] * t;
        fldLocalMapFirstCameraVector.v[2] = D_003900C0.v[2] * (1.0f - t) + D_00390010[index][2] * t;
        fldLocalMapFirstCameraVector.v[3] = 1.0f;
        sdfQuatSlerp(fldLocalMapSecondCameraVector.v, D_003900D0.v, D_00390050[index], t);
        if ((s8)(D_003BD270 + 1) >= 45) D_003BD270 = 0;
        else D_003BD270++;
    } else {
        fldLocalMapFirstCameraVector.v[0] = D_00390010[index][0];
        fldLocalMapFirstCameraVector.v[1] = D_00390010[index][1];
        fldLocalMapFirstCameraVector.v[2] = D_00390010[index][2];
        fldLocalMapFirstCameraVector.v[3] = D_00390010[index][3];
        sdfQuaternionNormalize(D_00390050[index]);
        fldLocalMapSecondCameraVector.v[0] = D_00390050[index][0];
        fldLocalMapSecondCameraVector.v[1] = D_00390050[index][1];
        fldLocalMapSecondCameraVector.v[2] = D_00390050[index][2];
        fldLocalMapSecondCameraVector.v[3] = D_00390050[index][3];
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_002C3868", func_002C4160);

extern s32 mnuCreateListState(s32, s32, s32);
extern void *func_002CFEB8(s32);
extern void *memset(void *, s32, u32);
extern s32 mdlCollectFlagBitsIntoMask(void);
extern SdfCounterChannel *mnuListAppendNode(s32, s32);
extern u8 *func_002C44D0(s32);
extern void sdfCounterSelectChannelByIndex(SdfCounterRuntime *, s32);
extern void func_002C4C88();
extern u8 D_00390220[];

s32 sdfCreateMaskedCounterChannels(s32 mask, s32 index) {
    SdfCounterDisplay *display;
    SdfCounterChannel *channel;
    s32 completedMask;
    s32 count;
    s32 i;

    count = 0;
    sdfActiveCounterRuntime = mnuCreateListState(0, 6, 0x16);
    ((SdfCounterRuntime *)sdfActiveCounterRuntime)->timer = func_002CFEB8(0x24);
    memset(((SdfCounterRuntime *)sdfActiveCounterRuntime)->timer, 0, 0x24);
    ((SdfCounterRuntime *)sdfActiveCounterRuntime)->draw = func_002C4C88;
    completedMask = mdlCollectFlagBitsIntoMask();
    for (i = 0; i != 10; i++) {
        if ((mask >> i) & 1) {
            channel = mnuListAppendNode(sdfActiveCounterRuntime, 0);
            display = func_002CFEB8(0x10);
            memset(display, 0, 0x10);
            display->value = i + 1;
            display->word = (u32 *)(D_00390220 + i * 0x18);
            display->info = func_002C44D0(i + 1);
            if ((completedMask >> i) & 1) {
                display->flag = 1;
            }
            channel->display = display;
            count++;
        }
    }
    if (count < 6) {
        ((SdfCounterRuntime *)sdfActiveCounterRuntime)->base = count;
    }
    if (count == 3) {
        ((SdfCounterRuntime *)sdfActiveCounterRuntime)->posX = 0x148;
        ((SdfCounterRuntime *)sdfActiveCounterRuntime)->timer->y = 0x8F;
    } else if (count == 4) {
        ((SdfCounterRuntime *)sdfActiveCounterRuntime)->posX = 0x100;
        ((SdfCounterRuntime *)sdfActiveCounterRuntime)->timer->y = 0x84;
    } else if (count == 5) {
        ((SdfCounterRuntime *)sdfActiveCounterRuntime)->posX = 0xD8;
        ((SdfCounterRuntime *)sdfActiveCounterRuntime)->timer->y = 0x7F;
    } else {
        ((SdfCounterRuntime *)sdfActiveCounterRuntime)->posX = 0xB0;
        ((SdfCounterRuntime *)sdfActiveCounterRuntime)->timer->y = 0x7E;
    }
    sdfCounterSelectChannelByIndex(sdfActiveCounterRuntime, index);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_002C3868", func_002C44D0);

void sdfCounterDestroyRuntime(SdfCounterRuntime *rt) {
    SdfCounterChannel *channel;

    if (rt != NULL) {
        for (channel = rt->first; channel != NULL; channel = channel->next) {
            sdfReleaseChipBlock(channel->display);
            channel->display = NULL;
        }
        sdfReleaseChipBlock(rt->timer);
        rt->timer = NULL;
        mnuDestroyListState(rt);
    }
}

void sdfDestroyActiveCounterRuntime(void) {
    sdfCounterDestroyRuntime(sdfActiveCounterRuntime);
}

void sdfCounterTickCountdownAndMapTimers(void) {
    sdfCounterTickCountdown();
    mnuTickMapTimers();
    sdfCounterTickPositionTransition();
}

INCLUDE_ASM(const s32, "game/code_002C3868", func_002C4680);

INCLUDE_ASM(const s32, "game/code_002C3868", func_002C4850);

u32 sdfCounterGetDisplayWordPointer(void) {
    return (u32)((SdfCounterRuntime *)sdfActiveCounterRuntime)->channel->display->word;
}

s32 sdfCounterGetDisplayValue(void) {
    return ((SdfCounterRuntime *)sdfActiveCounterRuntime)->channel->display->value;
}

float sdfCounterGetScaledValue(void) {
    SdfCounterTimer *timer;

    timer = ((SdfCounterRuntime *)sdfActiveCounterRuntime)->timer;
    return (float)timer->value / 10.0f;
}

INCLUDE_ASM(const s32, "game/code_002C3868", func_002C4A58);

INCLUDE_ASM(const s32, "game/code_002C3868", func_002C4C88);

/* Draw one counter channel's label plate at (x, y): a shaded frame whose alpha follows the timer fraction, then the channel's text centred in it. */
void sdfCounterDrawChannelPlate(s32 x, s32 y, s32 unused, SdfCounterRuntime *rt, SdfCounterChannel *channel) {
    f32 fade = (f32)rt->timer->value / 10.0f;
    s32 base = 0;
    s32 width;

    if (channel->index == rt->channel->index) {
        base = 0x40;
    }
    evtSubmitGsRegister47(1, 0, 0x80, 3, 0, 0, 1, 1);
    func_001093B8(x + 1, y + 1, 0x80, 0x16,
                  PACK(base + 0x40, base + 0x40, base + 0x40, (u32)(fade * 32.0f)),
                  PACK(base, base, base, (u32)((f32)(base + 0x10) * fade)),
                  PACK(base + 0x30, base + 0x30, base + 0x30, (u32)(fade * 64.0f)),
                  PACK(base + 0x60, base + 0x60, base + 0x60, (u32)((f32)(base + 0x70) * fade)));
    width = func_002C5FB8((u32)channel->display->word);
    evtPrepareSizedDrawResource(x + (0x80 - width) / 2 + 1, y + 1,
                                PACK((u32)(fade * 128.0f), (u32)(fade * 128.0f), (u32)((f32)(base + 0x80) * fade), (u32)(fade * 128.0f)),
                                channel->display->word);
}

/* The display counter saturates at ten rather than wrapping. */
void sdfCounterIncrease(void) {
    s32 currentValue;

    currentValue = ((SdfCounterRuntime *)sdfActiveCounterRuntime)->timer->value;
    if (currentValue < 10) {
        ((SdfCounterRuntime *)sdfActiveCounterRuntime)->timer->value = currentValue + 1;
    }
}

void sdfCounterDecrease(void) {
    s32 currentValue;

    currentValue = ((SdfCounterRuntime *)sdfActiveCounterRuntime)->timer->value;
    if (currentValue != 0) {
        ((SdfCounterRuntime *)sdfActiveCounterRuntime)->timer->value = currentValue - 1;
    }
}

void sdfCounterSetMode(s32 mode) {
    SdfCounterTimer *timer;

    timer = ((SdfCounterRuntime *)sdfActiveCounterRuntime)->timer;
    timer->mode = mode;
    timer->countdown = 8;
}

void sdfCounterTickCountdown(void) {
    SdfCounterTimer *timer;

    timer = ((SdfCounterRuntime *)sdfActiveCounterRuntime)->timer;
    if (0 < timer->countdown) {
        timer->countdown = timer->countdown - 1;
    }
}

/* Enabling a stopped timer starts it at one; disabling clears it. */
void mnuSetMapTimerFlags(s32 flags) {
    SdfCounterTimer *timer = ((SdfCounterRuntime *)sdfActiveCounterRuntime)->timer;
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
    SdfCounterTimer *timer = ((SdfCounterRuntime *)sdfActiveCounterRuntime)->timer;
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

s32 sdfCounterGetSelectionBoundaryFlags(void) {
    SdfCounterRuntime *rt = (SdfCounterRuntime *)sdfActiveCounterRuntime;
    s32 count;
    s32 done;
    if (rt->active == 0) {
        return 0;
    }
    count = rt->selected->index;
    done = count != 0;
    return ((count + rt->base - 1) ^ rt->last->index) != 0 ? (done | 2) : done;
}

void sdfCounterStartTimerPositionTransition(s16 x, s16 y) {
    SdfCounterTimer *timer = ((SdfCounterRuntime *)sdfActiveCounterRuntime)->timer;

    timer->startX = timer->curX;
    timer->startY = timer->curY;
    timer->targetX = x;
    timer->targetY = y;
    timer->frames = 3;
}

/* Advance the timer's position transition one frame (3-frame lerp start -> target). */
void sdfCounterTickPositionTransition(void) {
    SdfCounterTimer *timer = ((SdfCounterRuntime *)sdfActiveCounterRuntime)->timer;
    s32 frames = timer->frames;
    f32 t;

    if (frames != 0) {
        frames = --timer->frames;
    }
    t = (f32)frames / 3.0f;
    if (frames != 0) {
        timer->curX = timer->startX * t + timer->targetX * (1.0f - t);
        timer->curY = timer->startY * t + timer->targetY * (1.0f - t);
    } else {
        timer->curX = timer->targetX;
        timer->curY = timer->targetY;
    }
}

void sdfUpdateCounterSelectionFade(void) {
}

void sdfCounterDrawSelectedTimerFade(s32 arg0, s32 x, f32 fade) {
    SdfCounterTimer *timer;
    s32 offset;
    f32 grow;
    f32 shrink;

    grow = fade * 0.5f + (1.0f - fade) * 2.5f;
    offset = ((SdfCounterRuntime *)sdfActiveCounterRuntime)->selected->index * (((SdfCounterRuntime *)sdfActiveCounterRuntime)->posX >> 3);
    timer = ((SdfCounterRuntime *)sdfActiveCounterRuntime)->timer;
    evtSubmitGsRegister47(1, 0, 0x80, 3, 0, 0, 1, 1);
    func_00108CB8(1);
    shrink = (1.0f - grow) * 8.5f;
    func_00108FA0((s32)(shrink + 13.0f), (s32)((f32)(x - offset + timer->y + 7) + shrink), (s32)(grow * 17.0f), (s32)(grow * 17.0f),
                  0xB6, 0x151, 0x11, 0x11,
                  ((u32)(fade * 64.0f) << 24) | 0x808080,
                  ((u32)(fade * 64.0f) << 24) | 0x808080,
                  ((u32)(fade * 64.0f) << 24) | 0x808080,
                  ((u32)(fade * 64.0f) << 24) | 0x808080,
                  fldLocalMapTextureResource.image);
    func_00108CB8(0);
}

INCLUDE_SDATA(const s32, "game/code_002C3868", sdfSelectedCounterIndex);

INCLUDE_SDATA(const s32, "game/code_002C3868", sdfCounterSelectionCount);

INCLUDE_SDATA(const s32, "game/code_002C3868", D_003BD270);

INCLUDE_SDATA(const s32, "game/code_002C3868", D_003BD271);

INCLUDE_SDATA(const s32, "game/code_002C3868", sdfActiveCounterRuntime);

