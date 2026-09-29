#include "common.h"

extern s32 func_002CFEB8(u32);

void func_002BDD60(u32);

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

INCLUDE_ASM(const s32, "game/code_0025BC38", func_0025BCA0);

INCLUDE_ASM(const s32, "game/code_0025BC38", func_0025BD18);

INCLUDE_ASM(const s32, "game/code_0025BC38", func_0025BDD0);

INCLUDE_ASM(const s32, "game/code_0025BC38", func_0025BF18);

typedef struct {
    u8 pad00[0x6C];
    u32 resourceHandle; /* 0x6C */
} MenuResourceWork;

u32 func_0025C030(u32 ctx, u32 config) {
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

u8 func_0025C098(MenuResourceWork *work) {
    return work->resourceHandle != 0;
}

void func_0025C0A8(MenuResourceWork *work) {
    func_002BDD60(work->resourceHandle);
    func_002CFF98(work);
}

INCLUDE_ASM(const s32, "game/code_0025BC38", func_0025C0D8);

INCLUDE_ASM(const s32, "game/code_0025BC38", func_0025C1C8);

INCLUDE_ASM(const s32, "game/code_0025BC38", func_0025C278);

INCLUDE_ASM(const s32, "game/code_0025BC38", func_0025C350);

INCLUDE_ASM(const s32, "game/code_0025BC38", func_0025C418);

INCLUDE_ASM(const s32, "game/code_0025BC38", func_0025C4C8);

void func_0025C568(s32 *frame) {
    s32 oldFrame;

    oldFrame = *frame;
    *frame = oldFrame + 1;
    if (0x3c < oldFrame + 1) {
        *frame = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_0025BC38", func_0025C588);

INCLUDE_ASM(const s32, "game/code_0025BC38", func_0025C7E0);

INCLUDE_ASM(const s32, "game/code_0025BC38", func_0025C830);

INCLUDE_ASM(const s32, "game/code_0025BC38", func_0025C8D0);

INCLUDE_ASM(const s32, "game/code_0025BC38", func_0025CA50);

INCLUDE_ASM(const s32, "game/code_0025BC38", func_0025CFA0);

INCLUDE_ASM(const s32, "game/code_0025BC38", func_0025D100);

INCLUDE_ASM(const s32, "game/code_0025BC38", func_0025D2C0);

INCLUDE_ASM(const s32, "game/code_0025BC38", func_0025D2F8);

INCLUDE_ASM(const s32, "game/code_0025BC38", func_0025D628);

INCLUDE_ASM(const s32, "game/code_0025BC38", func_0025D798);

INCLUDE_ASM(const s32, "game/code_0025BC38", func_0025D7F8);

MenuListNode *mnuAllocateMenuListNode(void) {
    MenuListNode *node = (MenuListNode *)func_002CFEB8(0x14);

    memset(node, 0, 0x14);
    return node;
}

DspListNode *func_0025DAD0(DspListHead *head) {
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

MenuListNode *func_0025DB58(MenuListNode *node) {
    MenuListNode *next;

    next = node->next;
    func_002CFF98();
    return next;
}

void mnuReleaseListNodes(MenuListHead *head) {
    MenuListNode *node = head->first;

    while (node != NULL) {
        node = func_0025DB58(node);
    }
}

INCLUDE_ASM(const s32, "game/code_0025BC38", func_0025DBB0);

INCLUDE_ASM(const s32, "game/code_0025BC38", func_0025DCC8);

INCLUDE_ASM(const s32, "game/code_0025BC38", func_0025DD80);

INCLUDE_ASM(const s32, "game/code_0025BC38", func_0025DDF0);

INCLUDE_ASM(const s32, "game/code_0025BC38", func_0025DE60);

INCLUDE_RODATA(const s32, "game/code_0025BC38", D_003AFA00);

INCLUDE_RODATA(const s32, "game/code_0025BC38", D_003AFA18);

INCLUDE_SDATA(const s32, "game/code_0025BC38", D_003BC4D0);

INCLUDE_SDATA(const s32, "game/code_0025BC38", D_003BC4D8);

INCLUDE_SDATA(const s32, "game/code_0025BC38", D_003BC4E0);

INCLUDE_SDATA(const s32, "game/code_0025BC38", D_003BC4E8);
