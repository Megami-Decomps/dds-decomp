#include "common.h"
#include "fpu.h"
#include "pcp_vu0.h"
#include "fld_lmap_task.h"

struct SdfTex;
extern void sdfTexReleaseReferenceViaHandler(struct SdfTex *texture);

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

extern struct SdfMemBlock *sdfReadNamedResource(const char *name, u32 *outAddress, u32 *outSize);

extern struct SdfTex *sdfTexAcquireResourceTexture(void *resourceAddress);



extern void func_0030E940(s32, s32, s32, MapRequestState *, MapRequestNode *, f32);


extern void func_0030E958(s32, s32, s32, MapRequestState *, MapRequestNode *, f32);

extern MapRequestState *D_004390AC;

extern MapRequestState *D_004390B0;

extern s32 D_004390A8;



extern s32 sdfAllocGeneralBlock(s32);

extern void *sdfMemoryGetBlockAddress(u32);

extern void *memset(void *, s32, u32);


extern s32 D_0043909C;
extern void func_0030DBF0(s32 x, s32 y, s32 z, u32 color0, u32 color1, u32 color2, u32 color3, s32 image, s32 flags,
                          s32 depth);
extern void sdfDrawPositionedSlotImage(s32 x, s32 y, s32 z, s32 alpha, s32 image, s32 flags, s32 depth);

/* Field transition backdrop at `progress` (0..1): a corner-faded panel, the style's caption, the frame and the
 * sliding banner. */
void fldDrawTransitionBackdrop(f32 progress) {
    f32 alpha;
    f32 fade;
    s32 depth = 0x54; /* retail keeps the shared draw depth in s0 across every call */

    if (progress < 0.5f) {
        alpha = progress * 0.2f;
    } else {
        alpha = 1.0f;
    }
    if (progress < 0.25f) {
        fade = 0.0f;
    } else if (progress < 0.75f) {
        fade = (progress - 0.25f) + (progress - 0.25f);
    } else {
        fade = 1.0f;
    }
    func_0030DBF0(0, 0, 0, (u32)(fade * 128.0f) | 0x80808000, (u32)(alpha * 128.0f) | 0x80808000,
                  (u32)(fade * 128.0f) | 0x80808000, (u32)(alpha * 128.0f) | 0x80808000, 0x1F, 0, depth);
    if (progress < 0.3f) {
        alpha = 0.0f;
    } else if (progress < 0.8f) {
        alpha = (progress - 0.3f) + (progress - 0.3f);
    } else {
        alpha = 1.0f;
    }
    if (D_0043888C != NULL) {
        switch (D_0043888C->variant) {
        case 1:
            sdfDrawPositionedSlotImage(0, -2, 0, (s32)(alpha * 128.0f), D_0043909C + 0x25, 0, depth);
            break;
        case 2:
            sdfDrawPositionedSlotImage(0, -2, 0, (s32)(alpha * 128.0f), D_0043909C + 0x2D, 0, depth);
            break;
        case 3:
            func_0030DBF0(0, -2, 0, (u32)(alpha * 128.0f) | 0x80808000, (u32)(alpha * 104.0f) | 0x80808000,
                          (u32)(alpha * 104.0f) | 0x80808000, (u32)(alpha * 128.0f) | 0x80808000,
                          D_0043909C + 0x35, 0, depth);
            break;
        }
    }
    sdfDrawPositionedSlotImage(0, -2, 0, (s32)(alpha * 128.0f), 0x16, 0, depth);
    alpha = 0.0f;
    if (!(progress < 0.6f)) {
        alpha = (progress - 0.6f) / 0.4f;
    }
    if (progress < 0.4f) {
        fade = 0.0f;
    } else if (progress < 0.8f) {
        fade = (progress - 0.4f) / 0.4f;
    } else {
        fade = 1.0f;
    }
    sdfDrawPositionedSlotImage((s32)((1.0f - fade) * 50.0f), 1, 0, (s32)(alpha * 128.0f), D_0043909C + 0x17, 0,
                               depth);
}

void func_0030E878(void) {
}

void func_0030E880(void) {
    MapRequestState *state;
    state = sdfCreateLinkedRequestRing(0x14, 0xC);
    D_004390AC = state;
    D_004390AC->callback = func_0030E940;
    fldSetMapRequestInterval(state, 0);
    D_004390B0 = sdfCreateLinkedRequestRing(0x14, 0x18);
    D_004390B0->callback = func_0030E958;
    D_004390A8 = 5;
    D_004390A4 = 0;
}

extern void sdfDrawUniformlyScaledSlotImage(s32, s32, s32, s32, s32, s32, s32, f32);

extern void fldProjectPointToGridCell(s32 *, s32 *, f32, f32, f32);

void func_0030EF18(MapRequestState *state);

void fldReleaseMapRequestQueues(void) {
    func_0030EF18(D_004390AC);
    func_0030EF18(D_004390B0);
}

void fldDrawMapRequestMarker(s32 x, s32 y, s32 alpha, f32 scale) {
    sdfDrawUniformlyScaledSlotImage(x, y, 0, alpha, 0x20, 0, 0x54, scale);
}

INCLUDE_ASM(const s32, "game/code_0030E390", func_0030E940);

/* The current request pulses; other requests enlarge and fade in later. */
void func_0030E958(s32 x, s32 y, s32 z, MapRequestState *queue, MapRequestNode *selected, f32 progress) {
    s32 gridX;
    s32 gridY;
    f32 scale = (1.0f - progress) * 3.0f + progress;

    if (queue->next->prev != selected) {
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

extern s32 sdfCounterGetDisplayValue(void);
extern f32 D_00400700[][4];
extern s8 D_004388D1;
extern void func_0030EF90(MapRequestState *);

void fldUpdateMapRequestQueues(s32 enabled) {
    s8 index = sdfCounterGetDisplayValue() - 1;

    if (D_004390A4 != 0 && enabled != 0) {
        D_004388D1 = 0;
        if (D_004390A4 == 1) {
            fldAdvanceMapRequest(D_004390B0, D_0045C7B0[0], D_0045C7B0[1], D_0045C7B0[2]);
            fldAdvanceMapRequest(D_004390B0, (s32)D_00400700[index][0],
                (s32)D_00400700[index][1], (s32)D_00400700[index][2]);
        }
        D_004390A4++;
        D_0045C7A0[0] = (s32)D_00400700[index][0];
        D_0045C7A0[1] = (s32)D_00400700[index][1];
        D_0045C7A0[2] = (s32)D_00400700[index][2];
        if (D_004390A4 >= 7) {
            D_004390A4 = 0;
        }
        fldAdvanceMapRequest(D_004390AC, D_0045C7A0[0], D_0045C7A0[1], D_0045C7A0[2]);
    } else {
        if (++D_004388D1 == 0x1C) {
            if (enabled != 0) {
                fldAdvanceMapRequest(D_004390B0, (s32)D_00400700[index][0],
                    (s32)D_00400700[index][1], (s32)D_00400700[index][2]);
            }
            D_004388D1 = 0;
        }
        D_0045C7A0[0] = (s32)D_00400700[index][0];
        D_0045C7A0[1] = (s32)D_00400700[index][1];
        D_0045C7A0[2] = (s32)D_00400700[index][2];
    }
    func_0030EF90(D_004390AC);
    func_0030EF90(D_004390B0);
}

extern f32 sdfCounterGetScaledValue(void);
extern void func_0030F038(MapRequestState *);

/* Draw the selected map marker and overlay its highlight twice. */
void fldDrawSelectedMapMarker(void) {
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
    func_0030F038(D_004390B0);
    fldProjectPointToGridCell(&gridX, &gridY, D_00400700[index][0], D_00400700[index][1], D_00400700[index][2]);
    opacity = alpha * 32.0f;
    sdfDrawUniformlyScaledSlotImage(gridX, gridY, 0, opacity, 0x20, 0, 0x54, 2.0f);
    sdfDrawUniformlyScaledSlotImage(gridX, gridY, 0, opacity, 0x23, 0, 0x54, 1.0f);
    sdfDrawUniformlyScaledSlotImage(gridX, gridY, 0, opacity, 0x23, 0, 0x54, 1.0f);
}

/* Build a ring of `count` request nodes (0x20 bytes each) behind a 0x44-byte queue header. */
MapRequestState *sdfCreateLinkedRequestRing(s16 count, s16 limit) {
    s32 size = count * 0x20 + 0x44;
    u32 allocation = sdfAllocGeneralBlock(size);
    MapRequestRing *block = (MapRequestRing *)sdfMemoryGetBlockAddress(allocation);
    MapRequestState *ring = &block->header;
    MapRequestNode *node;
    MapRequestNode *next;
    MapRequestNode *first;
    s32 n;

    memset(ring, 0, size);
    ring->handle = allocation;
    node = block->nodes;
    ring->first = node;
    ring->third = node;
    ring->next = node;
    for (n = count - 2; n != -1; n--) {
        next = node + 1;
        node->next = next;
        next->prev = node;
        node = node->next;
    }
    first = ring->first;
    node->next = first;
    first->prev = node;
    ring->arg = limit;
    ring->count = count;
    ring->interval = 0;
    return ring;
}

void func_0030EF18(MapRequestState *state) {
    if (state != NULL) {
        sdfQueueNonzeroResourceId(state->handle);
    }
}

void fldAdvanceMapRequest(MapRequestState *queue, u32 first, u32 second, u32 third) {
    MapRequestNode *entry;

    entry = queue->next;
    if (queue->elapsed == queue->interval) {
        if (entry->active == 0) {
            entry->value = first;
            entry->argument1 = second;
            entry->argument2 = third;
            queue->next = entry->next;
            entry->active = 1;
        }
        queue->elapsed = 0;
        return;
    }
    queue->elapsed = queue->elapsed + 1;
}

void fldSetMapRequestInterval(MapRequestState *queue, u16 interval) {
    queue->interval = interval;
}

/* Advance active ring nodes, retiring each node when it reaches the queue limit. */
INCLUDE_ASM(const s32, "game/code_0030E390", func_0030EF90);

/* Report normalized progress for each active request in the ring. */
INCLUDE_ASM(const s32, "game/code_0030E390", func_0030F038);

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
