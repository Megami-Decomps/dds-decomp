#include "common.h"
#include "pcp_vu0.h"

extern u64 effParamTableGetBlock(u64, u64);

INCLUDE_ASM(const s32, "game/code_00167178", func_00167178);

INCLUDE_ASM(const s32, "game/code_00167178", func_00167350);

u32 func_001673C0(u32 arg0) {
    return arg0;
}

void func_001673C8(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x120) = arg1;
}

INCLUDE_ASM(const s32, "game/code_00167178", func_001673D0);

INCLUDE_ASM(const s32, "game/code_00167178", func_00167A78);

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
    u32 allocation;     /* 0x24: released by func_002D0918 */
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
extern void func_002D0918(u32 allocation);

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
    func_002D0918(group->allocation);
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
    func_002D0918(work->allocation);
}

/* Restore the neutral gray color and default effect mode before rendering. */
void effInitializeColorState(EffectColorState *state) {
    state->mode = 3;
    state->color = 0x80808080;
    state->valueC = 0;
}

INCLUDE_ASM(const s32, "game/code_00167178", func_00169B90);

INCLUDE_ASM(const s32, "game/code_00167178", func_00169D78);

INCLUDE_ASM(const s32, "game/code_00167178", func_00169E10);

INCLUDE_ASM(const s32, "game/code_00167178", func_0016A088);
