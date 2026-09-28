#include "common.h"

extern s32 D_00436400;

extern s32 D_00436404;

extern void (*D_003AAF10[])(void *, void *, void *);

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

extern BillDispatch D_003AAB88[];

extern void func_001622D0();

extern void parObjGetMode();

void func_00162348(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0xf0) = arg1;
}

INCLUDE_ASM(const s32, "game/code_00162348", parObjSetMode);

INCLUDE_ASM(const s32, "game/code_00162348", parObjGetMode);

INCLUDE_ASM(const s32, "game/code_00162348", func_001623D0);

INCLUDE_ASM(const s32, "game/code_00162348", func_001624B8);

INCLUDE_ASM(const s32, "game/code_00162348", parObjDispatch);

INCLUDE_ASM(const s32, "game/code_00162348", func_00162590);

void func_001628E0(void) {
    parRestartKind();
}

void func_001628F8(float arg0, s32 arg1) {
    func_00162248();
    *(float *)(arg1 + 0x8c) = *(float *)(arg1 + 0x8c) * arg0;
}

INCLUDE_ASM(const s32, "game/code_00162348", func_00162938);

INCLUDE_ASM(const s32, "game/code_00162348", func_00162958);

void func_00162968(void) {
    func_001622E8();
}

void func_00162980(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0xf0) = arg1;
}

void func_00162988(u32 arg0, u8 arg1) {
    parObjSetMode(arg0, arg1);
}

INCLUDE_ASM(const s32, "game/code_00162348", func_001629A0);

INCLUDE_ASM(const s32, "game/code_00162348", func_001629C0);

INCLUDE_ASM(const s32, "game/code_00162348", func_00162A30);

INCLUDE_ASM(const s32, "game/code_00162348", func_00162AC8);

INCLUDE_ASM(const s32, "game/code_00162348", func_00162B60);

INCLUDE_ASM(const s32, "game/code_00162348", func_00162C48);

INCLUDE_ASM(const s32, "game/code_00162348", func_00162D38);

u32 func_00162E10(void) {
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00162348", parSysReset);

void func_00162E40(void) {
}

INCLUDE_ASM(const s32, "game/code_00162348", func_00162E48);

void func_00162FC8(u16 *arg0) {
    *arg0 = 1;
    func_00333918(*(u32 *)(arg0 + 0x20));
    func_003297C8(*(u32 *)(arg0 + 8));
}

void func_00163000(s32 arg0) {
    *(s32 *)(arg0 + 0x54) = D_00436400;
    D_00436400 = arg0;
}

INCLUDE_ASM(const s32, "game/code_00162348", func_00163010);

void func_00163238(s32 arg0) {
    func_003332E8(*(u32 *)(arg0 + 0x40));
}

INCLUDE_ASM(const s32, "game/code_00162348", parControlInit);

INCLUDE_ASM(const s32, "game/code_00162348", func_00163290);

void func_001634A8(s32 arg0) {
    func_00333918(*(u32 *)(arg0 + 0x20));
    func_003297C8(*(u32 *)(arg0 + 0x10));
}

INCLUDE_ASM(const s32, "game/code_00162348", parCellInit);

void func_00163508(s32 arg0) {
    *(s32 *)(arg0 + 0x24) = D_00436404;
    D_00436404 = arg0;
}

INCLUDE_ASM(const s32, "game/code_00162348", func_00163518);

INCLUDE_ASM(const s32, "game/code_00162348", func_001635D0);

INCLUDE_ASM(const s32, "game/code_00162348", func_00163628);

INCLUDE_ASM(const s32, "game/code_00162348", func_001636F0);

INCLUDE_ASM(const s32, "game/code_00162348", func_00163780);

INCLUDE_ASM(const s32, "game/code_00162348", func_001638D8);

INCLUDE_ASM(const s32, "game/code_00162348", func_00163B68);

INCLUDE_ASM(const s32, "game/code_00162348", func_00163BC8);

INCLUDE_ASM(const s32, "game/code_00162348", func_00163D18);

INCLUDE_ASM(const s32, "game/code_00162348", func_00163EE0);

INCLUDE_ASM(const s32, "game/code_00162348", func_00163F50);

INCLUDE_ASM(const s32, "game/code_00162348", func_00164208);

INCLUDE_ASM(const s32, "game/code_00162348", func_00164318);

INCLUDE_ASM(const s32, "game/code_00162348", func_00164390);

INCLUDE_ASM(const s32, "game/code_00162348", func_001644B0);

INCLUDE_ASM(const s32, "game/code_00162348", func_00164630);

INCLUDE_ASM(const s32, "game/code_00162348", func_00164690);

INCLUDE_ASM(const s32, "game/code_00162348", func_00164748);

INCLUDE_ASM(const s32, "game/code_00162348", func_00164848);

INCLUDE_ASM(const s32, "game/code_00162348", func_001648C0);

INCLUDE_ASM(const s32, "game/code_00162348", func_001649E0);

INCLUDE_ASM(const s32, "game/code_00162348", func_00164AE8);

void func_00164C68(s32 arg0, u16 arg1) {
    *(u16 *)(arg0 + 2) = arg1;
}

void parDispatchSub(void *work, s32 sub, void *a2, void *a3) {
    u16 id = *(u16 *)work;

    D_003AAF10[id * 3 + sub](work, a2, a3);
}

INCLUDE_ASM(const s32, "game/code_00162348", func_00164CB0);

INCLUDE_ASM(const s32, "game/code_00162348", func_00165300);

void func_001653A8(s32 arg0) {
    func_00333918(*(u32 *)(arg0 + 0x10));
    func_003297C8(*(u32 *)(arg0 + 0x14));
}

INCLUDE_ASM(const s32, "game/code_00162348", func_001653D8);

INCLUDE_ASM(const s32, "game/code_00162348", func_00165500);

INCLUDE_SDATA(const s32, "game/code_00162348", D_00436400);

INCLUDE_SDATA(const s32, "game/code_00162348", D_00436404);

