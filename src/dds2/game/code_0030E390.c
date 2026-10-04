#include "common.h"
#include "fpu.h"
#include "pcp_vu0.h"

extern s32 D_004390A4;

extern u32 D_0045C7A0[];

extern u32 D_0045C7B0[];

void sdfQueueNonzeroResourceId(s32 sprite);

typedef struct MapResource {
    u32 image;
    u32 handle;
    u32 descriptor;
    u32 unkC;
} MapResource;

extern u32 fldReleaseMapResource(s32 *);

extern u32 sdfReadNamedResource(const char *, void *, s32);

extern u32 sdfTexAcquireResourceTexture(u32);

struct SdfRing;

extern struct SdfRing *sdfCreateLinkedRequestRing(s16, s16);

extern void fldSetMapRequestInterval(s32, u16);

extern void func_0030E940(void);

struct MapRequestQueue;

extern void func_0030E958(s32, s32, s32, struct MapRequestQueue *, s32, f32);

extern s32 D_004390AC;

extern s32 D_004390B0;

extern s32 D_004390A8;

typedef struct SdfRingNode {
    u32 value;                       /* 0x00 */
    u32 argument1;                   /* 0x04 */
    u32 argument2;                   /* 0x08 */
    s32 f0C;                        /* 0x0C */
    struct SdfRingNode *next;       /* 0x10 */
    struct SdfRingNode *prev;       /* 0x14 */
    u8 pad18[8];
} SdfRingNode;

typedef struct SdfRing {
    s32 allocation;                 /* 0x00 */
    SdfRingNode *head;              /* 0x04 */
    SdfRingNode *cursor;            /* 0x08 */
    SdfRingNode *last;              /* 0x0C */
    s16 count;                      /* 0x10 */
    s16 limit;                      /* 0x12 */
    s16 pad14;                      /* 0x14 */
    s32 callback;                   /* 0x18 */
} SdfRing;

/* The ring of nodes lives inside the same block, 0x2C past the header. */
typedef struct SdfRingBlock {
    SdfRing header;
    u8 pad1C[0x28];
    SdfRingNode nodes[1]; /* 0x44 */
} SdfRingBlock;

typedef void (*MapRequestCallback)(u32, u32, u32, SdfRing *, SdfRingNode *, f32);

extern s32 sdfAllocGeneralBlock(s32);

extern void *sdfMemoryGetBlockAddress(u32);

extern void *memset(void *, s32, u32);

/* The map-request queue stores its cursor at +8 and two halfword timers at +0x14. */
typedef struct MapRequestQueue {
    u8 pad00[8];
    u32 *cursor;   /* 0x08: current request node */
    u8 pad0C[8];
    s16 interval;  /* 0x14 */
    s16 elapsed;   /* 0x16 */
    s32 callback;  /* 0x18: handler installed after queue creation */
} MapRequestQueue;

INCLUDE_ASM(const s32, "game/code_0030E390", func_0030E390);

void func_0030E878(void) {
}

void func_0030E880(void) {
    s32 handler;
    handler = (s32)sdfCreateLinkedRequestRing(0x14, 0xC);
    D_004390AC = handler;
    ((MapRequestQueue *)D_004390AC)->callback = (s32)func_0030E940;
    fldSetMapRequestInterval(handler, 0);
    D_004390B0 = (s32)sdfCreateLinkedRequestRing(0x14, 0x18);
    ((MapRequestQueue *)D_004390B0)->callback = (s32)func_0030E958;
    D_004390A8 = 5;
    D_004390A4 = 0;
}

extern s32 sdfDrawUniformlyScaledSlotImage(s32, s32, s32, s32, s32, s32, s32, f32);

extern void fldProjectPointToGridCell(s32 *, s32 *, f32, f32, f32);

void func_0030EF18(u32 *resource);

void fldReleaseMapRequestQueues(void) {
    func_0030EF18((u32 *)D_004390AC);
    func_0030EF18((u32 *)D_004390B0);
}

INCLUDE_ASM(const s32, "game/code_0030E390", func_0030E910);

INCLUDE_ASM(const s32, "game/code_0030E390", func_0030E940);

/* The current request pulses; other requests enlarge and fade in later. */
void func_0030E958(s32 x, s32 y, s32 z, MapRequestQueue *queue, s32 selected, f32 progress) {
    s32 gridX;
    s32 gridY;
    f32 scale = (1.0f - progress) * 3.0f + progress;

    if (queue->cursor[5] != selected) {
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
    sdfDrawUniformlyScaledSlotImage(gridX, gridY, 0, (s32)(progress * 64.0f), 0x20, 0, 0x54, scale);
}

void sdfCommitPendingVectorAndMarkChanged(void) {
    D_0045C7B0[0] = D_0045C7A0[0];
    D_0045C7B0[1] = D_0045C7A0[1];
    D_0045C7B0[2] = D_0045C7A0[2];
    D_004390A4 = 1;
}

INCLUDE_ASM(const s32, "game/code_0030E390", func_0030EAA8);

extern s32 sdfCounterGetDisplayValue(void);
extern f32 sdfCounterGetScaledValue(void);
extern void func_0030F038(SdfRing *);
extern f32 D_00400700[][4];

/* Draw the selected map marker and overlay its highlight twice. */
void func_0030ECC0(void) {
    s8 index = sdfCounterGetDisplayValue() - 1;
    f32 alpha = 1.0f;
    f32 remaining = 1.0f - sdfCounterGetScaledValue();
    s32 gridX;
    s32 gridY;
    s32 opacity;

    if (D_004390A4 != 0) {
        alpha = (f32)D_004390A4 / 7.0f;
    }
    if (D_004390A8 != 0) {
        alpha = (f32)(5 - D_004390A8) / 5.0f;
        D_004390A8--;
    } else if (remaining > 0.0f) {
        alpha = remaining;
    }
    func_0030F038((SdfRing *)D_004390B0);
    fldProjectPointToGridCell(&gridX, &gridY, D_00400700[index][0], D_00400700[index][1], D_00400700[index][2]);
    opacity = alpha * 32.0f;
    sdfDrawUniformlyScaledSlotImage(gridX, gridY, 0, opacity, 0x20, 0, 0x54, 2.0f);
    sdfDrawUniformlyScaledSlotImage(gridX, gridY, 0, opacity, 0x23, 0, 0x54, 1.0f);
    sdfDrawUniformlyScaledSlotImage(gridX, gridY, 0, opacity, 0x23, 0, 0x54, 1.0f);
}

/* Build a ring of `count` request nodes (0x20 bytes each) behind a 0x44-byte queue header. */
SdfRing *sdfCreateLinkedRequestRing(s16 count, s16 limit) {
    s32 size = count * 0x20 + 0x44;
    s32 allocation = sdfAllocGeneralBlock(size);
    SdfRingBlock *block = (SdfRingBlock *)sdfMemoryGetBlockAddress(allocation);
    SdfRing *ring = &block->header;
    SdfRingNode *node;
    SdfRingNode *next;
    SdfRingNode *first;
    s32 n;

    memset(ring, 0, size);
    ring->allocation = allocation;
    node = block->nodes;
    ring->head = node;
    ring->last = node;
    ring->cursor = node;
    for (n = count - 2; n != -1; n--) {
        next = node + 1;
        node->next = next;
        next->prev = node;
        node = node->next;
    }
    first = ring->head;
    node->next = first;
    first->prev = node;
    ring->limit = limit;
    ring->count = count;
    ring->pad14 = 0;
    return ring;
}

void func_0030EF18(u32 *resource) {
    if (resource != NULL) {
        sdfQueueNonzeroResourceId(*resource);
    }
}

void fldAdvanceMapRequest(s32 queue, u32 first, u32 second, u32 third) {
    u32 *entry;

    entry = ((MapRequestQueue *)queue)->cursor;
    if (((MapRequestQueue *)queue)->elapsed == ((MapRequestQueue *)queue)->interval) {
        if (entry[3] == 0) {
            *entry = first;
            entry[1] = second;
            entry[2] = third;
            ((MapRequestQueue *)queue)->cursor = (u32 *)entry[4];
            entry[3] = 1;
        }
        ((MapRequestQueue *)queue)->elapsed = 0;
        return;
    }
    ((MapRequestQueue *)queue)->elapsed = ((MapRequestQueue *)queue)->elapsed + 1;
}

void fldSetMapRequestInterval(s32 queue, u16 interval) {
    ((MapRequestQueue *)queue)->interval = interval;
}

/* Advance active ring nodes, retiring each node when it reaches the queue limit. */
void func_0030EF90(SdfRing *ring) {
    s32 remaining;
    s32 pending;
    u16 limit;
    SdfRingNode *first;
    SdfRingNode *node;

    remaining = ring->count;
    first = ring->last;
    if (--remaining == -1) {
        goto done;
    }
    if (first->f0C == 0) {
        goto done;
    }
    first->f0C++;
    pending = first->f0C < ring->limit;
    limit = ring->limit;
    if (!pending) {
        node = first->next;
        first->f0C = 0;
        ring->last = node;
        goto loop;
    }
    node = first->next;
    goto loop;

advance:
    node = node->next;
loop:
    if (--remaining == -1) {
        goto done;
    }
    if (node->f0C == 0) {
        goto done;
    }
    node->f0C++;
    if (node->f0C < (s16)limit) {
        goto advance;
    }
    {
        SdfRingNode *nextHead = ring->last->next;
        node->f0C = 0;
        node = node->next;
        ring->last = nextHead;
    }
    goto loop;

done:
    return;
}

/* Report normalized progress for each active request in the ring. */
void func_0030F038(SdfRing *ring) {
    SdfRingNode *node;
    s32 remaining;
    s32 end;
    f32 one;

    node = ring->last;
    remaining = ring->count;
    end = -1;
    one = 1.0f;
    goto loop;

advance:
    node = node->next;
loop:
    remaining--;
    if (remaining == end) {
        goto done;
    }
    if (node->f0C == 0) {
        goto done;
    }
    {
        f32 progress;
        MapRequestCallback callback = (MapRequestCallback)ring->callback;

        progress = (f32)node->f0C / (f32)ring->limit;
        progress = one - progress;
        if (callback == NULL) {
            goto advance;
        }
        callback(node->value, node->argument1, node->argument2, ring, node, progress);
    }
    node = node->next;
    goto loop;

done:
    return;
}

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

extern u8 D_0037F650[];

extern u8 D_0037F660[];

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
    VU0_LOAD_VF(vf11, D_0037F650);
    VU0_MUL(vf10, vf10, vf11);
    VU0_LOAD_VF(vf11, D_0037F660);
    VU0_ADD(vf10, vf10, vf11);
    VU0_STORE_VF(vf10, screen);
    *gridX = ((s32)(screen[0] * 16.0f) - 0x7000) >> 4;
    *gridY = ((s32)(screen[1] * 16.0f) - 0x7900) >> 3;
}
