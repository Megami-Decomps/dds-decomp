#include "common.h"
#include "fpu.h"
#include "gs_packet.h"
#include "sdf.h"

extern s32 func_0035C860(char *, const char *, ...);
extern s32 fldLocalMapCameraObject;

typedef struct {
    f32 v[4];
} SdfCameraVector;

extern SdfCameraVector fldLocalMapFirstCameraVector;
extern SdfCameraVector fldLocalMapSecondCameraVector;
extern f32 D_004008E0[][4];
extern f32 D_00400920[][4];
extern s8 D_004388C1;
extern void sdfQuatSlerp(f32 *, f32 *, f32 *, f32);
extern void sdfQuaternionNormalize(f32 *);

extern void effObjSetInnerFirstVec(s32, void *);

extern void effObjSetInnerSecondVec(s32, void *);

extern s32 func_0030DE08(f32, f32);

extern void sdfCounterDestroyRuntime();

extern s32 frFontMeasureLines(u64);

extern u64 func_0019F448(u64, u64, u64, u64, u64, u64);

extern u32 sdfSelectedCounterIndex;

extern s32 sdfActiveCounterRuntime;

typedef struct {
    u32 *word;         /* 0x00 */
    u8 *info;          /* 0x04 */
    s16 value;         /* 0x08 */
    u8 pad0A[4];
    s16 flag;          /* 0x0E */
} SdfCounterDisplay;

typedef struct {
    s32 value;            /* 0x00 */
    s16 countdown;        /* 0x04 */
    s16 mode;             /* 0x06 */
    u8 pad08[4];
    s16 mapTimerPrimary;  /* 0x0C */
    s16 mapTimerSecondary; /* 0x0E */
    s32 y;                /* 0x10 */
    s16 startX;           /* 0x14 */
    s16 startY;           /* 0x16 */
    s16 targetX;          /* 0x18 */
    s16 targetY;          /* 0x1A */
    s16 curX;             /* 0x1C */
    s16 curY;             /* 0x1E */
    s32 frames;           /* 0x20 */
} SdfCounterTimer;
typedef struct SdfCounterChannel {
    s32 index;                      /* 0x00 */
    u8 pad04[0x54];
    struct SdfCounterChannel *next; /* 0x58 */
    struct SdfCounterChannel *prev; /* 0x5C */
    u8 pad60[0x10];
    SdfCounterDisplay *display;     /* 0x70 */
} SdfCounterChannel;

struct SdfCounterRuntime;
typedef void (*SdfCounterDrawFn)(s32, s32, s32, struct SdfCounterRuntime *, SdfCounterChannel *, s32);

typedef struct SdfCounterRuntime {
    u8 pad00[0xC];
    s32 base;                       /* 0x0C */
    SdfCounterChannel *first;       /* 0x10 */
    SdfCounterChannel *last;        /* 0x14 */
    SdfCounterChannel *selected;    /* 0x18 */
    SdfCounterChannel *channel;     /* 0x1C */
    s32 active;                     /* 0x20 */
    s32 scroll;                     /* 0x24 */
    s32 posX;                       /* 0x28 */
    SdfCounterDrawFn draw;          /* 0x2C */
    SdfCounterTimer *timer;         /* 0x30 */
} SdfCounterRuntime;

extern u32 sdfCounterAnimationValue;

extern s32 D_0043909C;

extern u32 D_004390A0;

extern s32 sdfCounterGetDisplayValue(void);

extern s32 sdfCounterSelectionCount;

extern void sdfLatchBaseVectorsForSelection(void);

extern s8 D_004388C0;

extern void sdfCounterTickCountdown(void);

extern void mnuTickMapTimers(void);

extern void sdfCounterTickPositionTransition(void);

extern s32 func_0030BD10();

typedef struct MapResource {
    u32 image;
    u32 handle;
    u32 descriptor;
    u32 unkC;
} MapResource;

extern MapResource fldLocalMapNameTextures[10];

extern MapResource fldLocalMapAuxTextureResource;

extern MapResource fldLocalMapTextureResource;

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

extern u32 sdfInstalledSpriteSlots[];

typedef struct SdfSlotSet {
    u8 pad00[0xC];
    u32 slot[25];
} SdfSlotSet;
extern SdfCameraVector D_00400990;
extern SdfCameraVector D_004009A0;
extern SdfCameraVector D_004009B0[];
extern SdfCameraVector D_00400A50[];
extern u32 func_0030B678(void);

extern struct MenuList *mnuCreateListState(u32, u32, s32);
extern void *sdfAllocSizeClassBlock(s32);
extern void *memset(void *, s32, u32);
extern s32 mdlCollectFlagBitsIntoMask(void);
extern SdfCounterChannel *mnuListAppendNode(s32, s32);
extern u8 *sdfResolveSceneCounterInfo(s32);
extern void sdfCounterSelectChannelByIndex(SdfCounterRuntime *, s32);
extern s16 sdfGetCounterChannelValueAtIndex(s32);
extern void func_0030C250(s32, s32);
extern void func_0030CC68();
extern u8 D_00400AF0[];

extern void sdfReleaseChipBlock();
extern void mnuDestroyListState();
extern void sdfCounterIncrease(void);
extern void sdfCounterDecrease(void);
extern void sdfDrawCounterChannelInfoLabel(s32, s32);
extern void sdfUpdateCounterSelectionFade(void);
extern void mnuCallInitWide();

extern void func_001094F8(s32, s32, s32, s32, u32, u32, u32, u32);

extern s32 frMeasureAndQueueCounterText();

extern void evtPrepareSizedDrawResource();

extern void sdfCounterDrawGlyphAtGridCell(s32, s32, u32, u8 *);

extern void evtSubmitGsRegister47(s32, s32, s32, s32, s32, s32, s32, s32);
extern void func_00108BD8(s32);
extern void func_00108EC0(s32, s32, s32, s32, s32, s32, s32, s32, u32, u32, u32, u32, u32);


typedef struct SdfSpriteEntry {
    u8 pad00[0xC];
    s32 outX;      /* 0x0C */
    s32 outY;      /* 0x10 */
    s32 f14;       /* 0x14 */
    s32 f18;       /* 0x18 */
    s32 f1C;       /* 0x1C */
    s32 f20;       /* 0x20 */
    u8 pad24[0x58];
    s32 f7C;       /* 0x7C */
    s32 f80;       /* 0x80 */
    s32 f84;       /* 0x84 */
    s32 f88;       /* 0x88 */
    s32 f8C;       /* 0x8C */
    s32 f90;       /* 0x90 */
    u8 pad94[0xC];
} SdfSpriteEntry;

typedef struct SdfSpriteSet {
    u8 pad00[0x18];
    SdfSpriteEntry *entries; /* 0x18 */
} SdfSpriteSet;

typedef struct SdfSpritePlace {
    s32 bank;      /* 0x00 */
    s32 slot;      /* 0x04 */
    s32 offsetX;   /* 0x08 */
    s32 offsetY;   /* 0x0C */
} SdfSpritePlace;

extern SdfSpritePlace D_00400DF0[];
extern void func_00306CD0(s32, s32, s32, u32, s32, u32, s32, s32);
extern void itfDrawGridWithResolvedSlot(s32, s32, s32, s32, u32, s32, s32, s32);

#define SDF_SPRITE(index) (((SdfSpriteSet *)sdfInstalledSpriteSlots[D_00400DF0[index].bank])->entries + D_00400DF0[index].slot)

static inline s32 sprPlaceBank(SdfSpritePlace *p) { return p->bank; }
static inline s32 sprPlaceSlot(SdfSpritePlace *p) { return p->slot; }
static inline s32 sprPlaceX(SdfSpritePlace *p) { return p->offsetX; }
static inline s32 sprPlaceY(SdfSpritePlace *p) { return p->offsetY; }

extern s32 fldLoadMapResource(const char *, MapResource *);


void sdfInitInnerVectors(void) {
    effObjSetInnerFirstVec(fldLocalMapCameraObject, &fldLocalMapFirstCameraVector);
    effObjSetInnerSecondVec(fldLocalMapCameraObject, &fldLocalMapSecondCameraVector);
    ((EffObjHeader *)fldLocalMapCameraObject)->vtbl->refresh(fldLocalMapCameraObject);
}

INCLUDE_ASM(const s32, "game/code_0030B838", func_0030B880);

void sdfInitializeMapCounterSelection(s32 index, s32 count) {
    fldLocalMapFirstCameraVector = D_004009B0[index];
    fldLocalMapSecondCameraVector = D_00400A50[index];
    D_00400990 = fldLocalMapFirstCameraVector;
    D_004009A0 = fldLocalMapSecondCameraVector;
    sdfCounterSelectionCount = count;
    sdfSelectedCounterIndex = index;
    D_004388C0 = 0;
    D_004388C1 = func_0030B678();
}

extern void sdfCommitPendingVectorAndMarkChanged();

/* Latch the base vectors into the pending pair and flag the change. */
void sdfLatchBaseVectorsForSelection(void) {
    D_004388C0 = 1;
    D_00400990 = fldLocalMapFirstCameraVector;
    D_004009A0 = fldLocalMapSecondCameraVector;
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

s8 sdfGetMapCameraTransitionFrame(void) {
    return D_004388C0;
}

void func_0030BCF0(void) {
    func_0030BD10();
}

/* Apply the selected camera pose while the map-camera transition is active. */
s32 func_0030BD10(void) {
    f32 t;

    if (D_004388C1 == 0) return 1;

    if (D_004388C0 != 0) {
        t = (45 - D_004388C0) / 45.0f;
        t = 1.0f - t * t;
        if (t > 1.0f) t = 1.0f;
        fldLocalMapFirstCameraVector.v[0] = D_00400990.v[0] * (1.0f - t) + D_004009B0[sdfSelectedCounterIndex].v[0] * t;
        fldLocalMapFirstCameraVector.v[1] = D_00400990.v[1] * (1.0f - t) + D_004009B0[sdfSelectedCounterIndex].v[1] * t;
        fldLocalMapFirstCameraVector.v[2] = D_00400990.v[2] * (1.0f - t) + D_004009B0[sdfSelectedCounterIndex].v[2] * t;
        fldLocalMapFirstCameraVector.v[3] = 1.0f;
        sdfQuatSlerp(fldLocalMapSecondCameraVector.v, D_004009A0.v, D_00400A50[sdfSelectedCounterIndex].v, t);
        if ((s8)(D_004388C0 + 1) >= 45) D_004388C0 = 0;
        else D_004388C0++;
    } else {
        fldLocalMapFirstCameraVector.v[0] = D_004009B0[sdfSelectedCounterIndex].v[0];
        fldLocalMapFirstCameraVector.v[1] = D_004009B0[sdfSelectedCounterIndex].v[1];
        fldLocalMapFirstCameraVector.v[2] = D_004009B0[sdfSelectedCounterIndex].v[2];
        fldLocalMapFirstCameraVector.v[3] = D_004009B0[sdfSelectedCounterIndex].v[3];
        fldLocalMapSecondCameraVector.v[0] = D_00400A50[sdfSelectedCounterIndex].v[0];
        fldLocalMapSecondCameraVector.v[1] = D_00400A50[sdfSelectedCounterIndex].v[1];
        fldLocalMapSecondCameraVector.v[2] = D_00400A50[sdfSelectedCounterIndex].v[2];
        fldLocalMapSecondCameraVector.v[3] = D_00400A50[sdfSelectedCounterIndex].v[3];
    }
    return 1;
}

s32 sdfStepSelectedMapCameraTransition(void) {
    f32 t;
    s32 index = D_004388C1;

    if (D_004388C0 != 0) {
        t = (45 - D_004388C0) / 45.0f;
        t = 1.0f - t * t;
        if (t > 1.0f) t = 1.0f;
        fldLocalMapFirstCameraVector.v[0] = D_00400990.v[0] * (1.0f - t) + D_004008E0[index][0] * t;
        fldLocalMapFirstCameraVector.v[1] = D_00400990.v[1] * (1.0f - t) + D_004008E0[index][1] * t;
        fldLocalMapFirstCameraVector.v[2] = D_00400990.v[2] * (1.0f - t) + D_004008E0[index][2] * t;
        fldLocalMapFirstCameraVector.v[3] = 1.0f;
        sdfQuatSlerp(fldLocalMapSecondCameraVector.v, D_004009A0.v, D_00400920[index], t);
        if ((s8)(D_004388C0 + 1) >= 45) D_004388C0 = 0;
        else D_004388C0++;
    } else {
        fldLocalMapFirstCameraVector.v[0] = D_004008E0[index][0];
        fldLocalMapFirstCameraVector.v[1] = D_004008E0[index][1];
        fldLocalMapFirstCameraVector.v[2] = D_004008E0[index][2];
        fldLocalMapFirstCameraVector.v[3] = D_004008E0[index][3];
        sdfQuaternionNormalize(D_00400920[index]);
        fldLocalMapSecondCameraVector.v[0] = D_00400920[index][0];
        fldLocalMapSecondCameraVector.v[1] = D_00400920[index][1];
        fldLocalMapSecondCameraVector.v[2] = D_00400920[index][2];
        fldLocalMapSecondCameraVector.v[3] = D_00400920[index][3];
    }
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0030B838", func_0030C0C0);

typedef struct WorldObjectPointer WorldObjectPointer;
typedef struct WorldChainNode WorldChainNode;
typedef struct EffectObject EffectObject;

extern void *dds3GetWorldSecondaryObject(void);
extern WorldChainNode *dds3FindIndexedObjectChainNodeByName(WorldObjectPointer *world, s32 type, const u8 *name);
extern void evtSetObjectTransitionWork(EffectObject *object, u32 value);

/* Fixed-width names identify the corresponding local-map model chain. */
void func_0030C250(s32 index, s32 value) {
    char names[7][16] = {
        "md_01all_02",
        "md_01all_03",
        "md_01all_04",
        "md_01all_02",
        "md_01all_05",
        "md_01all_02",
        "md_01all_02"
    };
    WorldChainNode *node;
    s32 modelIndex;

    if (index != 0) {
        modelIndex = index - 1;
        node = dds3FindIndexedObjectChainNodeByName(dds3GetWorldSecondaryObject(), 6,
                                                  (const u8 *)names[modelIndex]);
        if (node != NULL) {
            evtSetObjectTransitionWork((EffectObject *)node, value);
        }
    }
}

/* Create display channels for the enabled bits of the counter mask. */
s32 sdfCreateMaskedCounterChannels(s32 mask, s32 index) {
    SdfCounterDisplay *display;
    SdfCounterChannel *channel;
    s32 completedMask;
    s32 count;
    s32 i;

    count = 0;
    sdfActiveCounterRuntime = (s32)mnuCreateListState(0, 8, 0x16);
    ((SdfCounterRuntime *)sdfActiveCounterRuntime)->timer = sdfAllocSizeClassBlock(0x24);
    memset(((SdfCounterRuntime *)sdfActiveCounterRuntime)->timer, 0, 0x24);
    ((SdfCounterRuntime *)sdfActiveCounterRuntime)->draw = (SdfCounterDrawFn)func_0030CC68;
    completedMask = mdlCollectFlagBitsIntoMask();
    for (i = 0; i != 8; i++) {
        if ((mask >> i) & 1) {
            channel = mnuListAppendNode(sdfActiveCounterRuntime, 0);
            display = sdfAllocSizeClassBlock(0x10);
            memset(display, 0, 0x10);
            display->value = i + 1;
            display->word = (u32 *)(D_00400AF0 + i * 0x18);
            display->info = sdfResolveSceneCounterInfo(i + 1);
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
    func_0030C250(sdfGetCounterChannelValueAtIndex(index) - 1, 0xB);
    return 1;
}

u8 *sdfResolveSceneCounterInfo(s32 scene) {
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

INCLUDE_ASM(const s32, "game/code_0030B838", func_0030C690);

INCLUDE_ASM(const s32, "game/code_0030B838", func_0030C8E8);

u32 sdfCounterGetDisplayWordPointer(void) {
    return (u32)((SdfCounterRuntime *)sdfActiveCounterRuntime)->channel->display->word;
}

s32 sdfCounterGetDisplayValue(void) {
    return ((SdfCounterRuntime *)sdfActiveCounterRuntime)->channel->display->value;
}

s16 sdfGetCounterChannelValueAtIndex(s32 remaining) {
    SdfCounterChannel *task = ((SdfCounterRuntime *)sdfActiveCounterRuntime)->first;
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

    timer = ((SdfCounterRuntime *)sdfActiveCounterRuntime)->timer;
    return (float)timer->value / 10.0f;
}

INCLUDE_ASM(const s32, "game/code_0030B838", func_0030CA38);

INCLUDE_ASM(const s32, "game/code_0030B838", func_0030CC68);

/* Draw one counter channel's label plate at (x, y): a shaded frame whose alpha follows the timer fraction, then the channel's text centred in it. */
void sdfCounterDrawChannelPlate(s32 x, s32 y, s32 unused, SdfCounterRuntime *rt, SdfCounterChannel *channel) {
    f32 fade = (f32)rt->timer->value / 10.0f;
    s32 base = 0;
    s32 width;

    if (channel->index == rt->channel->index) {
        base = 0x40;
    }
    evtSubmitGsRegister47(1, 0, 0x80, 3, 0, 0, 1, 1);
    func_001094F8(x + 1, y + 1, 0x80, 0x16,
                  PACK(base + 0x40, base + 0x40, base + 0x40, (u32)(fade * 32.0f)),
                  PACK(base, base, base, (u32)((f32)(base + 0x10) * fade)),
                  PACK(base + 0x30, base + 0x30, base + 0x30, (u32)(fade * 64.0f)),
                  PACK(base + 0x60, base + 0x60, base + 0x60, (u32)((f32)(base + 0x70) * fade)));
    width = frMeasureAndQueueCounterText(channel->display->word);
    evtPrepareSizedDrawResource(x + (0x80 - width) / 2 + 1, y + 1,
                                PACK((u32)(fade * 128.0f), (u32)(fade * 128.0f), (u32)((f32)(base + 0x80) * fade), (u32)(fade * 128.0f)),
                                channel->display->word);
}

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
    SdfCounterTimer *timers = ((SdfCounterRuntime *)sdfActiveCounterRuntime)->timer;
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

/* Active timers repeat every 60 ticks rather than stopping at zero. */
void mnuTickMapTimers(void) {
    SdfCounterTimer *timers = ((SdfCounterRuntime *)sdfActiveCounterRuntime)->timer;
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

/* Draw the selected timer icon; the first callback argument is unused.
 * Fade controls both alpha and a scale shrinking from 2.5 to 0.5. */
void sdfCounterDrawSelectedTimerFade(s32 unused, s32 x, f32 fade) {
    SdfCounterTimer *timer;
    s32 offset;
    f32 grow;
    f32 shrink;

    grow = fade * 0.5f + (1.0f - fade) * 2.5f;
    offset = ((SdfCounterRuntime *)sdfActiveCounterRuntime)->selected->index * (((SdfCounterRuntime *)sdfActiveCounterRuntime)->posX >> 3);
    timer = ((SdfCounterRuntime *)sdfActiveCounterRuntime)->timer;
    evtSubmitGsRegister47(1, 0, 0x80, 3, 0, 0, 1, 1);
    func_00108BD8(1);
    shrink = (1.0f - grow) * 8.5f;
    func_00108EC0((s32)(shrink + 13.0f), (s32)((f32)(x - offset + timer->y + 7) + shrink), (s32)(grow * 17.0f), (s32)(grow * 17.0f),
                  0xB6, 0x151, 0x11, 0x11,
                  ((u32)(fade * 64.0f) << 24) | 0x808080,
                  ((u32)(fade * 64.0f) << 24) | 0x808080,
                  ((u32)(fade * 64.0f) << 24) | 0x808080,
                  ((u32)(fade * 64.0f) << 24) | 0x808080,
                  fldLocalMapTextureResource.image);
    func_00108BD8(0);
}

s32 frMeasureAndQueueCounterText(u64 arg0) {
    u64 text;
    s32 width;

    text = func_0019F448(0, 0, 0, 0, arg0, 0);
    width = frFontMeasureLines(text);
    frFontQueueGlyphInSelectedSlot(text);
    return width;
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

void sdfCounterSelectChannelByIndex(SdfCounterRuntime *rt, s32 target) {
    SdfCounterChannel *channel;
    SdfCounterChannel *prev;
    s32 i;

    if (target < rt->active) {
        channel = rt->first;
        rt->scroll = 0;
        rt->channel = channel;
        rt->selected = channel;
        for (i = 0; i <= target; i++) {
            prev = channel->prev;
            if (prev != NULL) {
                if (rt->active - i >= rt->base - 1) {
                    rt->selected = prev;
                    rt->scroll = 1;
                } else {
                    rt->scroll = rt->scroll + 1;
                }
            }
            rt->channel = channel;
            channel = channel->next;
            if (channel == NULL) {
                break;
            }
        }
    }
}

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

u32 fldCollectMapSelectionMask(s32 context) {
    s32 task = *(s32 *)(context + 0x10);
    u32 mask = 0;
    do {
        mask |= 1 << (*(s16 *)(*(s32 *)(task + 0x70) + 8) - 1);
        task = *(s32 *)(task + 0x58);
    } while (task != 0);
    return mask;
}

void sdfDrawCounterChannelInfoLabel(s32 x, s32 y) {
    SdfCounterDisplay *display;
    f32 fade;
    s32 width;

    display = ((SdfCounterRuntime *)sdfActiveCounterRuntime)->channel->display;
    fade = 1.0f - (f32)((SdfCounterRuntime *)sdfActiveCounterRuntime)->timer->value / 10.0f;
    width = frMeasureAndQueueCounterText((u64)display->info);
    x = (f32)x - (f32)width * 0.5f;
    if (display->value == 2) {
        x -= 0x10;
    } else if (display->value == 3) {
        x -= 8;
    } else if (display->value == 4) {
        x -= 0x12;
    } else if (display->value == 5) {
        x -= 0x16;
    } else if (display->value == 6) {
        x -= 2;
    } else if (display->value == 8) {
        x -= 9;
    } else {
        x -= 0x14;
    }
    sdfCounterDrawGlyphAtGridCell(x, y, (u8)(u32)(fade * 128.0f) | 0x80808000, display->info);
}

void sdfInstallNonzeroSpriteSlots(SdfSlotSet *set) {
    s32 i;
    for (i = 0; i < 25; i++) {
        if (set->slot[i] != 0) {
            sdfInstalledSpriteSlots[i] = set->slot[i];
        }
    }
}

void sdfReleaseAllSpriteSlots(void) {
    s32 remaining = 24;
    u32 *slot = sdfInstalledSpriteSlots;
    do {
        if (*slot != 0) {
            effDestroyResourceSlotSet(*slot);
        }
        *slot++ = 0;
    } while (--remaining >= 0);
}

INCLUDE_ASM(const s32, "game/code_0030B838", func_0030DB40);

INCLUDE_ASM(const s32, "game/code_0030B838", func_0030DBF0);

INCLUDE_ASM(const s32, "game/code_0030B838", func_0030DE08);

INCLUDE_ASM(const s32, "game/code_0030B838", sdfDrawUniformlyScaledSlotImage);

/* Load the ten numbered "sname" tiles plus the two fixed local-map images. */
s32 fldLoadLocalMapResources(void) {
    char name[32];
    s32 i;
    MapResource *item = fldLocalMapNameTextures;

    for (i = 0; i < 10; i++) {
        func_0035C860(name, "/lmap/sname_%02d.tmx", i + 1);
        fldLoadMapResource(name, item);
        item++;
    }
    fldLoadMapResource("/lmap/1006.tmx", &fldLocalMapAuxTextureResource);
    fldLoadMapResource("/lmap/l_map00.tmx", &fldLocalMapTextureResource);
    return 1;
}

s32 fldReleaseLocalMapResources(void) {
    s32 i = 9;
    MapResource *item = fldLocalMapNameTextures;
    do {
        fldReleaseMapResource((s32 *)item);
        item++;
        --i;
    } while (i >= 0);
    fldReleaseMapResource((s32 *)&fldLocalMapAuxTextureResource);
    fldReleaseMapResource((s32 *)&fldLocalMapTextureResource);
    return 1;
}

void sdfCounterInitializeDisplayAnimation(void) {
    s32 value;

    sdfCounterAnimationValue = 0;
    value = sdfCounterGetDisplayValue();
    D_0043909C = value - 1;
    D_004390A0 = 0x3c;
}

void sdfCounterAdvanceBoundedAnimationValue(void) {
    if ((s32)sdfCounterAnimationValue < 0x3C) {
        sdfCounterAnimationValue++;
    }
}

void sdfCounterStepDownAnimationValue(void) {
    if ((s32)sdfCounterAnimationValue > 0) {
        sdfCounterAnimationValue -= 2;
    } else {
        sdfCounterAnimationValue = 0;
    }
}
