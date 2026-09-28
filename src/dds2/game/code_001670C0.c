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
    void (*unk28)(); /* 0x28 invoked by func_00151F38 */
    u16 unk2C;       /* 0x2C kind */
    u16 unk2E;       /* 0x2E */
    void *unk30;     /* 0x30 child (kind 0) or data (kind 1) */
    u8 pad34[8];     /* 0x34 */
    u16 unk3C;       /* 0x3C kind-1 slot set by func_00151260 */
    u8 pad3E[10];    /* 0x3E */
    u32 unk48;       /* 0x48 */
    u32 unk4C;       /* 0x4C */
    u16 unk50;       /* 0x50 set to 1 by func_001512E8/func_00151260 */
    u8 pad52[6];     /* 0x52 */
    u32 unk58;       /* 0x58 compared by func_00152200 */
    s32 unk5C;       /* 0x5C entry count read by func_00152288 */
    void *unk60;     /* 0x60 entry list */
} BillObj;

/* Dispatch entry (0xC bytes). func creates an instance (func_00151D88)
   or runs a command on one (func_00151F00); unk4 is copied onto the new
   instance's unk28 by func_00151D88. */
typedef struct {
    void *(*func)(); /* 0x0 */
    void (*unk4)();  /* 0x4 */
    u32 unk8;        /* 0x8 */
} BillDispatch; /* 0xC bytes */

extern BillDispatch D_003AAF88[];

extern BillDispatch D_003AAF84[];

INCLUDE_ASM(const s32, "game/code_001670C0", func_001670C0);

INCLUDE_ASM(const s32, "game/code_001670C0", func_00167110);

INCLUDE_ASM(const s32, "game/code_001670C0", func_00167168);

INCLUDE_ASM(const s32, "game/code_001670C0", func_001671A0);

INCLUDE_ASM(const s32, "game/code_001670C0", func_001671D8);

INCLUDE_ASM(const s32, "game/code_001670C0", func_00167220);

u16 func_00167260(s32 arg0) {
    return *(u16 *)(arg0 + 0xb2);
}

INCLUDE_ASM(const s32, "game/code_001670C0", func_00167268);

INCLUDE_ASM(const s32, "game/code_001670C0", func_00167278);

void func_001672D8(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x60) = arg1;
}

INCLUDE_ASM(const s32, "game/code_001670C0", func_001672E0);

u8 func_001672F8(s32 arg0) {
    return *(u8 *)(arg0 + 100);
}

void func_00167300(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x1c) = arg1;
}

INCLUDE_ASM(const s32, "game/code_001670C0", func_00167308);

void func_001673E0(s32 arg0, s32 arg1, u32 arg2) {
    *(u32 *)(arg1 * 0x14 + *(s32 *)(arg0 + 0x14) + 0x10) = arg2;
}

s32 func_00167400(s32 arg0) {
    return arg0 + 0x20;
}

INCLUDE_ASM(const s32, "game/code_001670C0", func_00167408);

INCLUDE_ASM(const s32, "game/code_001670C0", func_00167480);

INCLUDE_ASM(const s32, "game/code_001670C0", func_001675B8);

INCLUDE_ASM(const s32, "game/code_001670C0", func_00167778);

INCLUDE_ASM(const s32, "game/code_001670C0", func_00167838);

INCLUDE_ASM(const s32, "game/code_001670C0", func_00167988);

INCLUDE_ASM(const s32, "game/code_001670C0", func_00167A10);

INCLUDE_ASM(const s32, "game/code_001670C0", func_00167E00);

INCLUDE_ASM(const s32, "game/code_001670C0", func_00167EE8);

INCLUDE_ASM(const s32, "game/code_001670C0", func_00168280);

INCLUDE_ASM(const s32, "game/code_001670C0", func_001682B0);

INCLUDE_ASM(const s32, "game/code_001670C0", func_001683F0);

INCLUDE_ASM(const s32, "game/code_001670C0", func_00168448);

INCLUDE_ASM(const s32, "game/code_001670C0", func_00168478);

INCLUDE_ASM(const s32, "game/code_001670C0", func_001684A8);

INCLUDE_ASM(const s32, "game/code_001670C0", func_00168500);

INCLUDE_ASM(const s32, "game/code_001670C0", func_00168548);

INCLUDE_RODATA(const s32, "game/code_001670C0", D_00414478);

INCLUDE_SDATA(const s32, "game/code_001670C0", D_00436408);

INCLUDE_SDATA(const s32, "game/code_001670C0", D_0043640C);

INCLUDE_SDATA(const s32, "game/code_001670C0", D_00436410);

