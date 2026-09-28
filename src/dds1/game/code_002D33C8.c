#include "common.h"

typedef struct SdfListHead {
    u32 unk0; /* 0x0 */
    u32 unk4; /* 0x4: list head for the func_002D4038 family */
    u32 unk8; /* 0x8: list tail */
    u32 unkC; /* 0xC: 0xFFFF when fresh (func_002D4010) */
    u32 unk10; /* 0x10 */
    u32 unk14; /* 0x14 */
    u32 unk18; /* 0x18 */
    u32 unk1C; /* 0x1C */
} SdfListHead;

typedef struct SdfPacket {
    u64 unk0; /* 0x0 */
    u64 unk8; /* 0x8 */
    u64 unk10; /* 0x10 */
    u64 unk18; /* 0x18 */
} SdfPacket;

typedef struct SdfDmaSrc {
    u16 unk0; /* 0x0 */
    u8 pad2[6]; /* 0x2 */
    u64 unk8; /* 0x8 */
} SdfDmaSrc;

typedef struct SdfDmaNode {
    u64 unk0; /* 0x0 */
    u64 unk8; /* 0x8 */
    int __attribute__((mode(TI))) unk10; /* 0x10: cleared with por/sq */
} SdfDmaNode;

typedef struct SdfResEntry {
    u8 pad00[0xC]; /* 0x0 */
    u32 unk0C; /* 0xC */
} SdfResEntry;

typedef struct SdfBigPacket {
    u8 pad00[8]; /* 0x0 */
    s32 unk08; /* 0x8 */
    u8 pad0C[0x24]; /* 0xC */
    u64 unk30; /* 0x30 */
    u8 pad38[0x48]; /* 0x38 */
    u64 unk80; /* 0x80 */
} SdfBigPacket;

extern u32 func_002CFEB8(u32);

extern u32 func_002D42B8(u32);

extern s32 D_003BD314;
extern u32 D_003BD318;
extern s32 D_003BD320;
extern s32 D_003BD324;

extern u64 func_002D3E88(void);
extern s64 func_00312C08(void);

extern s32 D_003BD30C;
extern s32 func_002CF440(u32, u32, u32);
extern u8 D_003BD9F0;
extern u8 D_003BDA08;
extern volatile s8 D_003BD333;
extern s32 D_003BD338;
extern u32 D_00398158[];
extern SdfResEntry *D_003980E8[];

void func_002D2C30(void);
void func_002D3BE0(void *arg0, void (*arg1)(void));
void func_002D4160(s32 arg0, s32 arg1);
void func_002D5A68(SdfPacket *arg0, u32 arg1, s32 arg2, s64 arg3, s64 arg4, s64 arg5, u32 arg6, s32 arg7, s32 arg_sp0, s32 arg_sp8, s32 arg_sp10, s32 arg_sp18, s32 arg_sp20, s32 arg_sp28);
void func_002D7A50(void);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D33C8);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D3558);

void func_002D3598(void) {
    func_002D3BE0(&D_003BD9F0, func_002D2C30);
}

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D35B8);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D37A8);

void func_002D3880(SdfBigPacket *arg0, s32 arg1) {
    arg0->unk80 = (arg0->unk80 & ~0x3FFF) | (u64)(u32)(D_003980E8[arg1]->unk0C >> 6);
}

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D38B8);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D39B0);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D3B28);

void func_002D3BE0(void *arg0, void (*arg1)(void)) {
    void **head = arg0;

    if (D_003BD30C < 0) {
        D_003BD30C = func_002CF440(1, 0x7f, 0);
    }
    head[0] = (void *)arg1;
    head[1] = NULL;
}

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D3C30);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D3D00);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D3D40);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D3E10);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D3E88);

u64 func_002D3EE8(void) {
    s64 temp_v0;
    u64 temp_v1;

    temp_v0 = func_00312C08();
    temp_v1 = func_002D3E88();
    if (temp_v0 != 0) {
        EIntr();
    }
    return temp_v1;
}

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D3F30);

void func_002D3FA0(s32 arg0) {
    D_003BD320 = (&D_003BD318)[arg0];
    D_003BD324 = (&D_003BD318)[arg0] + D_003BD314;
}

s32 func_002D3FC0(void) {
    return D_003BD324 - D_003BD320;
}

s32 func_002D3FD0(s32 arg0) {
    s32 temp_v0;

    temp_v0 = D_003BD320;
    D_003BD320 = D_003BD320 + ((arg0 + 0xfU) & 0xfffffff0);
    return temp_v0;
}

s32 func_002D3FF0(void) {
    return D_003BD320;
}

void func_002D3FF8(s32 arg0) {
    D_003BD320 = (arg0 + 0xF) & ~0xF;
}

void func_002D4010(SdfListHead *arg0) {
    arg0->unkC = 0xFFFF;
    arg0->unk0 = 0;
    arg0->unk4 = 0;
    arg0->unk8 = 0;
    arg0->unk10 = 0;
    arg0->unk14 = 0;
    arg0->unk18 = 0;
    arg0->unk1C = 0;
}

void func_002D4038(s32 arg0, u32 arg1) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 8);
    if (temp_v0 == 0) {
        *(u32 *)(arg0 + 4) = arg1;
    }
    else {
        *(u8 *)(temp_v0 + 3) = 0x20;
        *(u32 *)(temp_v0 + 4) = arg1 & 0xfffffff;
    }
    *(u32 *)(arg0 + 8) = arg1;
}

void func_002D4070(s32 arg0, u32 arg1, u32 arg2) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 8);
    if (temp_v0 == 0) {
        *(u32 *)(arg0 + 4) = arg1;
    }
    else {
        *(u8 *)(temp_v0 + 3) = 0x20;
        *(u32 *)(temp_v0 + 4) = arg1 & 0xfffffff;
    }
    *(u32 *)(arg0 + 8) = arg2;
}

void func_002D40A8(s32 arg0, u32 arg1) {
    s32 temp_v0;

    *(u8 *)(arg1 + 3) = 0x30;
    temp_v0 = *(s32 *)(arg0 + 8);
    if (temp_v0 == 0) {
        *(u32 *)(arg0 + 4) = arg1;
    }
    else {
        *(u8 *)(temp_v0 + 3) = 0x20;
        *(u32 *)(temp_v0 + 4) = arg1 & 0xfffffff;
    }
    *(u32 *)(arg0 + 8) = arg1 + 0x10;
}

void func_002D40E8(s32 arg0, u32 arg1) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 8);
    if (temp_v0 == 0) {
        *(u32 *)(arg0 + 4) = arg1;
    }
    else {
        *(u8 *)(temp_v0 + 3) = 0x20;
        *(u32 *)(temp_v0 + 4) = arg1 & 0xfffffff;
    }
    *(u32 *)(arg0 + 8) = arg1 + 0x30;
}

void func_002D4120(s32 arg0, u32 arg1) {
    s32 temp_v0;

    *(u8 *)(arg1 + 3) = 0x50;
    temp_v0 = *(s32 *)(arg0 + 8);
    if (temp_v0 == 0) {
        *(u32 *)(arg0 + 4) = arg1;
    }
    else {
        *(u8 *)(temp_v0 + 3) = 0x20;
        *(u32 *)(temp_v0 + 4) = arg1 & 0xfffffff;
    }
    *(u32 *)(arg0 + 8) = arg1 + 0x10;
}

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D4160);

void func_002D41C0(s32 arg0, s32 arg1) {
    s32 *piVar1;

    if (*(s32 *)(arg1 + 4) != 0) {
        piVar1 = *(s32 **)(arg0 + 8);
        if (piVar1 == (s32 *)0x0) {
            *(s32 *)(arg0 + 4) = arg1;
        }
        else {
            *piVar1 = arg1;
            func_002D4368(piVar1);
        }
        *(s32 *)(arg0 + 8) = arg1;
    }
}

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D4218);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D4240);

u32 func_002D42B8(u32 arg0) {
    SdfDmaNode *temp = (SdfDmaNode *)func_002D3FD0(0x20);
    SdfDmaSrc *src = (SdfDmaSrc *)arg0;
    u64 id = src->unk0;
    u32 addr = ((u32)src + 0x10) & 0x0FFFFFFF;
    s64 shifted = (s64)addr << 32;

    id |= 0x30000000;
    id |= shifted;
    temp->unk0 = id;
    temp->unk10 = 0;
    temp->unk8 = src->unk8;
    return (u32)temp;
}

s32 func_002D4320(s32 arg0, u32 arg1) {
    u32 temp_v0;

    temp_v0 = func_002D42B8(arg1);
    *(u8 *)(arg0 + 3) = 0x20;
    *(u32 *)(arg0 + 4) = temp_v0 & 0xfffffff;
    return temp_v0 + 0x10;
}

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D4368);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D43F8);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D4490);

void func_002D4540(SdfListHead *arg0) {
    arg0->unk0 = 0;
    arg0->unk4 = 0;
    arg0->unk8 = 0;
    arg0->unkC = 0;
}

void func_002D4558(s32 arg0, u32 *arg1) {
    if (*(u32 **)(arg0 + 8) == (u32 *)0x0) {
        *(u32 **)(arg0 + 4) = arg1;
    }
    else {
        **(u32 **)(arg0 + 8) = arg1;
    }
    *(u32 **)(arg0 + 8) = arg1;
    *arg1 = 0;
}

void func_002D4578(SdfListHead *arg0) {
    arg0->unk0 = 0;
    arg0->unk4 = 0;
}

void func_002D4588(s32 *arg0, s32 arg1) {
    if (arg0[1] == 0) {
        *arg0 = arg1;
    }
    else {
        **(u32 **)(arg0[1] + 8) = *(u32 *)(arg1 + 8);
    }
    arg0[1] = arg1;
}

void func_002D45B0(SdfPacket *arg0, s32 arg1) {
    s64 temp;

    temp = arg1 + 1;
    arg0->unk8 = (((temp | 0x50000000) << 32) | 0x10000000);
    arg0->unk10 = (arg1 | (((s64)0x10000000 << 32) | 0x8000));
    arg0->unk0 = temp;
    arg0->unk18 = 0xE;
}

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D45F0);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D4678);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D4730);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D47B8);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D4800);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D48A8);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D49E8);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D4BA0);

void func_002D4C40(u32 arg0, u32 arg1, u32 arg2) {
    func_002D4558(arg1, arg2);
    func_002D4038(arg0, (s32)arg2 + 0x10);
}

void func_002D4C80(s32 arg0, u32 arg1, s32 arg2) {
    if (arg2 == 0) {
        func_002D45F0(arg1, arg0 + 0x180, 1);
        return;
    }
    func_002D45F0(arg1, arg0 + 400, 1);
}

void func_002D4CC8(s32 arg0, u32 arg1, s32 arg2) {
    if (arg2 == 0) {
        func_002D45F0(arg1, arg0 + 0x70, 1);
        return;
    }
    func_002D45F0(arg1, arg0 + 0xb0, 1);
}

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D4D10);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D4D70);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D4DD0);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D4E70);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D4EE8);

void func_002D4FE8(s32 arg0) {
    D_003BD333 = (arg0 < 0) ? 0 : arg0;
}

void func_002D5000(void) {
    D_003BD333 = 0;
    D_00398158[0] = 0;
    D_003BD338 = 0;
}

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D5018);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D5498);

s32 func_002D5510(s32 (*arg0)(s32)) {
    s32 (*alloc)(s32) = arg0;
    s32 mem;

    if (alloc == NULL) {
        alloc = func_002D3FD0;
    }
    mem = alloc(0x20);
    func_002D4010((SdfListHead *)mem);
    return mem;
}

void func_002D5558(s32 arg0, void (*arg1)(s32), s32 arg2, s32 (*arg3)(s32)) {
    s32 (*alloc)(s32) = arg3;
    s32 mem;

    if (alloc == NULL) {
        alloc = func_002D3FD0;
    }
    mem = alloc(arg2);
    arg1(mem);
    func_002D4038(arg0, mem);
}

void func_002D55B8(SdfPacket *arg0) {
    arg0->unk0 = 0x717FB;
    arg0->unk8 = 0x47;
    arg0->unk10 = 0x44;
    arg0->unk18 = 0x42;
}

void func_002D55E0(SdfPacket *arg0) {
    arg0->unk0 = 0x717FB;
    arg0->unk8 = 0x48;
    arg0->unk10 = 0x44;
    arg0->unk18 = 0x43;
}

void func_002D5608(SdfPacket *arg0) {
    func_002D55B8(arg0 + 1);
    arg0->unk0 = 3;
    arg0->unk8 = (((u64)0x50000003 << 16 | 0x1000) << 16);
    arg0->unk10 = (((u64)0x10000000 << 32) | 0x8002);
    arg0->unk18 = 0xE;
}

void func_002D5668(SdfPacket *arg0) {
    func_002D55E0(arg0 + 1);
    arg0->unk0 = 3;
    arg0->unk8 = (((u64)0x50000003 << 16 | 0x1000) << 16);
    arg0->unk10 = (((u64)0x10000000 << 32) | 0x8002);
    arg0->unk18 = 0xE;
}

void func_002D56C8(SdfPacket *arg0) {
    arg0->unk0 = 0x717FB;
    arg0->unk8 = 0x47;
    arg0->unk10 = 0x44;
    arg0->unk18 = 0x42;
}

void func_002D56F0(SdfPacket *arg0) {
    arg0->unk0 = 0x717FB;
    arg0->unk8 = 0x48;
    arg0->unk10 = 0x44;
    arg0->unk18 = 0x43;
}

void func_002D5718(SdfPacket *arg0) {
    func_002D56C8(arg0 + 1);
    arg0->unk0 = 3;
    arg0->unk8 = (((u64)0x50000003 << 16 | 0x1000) << 16);
    arg0->unk10 = (((u64)0x10000000 << 32) | 0x8002);
    arg0->unk18 = 0xE;
}

void func_002D5778(SdfPacket *arg0) {
    func_002D56F0(arg0 + 1);
    arg0->unk0 = 3;
    arg0->unk8 = (((u64)0x50000003 << 16 | 0x1000) << 16);
    arg0->unk10 = (((u64)0x10000000 << 32) | 0x8002);
    arg0->unk18 = 0xE;
}

void func_002D57D8(SdfPacket *arg0) {
    arg0->unk0 = 0x71801;
    arg0->unk8 = 0x47;
    arg0->unk10 = 0x48;
    arg0->unk18 = 0x42;
}

void func_002D5800(SdfPacket *arg0) {
    arg0->unk0 = 0x71801;
    arg0->unk8 = 0x48;
    arg0->unk10 = 0x48;
    arg0->unk18 = 0x43;
}

void func_002D5828(SdfPacket *arg0) {
    func_002D57D8(arg0 + 1);
    arg0->unk0 = 3;
    arg0->unk8 = (((u64)0x50000003 << 16 | 0x1000) << 16);
    arg0->unk10 = (((u64)0x10000000 << 32) | 0x8002);
    arg0->unk18 = 0xE;
}

void func_002D5888(SdfPacket *arg0) {
    func_002D5800(arg0 + 1);
    arg0->unk0 = 3;
    arg0->unk8 = (((u64)0x50000003 << 16 | 0x1000) << 16);
    arg0->unk10 = (((u64)0x10000000 << 32) | 0x8002);
    arg0->unk18 = 0xE;
}

void func_002D58E8(SdfPacket *arg0) {
    arg0->unk0 = 0x71801;
    arg0->unk8 = 0x47;
    arg0->unk10 = 0x42;
    arg0->unk18 = 0x42;
}

void func_002D5910(SdfPacket *arg0) {
    arg0->unk0 = 0x71801;
    arg0->unk8 = 0x48;
    arg0->unk10 = 0x42;
    arg0->unk18 = 0x43;
}

void func_002D5938(SdfPacket *arg0) {
    func_002D58E8(arg0 + 1);
    arg0->unk0 = 3;
    arg0->unk8 = (((u64)0x50000003 << 16 | 0x1000) << 16);
    arg0->unk10 = (((u64)0x10000000 << 32) | 0x8002);
    arg0->unk18 = 0xE;
}

void func_002D5998(SdfPacket *arg0) {
    func_002D5910(arg0 + 1);
    arg0->unk0 = 3;
    arg0->unk8 = (((u64)0x50000003 << 16 | 0x1000) << 16);
    arg0->unk10 = (((u64)0x10000000 << 32) | 0x8002);
    arg0->unk18 = 0xE;
}

void func_002D59F8(u64 *arg0) {
    *arg0 = 0;
    arg0[1] = 0x3f;
}

void func_002D5A08(SdfPacket *arg0) {
    func_002D59F8((u64 *)(arg0 + 1));
    arg0->unk0 = 2;
    arg0->unk8 = (((u64)0x50000002 << 16 | 0x1000) << 16);
    arg0->unk10 = (((u64)0x10000000 << 32) | 0x8001);
    arg0->unk18 = 0xE;
}

void func_002D5B18(SdfPacket *arg0, u32 arg1, s32 arg2, s64 arg3, s64 arg4, s64 arg5, u32 arg6, s32 arg7, s32 arg_sp0, s32 arg_sp8, s32 arg_sp10, s32 arg_sp18, s32 arg_sp20, s32 arg_sp28) {
    /* arg1 and below flow through untouched to func_002D5A68. */
    arg0->unk0 = 5;
    arg0->unk8 = (((u64)0x50000005 << 16 | 0x1000) << 16);
    arg0->unk10 = (((u64)0x10000000 << 32) | 0x8004);
    arg0->unk18 = 0xE;
    func_002D5A68(arg0 + 1, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg_sp0, arg_sp8, arg_sp10, arg_sp18, arg_sp20, arg_sp28);
}

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D5B70);

void func_002D5C90(SdfBigPacket *arg0, s32 arg1) {
    arg0->unk30 = (arg0->unk30 & ~0x3FFF) | (u64)(u32)(D_003980E8[arg1 ^ arg0->unk08]->unk0C >> 6);
}

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D5CD0);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D5DF8);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D5EB0);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D5FD8);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D6080);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D6188);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D6258);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D6380);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D6450);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D6578);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D6690);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D67E8);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D68D8);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D6A40);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D6B98);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D6D40);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D6E80);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D7008);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D71B8);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D7390);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D7410);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D7500);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D7580);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D7670);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D7720);

void func_002D7810(void) {
    func_002D3BE0(&D_003BDA08, func_002D7A50);
}

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D7830);

void func_002D78B8(s32 arg0) {
    u32 temp_v0;

    if (*(s32 *)(arg0 + 0x30) == 0) {
        temp_v0 = func_002CFEB8(0x100);
        *(u32 *)(arg0 + 0x30) = temp_v0;
    }
}

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D78F0);

void func_002D7988(u32 arg0) {
    func_002D78F0();
    func_002CFF98(*(u32 *)((s32)arg0 + 0x30));
    *(u32 *)((s32)arg0 + 0x30) = 0;
    func_002CFF98(arg0);
}

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D79C0);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D7A50);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D7AC8);

void func_002D7B50(u32 *arg0) {
    func_002E7730(*arg0);
}

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D7B68);

void func_002D7BD8(s32 *arg0, u32 arg1, u32 arg2) {
    s16 temp_v0;
    s32 temp_v1;
    s32 temp_v2;
    s32 temp_v3;

    temp_v2 = *arg0;
    temp_v0 = *(s16 *)(temp_v2 + 4);
    temp_v3 = temp_v0 + 1;
    if ((s64)*(s16 *)(temp_v2 + 6) < (s64)temp_v3) {
        func_002E76B0(temp_v2);
        temp_v2 = *arg0;
    }
    temp_v1 = *(s32 *)(temp_v2 + 0xc);
    *(s32 **)((s32)arg2 + 0x10) = arg0;
    *(s16 *)(temp_v2 + 4) = (s16)temp_v3;
    *(s32 *)(temp_v0 * 4 + temp_v1) = (s32)arg2;
    func_002D7CD0(arg2, arg1);
}

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D7C68);

INCLUDE_ASM(const s32, "game/code_002D33C8", func_002D7CD0);
