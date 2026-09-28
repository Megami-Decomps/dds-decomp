#include "common.h"
#include "eff.h"

/* Particle dispatch object; see the matching DDS1 game unit. */
typedef struct ParObj {
    u8 pad0[0x8C];
    f32 scale8C;
    u8 pad90[0x60];
    u32 unkF0;
} ParObj;

typedef struct ParListNode {
    u8 pad00[0x54];
    struct ParListNode *next;
} ParListNode;

typedef struct ParCellNode {
    u8 pad00[0x24];
    struct ParCellNode *next;
} ParCellNode;

extern s32 D_00436400;

extern s32 D_00436404;

extern void (*D_003AAF10[])(void *, void *, void *);

extern BillDispatch D_003AAB88[];

extern void func_001622D0();

extern void parObjGetMode();

void func_00162348(ParObj *work, u32 value) {
    work->unkF0 = value;
}

INCLUDE_ASM(const s32, "game/code_00162348", parObjSetMode);

INCLUDE_ASM(const s32, "game/code_00162348", parObjGetMode);

INCLUDE_ASM(const s32, "game/code_00162348", func_001623D0);

INCLUDE_ASM(const s32, "game/code_00162348", parInstantiateKind);

INCLUDE_ASM(const s32, "game/code_00162348", parObjDispatch);

INCLUDE_ASM(const s32, "game/code_00162348", func_00162590);

void func_001628E0(void) {
    parRestartKind();
}

void func_001628F8(float scale, ParObj *work) {
    func_00162248();
    work->scale8C = work->scale8C * scale;
}

INCLUDE_ASM(const s32, "game/code_00162348", func_00162938);

INCLUDE_ASM(const s32, "game/code_00162348", func_00162958);

void func_00162968(void) {
    func_001622E8();
}

void func_00162980(ParObj *work, u32 value) {
    work->unkF0 = value;
}

void func_00162988(u32 arg0, u8 arg1) {
    parObjSetMode(arg0, arg1);
}

INCLUDE_ASM(const s32, "game/code_00162348", func_001629A0);

INCLUDE_ASM(const s32, "game/code_00162348", func_001629C0);

INCLUDE_ASM(const s32, "game/code_00162348", parDispatchKindUpdate);

INCLUDE_ASM(const s32, "game/code_00162348", parDispatchKindInit);

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

void func_00163000(ParListNode *node) {
    node->next = (ParListNode *)D_00436400;
    D_00436400 = (s32)node;
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

void func_00163508(ParCellNode *node) {
    node->next = (ParCellNode *)D_00436404;
    D_00436404 = (s32)node;
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

