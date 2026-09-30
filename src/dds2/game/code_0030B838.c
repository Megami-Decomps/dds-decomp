#include "common.h"
#include "fpu.h"
#include "sdf.h"

extern s32 D_004388B4;

extern SdfQuad D_00400970;

extern SdfQuad D_00400980;

extern void effObjSetInnerFirstVec(s32, void *);

extern void effObjSetInnerSecondVec(s32, void *);

extern s32 func_0030DE08(f32, f32);

extern void func_0030C5D8();

extern s32 frFontMeasureLines(u64);

extern u64 func_0019F448(u64, u64, u64, u64, u64, u64);

extern u32 D_004388B8;

extern s32 D_004388C4;

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

extern u32 D_00439098;

extern s32 D_0043909C;

extern u32 D_004390A0;

extern s32 sdfCounterGetDisplayValue(void);

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

typedef struct SdfSlotSet {
    u8 pad00[0xC];
    u32 slot[25];
} SdfSlotSet;
extern SdfQuad D_00400990;
extern SdfQuad D_004009A0;

extern s32 mnuCreateListState(s32, s32, s32);
extern void *func_00328D68(s32);
extern void *memset(void *, s32, u32);
extern s32 func_0030B600(void);
extern SdfCounterChannel *mnuListAppendNode(s32, s32);
extern u8 *func_0030C568(s32);
extern void func_0030D818(SdfCounterRuntime *, s32);
extern s16 func_0030C9D0(s32);
extern void func_0030C250(s32, s32);
extern void func_0030CC68();
extern u8 D_00400AF0[];

extern void func_00328E48();
extern void mnuDestroyListState();
extern void sdfCounterIncrease(void);
extern void sdfCounterDecrease(void);
extern void func_0030D938(s32, s32);
extern void func_0030D4F8(void);
extern void mnuCallInitWide();

extern void func_001094F8(s32, s32, s32, s32, u32, u32, u32, u32);

extern void func_0030F2A8(s32, s32, u32, u8 *);

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
extern void func_00306F80(s32, s32, s32, s32, u32, s32, s32, s32);

#define SDF_SPRITE(index) (((SdfSpriteSet *)D_0045C7C0[D_00400DF0[index].bank])->entries + D_00400DF0[index].slot)

static inline s32 sprPlaceBank(SdfSpritePlace *p) { return p->bank; }
static inline s32 sprPlaceSlot(SdfSpritePlace *p) { return p->slot; }
static inline s32 sprPlaceX(SdfSpritePlace *p) { return p->offsetX; }
static inline s32 sprPlaceY(SdfSpritePlace *p) { return p->offsetY; }

extern void func_0035C860(char *, const char *, ...);
extern s32 fldLoadMapResource(const char *, MapResource *);


void sdfInitInnerVectors(void) {
    effObjSetInnerFirstVec(D_004388B4, &D_00400970);
    effObjSetInnerSecondVec(D_004388B4, &D_00400980);
    ((EffObjHeader *)D_004388B4)->vtbl->refresh(D_004388B4);
}

INCLUDE_ASM(const s32, "game/code_0030B838", func_0030B880);

INCLUDE_ASM(const s32, "game/code_0030B838", func_0030BA98);

INCLUDE_ASM(const s32, "game/code_0030B838", func_0030BBA8);

/* Reset the current selection before installing the requested index. */
void sdfSetSelectedIndex(u32 index) {
    func_0030BBA8();
    D_004388B8 = index;
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

/* Cycle through the same bounded selection in the opposite direction. */
void sdfCycleBackward(void) {
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

/* Create display channels for the enabled bits of the counter mask. */
s32 func_0030C378(s32 mask, s32 index) {
    SdfCounterDisplay *display;
    SdfCounterChannel *channel;
    s32 completedMask;
    s32 count;
    s32 i;

    count = 0;
    D_004388C4 = mnuCreateListState(0, 8, 0x16);
    ((SdfCounterRuntime *)D_004388C4)->timer = func_00328D68(0x24);
    memset(((SdfCounterRuntime *)D_004388C4)->timer, 0, 0x24);
    ((SdfCounterRuntime *)D_004388C4)->draw = (SdfCounterDrawFn)func_0030CC68;
    completedMask = func_0030B600();
    for (i = 0; i != 8; i++) {
        if ((mask >> i) & 1) {
            channel = mnuListAppendNode(D_004388C4, 0);
            display = func_00328D68(0x10);
            memset(display, 0, 0x10);
            display->value = i + 1;
            display->word = (u32 *)(D_00400AF0 + i * 0x18);
            display->info = func_0030C568(i + 1);
            if ((completedMask >> i) & 1) {
                display->flag = 1;
            }
            channel->display = display;
            count++;
        }
    }
    if (count < 6) {
        ((SdfCounterRuntime *)D_004388C4)->base = count;
    }
    if (count == 3) {
        ((SdfCounterRuntime *)D_004388C4)->posX = 0x148;
        ((SdfCounterRuntime *)D_004388C4)->timer->y = 0x8F;
    } else if (count == 4) {
        ((SdfCounterRuntime *)D_004388C4)->posX = 0x100;
        ((SdfCounterRuntime *)D_004388C4)->timer->y = 0x84;
    } else if (count == 5) {
        ((SdfCounterRuntime *)D_004388C4)->posX = 0xD8;
        ((SdfCounterRuntime *)D_004388C4)->timer->y = 0x7F;
    } else {
        ((SdfCounterRuntime *)D_004388C4)->posX = 0xB0;
        ((SdfCounterRuntime *)D_004388C4)->timer->y = 0x7E;
    }
    func_0030D818(D_004388C4, index);
    func_0030C250(func_0030C9D0(index) - 1, 0xB);
    return 1;
}

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

void func_0030C5D8(SdfCounterRuntime *rt) {
    SdfCounterChannel *channel;

    if (rt != NULL) {
        for (channel = rt->first; channel != NULL; channel = channel->next) {
            func_00328E48(channel->display);
            channel->display = NULL;
        }
        func_00328E48(rt->timer);
        rt->timer = NULL;
        mnuDestroyListState(rt);
    }
}

void func_0030C640(void) {
    func_0030C5D8(D_004388C4);
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

s32 sdfCounterGetDisplayValue(void) {
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

/* Enabling a stopped timer starts it at one; disabling clears it. */
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

/* Active timers repeat every 60 ticks rather than stopping at zero. */
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

s32 func_0030D3C0(void) {
    SdfCounterRuntime *rt = (SdfCounterRuntime *)D_004388C4;
    s32 count;
    s32 done;
    if (rt->active == 0) {
        return 0;
    }
    count = rt->selected->index;
    done = count != 0;
    return ((count + rt->base - 1) ^ rt->last->index) != 0 ? (done | 2) : done;
}

void func_0030D408(s16 x, s16 y) {
    SdfCounterTimer *timer = ((SdfCounterRuntime *)D_004388C4)->timer;
    timer->startX = timer->curX;
    timer->startY = timer->curY;
    timer->targetX = x;
    timer->targetY = y;
    timer->frames = 3;
}

INCLUDE_ASM(const s32, "game/code_0030B838", func_0030D438);

void func_0030D4F8(void) {
}

void func_0030D500(s32 arg0, s32 x, f32 fade) {
    SdfCounterTimer *timer;
    s32 offset;
    f32 grow;
    f32 shrink;

    grow = fade * 0.5f + (1.0f - fade) * 2.5f;
    offset = ((SdfCounterRuntime *)D_004388C4)->selected->index * (((SdfCounterRuntime *)D_004388C4)->posX >> 3);
    timer = ((SdfCounterRuntime *)D_004388C4)->timer;
    evtSubmitGsRegister47(1, 0, 0x80, 3, 0, 0, 1, 1);
    func_00108BD8(1);
    shrink = (1.0f - grow) * 8.5f;
    func_00108EC0((s32)(shrink + 13.0f), (s32)((f32)(x - offset + timer->y + 7) + shrink), (s32)(grow * 17.0f), (s32)(grow * 17.0f),
                  0xB6, 0x151, 0x11, 0x11,
                  ((u32)(fade * 64.0f) << 24) | 0x808080,
                  ((u32)(fade * 64.0f) << 24) | 0x808080,
                  ((u32)(fade * 64.0f) << 24) | 0x808080,
                  ((u32)(fade * 64.0f) << 24) | 0x808080,
                  D_00401270.image);
    func_00108BD8(0);
}

s32 func_0030D780(u64 arg0) {
    u64 temp_v0;
    s32 temp_v1;

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

void func_0030D818(SdfCounterRuntime *rt, s32 target) {
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

u32 func_0030D900(s32 context) {
    s32 task = *(s32 *)(context + 0x10);
    u32 mask = 0;
    do {
        mask |= 1 << (*(s16 *)(*(s32 *)(task + 0x70) + 8) - 1);
        task = *(s32 *)(task + 0x58);
    } while (task != 0);
    return mask;
}

void func_0030D938(s32 x, s32 y) {
    SdfCounterDisplay *display;
    f32 fade;
    s32 width;

    display = ((SdfCounterRuntime *)D_004388C4)->channel->display;
    fade = 1.0f - (f32)((SdfCounterRuntime *)D_004388C4)->timer->value / 10.0f;
    width = func_0030D780((u64)display->info);
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
    func_0030F2A8(x, y, (u8)(u32)(fade * 128.0f) | 0x80808000, display->info);
}

void func_0030DAA0(SdfSlotSet *set) {
    s32 i;
    for (i = 0; i < 25; i++) {
        if (set->slot[i] != 0) {
            D_0045C7C0[i] = set->slot[i];
        }
    }
}

void func_0030DAE8(void) {
    s32 remaining = 24;
    u32 *slot = D_0045C7C0;
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

s64 func_0030E010(f32 x) {
    return func_0030DE08(x, x);
}

/* Load the ten numbered "sname" tiles plus the two fixed local-map images. */
s32 fldLoadLocalMapResources(void) {
    char name[32];
    s32 i;
    MapResource *item = D_00401280;

    for (i = 0; i < 10; i++) {
        func_0035C860(name, "/lmap/sname_%02d.tmx", i + 1);
        fldLoadMapResource(name, item);
        item++;
    }
    fldLoadMapResource("/lmap/1006.tmx", &D_00401260);
    fldLoadMapResource("/lmap/l_map00.tmx", &D_00401270);
    return 1;
}

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
    temp_v0 = sdfCounterGetDisplayValue();
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

