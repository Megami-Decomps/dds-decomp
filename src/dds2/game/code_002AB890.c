#include "mnu.h"

extern s64 func_002ACF38(void);

extern s32 D_00435DD0;

extern s32 kwlnTaskGetUserValue();

extern s64 func_002C4038(s32, s32 *, u64, u64);

extern void func_002AAE80(s32);

extern void mnuCreateStaffImageSprite(s32);

extern void func_002AAC98(s32, s32, s32, s32, s32, s32);

extern void func_002BB0E8(s32, s32, s32, s32, s32);

extern void func_002AA7A0(s32, s32);

extern u8 D_003E7050[];

typedef struct MenuResourceSet {
    u8 pad00[8];
    u32 first;
    u32 second;
    u32 third;
    u32 fourth;
    u32 fifth;
    u8 pad1C[0x1C];
    s32 selection;
} MenuResourceSet;

typedef struct MenuResourceOwner {
    u8 pad00[0xAA48];
    MenuResourceSet *resources;
} MenuResourceOwner;

extern void func_002AB690(s32, s32, s32, s32, s32, s32, s32);

void func_002AB890(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    func_002AB690(arg0, arg1, arg2, 0, arg3, arg4, arg5);
}

void func_002AB8C0(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    func_002AB690(arg0, arg1, arg2, 0x200, arg3, arg4, arg5);
}

INCLUDE_ASM(const s32, "game/code_002AB890", func_002AB8F0);

void func_002ABCD0(MenuResourceOwner *object) {
    MenuResourceSet *resources;

    resources = object->resources;
    mnuDestroyWindowContainer(resources->first);
    mnuDestroyWindowContainer(resources->second);
}

INCLUDE_ASM(const s32, "game/code_002AB890", func_002ABD08);

INCLUDE_ASM(const s32, "game/code_002AB890", func_002ABD60);

void func_002ABEB0(MenuResourceOwner *object) {
    mnuDestroyWindowContainer(object->resources->third);
}

INCLUDE_ASM(const s32, "game/code_002AB890", func_002ABED8);

INCLUDE_ASM(const s32, "game/code_002AB890", func_002AC050);

INCLUDE_ASM(const s32, "game/code_002AB890", func_002AC408);

void func_002AC660(MenuResourceOwner *object) {
    mnuDestroyWindowContainer(object->resources->fourth);
}

INCLUDE_ASM(const s32, "game/code_002AB890", func_002AC688);

INCLUDE_ASM(const s32, "game/code_002AB890", func_002AC750);

INCLUDE_ASM(const s32, "game/code_002AB890", func_002AC8F0);

void func_002ACA98(MenuResourceOwner *object) {
    mnuDestroyWindowContainer(object->resources->fifth);
}

INCLUDE_ASM(const s32, "game/code_002AB890", func_002ACAC0);

void func_002ACB18(u32 arg0) {
    func_002A9460(2, arg0);
}

void func_002ACB38(void) {
}

INCLUDE_ASM(const s32, "game/code_002AB890", func_002ACB40);

INCLUDE_ASM(const s32, "game/code_002AB890", func_002ACBF8);

INCLUDE_ASM(const s32, "game/code_002AB890", func_002ACC50);

INCLUDE_ASM(const s32, "game/code_002AB890", func_002ACE58);

s64 func_002ACF00(s32 callback) {
    return menuSetHandler(kwlnTaskGetUserValue(), 2, callback);
}

INCLUDE_ASM(const s32, "game/code_002AB890", func_002ACF38);

/* Record the choice only while the resource is active; the follow-up runs regardless. */
void mnuApplyResourceSelection(s32 index, s32 context) {
    MenuResourceSet *resources;
    s64 resourceActive;

    resources = ((MenuResourceOwner *)context)->resources;
    resourceActive = func_002ACF38();
    if (resourceActive != 0) {
        *(u32 *)(*(s32 *)(*(s32 *)(resources->first + 0x18) + 0x1c) + 0x60) =
                  (u32)*(u8 *)(index + D_00435DD0 + 0x1340);
        resources->selection = index;
    }
    func_002C1B68(context + 0xaa50, 1);
}

u32 func_002AD0A8(void) {
    s32 context;

    context = kwlnTaskGetUserValue();
    func_002BAF50(((MenuResourceOwner *)context)->resources->first, context + 0xb10c);
    return 1;
}

u32 func_002AD0E8(void) {
    s32 context;

    context = kwlnTaskGetUserValue();
    func_002BAF50(*(u32 *)(context + 0x108), context + 0xb10c);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_002AB890", func_002AD118);

INCLUDE_ASM(const s32, "game/code_002AB890", func_002AD330);
