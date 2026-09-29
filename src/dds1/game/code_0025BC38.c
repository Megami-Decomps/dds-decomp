#include "common.h"

extern s32 func_002CFEB8(u32);

void func_002BDD60(u32);

INCLUDE_ASM(const s32, "game/code_0025BC38", func_0025BC38);

INCLUDE_ASM(const s32, "game/code_0025BC38", func_0025BCA0);

INCLUDE_ASM(const s32, "game/code_0025BC38", func_0025BD18);

INCLUDE_ASM(const s32, "game/code_0025BC38", func_0025BDD0);

INCLUDE_ASM(const s32, "game/code_0025BC38", func_0025BF18);

u32 func_0025C030(u32 ctx, u32 config) {
    u32 data = func_002CFEB8(0x70);
    memset((void *)data, 0, 0x70);
    effRequestResourceByMode(ctx, config, 0, data + 0x6c);
    return data;
}

typedef struct MenuListNode {
    u8 pad00[0x10];
    struct MenuListNode *next; /* 0x10 */
} MenuListNode;

typedef struct {
    u32 pad00[2];
    MenuListNode *first; /* 0x08 */
} MenuListHead;

u8 func_0025C098(s32 arg0) {
    return *(s32 *)(arg0 + 0x6c) != 0;
}

void func_0025C0A8(u32 obj) {
    func_002BDD60(*(u32 *)(obj + 0x6c));
    func_002CFF98(obj);
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

INCLUDE_ASM(const s32, "game/code_0025BC38", func_0025DAD0);

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
