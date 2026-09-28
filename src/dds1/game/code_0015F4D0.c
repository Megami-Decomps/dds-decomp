#include "common.h"

/* Billboard instance. Kind in unk2C (0 = data-driven child in unk30,
   1 = entry list at unk60). Floats/int witnesses: defaults set by
   func_00151178, color/mode by code_00151F58 setters, child released by
   func_00151210, list compared by func_00152200. */
typedef struct BillObj {
    f32 unk0;        /* 0x0 */
    f32 unk4;        /* 0x4 */
    f32 unk8;        /* 0x8 */
    f32 unkC;        /* 0xC */
    f32 unk10;       /* 0x10 */
    f32 unk14;       /* 0x14 */
    f32 unk18;       /* 0x18 */
    f32 unk1C;       /* 0x1C */
    f32 unk20;       /* 0x20 */
    u32 unk24;       /* 0x24 */
    void (*unk28)(); /* 0x28 invoked by billInvokeCallback */
    u16 unk2C;       /* 0x2C kind */
    u16 unk2E;       /* 0x2E */
    void *unk30;     /* 0x30 child (kind 0) or data (kind 1) */
    u8 pad34[8];     /* 0x34 */
    u16 unk3C;       /* 0x3C kind-1 slot set by billAllocList */
    u8 pad3E[10];    /* 0x3E */
    u32 unk48;       /* 0x48 */
    u32 unk4C;       /* 0x4C */
    u16 unk50;       /* 0x50 set to 1 by billCloneList/billAllocList */
    u8 pad52[6];     /* 0x52 */
    u32 unk58;       /* 0x58 compared by func_00152200 */
    s32 unk5C;       /* 0x5C entry count read by func_00152288 */
    void *unk60;     /* 0x60 entry list */
} BillObj;

/* Dispatch entry (0xC bytes). func creates an instance (billCreateIndexed)
   or runs a command on one (billDispatchByKind); unk4 is copied onto the new
   instance's unk28 by billCreateIndexed. */
typedef struct {
    void *(*func)(); /* 0x0 */
    void (*unk4)();  /* 0x4 */
    u32 unk8;        /* 0x8 */
} BillDispatch; /* 0xC bytes */

extern BillDispatch D_0034E658[];

extern BillDispatch D_0034E654[];

INCLUDE_ASM(const s32, "game/code_0015F4D0", func_0015F4D0);

INCLUDE_ASM(const s32, "game/code_0015F4D0", func_0015F520);

INCLUDE_ASM(const s32, "game/code_0015F4D0", func_0015F578);

INCLUDE_ASM(const s32, "game/code_0015F4D0", func_0015F5B0);

INCLUDE_ASM(const s32, "game/code_0015F4D0", func_0015F5E8);

INCLUDE_ASM(const s32, "game/code_0015F4D0", func_0015F630);

u16 func_0015F670(s32 arg0) {
    return *(u16 *)(arg0 + 0xb2);
}

INCLUDE_ASM(const s32, "game/code_0015F4D0", func_0015F678);

INCLUDE_ASM(const s32, "game/code_0015F4D0", func_0015F688);

void func_0015F6E8(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x60) = arg1;
}

INCLUDE_ASM(const s32, "game/code_0015F4D0", func_0015F6F0);

u8 func_0015F708(s32 arg0) {
    return *(u8 *)(arg0 + 100);
}

void func_0015F710(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x1c) = arg1;
}

INCLUDE_ASM(const s32, "game/code_0015F4D0", func_0015F718);

void func_0015F7F0(s32 arg0, s32 arg1, u32 arg2) {
    *(u32 *)(arg1 * 0x14 + *(s32 *)(arg0 + 0x14) + 0x10) = arg2;
}

s32 func_0015F810(s32 arg0) {
    return arg0 + 0x20;
}

INCLUDE_ASM(const s32, "game/code_0015F4D0", func_0015F818);

INCLUDE_ASM(const s32, "game/code_0015F4D0", func_0015F890);

INCLUDE_ASM(const s32, "game/code_0015F4D0", func_0015F9C8);

INCLUDE_ASM(const s32, "game/code_0015F4D0", func_0015FB88);

INCLUDE_ASM(const s32, "game/code_0015F4D0", func_0015FC48);

INCLUDE_ASM(const s32, "game/code_0015F4D0", func_0015FD98);

INCLUDE_ASM(const s32, "game/code_0015F4D0", func_0015FE20);

INCLUDE_ASM(const s32, "game/code_0015F4D0", func_00160210);

INCLUDE_ASM(const s32, "game/code_0015F4D0", func_001602F8);

INCLUDE_ASM(const s32, "game/code_0015F4D0", func_00160690);

INCLUDE_ASM(const s32, "game/code_0015F4D0", func_001606C0);

INCLUDE_ASM(const s32, "game/code_0015F4D0", func_00160800);

INCLUDE_ASM(const s32, "game/code_0015F4D0", func_00160858);

INCLUDE_ASM(const s32, "game/code_0015F4D0", func_00160888);

INCLUDE_ASM(const s32, "game/code_0015F4D0", func_001608B8);

INCLUDE_ASM(const s32, "game/code_0015F4D0", func_00160910);

INCLUDE_ASM(const s32, "game/code_0015F4D0", func_00160958);

INCLUDE_RODATA(const s32, "game/code_0015F4D0", D_003A0DB8);

INCLUDE_SDATA(const s32, "game/code_0015F4D0", D_003BB018);

INCLUDE_SDATA(const s32, "game/code_0015F4D0", D_003BB01C);

INCLUDE_SDATA(const s32, "game/code_0015F4D0", D_003BB020);

