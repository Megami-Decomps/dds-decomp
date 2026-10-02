#include "common.h"
#include "pcp_vu0.h"

extern u64 effParamTableGetBlock(u64, u64);

INCLUDE_ASM(const s32, "game/code_00167178", func_00167178);

typedef struct EffThunderFragmentParams {
    f32 start[4];
    f32 end[4];
    u16 systemParam;
    u8 pad22[2];
    u32 fragmentCount;
    u8 pad28[8];
    u32 fragmentFirstRange;
    u32 fragmentSecondRange;
    u16 halfLife;
    u8 pad3A[6];
    u32 arg40;
    u8 pad44[4];
    u32 arg48;
    u8 pad4C[4];
    u32 arg50;
} EffThunderFragmentParams;

typedef struct ParCell {
    u128 *history;
    void *vertices;
    s32 vertexCount;
    s32 unk0C;
    u32 color;
} ParCell;

typedef struct EffThunderParSystem {
    u8 pad00[0x14];
    ParCell *cells;
} EffThunderParSystem;

typedef struct EffThunderFragmentWork {
    EffThunderFragmentParams head;
    void *fragments;
    u32 color;
    union {
        u32 updateCount;
        void *secondarySystem;
    } state;
    EffThunderParSystem *system;
    u32 handle;
} EffThunderFragmentWork;

typedef struct EffThunderGroup {
    f32 points[10][4];
    s32 count;
    EffThunderFragmentParams params;
    u32 handles[10];
    u32 color;
} EffThunderGroup;

extern s32 effMultiplyPackedColors(s32, s32);
extern void func_00165758(void *work, s32 index);
extern void func_001673D0(void *work, s32 index, void *seed);
extern void parPrependCellNode(void *system);

extern void effPCPThunderFree3(u32 handle);
extern void sdfReleaseChipBlock(void *block);

void effThunderGroupRelease(EffThunderGroup *group) {
    s32 i;

    for (i = 0; i < group->count - 1; i++) {
        effPCPThunderFree3(group->handles[i]);
    }
    sdfReleaseChipBlock(group);
}

u32 func_001673C0(u32 arg0) {
    return arg0;
}

/* Sets the blend colour word of a resource handle (see the blur caller in
   code_00185E18.c). */
void effSetResourceBlendColor(u32 handle, u32 color) {
    *(u32 *)(handle + 0x120) = color;
}

INCLUDE_ASM(const s32, "game/code_00167178", func_001673D0);

void effThunderUpdateChainSegments(EffThunderGroup *group) {
    s32 i = 0;
    s32 segmentCount = group->count - 1;
    s32 j;
    s32 fragmentCount;
    u32 color;
    f32 (*points)[4] = group->points;
    EffThunderFragmentWork *work;
    EffThunderFragmentWork *previous;
    ParCell *cell;

    for (; i < segmentCount; i++) {
        work = (EffThunderFragmentWork *)group->handles[i];
        PCP_COPY_VECTOR(work->head.start, points[0]);
        PCP_COPY_VECTOR(work->head.end, points[1]);
        points++;
        fragmentCount = work->head.fragmentCount;
        color = effMultiplyPackedColors(group->color, work->color);
        cell = work->system->cells;
        for (j = 0; j < fragmentCount; j++) {
            if (i > 0) {
                previous = (EffThunderFragmentWork *)group->handles[i - 1];
                func_001673D0(work, j,
                    previous->system->cells[j].history + previous->system->cells[j].vertexCount - 5);
            } else {
                func_00165758(work, j);
            }
            cell[j].color = color;
        }
        work->state.updateCount++;
        parPrependCellNode(work->system);
    }
}

INCLUDE_ASM(const s32, "game/code_00167178", func_00167BF8);

/* Forward the first two parameter-table blocks as one effect-handler pair. */
void effApplyParamBlockPair(u64 paramTable) {
    u64 firstBlock;
    u64 secondBlock;

    firstBlock = effParamTableGetBlock(paramTable, 0);
    secondBlock = effParamTableGetBlock(paramTable, 1);
    func_00167BF8(firstBlock, secondBlock);
}

INCLUDE_ASM(const s32, "game/code_00167178", func_00167EC0);

typedef struct EffFragmentResources {
    u8 pad00[0x20];
    u32 resourceHandle; /* 0x20: released by sdfQueueAssetRelease */
    u32 allocation;     /* 0x24: released by sdfReleaseResourceAllocation */
} EffFragmentResources;

typedef struct EffGroupSlot {
    u8 pad00[0x18];
    EffFragmentResources *resources; /* 0x18 */
    u8 pad1C[0x60];
    void *node;      /* 0x7C: passed to effEventReleaseNode */
} EffGroupSlot; /* 0x80 */

typedef struct EffGroup {
    u8 pad00[0x20];
    u32 count;          /* 0x20 */
    u8 pad24[0x2C];
    EffGroupSlot *slots; /* 0x50 */
    u8 pad54[4];
    u32 handle58;       /* 0x58 */
    u8 hasHandle58;     /* 0x5C */
    u8 pad5D[3];
    u32 allocation;     /* 0x60 */
} EffGroup;

extern void effReleaseEffectResources(EffFragmentResources *work);
extern void effEventReleaseNode(void *node);
extern void func_00190118(u32 handle);
extern void sdfReleaseResourceAllocation(u32 allocation);

/* Release every slot's effect resources and event node, then the optional handle and the group allocation. */
void effReleaseGroupSlotsAndResources(EffGroup *group) {
    u32 i = 0;
    u32 count = group->count;
    EffGroupSlot *slot = group->slots;

    if (count != 0) {
        do {
            i++;
            effReleaseEffectResources(slot->resources);
            effEventReleaseNode(slot->node);
            slot++;
        } while (i < count);
    }
    if (group->hasHandle58 != 0) {
        func_00190118(group->handle58);
    }
    sdfReleaseResourceAllocation(group->allocation);
}

INCLUDE_ASM(const s32, "game/code_00167178", func_001681C0);

void func_00169928(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_00169938(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x54) = arg1;
}

INCLUDE_ASM(const s32, "game/code_00167178", func_00169940);

/* The packed color state is identical to the sequel's effect state layout. */
typedef struct EffectColorState {
    u32 color;    /* 0x00 */
    u8 pad04[8];
    u32 valueC;   /* 0x0C */
    u32 mode;     /* 0x10 */
} EffectColorState;

void effReleaseEffectResources(EffFragmentResources *work) {
    sdfQueueAssetRelease(work->resourceHandle);
    sdfReleaseResourceAllocation(work->allocation);
}

/* Restore the neutral gray color and default effect mode before rendering. */
void effInitializeColorState(EffectColorState *state) {
    state->mode = 3;
    state->color = 0x80808080;
    state->valueC = 0;
}

typedef struct EffThunderPointHistory {
    u8 pad00[8];
    s32 count; /* 0x08: history slots */
    s32 activePointCount; /* 0x0C: populated point slots */
    s32 position; /* 0x10: next point index */
    u8 pad14[4];
    u128 *points; /* 0x18 */
    u32 *colors; /* 0x1C */
} EffThunderPointHistory;

extern u32 effBlendColor(u32 colorA, u32 colorB, f32 t);

void func_00169B90(EffThunderPointHistory *history, u32 *gradientColors) {
    f32 t = 0.0f;
    u32 count = history->count / 3;
    u32 alphaCount = count >> 1;
    f32 step = 1.0f / count;
    f32 alpha = 0.0f;
    f32 alphaStep = 1.0f / alphaCount;
    u32 *colors = history->colors;
    u32 innerStart = gradientColors[0] & 0xFFFFFF;
    u32 innerEnd = gradientColors[2] & 0xFFFFFF;
    u32 outerStart = gradientColors[1];
    u32 outerEnd = gradientColors[3];
    u32 i = 0;

    if (count != 0) {
        do {
            u32 alphaMask = (u32)(alpha * 2147483648.0f) & 0xFF000000;

            colors[0] = effBlendColor(outerEnd, outerStart, t);
            colors[1] = effBlendColor(innerEnd, innerStart, t) | alphaMask;
            colors[2] = colors[0];
            t += step;
            if (i < alphaCount) {
                alpha += alphaStep;
            }
            i++;
            colors += 3;
        } while (i < count);
    }
}

void func_00169D78(EffThunderPointHistory *history, u128 *source) {
    s32 position = history->position;
    u128 *points = history->points;
    s32 count;

    PCP_COPY_VECTOR(&points[position], source);
    PCP_COPY_VECTOR(&points[position + 1], source + 1);
    PCP_COPY_VECTOR(&points[position + 2], source + 2);
    position += 3;
    count = history->count;
    history->position = position;
    if (position == count) {
        PCP_COPY_VECTOR(&points[0], source);
        PCP_COPY_VECTOR(&points[1], source + 1);
        PCP_COPY_VECTOR(&points[2], source + 2);
        history->position = 3;
    }
    if (history->activePointCount < count - 3) {
        history->activePointCount += 3;
    }
}

INCLUDE_ASM(const s32, "game/code_00167178", func_00169E10);

typedef struct EffFlashRecordPart {
    u32 unk00;
    s32 age; /* 0x04 */
    u8 pad08[8];
} EffFlashRecordPart; /* 0x10 */

typedef struct EffFlashRecordHandle {
    u8 pad00[0x50];
    u32 unk50;
} EffFlashRecordHandle;

typedef struct EffFlashRecordWork {
    u8 pad00[0x10];
    u32 particleCount;    /* 0x10 */
    u8 pad14[0x18];
    u32 unk2C;            /* 0x2C */
    EffFlashRecordPart *parts; /* 0x30 */
    u32 updateCount;      /* 0x34 */
    u32 colorParam;       /* 0x38 */
    f32 renderScale;      /* 0x3C */
    u32 ownedBuffer;      /* 0x40 */
    u32 resourceHandle;   /* 0x44 */
} EffFlashRecordWork; /* 0x48 */

extern u32 sdfAllocGeneralBlock(s32 size);
extern u8 *sdfResourceRetainAddress(u32 handle);
extern void *memcpy(void *dst, const void *src, u32 size);
extern s32 effRecordPoolCreateTriad();

/* Clone the 0x30-byte parameter block, create the record pool and clear every particle's age. */
EffFlashRecordWork *effFlashTrianglePulseCreate(src)
    EffFlashRecordWork *src;
{
    u32 handle = sdfAllocGeneralBlock(src->particleCount * sizeof(EffFlashRecordPart) + sizeof(EffFlashRecordWork));
    EffFlashRecordWork *work = (EffFlashRecordWork *)sdfResourceRetainAddress(handle);
    EffFlashRecordHandle *record;
    u32 i;

    memcpy(work, src, 0x30);
    work->parts = (EffFlashRecordPart *)(work + 1);
    work->colorParam = 0x80808080;
    work->ownedBuffer = handle;
    work->renderScale = 1.0f;
    work->updateCount = 0;
    record = (EffFlashRecordHandle *)effRecordPoolCreateTriad(work->particleCount);
    work->resourceHandle = (u32)record;
    record->unk50 = work->unk2C;
    for (i = 0; i < work->particleCount; i++) {
        work->parts[i].age = 0;
    }
    return work;
}
