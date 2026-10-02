#include "common.h"
#include "fpu.h"
#include "pcp_vu0.h"

extern u32 sdfCounterAnimationValue;

extern s32 D_003BD97C;

extern u32 D_003BD980;

extern s32 sdfCounterGetDisplayValue(void);

typedef struct MapResource {
    u32 image;
    u32 handle;
    u32 descriptor;
    u32 unkC;
} MapResource;

extern MapResource fldLocalMapNameTextures[10];

extern MapResource fldLocalMapAuxTextureResource;

extern MapResource fldLocalMapTextureResource;

extern s32 fldLoadMapResource(const char *, MapResource *);

extern s32 func_003014F0(char *, const char *, ...);

extern u32 fldReleaseMapResource(s32 *);

extern u32 sdfReadNamedResource(const char *, void *, s32);

extern u32 sdfTexAcquireResourceTexture(u32);

extern u32 D_003BD984;

typedef struct MapRequestNode {
    u32 value;
    u32 argument1;
    u32 argument2;
    s32 active;
    struct MapRequestNode *next;
    struct MapRequestNode *prev;
    u8 pad18[8];
} MapRequestNode;

typedef struct {
    u32 handle;           /* 0x00 */
    MapRequestNode *first; /* 0x04 */
    MapRequestNode *next; /* 0x08 */
    MapRequestNode *third; /* 0x0C */
    s16 count;            /* 0x10 */
    s16 arg;              /* 0x12 */
    s16 interval;         /* 0x14 */
    s16 elapsed;          /* 0x16 */
    void (*callback)(void); /* 0x18 */
} MapRequestState;

/* The ring of nodes lives inside the same block, 0x2C past the header. */
typedef struct MapRequestRing {
    MapRequestState header;
    u8 pad1C[0x28];
    MapRequestNode nodes[1]; /* 0x44 */
} MapRequestRing;

extern MapRequestState *D_003BD988;

extern MapRequestState *D_003BD98C;

extern u32 D_003DFED0[];

extern u32 D_003DFEE0[];

s32 sdfQueueNonzeroResourceId(u32 sprite);

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

void func_002C5FB8(u32 arg0);

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

typedef struct {
    u8 pad00[0x0C];
    s32 selectedCount; /* 0x0C */
    u8 pad10[0x10];
    s32 maxCount;      /* 0x20 */
} MapSelection;

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

typedef struct MapSelectionNode {
    u8 pad00[8];
    s16 ordinal;           /* 0x08: 1-based bit position */
} MapSelectionNode;

typedef struct MapSelectionLink {
    u8 pad00[0x58];
    struct MapSelectionLink *next; /* 0x58 */
    u8 pad5C[0x14];
    MapSelectionNode *selection;   /* 0x70 */
} MapSelectionLink;

typedef struct MapSelectionContext {
    u8 pad00[0x10];
    MapSelectionLink *first;       /* 0x10 */
} MapSelectionContext;

/* Collect the 1-based selection ids in the linked map nodes into a bitmask. */
s32 fldCollectMapSelectionMask(MapSelectionContext *context) {
    MapSelectionLink *node;
    s32 mask;

    node = context->first;
    mask = 0;
    do {
        MapSelectionNode *selection = node->selection;
        node = node->next;
        mask |= 1 << (selection->ordinal - 1);
    } while (node != NULL);
    return mask;
}

INCLUDE_ASM(const s32, "game/code_002C5FD8", func_002C6130);

/* Load the ten numbered "sname" tiles plus the two fixed local-map images. */
s32 fldLoadLocalMapResources(void) {
    char name[32];
    s32 i;
    MapResource *item = fldLocalMapNameTextures;

    for (i = 0; i < 10; i++) {
        func_003014F0(name, "/lmap/sname_%02d.tmx", i + 1);
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
    s32 count;

    sdfCounterAnimationValue = 0;
    count = sdfCounterGetDisplayValue();
    D_003BD97C = count - 1;
    D_003BD980 = 0x3c;
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

INCLUDE_ASM(const s32, "game/code_002C5FD8", func_002C6448);

INCLUDE_ASM(const s32, "game/code_002C5FD8", func_002C6948);

INCLUDE_ASM(const s32, "game/code_002C5FD8", func_002C6EC8);

extern MapRequestState *sdfCreateLinkedRequestRing(s16, s16);

extern void func_002C7180(void);

extern void func_002C7430(void);

void fldSetMapRequestInterval(MapRequestState *state, u16 interval);

/* Allocate the two map request queues and install their dispatch callbacks. */
void fldCreateMapRequestQueues(void) {
    D_003BD988 = sdfCreateLinkedRequestRing(0x14, 0xC);
    D_003BD988->callback = func_002C7180;
    fldSetMapRequestInterval(D_003BD988, 0);
    D_003BD98C = sdfCreateLinkedRequestRing(0x14, 0x18);
    D_003BD98C->callback = func_002C7430;
}

/* Release both map request queues. */
void fldReleaseMapRequestQueues(void) {
    func_002C7B38(D_003BD988);
    func_002C7B38(D_003BD98C);
}

INCLUDE_ASM(const s32, "game/code_002C5FD8", func_002C7080);

INCLUDE_ASM(const s32, "game/code_002C5FD8", func_002C7180);

INCLUDE_ASM(const s32, "game/code_002C5FD8", func_002C7430);

void sdfCommitPendingVectorAndMarkChanged(void) {
    D_003DFEE0[0] = D_003DFED0[0];
    D_003DFEE0[1] = D_003DFED0[1];
    D_003DFEE0[2] = D_003DFED0[2];
    D_003BD984 = 1;
}

INCLUDE_ASM(const s32, "game/code_002C5FD8", func_002C7738);

INCLUDE_ASM(const s32, "game/code_002C5FD8", func_002C7950);

extern u32 sdfAllocGeneralBlock(s32 size);

extern void *sdfMemoryGetBlockAddress(u32 handle);

/* Build a ring of `count` request nodes (0x20 bytes each) behind a 0x44-byte queue header. */
MapRequestState *sdfCreateLinkedRequestRing(s16 count, s16 arg) {
    s32 size = count * 0x20 + 0x44;
    u32 handle = sdfAllocGeneralBlock(size);
    MapRequestRing *pool = (MapRequestRing *)sdfMemoryGetBlockAddress(handle);
    MapRequestState *state = &pool->header;
    MapRequestNode *node;
    MapRequestNode *next;
    MapRequestNode *first;
    s32 n;

    memset(state, 0, size);
    state->handle = handle;
    node = pool->nodes;
    state->first = node;
    state->third = node;
    state->next = node;
    for (n = count - 2; n != -1; n--) {
        next = node + 1;
        node->next = next;
        next->prev = node;
        node = node->next;
    }
    first = state->first;
    node->next = first;
    first->prev = node;
    state->arg = arg;
    state->count = count;
    state->interval = 0;
    return state;
}

void func_002C7B38(u32 *sprite) {
    if (sprite != NULL) {
        sdfQueueNonzeroResourceId(*sprite);
    }
}

/* Dispatch one queued request per interval; inactive nodes are reused. */
void fldAdvanceMapRequest(MapRequestState *state, u32 value, u32 argument1, u32 argument2) {
    MapRequestNode *node;

    node = state->next;
    if (state->elapsed == state->interval) {
        if (node->active == 0) {
            node->value = value;
            node->argument1 = argument1;
            node->argument2 = argument2;
            state->next = node->next;
            node->active = 1;
        }
        state->elapsed = 0;
        return;
    }
    state->elapsed = state->elapsed + 1;
}

void fldSetMapRequestInterval(MapRequestState *state, u16 interval) {
    state->interval = interval;
}

INCLUDE_ASM(const s32, "game/code_002C5FD8", func_002C7BB0);

INCLUDE_ASM(const s32, "game/code_002C5FD8", func_002C7C58);

s32 fldLoadMapResource(const char *name, MapResource *record) {
    u32 handle = sdfReadNamedResource(name, &record->descriptor, 0);
    u32 descriptor = record->descriptor;
    record->handle = handle;
    record->image = sdfTexAcquireResourceTexture(descriptor);
    if (record->handle != 0) {
        sdfQueueNonzeroResourceId((void *)record->handle);
        record->handle = 0;
        record->descriptor = 0;
    }
    return 1;
}

u32 fldReleaseMapResource(s32 *image) {
    if (*image != 0) {
        sdfTexReleaseReferenceViaHandler(*image);
        *image = 0;
    }
    return 1;
}

extern u8 sdfViewMatrix[];

extern u8 sdfProjectionMatrix[];

extern u8 D_00324650[];

extern u8 D_00324660[];

extern void sdfPostmultiplyVuMatrixFromMemory(void *src);

/* vu0 routine: project a world point to the screen and return its GS grid cell. */
void fldProjectPointToGridCell(s32 *gridX, s32 *gridY, f32 x, f32 y, f32 z) {
    f32 point[4];
    f32 screen[4];

    point[0] = x;
    point[1] = y;
    point[2] = z;
    point[3] = 1.0f;
    VU0_LOAD_MATRIX(sdfViewMatrix);
    sdfPostmultiplyVuMatrixFromMemory(sdfProjectionMatrix);
    VU0_MOVE_MATRIX_TO_B();
    VU0_LOAD_VF(vf10, point);
    VU0_TRANSFORM_POINT(vf10, vf10);
    VU0_PERSPECTIVE_DIVIDE_VF10();
    VU0_LOAD_VF(vf11, D_00324650);
    VU0_MUL(vf10, vf10, vf11);
    VU0_LOAD_VF(vf11, D_00324660);
    VU0_ADD(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, screen);
    *gridX = ((s32)(screen[0] * 16.0f) - 0x7000) >> 4;
    *gridY = ((s32)(screen[1] * 16.0f) - 0x7900) >> 3;
}

INCLUDE_RODATA(const s32, "game/code_002C5FD8", D_003B3DC0);

INCLUDE_RODATA(const s32, "game/code_002C5FD8", D_003B3E00);

INCLUDE_RODATA(const s32, "game/code_002C5FD8", D_003B3E40);

INCLUDE_SDATA(const s32, "game/code_002C5FD8", D_003BD281);

