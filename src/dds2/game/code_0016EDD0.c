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

INCLUDE_ASM(const s32, "game/code_0016EDD0", func_0016EFA8);

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

INCLUDE_ASM(const s32, "game/code_0016EDD0", func_00171CE0);
