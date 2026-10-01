#include "common.h"
#include "pcp_vu0.h"

extern u64 effParamTableGetBlock(u64, u64);
typedef struct EffectResourceWork {
    u8 pad00[0x20];
    u32 resourceHandle;
    u32 allocation;
} EffectResourceWork;

typedef struct EffectColorState {
    u32 color;
    u8 pad04[8];
    u32 valueC;
    u32 mode;
} EffectColorState;


INCLUDE_ASM(const s32, "game/code_0016EDD0", func_0016EDD0);

typedef struct EffThunderGroup {
    u8 pad00[0xA0];
    s32 count;         /* 0xA0 */
    u8 pad_A4[0x54];
    u32 handles[1];    /* 0xF8 */
} EffThunderGroup;

extern void effPCPThunderFree3(u32 handle);
extern void sdfReleaseChipBlock(void *block);

void effThunderGroupRelease(EffThunderGroup *group) {
    s32 i;

    for (i = 0; i < group->count - 1; i++) {
        effPCPThunderFree3(group->handles[i]);
    }
    sdfReleaseChipBlock(group);
}

u32 func_0016F018(u32 arg0) {
    return arg0;
}

void func_0016F020(s32 work, u32 value) {
    *(u32 *)(work + 0x120) = value;
}

INCLUDE_ASM(const s32, "game/code_0016EDD0", func_0016F028);

INCLUDE_ASM(const s32, "game/code_0016EDD0", func_0016F6D0);

INCLUDE_ASM(const s32, "game/code_0016EDD0", func_0016F850);

/* Forward the first two parameter-table blocks as one effect-handler pair. */
void effApplyParamBlockPair(u64 table) {
    u64 firstBlock;
    u64 secondBlock;

    firstBlock = effParamTableGetBlock(table, 0);
    secondBlock = effParamTableGetBlock(table, 1);
    func_0016F850(firstBlock, secondBlock);
}

INCLUDE_ASM(const s32, "game/code_0016EDD0", func_0016FB18);

typedef struct EffGroupSlot {
    u8 pad00[0x18];
    EffectResourceWork *resources; /* 0x18 */
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

extern void effReleaseEffectResources(EffectResourceWork *work);
extern void effEventReleaseNode(void *node);
extern void func_00197D50(u32 handle);
extern void func_003297C8(u32 allocation);

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
        func_00197D50(group->handle58);
    }
    func_003297C8(group->allocation);
}

INCLUDE_ASM(const s32, "game/code_0016EDD0", func_0016FE18);

void effCopyResourceGroupVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_00171590(s32 work, u32 value) {
    *(u32 *)(work + 0x54) = value;
}

INCLUDE_ASM(const s32, "game/code_0016EDD0", func_00171598);

void effReleaseEffectResources(EffectResourceWork *work) {
    sdfQueueAssetRelease(work->resourceHandle);
    func_003297C8(work->allocation);
}

/* Restore the neutral gray color and default effect mode before rendering. */
void effInitializeColorState(EffectColorState *state) {
    state->mode = 3;
    state->color = 0x80808080;
    state->valueC = 0;
}

INCLUDE_ASM(const s32, "game/code_0016EDD0", func_001717E8);

INCLUDE_ASM(const s32, "game/code_0016EDD0", func_001719D0);

INCLUDE_ASM(const s32, "game/code_0016EDD0", func_00171A68);

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

extern u32 func_003292A8(s32 size);
extern u8 *sdfResourceRetainAddress(u32 handle);
extern void *memcpy(void *dst, const void *src, u32 size);
extern s32 effRecordPoolCreateTriad();

/* Clone the 0x30-byte parameter block, create the record pool and clear every particle's age. */
EffFlashRecordWork *effFlashRecordCreate(src)
    EffFlashRecordWork *src;
{
    u32 handle = func_003292A8(src->particleCount * sizeof(EffFlashRecordPart) + sizeof(EffFlashRecordWork));
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
