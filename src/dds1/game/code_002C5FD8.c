#include "common.h"
#include "fpu.h"
#include "pcp_vu0.h"

struct SdfTex;
extern void sdfTexReleaseReferenceViaHandler(struct SdfTex *texture);

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

extern void evtSubmitPrimaryGsTest(s32, s32, s32, s32, s32, s32, s32, s32);
extern void func_00108FA0(s32, s32, s32, s32, s32, s32, s32, s32,
                         u32, u32, u32, u32, u32);
extern void evtSubmitPrimaryAlphaBlendMode(s32);
extern void fldProjectPointToGridCell(s32 *, s32 *, f32, f32, f32);

extern s32 func_003014F0(char *, const char *, ...);

extern u32 fldReleaseMapResource(s32 *);

extern struct SdfMemBlock *sdfReadNamedResource(const char *name, u32 *outAddress, u32 *outSize);

extern struct SdfTex *sdfTexAcquireResourceTexture(void *resourceAddress);

extern s32 D_003BD984;

typedef struct MapRequestNode {
    u32 value;
    u32 argument1;
    u32 argument2;
    s32 active;
    struct MapRequestNode *next;
    struct MapRequestNode *prev;
    u8 pad18[8];
} MapRequestNode;

typedef struct MapRequestState {
    u32 handle;           /* 0x00 */
    MapRequestNode *first; /* 0x04 */
    MapRequestNode *next; /* 0x08 */
    MapRequestNode *third; /* 0x0C */
    s16 count;            /* 0x10 */
    s16 arg;              /* 0x12 */
    s16 interval;         /* 0x14 */
    s16 elapsed;          /* 0x16 */
    void (*callback)(s32, s32, s32, struct MapRequestState *, MapRequestNode *, f32); /* 0x18 */
} MapRequestState;

/* The ring of nodes lives inside the same block, 0x2C past the header. */
typedef struct MapRequestRing {
    MapRequestState header;
    u8 pad1C[0x28];
    MapRequestNode nodes[1]; /* 0x44 */
} MapRequestRing;


extern MapRequestState *D_003BD988;

extern MapRequestState *D_003BD98C;
extern void func_002C7C58(MapRequestState *);
extern f32 sdfCounterGetScaledValue(void);
extern f32 D_0038FE30[][4];

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

s32 sdfCounterMeasureLabelWidth(u32 arg0);

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

extern SdfCounterRuntime *sdfActiveCounterRuntime;
extern void evtSubmitDefaultDepthGradientRect(s32, s32, s32, s32, u32, u32, u32, u32);
extern void sdfCounterDrawGlyphAtGridCell(s32, s32, u32, const u8 *);

void sdfCounterDrawFadingLabelPanel(s32 x, s32 y) {
    SdfCounterDisplay *display = sdfActiveCounterRuntime->channel->display;
    f32 fade = 1.0f;
    s32 color;
    s32 width;
    u32 style;

    fade -= (f32)sdfActiveCounterRuntime->timer->value / 10.0f;
    evtSubmitDefaultDepthGradientRect(0, 0x120, 0x200, 0xA0, 0, 0,
                 (s32)(fade * 64.0f) << 24, (s32)(fade * 64.0f) << 24);
    fade *= 128.0f;
    color = ((s32)(fade * 0.3f) << 24) | 0x5A3335;
    evtSubmitDefaultDepthGradientRect(0, 0x17E, 0x100, 0x1A, 0x5A3335, color, color, 0x5A3335);
    evtSubmitDefaultDepthGradientRect(0x100, 0x17E, 0x100, 0x1A, color, 0x5A3335, 0x5A3335, color);
    width = sdfCounterMeasureLabelWidth((u32)display->info);
    x = (s32)((f32)x - (f32)width * 0.5f);
    style = ((u32)fade & 0xFF) | 0x80808000;
    sdfCounterDrawGlyphAtGridCell(x, y, style, display->info);
}

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

extern void func_002C6948(f32 progress);

/* Draw the three local-map frame pieces: the side panels slide in from x 427 / 353. */
static inline void fldDrawLocalMapFrame(f32 slide, f32 alpha1, f32 alpha2, f32 alpha3) {
    s16 rects[4][8] = {
        {0x5F, 0x50, 0xE, 0xF, 0xB5, 0x164, 0xE, 0xF},
        {0x129, 0x133, 0xE, 0xF, 0xB5, 0x174, 0xE, 0xF},
        {0x1AB, 0x133, 0xE, 0xF, 0xB5, 0x174, 0xE, 0xF},
        {0xDF, 0x133, 0x60, 0xF, 0xC7, 0x166, 0x20, 0xF},
    };

    evtSubmitPrimaryAlphaBlendMode(1);
    func_00108FA0((s32)(slide * 427.0f + rects[1][0] * (1.0f - slide)), rects[1][1], rects[1][2], rects[1][3],
                  rects[1][4], rects[1][5], rects[1][6], rects[1][7], ((s32)(alpha1 * 128.0f) << 24) | 0x808080,
                  ((s32)(alpha1 * 128.0f) << 24) | 0x808080, ((s32)(alpha1 * 128.0f) << 24) | 0x808080,
                  ((s32)(alpha1 * 128.0f) << 24) | 0x808080, fldLocalMapTextureResource.image);
    func_00108FA0(rects[2][0], rects[2][1], rects[2][2], rects[2][3], rects[2][4], rects[2][5], rects[2][6],
                  rects[2][7], ((s32)(alpha2 * 128.0f) << 24) | 0x808080, ((s32)(alpha2 * 128.0f) << 24) | 0x808080,
                  ((s32)(alpha2 * 128.0f) << 24) | 0x808080, ((s32)(alpha2 * 128.0f) << 24) | 0x808080,
                  fldLocalMapTextureResource.image);
    func_00108FA0((s32)(slide * 353.0f + rects[3][0] * (1.0f - slide)), rects[3][1], rects[3][2], rects[3][3],
                  rects[3][4], rects[3][5], rects[3][6], rects[3][7], ((s32)(alpha3 * 128.0f) << 24) | 0x808080,
                  ((s32)(alpha3 * 128.0f) << 24) | 0x808080, ((s32)(alpha3 * 128.0f) << 24) | 0x808080,
                  ((s32)(alpha3 * 128.0f) << 24) | 0x808080, fldLocalMapTextureResource.image);
    evtSubmitPrimaryAlphaBlendMode(0);
}

/* Local-map open/close transition: fades the backdrop (func_002C6948), slides the side panels in, and fades the
 * frame pieces (each panel row is x, y, w, h, u, v, uw, vh). */
void fldDrawLocalMapTransition(s32 opening) {
    f32 progress;
    f32 slide = 0.0f;
    f32 alpha1 = 0.0f;
    f32 alpha2 = 0.0f;
    f32 alpha3 = 0.0f;
    s32 timer;

    if (opening != 0) {
        timer = sdfCounterAnimationValue;
        if (timer < 20) {
            progress = timer / 20.0f;
        } else {
            progress = 1.0f;
        }
        if (timer >= 26) {
            if (timer < 33) {
                slide = (timer - 25) * 0.125f;
            } else {
                slide = 1.0f;
            }
        }
        slide = (slide + slide * slide) * 0.5f;
        if (timer >= 26) {
            alpha1 = 0.1f;
            if (timer >= 30) {
                if (timer < 33) {
                    alpha1 = (timer - 30) / 3.0f;
                } else if (timer < 48) {
                    alpha1 = 1.0f - (timer - 33) / 15.0f;
                } else {
                    alpha1 = 0.0f;
                }
            }
        }
        if (timer >= 29) {
            alpha2 = (timer - 28) / 7.0f;
        }
        if (alpha2 > 1.0f) {
            alpha2 = 1.0f - (timer - 35) / 15.0f;
            if (timer >= 51) {
                alpha2 = 0.0f;
            }
        }
        if (timer >= 26) {
            if (timer < 33) {
                alpha3 = (timer - 25) * 0.125f * 0.4f;
            } else if (timer < 48) {
                alpha3 = (1.0f - (timer - 33) / 15.0f) * 0.2f + 0.2f;
            } else {
                alpha3 = 0.2f;
            }
        }
        D_003BD980 = 0;
    } else {
        if (D_003BD980 == 0) {
            D_003BD980 = sdfCounterAnimationValue;
        }
        timer = D_003BD980 - sdfCounterAnimationValue;
        if (timer < 20) {
            progress = timer / 20.0f;
        } else {
            progress = 1.0f;
        }
        progress = 1.0f - progress;
        if (progress == 0.0f) {
            return;
        }
        if ((s32)D_003BD980 >= 26) {
            if ((s32)D_003BD980 < 33) {
                slide = ((s32)D_003BD980 - 25) * 0.125f;
            } else {
                slide = 1.0f;
            }
        }
        alpha3 = progress * 0.2f;
        alpha2 = 0.0f;
        alpha1 = alpha2;
    }
    func_002C6948(progress);
    fldDrawLocalMapFrame(slide, alpha1, alpha2, alpha3);
}

INCLUDE_ASM(const s32, "game/code_002C5FD8", func_002C6948);

INCLUDE_ASM(const s32, "game/code_002C5FD8", func_002C6EC8);

extern MapRequestState *sdfCreateLinkedRequestRing(s16, s16);

extern void fldDrawMapRequestHalo(s32, s32, s32, MapRequestState *, MapRequestNode *, f32);

extern void fldDrawMapRequestPulse(s32, s32, s32, MapRequestState *, MapRequestNode *, f32);

void fldSetMapRequestInterval(MapRequestState *state, u16 interval);

/* Allocate the two map request queues and install their dispatch callbacks. */
void fldCreateMapRequestQueues(void) {
    D_003BD988 = sdfCreateLinkedRequestRing(0x14, 0xC);
    D_003BD988->callback = fldDrawMapRequestHalo;
    fldSetMapRequestInterval(D_003BD988, 0);
    D_003BD98C = sdfCreateLinkedRequestRing(0x14, 0x18);
    D_003BD98C->callback = fldDrawMapRequestPulse;
}

/* Release both map request queues. */
void fldReleaseMapRequestQueues(void) {
    func_002C7B38(D_003BD988);
    func_002C7B38(D_003BD98C);
}

/* Draw the auxiliary map texture centred on the requested position. */
void fldDrawScaledAuxMapTexture(s32 x, s32 y, u32 colour, f32 scale) {
    evtSubmitPrimaryGsTest(1, 0, 0x80, 3, 0, 0, 1, 1);
    func_00108FA0((s32)(x - scale * 16.0f), (s32)(y - scale * 12.0f),
                  (s32)(scale * 32.0f), (s32)(scale * 24.0f),
                  0, 0, 32, 32, colour, colour, colour, colour,
                  fldLocalMapAuxTextureResource.image);
}

/* Four corners use the same grey pulse colour. */
#define MAP_GREY_COLOR(alpha) (((u32)(alpha) << 24) | 0x808080)

void fldDrawMapRequestHalo(s32 x, s32 y, s32 z, MapRequestState *state, MapRequestNode *node, f32 progress) {
    s32 gridX;
    s32 gridY;
    f32 scale = (1.0f - progress) * 2.0f + progress;

    if (progress >= 0.7f) {
        progress = (1.0f - progress) / 0.3f;
    } else {
        progress /= 0.7f;
    }
    if (state->next->prev != node) {
        progress -= 0.2f;
        if (progress < 0.0f) {
            progress = 0.0f;
        }
    }
    evtSubmitPrimaryGsTest(1, 0, 0x80, 3, 0, 0, 1, 1);
    fldProjectPointToGridCell(&gridX, &gridY, x, y, z);
    func_00108FA0((s32)(gridX - scale * 16.0f), (s32)(gridY - scale * 16.0f),
                  (s32)(scale * 32.0f), (s32)(scale * 32.0f), 0, 0, 32, 32,
                  MAP_GREY_COLOR(progress * 24.0f), MAP_GREY_COLOR(progress * 24.0f),
                  MAP_GREY_COLOR(progress * 24.0f), MAP_GREY_COLOR(progress * 24.0f),
                  fldLocalMapAuxTextureResource.image);
}


void fldDrawMapRequestPulse(s32 x, s32 y, s32 z, MapRequestState *state, MapRequestNode *node, f32 progress) {
    s32 gridX;
    s32 gridY;
    f32 scale = (1.0f - progress) * 3.0f + progress;

    if (state->next->prev != node) {
        progress -= 0.5f;
        if (progress < 0.0f) {
            progress = 0.0f;
        }
        scale *= 1.5f;
    } else if (progress >= 0.7f) {
        progress = (1.0f - progress) / 0.3f;
    } else {
        progress /= 0.7f;
    }
    fldProjectPointToGridCell(&gridX, &gridY, x, y, z);
    evtSubmitPrimaryGsTest(1, 0, 0x80, 3, 0, 0, 1, 1);
    evtSubmitPrimaryAlphaBlendMode(1);
    func_00108FA0((s32)(gridX - scale * 16.0f), (s32)(gridY - scale * 12.0f),
                  (s32)(scale * 32.0f), (s32)(scale * 24.0f), 0, 0, 32, 32,
                  MAP_GREY_COLOR(progress * 64.0f), MAP_GREY_COLOR(progress * 64.0f),
                  MAP_GREY_COLOR(progress * 64.0f), MAP_GREY_COLOR(progress * 64.0f),
                  fldLocalMapAuxTextureResource.image);
    evtSubmitPrimaryAlphaBlendMode(0);
}

void sdfCommitPendingVectorAndMarkChanged(void) {
    D_003DFEE0[0] = D_003DFED0[0];
    D_003DFEE0[1] = D_003DFED0[1];
    D_003DFEE0[2] = D_003DFED0[2];
    D_003BD984 = 1;
}

extern s8 D_003BD281;
extern void fldAdvanceMapRequest(MapRequestState *, u32, u32, u32);
extern void func_002C7BB0(MapRequestState *);

void fldUpdateMapRequestQueues(s32 enabled) {
    s8 index = sdfCounterGetDisplayValue() - 1;

    if (D_003BD984 != 0 && enabled != 0) {
        D_003BD281 = 0;
        if (D_003BD984 == 1) {
            fldAdvanceMapRequest(D_003BD98C, D_003DFEE0[0], D_003DFEE0[1], D_003DFEE0[2]);
            fldAdvanceMapRequest(D_003BD98C, (s32)D_0038FE30[index][0],
                (s32)D_0038FE30[index][1], (s32)D_0038FE30[index][2]);
        }
        D_003BD984++;
        D_003DFED0[0] = (s32)D_0038FE30[index][0];
        D_003DFED0[1] = (s32)D_0038FE30[index][1];
        D_003DFED0[2] = (s32)D_0038FE30[index][2];
        if (D_003BD984 >= 7) {
            D_003BD984 = 0;
        }
        fldAdvanceMapRequest(D_003BD988, D_003DFED0[0], D_003DFED0[1], D_003DFED0[2]);
    } else {
        if (++D_003BD281 == 0x1C) {
            if (enabled != 0) {
                fldAdvanceMapRequest(D_003BD98C, (s32)D_0038FE30[index][0],
                    (s32)D_0038FE30[index][1], (s32)D_0038FE30[index][2]);
            }
            D_003BD281 = 0;
        }
        D_003DFED0[0] = (s32)D_0038FE30[index][0];
        D_003DFED0[1] = (s32)D_0038FE30[index][1];
        D_003DFED0[2] = (s32)D_0038FE30[index][2];
    }
    func_002C7BB0(D_003BD988);
    func_002C7BB0(D_003BD98C);
}

void fldDrawCounterMapMarker(void) {
    s32 x;
    s32 y;
    s8 index = sdfCounterGetDisplayValue() - 1;
    f32 opacity = 1.0f;
    f32 remaining = 1.0f - sdfCounterGetScaledValue();

    if (D_003BD984 != 0) {
        opacity = (f32)D_003BD984 / 7.0f;
    }
    if (remaining > 0.0f) {
        opacity = remaining;
    }
    func_002C7C58(D_003BD98C);
    fldProjectPointToGridCell(&x, &y, D_0038FE30[index][0], D_0038FE30[index][1], D_0038FE30[index][2]);
    fldDrawScaledAuxMapTexture(x, y, MAP_GREY_COLOR(opacity * 32.0f), 5.0f);
}

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

/* Advance active ring nodes, retiring each node when it reaches the queue limit. */
INCLUDE_ASM(const s32, "game/code_002C5FD8", func_002C7BB0);

/* Report normalized progress for each active request in the ring. */
INCLUDE_ASM(const s32, "game/code_002C5FD8", func_002C7C58);

s32 fldLoadMapResource(const char *name, MapResource *record) {
    u32 handle = (u32)sdfReadNamedResource(name, &record->descriptor, 0);
    u32 descriptor = record->descriptor;
    record->handle = handle;
    record->image = (u32)sdfTexAcquireResourceTexture((void *)descriptor);
    if (record->handle != 0) {
        sdfQueueNonzeroResourceId((void *)record->handle);
        record->handle = 0;
        record->descriptor = 0;
    }
    return 1;
}

u32 fldReleaseMapResource(s32 *image) {
    if (*image != 0) {
        sdfTexReleaseReferenceViaHandler((struct SdfTex *)*image);
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

INCLUDE_RODATA(const s32, "game/code_002C5FD8", D_003B3E00);

INCLUDE_RODATA(const s32, "game/code_002C5FD8", D_003B3E40);

INCLUDE_SDATA(const s32, "game/code_002C5FD8", D_003BD280);

INCLUDE_SDATA(const s32, "game/code_002C5FD8", D_003BD281);

