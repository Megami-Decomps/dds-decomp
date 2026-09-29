#include "common.h"

extern s32 func_00101958();

extern s64 func_002C4038(s32, s32 *, u64, u64);

extern void func_0026C900(void);
extern void func_002AAE80(s32);
extern void mnuCreateStaffImageSprite(s32);
extern void func_002AAC98(s32, s32, s32, s32, s32, s32);
extern void func_002BB0E8(s32, s32, s32, s32, s32);
extern void func_002AA7A0(s32, s32);
extern u8 D_003E7050[];
extern void func_002BAF50(s32, s32);
extern void func_002ABEB0(s32);
extern void func_002AC660(s32);
extern void func_002ACA98(s32);
extern void func_002B2C88(s32, s32, s32, s32);
extern void func_002BD2E0(s32);
extern void mnuClearEntries(s32);
extern void mnuDestroyPanelGroup(s32);
extern void func_002C1050(s32);
extern void func_002C1B68(s32, s32);
extern void func_002B2860(s32);
extern void func_002C2AA8(s32, s32);

typedef struct MenuSceneConfig {
    u8 pad00[0x10];
    s32 entries[5];
} MenuSceneConfig;

INCLUDE_ASM(const s32, "game/code_002AD3B8", func_002AD3B8);

/* Ask the menu state machine to handle a new request after clearing stale state. */
void func_002AD4C0(s32 request) {
    s32 context = func_00101958();

    func_0026C900();
    func_002C4038(context + 8, context + 0x54, 2, request);
}

u32 func_002AD508(void) {
    return 1;
}

u32 func_002AD510(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_002AD3B8", func_002AD518);

/* Set up a staff image and its associated menu resources before entering the state. */
s64 func_002AD618(s32 callback) {
    s32 context = func_00101958();
    func_002AAE80(callback);
    mnuCreateStaffImageSprite(7);
    func_002AAC98(0,
        *(s32 *)(*(s32 *)(*(s32 *)(*(s32 *)(context + 0x108) + 0x18) + 0x1c) + 0x60),
        (s32)D_003E7050, context, 1, 0x53);
    func_002BB0E8(0x1e0, 0x350, 0, context + 0xb10c, 0x53);
    func_002AA7A0(0, *(s32 *)(context + 0x60));
    return func_002C4038(context + 8, (s32 *)(context + 0x54), 1, callback);
}

void func_002AD6C0(s32 input) {
    s32 context = func_00101958();
    func_002C4038(context + 8, context + 0x54, 2, input);
}

u32 func_002AD6F8(void) {
    return 1;
}

u32 func_002AD700(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_002AD3B8", func_002AD708);

/* The three staff image states share the same setup, but select different images. */
s64 func_002AD808(s32 callback) {
    s32 context = func_00101958();
    func_002AAE80(callback);
    mnuCreateStaffImageSprite(9);
    func_002AAC98(0,
        *(s32 *)(*(s32 *)(*(s32 *)(*(s32 *)(context + 0x108) + 0x18) + 0x1c) + 0x60),
        (s32)D_003E7050, context, 1, 0x53);
    func_002BB0E8(0x1e0, 0x350, 0, context + 0xb10c, 0x53);
    func_002AA7A0(0, *(s32 *)(context + 0x60));
    return func_002C4038(context + 8, (s32 *)(context + 0x54), 1, callback);
}

s64 func_002AD8B0(s32 callback) {
    s32 context = func_00101958();
    return func_002C4038(context + 8, (s32 *)(context + 0x54), 2, callback);
}

u32 func_002AD8E8(void) {
    return 1;
}

u32 func_002AD8F0(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_002AD3B8", func_002AD8F8);

s64 func_002AD9F8(s32 callback) {
    s32 context = func_00101958();
    func_002AAE80(callback);
    mnuCreateStaffImageSprite(11);
    func_002AAC98(0,
        *(s32 *)(*(s32 *)(*(s32 *)(*(s32 *)(context + 0x108) + 0x18) + 0x1c) + 0x60),
        (s32)D_003E7050, context, 1, 0x53);
    func_002BB0E8(0x1e0, 0x350, 0, context + 0xb10c, 0x53);
    func_002AA7A0(0, *(s32 *)(context + 0x60));
    return func_002C4038(context + 8, (s32 *)(context + 0x54), 1, callback);
}

s64 func_002ADAA0(s32 callback) {
    s32 context = func_00101958();
    return func_002C4038(context + 8, (s32 *)(context + 0x54), 2, callback);
}

u32 func_002ADAD8(void) {
    s32 context;

    context = func_00101958();
    func_002BAF50(*(u32 *)(*(s32 *)(context + 0xaa48) + 0xc), context + 0xb10c);
    return 1;
}

u32 func_002ADB18(void) {
    s32 context;

    context = func_00101958();
    func_002BAF50(*(u32 *)(context + 0x108), context + 0xb10c);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_002AD3B8", func_002ADB48);

INCLUDE_ASM(const s32, "game/code_002AD3B8", func_002ADC70);

s64 func_002ADD68(s32 callback) {
    s32 context = func_00101958();
    return func_002C4038(context + 8, (s32 *)(context + 0x54), 2, callback);
}

INCLUDE_ASM(const s32, "game/code_002AD3B8", func_002ADDA0);

INCLUDE_ASM(const s32, "game/code_002AD3B8", func_002ADF90);

s64 func_002AE078(s32 callback) {
    s32 context = func_00101958();
    return func_002C4038(context + 8, (s32 *)(context + 0x54), 2, callback);
}

INCLUDE_ASM(const s32, "game/code_002AD3B8", func_002AE0B0);

/* Release the current entry list, its panel group and its auxiliary resource. */
s32 func_002AE1F0(void) {
    s32 context = func_00101958();
    s32 entryList = context + 0x284;
    func_002BAF50(*(s32 *)(context + 0x108), context + 0xb10c);
    func_002ABEB0(context);
    func_002B2C88(entryList, 0, 0, 0);
    func_002BD2E0(entryList);
    mnuClearEntries(entryList);
    if (*(s32 *)(context + 0xaa34) != 0) {
        mnuDestroyPanelGroup(*(s32 *)(context + 0xaa34));
        *(s32 *)(context + 0xaa34) = 0;
    }
    if (*(s32 *)(context + 0xaa38) != 0) {
        func_002C1050(*(s32 *)(context + 0xaa38));
        *(s32 *)(context + 0xaa38) = 0;
    }
    func_002C1B68(context + 0xaa50, 0);
    func_002B2860(context + 0x60);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_002AD3B8", func_002AE2D0);

INCLUDE_ASM(const s32, "game/code_002AD3B8", func_002AE408);

INCLUDE_ASM(const s32, "game/code_002AD3B8", func_002AE580);

INCLUDE_ASM(const s32, "game/code_002AD3B8", func_002AE7C8);

INCLUDE_ASM(const s32, "game/code_002AD3B8", func_002AE888);

void func_002AEA58(s32 callback) {
    s32 context = func_00101958();

    func_0026C900();
    func_002C4038(context + 8, context + 0x54, 2, callback);
}

INCLUDE_ASM(const s32, "game/code_002AD3B8", func_002AEAA0);

/* Variant cleanup for the adjacent menu state; keep the same release ordering. */
s32 func_002AEC10(void) {
    s32 context = func_00101958();
    s32 entryList = context + 0x284;
    func_002BAF50(*(s32 *)(context + 0x108), context + 0xb10c);
    func_002AC660(context);
    func_002B2C88(entryList, 0, 0, 0);
    func_002BD2E0(entryList);
    mnuClearEntries(entryList);
    if (*(s32 *)(context + 0xaa34) != 0) {
        mnuDestroyPanelGroup(*(s32 *)(context + 0xaa34));
        *(s32 *)(context + 0xaa34) = 0;
    }
    if (*(s32 *)(context + 0xaa38) != 0) {
        func_002C1050(*(s32 *)(context + 0xaa38));
        *(s32 *)(context + 0xaa38) = 0;
    }
    func_002C1B68(context + 0xaa50, 0);
    func_002B2860(context + 0x60);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_002AD3B8", func_002AECF0);

INCLUDE_ASM(const s32, "game/code_002AD3B8", func_002AEEA8);

INCLUDE_ASM(const s32, "game/code_002AD3B8", func_002AF020);

INCLUDE_ASM(const s32, "game/code_002AD3B8", func_002AF2E0);

INCLUDE_RODATA(const s32, "game/code_002AD3B8", D_0042ACA0);

INCLUDE_RODATA(const s32, "game/code_002AD3B8", D_0042ACC8);

INCLUDE_ASM(const s32, "game/code_002AD3B8", func_002AF5E0);

void func_002AF898(s32 callback) {
    s32 context = func_00101958();

    func_0026C900();
    func_002C4038(context + 8, context + 0x54, 2, callback);
}

INCLUDE_ASM(const s32, "game/code_002AD3B8", func_002AF8E0);

/* Third menu-state cleanup uses the matching state-specific pre-release. */
s32 func_002AFA58(void) {
    s32 context = func_00101958();
    s32 entryList = context + 0x284;
    func_002BAF50(*(s32 *)(context + 0x108), context + 0xb10c);
    func_002ACA98(context);
    func_002B2C88(entryList, 0, 0, 0);
    func_002BD2E0(entryList);
    mnuClearEntries(entryList);
    if (*(s32 *)(context + 0xaa34) != 0) {
        mnuDestroyPanelGroup(*(s32 *)(context + 0xaa34));
        *(s32 *)(context + 0xaa34) = 0;
    }
    if (*(s32 *)(context + 0xaa38) != 0) {
        func_002C1050(*(s32 *)(context + 0xaa38));
        *(s32 *)(context + 0xaa38) = 0;
    }
    func_002C1B68(context + 0xaa50, 0);
    func_002B2860(context + 0x60);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_002AD3B8", func_002AFB38);

INCLUDE_ASM(const s32, "game/code_002AD3B8", func_002AFC58);

INCLUDE_ASM(const s32, "game/code_002AD3B8", func_002AFDD0);

INCLUDE_ASM(const s32, "game/code_002AD3B8", func_002AFE18);

INCLUDE_ASM(const s32, "game/code_002AD3B8", func_002B0170);

void func_002B0228(MenuSceneConfig *object) {
    s32 i;
    for (i = 0; i < 5; i++) {
        func_002C2AA8(object->entries[i], 0);
    }
}

INCLUDE_SDATA(const s32, "game/code_002AD3B8", D_00437BD0);

INCLUDE_SDATA(const s32, "game/code_002AD3B8", D_00437BD8);

