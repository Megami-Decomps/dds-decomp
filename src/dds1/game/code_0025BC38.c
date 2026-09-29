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

extern void func_002BF4E0(s32, s32, s32, u32, s32, s32, s32, s32);
extern s32 D_0036C698[];

void func_0025BCA0(s32 x, s32 y, s32 z, s32 alpha, s32 param) {
    func_002BF4E0(x << 4, y << 3, z, (u32)((f32)(alpha << 8) * 0.0078125f), 0, D_0036C698[0], 0x25, param);
}

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

extern void func_00255FF8(s32 *, s32);

void func_0025C7E0(s32 arg0, s32 arg1, u8 *arg2) {
    s32 *counter = *(s32 **)(arg2 + 4);
    s32 value = *counter + 1;

    *counter = value;
    if ((f32)value > 60.0f) {
        *counter = 0;
    }
    func_00255FF8(counter, arg0);
}

typedef struct EntrySlot8 {
    s32 a;
    s32 b;
} EntrySlot8;

void func_0025C830(s32 arg0, s32 arg1) {
    u8 *grid = *(u8 **)(arg1 + 0x484);
    s32 row;
    s32 col;

    for (row = 0; row < 0x11; row++) {
        EntrySlot8 *slot = (EntrySlot8 *)(*(s32 *)(grid + 4) + row * *(s32 *)(grid + 0x14) * 8);
        for (col = 0; col < 15; col++) {
            if (slot[col].b != 0) {
                func_0025C7E0(arg0, (s32)grid, (u8 *)&slot[col]);
            }
        }
    }
}

INCLUDE_ASM(const s32, "game/code_0025BC38", func_0025C8D0);

INCLUDE_ASM(const s32, "game/code_0025BC38", func_0025CA50);

INCLUDE_ASM(const s32, "game/code_0025BC38", func_0025CFA0);

INCLUDE_ASM(const s32, "game/code_0025BC38", func_0025D100);

extern void func_0025C830(s32, s32);

void func_0025D2C0(s32 arg0, s32 arg1) {
    func_0025C830(arg1, arg0);
    func_0025C568((s32 *)(arg0 + 0x490));
}

INCLUDE_ASM(const s32, "game/code_0025BC38", func_0025D2F8);

INCLUDE_ASM(const s32, "game/code_0025BC38", func_0025D628);

typedef struct Bytes7 {
    s8 b[7];
} Bytes7;

extern Bytes7 D_003BC4E8[];
extern void func_0024E260(s32, s32, s32, s32, s32, s32);

void func_0025D798(s32 x, s32 y, s32 z, s32 entry, s32 arg4, s32 arg5) {
    Bytes7 table = D_003BC4E8[0];

    func_0024E260(x - 0x19, y + 0x5C, z, arg4, table.b[*(u16 *)(entry + 4)], arg5);
}

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

extern s32 func_0025DCC8(s32, s32, s32);
extern void func_0025D798(s32, s32, s32, s32, s32, s32);

void func_0025DD80(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    if (func_0025DCC8(arg1, arg2, arg3) != 0) {
        func_0025D798(0, 0, 0, arg0, arg2, arg3);
    }
}

extern u16 D_0036C6D0[];
extern s32 mdlFlagTest(s32);

void func_0025DDF0(u8 *flags) {
    u32 i;

    for (i = 0; i < 0x29; i++, flags++) {
        if (mdlFlagTest(D_0036C6D0[i]) != 0) {
            *flags = 1;
        }
    }
}

extern u16 D_0036C6D0[];
extern void mdlFlagSet(s32);

void func_0025DE60(u8 *flags) {
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
