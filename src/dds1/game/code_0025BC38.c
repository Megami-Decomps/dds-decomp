#include "common.h"

extern s32 func_002CFEB8(u32);

void effDestroyResourceSlotSet(u32);

typedef struct DspListNode {
    u8 pad00[0x10];
    struct DspListNode *next; /* 0x10 */
} DspListNode;

typedef struct {
    u32 pad00[2];
    DspListNode *first; /* 0x08 */
} DspListHead;

typedef struct {
    s32 allocation;
    s32 tasks[10];
} MovieResourceGroup;

void func_0025BC38(MovieResourceGroup *resources) {
    s32 i;
    for (i = 0; i < 10; i++) {
        if (resources->tasks[i] != 0) {
            sdfDestroyTaskWork(resources->tasks[i]);
        }
    }
    func_002D0918(resources->allocation);
}

extern void func_002BF4E0(s32, s32, s32, u32, s32, s32, s32, s32);
extern s32 D_0036C698[];

void mnuDrawIconAlpha(s32 x, s32 y, s32 z, s32 alpha, s32 param) {
    func_002BF4E0(x << 4, y << 3, z, (u32)((f32)(alpha << 8) * 0.0078125f), 0, D_0036C698[0], 0x25, param);
}

typedef struct MovieCueNode {
    u8 pad00[8];
    s32 framesLeft;       /* 0x08 */
    s32 duration;         /* 0x0C */
    u8 pad10;
    s8 cueIndex;          /* 0x11 */
    u8 enabled;           /* 0x12 */
} MovieCueNode;

extern void func_0025BA20(s32, s32, u8 *, s32);
extern u8 *sdfListRemoveNode(s32, u8 *);

s32 mnuTickResourceGroup(s32 owner, s32 group) {
    u8 *list = *(u8 **)(group + 8);
    MovieCueNode *node;

    if (list == NULL) {
        sdfDestroyTaskWork(group);
        return 0;
    }
    do {
        node = *(MovieCueNode **)(list + 0x10);
        node->framesLeft = node->framesLeft - 1;
        /* Fire an enabled cue five frames before its node expires. */
        if (node->framesLeft == node->duration - 5 && node->enabled != 0) {
            func_0025BA20(owner, group, (u8 *)node, node->cueIndex);
        }
        if (node->framesLeft == 0) {
            list = sdfListRemoveNode(group, list);
        } else {
            list = *(u8 **)(list + 8);
        }
    } while (list != NULL);
    return group;
}

INCLUDE_ASM(const s32, "game/code_0025BC38", func_0025BDD0);

INCLUDE_ASM(const s32, "game/code_0025BC38", func_0025BF18);

typedef struct {
    u8 pad00[0x6C];
    u32 resourceHandle; /* 0x6C */
} MenuResourceWork;

u32 mnuRequestEffectResource(u32 ctx, u32 config) {
    MenuResourceWork *work = (MenuResourceWork *)func_002CFEB8(0x70);
    memset(work, 0, 0x70);
    effRequestResourceByMode(ctx, config, 0, (u32)&work->resourceHandle);
    return (u32)work;
}

typedef struct MenuListNode {
    u8 pad00[0x10];
    struct MenuListNode *next; /* 0x10 */
} MenuListNode;

typedef struct {
    u32 pad00[2];
    MenuListNode *first; /* 0x08 */
} MenuListHead;

u8 mnuHasEffectResourceHandle(MenuResourceWork *work) {
    return work->resourceHandle != 0;
}

void mnuReleaseEffectResource(MenuResourceWork *work) {
    effDestroyResourceSlotSet(work->resourceHandle);
    sdfReleaseChipBlock(work);
}

INCLUDE_ASM(const s32, "game/code_0025BC38", func_0025C0D8);

INCLUDE_ASM(const s32, "game/code_0025BC38", func_0025C1C8);

INCLUDE_ASM(const s32, "game/code_0025BC38", func_0025C278);

INCLUDE_ASM(const s32, "game/code_0025BC38", func_0025C350);

INCLUDE_ASM(const s32, "game/code_0025BC38", func_0025C418);

INCLUDE_ASM(const s32, "game/code_0025BC38", func_0025C4C8);

void mnuAdvanceLoopingFrame(s32 *frame) {
    s32 oldFrame;

    oldFrame = *frame;
    *frame = oldFrame + 1;
    if (0x3c < oldFrame + 1) {
        *frame = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_0025BC38", func_0025C588);

extern void func_00255FF8(s32 *, s32);

void mnuAdvanceGridSlotAnimation(s32 animationContext, s32 unusedGrid, u8 *slot) {
    s32 *counter = *(s32 **)(slot + 4);
    s32 value = *counter + 1;

    *counter = value;
    if ((f32)value > 60.0f) {
        *counter = 0;
    }
    func_00255FF8(counter, animationContext);
}

typedef struct MenuAnimationSlot {
    s32 unk00;
    s32 counterAddress; /* 0x04: address of the frame counter */
} MenuAnimationSlot;

void mnuAdvanceActiveGridSlotAnimations(s32 animationContext, s32 owner) {
    u8 *grid = *(u8 **)(owner + 0x484);
    s32 row;
    s32 col;

    for (row = 0; row < 0x11; row++) {
        MenuAnimationSlot *slot = (MenuAnimationSlot *)(*(s32 *)(grid + 4) + row * *(s32 *)(grid + 0x14) * 8);
        for (col = 0; col < 15; col++) {
            if (slot[col].counterAddress != 0) {
                mnuAdvanceGridSlotAnimation(animationContext, (s32)grid, (u8 *)&slot[col]);
            }
        }
    }
}

INCLUDE_ASM(const s32, "game/code_0025BC38", func_0025C8D0);

INCLUDE_ASM(const s32, "game/code_0025BC38", func_0025CA50);

INCLUDE_ASM(const s32, "game/code_0025BC38", func_0025CFA0);

INCLUDE_ASM(const s32, "game/code_0025BC38", func_0025D100);

extern void mnuAdvanceActiveGridSlotAnimations(s32, s32);

void mnuAdvanceDisplayGridAndLoopingFrame(s32 arg0, s32 arg1) {
    mnuAdvanceActiveGridSlotAnimations(arg1, arg0);
    mnuAdvanceLoopingFrame((s32 *)(arg0 + 0x490));
}

INCLUDE_ASM(const s32, "game/code_0025BC38", func_0025D2F8);

INCLUDE_ASM(const s32, "game/code_0025BC38", func_0025D628);

typedef struct Bytes7 {
    s8 b[7];
} Bytes7;

extern Bytes7 D_003BC4E8[];
extern void func_0024E260(s32, s32, s32, s32, s32, s32);

void mnuDrawMantraCostIcon(s32 x, s32 y, s32 z, s32 entry, s32 arg4, s32 arg5) {
    Bytes7 table = D_003BC4E8[0];

    func_0024E260(x - 0x19, y + 0x5C, z, arg4, table.b[*(u16 *)(entry + 4)], arg5);
}

INCLUDE_ASM(const s32, "game/code_0025BC38", func_0025D7F8);

MenuListNode *mnuAllocateMenuListNode(void) {
    MenuListNode *node = (MenuListNode *)func_002CFEB8(0x14);

    memset(node, 0, 0x14);
    return node;
}

DspListNode *mnuAppendNodeToDisplayList(DspListHead *head) {
    DspListNode *node = head->first;
    if (node == NULL) {
        node = mnuAllocateMenuListNode();
        head->first = node;
    } else {
        while (node->next != NULL) {
            node = node->next;
        }
        node->next = mnuAllocateMenuListNode();
        node = node->next;
    }
    return node;
}

MenuListNode *mnuFreeMenuListNodeAndGetNext(MenuListNode *node) {
    MenuListNode *next;

    next = node->next;
    sdfReleaseChipBlock();
    return next;
}

void mnuReleaseListNodes(MenuListHead *head) {
    MenuListNode *node = head->first;

    while (node != NULL) {
        node = mnuFreeMenuListNodeAndGetNext(node);
    }
}

INCLUDE_ASM(const s32, "game/code_0025BC38", func_0025DBB0);

extern s32 func_0025DBB0(s32);
extern s32 func_0025D7F8(MenuListNode *, s32, s32);

s32 mnuAdvanceDisplayList(s32 arg0, s32 arg1, s32 arg2) {
    s32 *counter = (s32 *)arg0;
    MenuListHead *head = (MenuListHead *)arg0;
    MenuListNode *node = head->first;
    s32 index = 0;

    if (func_0025DBB0(*counter) != 0) {
        *counter = 0;
    } else {
        *counter = *counter + 1;
    }
    if (node == NULL) {
        return 1;
    }
    do {
        s32 hit = func_0025D7F8(node, index, arg2);

        index++;
        if (hit != 0) {
            node = mnuFreeMenuListNodeAndGetNext(node);
            head->first = node;
        } else {
            node = node->next;
        }
    } while (node != NULL);
    return 0;
}

extern s32 mnuAdvanceDisplayList(s32, s32, s32);
extern void mnuDrawMantraCostIcon(s32, s32, s32, s32, s32, s32);

void mnuDrawMantraCostAfterListAdvance(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    if (mnuAdvanceDisplayList(arg1, arg2, arg3) != 0) {
        mnuDrawMantraCostIcon(0, 0, 0, arg0, arg2, arg3);
    }
}

extern u16 D_0036C6D0[];
extern s32 mdlFlagTest(s32);

void mnuCollectFlagArray(u8 *flags) {
    u32 i;

    for (i = 0; i < 0x29; i++, flags++) {
        if (mdlFlagTest(D_0036C6D0[i]) != 0) {
            *flags = 1;
        }
    }
}

extern u16 D_0036C6D0[];
extern void mdlFlagSet(s32);

void mnuApplyFlagArray(u8 *flags) {
    u32 i;

    for (i = 0; i < 0x29; i++) {
        if (*flags++ != 0) {
            mdlFlagSet(D_0036C6D0[i]);
        }
    }
}

INCLUDE_RODATA(const s32, "game/code_0025BC38", D_003AFA00);

INCLUDE_RODATA(const s32, "game/code_0025BC38", D_003AFA18);

INCLUDE_SDATA(const s32, "game/code_0025BC38", D_003BC4D0);

INCLUDE_SDATA(const s32, "game/code_0025BC38", D_003BC4D8);

INCLUDE_SDATA(const s32, "game/code_0025BC38", D_003BC4E0);

INCLUDE_SDATA(const s32, "game/code_0025BC38", D_003BC4E8);

