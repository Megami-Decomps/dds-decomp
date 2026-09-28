#include "common.h"

extern u32 D_00438A3C;

typedef struct SdfSubParam {
    u64 unk0; /* 0x0 */
    u64 unk8; /* 0x8 */
    u32 unk10; /* 0x10 */
    u32 unk14; /* 0x14 */
} SdfSubParam;

typedef struct SdfTextParam {
    u8 pad00[6]; /* 0x00 */
    u8 unk06; /* 0x06: dirty flags for the setters below */
    u8 pad07[9]; /* 0x07 */
    u32 unk10; /* 0x10 */
    u32 unk14; /* 0x14 */
    u8 unk18; /* 0x18: func_002DA5B0 stores a u32 over 0x18-0x1B */
    u8 unk19; /* 0x19: bit 0x2 selects unk88/unk8C over defaults */
    u8 unk1A; /* 0x1A */
    u8 unk1B; /* 0x1B */
    f32 unk1C; /* 0x1C */
    u32 unk20; /* 0x20 */
    u8 pad24[4]; /* 0x24 */
    f32 unk28; /* 0x28 */
    f32 unk2C; /* 0x2C */
    u32 unk30; /* 0x30 */
    u32 unk34; /* 0x34 */
    SdfSubParam *unk38; /* 0x38 */
    SdfSubParam *unk3C; /* 0x3C */
    f32 unk40; /* 0x40 */
    f32 unk44; /* 0x44 */
    u8 pad48[0x40]; /* 0x48 */
    f32 unk88; /* 0x88 */
    f32 unk8C; /* 0x8C */
    void *unk90; /* 0x90: resource chunk searched by tag */
} SdfTextParam;

extern f32 D_00438A48;

extern f32 D_00438A4C;

extern u8 D_00439170;

extern u8 D_00439178;

void func_0032CA90(void *arg0, void (*arg1)(void));

void func_00333140(void);

void func_003338B0(void);

extern SdfSubParam *func_00333300(void);

void *func_00328D68(s32 size);

typedef struct SdfNode {
    u16 unk0; /* 0x0 */
    u8 unk2; /* 0x2 */
    u8 unk3; /* 0x3: type tag (0x20/0x30/0x50) */
    u32 unk4; /* 0x4 */
    u32 unk8; /* 0x8 */
    u32 unkC; /* 0xC */
} SdfNode;

INCLUDE_ASM(const s32, "game/code_003325F8", func_003325F8);

INCLUDE_ASM(const s32, "game/code_003325F8", func_00332860);

INCLUDE_ASM(const s32, "game/code_003325F8", func_00332920);

INCLUDE_ASM(const s32, "game/code_003325F8", func_003329B8);

void func_00332A00(s32 arg0) {
    func_003329B8(*(u32 *)(arg0 + 0x90));
}

INCLUDE_ASM(const s32, "game/code_003325F8", func_00332A18);

INCLUDE_ASM(const s32, "game/code_003325F8", func_00332AD8);

INCLUDE_ASM(const s32, "game/code_003325F8", func_00332B78);

INCLUDE_ASM(const s32, "game/code_003325F8", func_00332BB0);

INCLUDE_ASM(const s32, "game/code_003325F8", func_00332C30);

INCLUDE_ASM(const s32, "game/code_003325F8", func_00332C88);

INCLUDE_ASM(const s32, "game/code_003325F8", func_00332D08);

INCLUDE_ASM(const s32, "game/code_003325F8", func_00332D48);

INCLUDE_ASM(const s32, "game/code_003325F8", func_00332D88);

INCLUDE_ASM(const s32, "game/code_003325F8", func_00332DB8);

void func_00332DE8(SdfTextParam *arg0, f32 fparg0, f32 fparg1) {
    arg0->unk88 = fparg0;
    arg0->unk8C = fparg1;
    arg0->unk19 = arg0->unk19 | 2;
}

void func_00332E00(s32 arg0) {
    *(u8 *)(arg0 + 0x19) = *(u8 *)(arg0 + 0x19) & 0xfd;
}

f32 func_00332E10(SdfTextParam *arg0) {
    if ((arg0->unk19 & 2) != 0) {
        return arg0->unk88;
    }
    return D_00438A48;
}

f32 func_00332E30(SdfTextParam *arg0) {
    if ((arg0->unk19 & 2) != 0) {
        return arg0->unk8C;
    }
    return D_00438A4C;
}

void func_00332E50(u32 arg0) {
    D_00438A3C = arg0;
}

void func_00332E58(u32 arg0) {
    func_00340498(arg0, 4, 4);
}

INCLUDE_ASM(const s32, "game/code_003325F8", func_00332E78);

INCLUDE_ASM(const s32, "game/code_003325F8", func_00332F08);

void func_00332F70(s32 arg0, s32 arg1, s32 arg2) {
    s32 temp_v0;

    if ((arg1 < *(s16 *)(arg0 + 4)) && (arg2 != 0)) {
        temp_v0 = (s32)arg1;
        do {
            temp_v0 = temp_v0 + 1;
        } while ((s64)temp_v0 != (s64)*(s16 *)(arg0 + 4));
        *(s16 *)(arg0 + 4) = (s16)arg1;
    }
    func_003405D8();
}

INCLUDE_ASM(const s32, "game/code_003325F8", func_00332FC8);

INCLUDE_ASM(const s32, "game/code_003325F8", func_00333060);

void func_003330F0(void) {
    func_0032CA90(&D_00439170, func_00333140);
    func_0032CA90(&D_00439178, func_003338B0);
}

void func_00333120(u32 arg0) {
    func_00340498(arg0, 4, 8);
}

INCLUDE_ASM(const s32, "game/code_003325F8", func_00333140);

INCLUDE_ASM(const s32, "game/code_003325F8", func_003331A8);

void func_003331F0(void) {
    func_00340558();
}

INCLUDE_ASM(const s32, "game/code_003325F8", func_00333208);

void func_00333270(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x10) = arg1;
    *(u8 *)(arg0 + 6) = *(u8 *)(arg0 + 6) | 3;
}

void func_00333288(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x14) = arg1;
    *(u8 *)(arg0 + 6) = *(u8 *)(arg0 + 6) | 3;
}

void func_003332A0(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x20) = arg1;
    *(u8 *)(arg0 + 6) = *(u8 *)(arg0 + 6) | 3;
}

void func_003332B8(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x28) = arg1;
    *(u8 *)(arg0 + 6) = *(u8 *)(arg0 + 6) | 3;
}

void func_003332D0(SdfTextParam *arg0, f32 fparg0) {
    arg0->unk1C = fparg0;
    arg0->unk06 = arg0->unk06 | 3;
}

void func_003332E8(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x2c) = arg1;
    *(u8 *)(arg0 + 6) = *(u8 *)(arg0 + 6) | 3;
}

SdfSubParam *func_00333300(void) {
    SdfSubParam *temp;

    temp = func_00328D68(0x18);
    temp->unk8 = (((u64)0x3F800000 << 16 | 0x3F80) << 16);
    temp->unk0 = 0;
    temp->unk10 = 0;
    return temp;
}

void func_00333340(s32 arg0) {
    u32 temp_v0;

    if (*(s32 *)(arg0 + 0x38) == 0) {
        temp_v0 = func_00333300();
        *(u32 *)(arg0 + 0x38) = temp_v0;
    }
}

INCLUDE_ASM(const s32, "game/code_003325F8", func_00333378);

INCLUDE_ASM(const s32, "game/code_003325F8", func_003333F8);

void func_00333460(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x18) = arg1;
    *(u8 *)(arg0 + 6) = *(u8 *)(arg0 + 6) | 0x30;
}

void func_00333478(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x34) = arg1;
    *(u8 *)(arg0 + 6) = *(u8 *)(arg0 + 6) | 0x30;
}

void func_00333490(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x30) = arg1;
    *(u8 *)(arg0 + 6) = *(u8 *)(arg0 + 6) | 0x30;
}

void func_003334A8(s32 arg0) {
    u32 temp_v0;

    if (*(s32 *)(arg0 + 0x3c) == 0) {
        temp_v0 = func_00333300();
        *(u32 *)(arg0 + 0x3c) = temp_v0;
    }
}

INCLUDE_ASM(const s32, "game/code_003325F8", func_003334E0);

INCLUDE_ASM(const s32, "game/code_003325F8", func_00333560);

void func_003335C8(SdfTextParam *arg0, f32 fparg0, f32 fparg1) {
    arg0->unk40 = fparg0;
    arg0->unk44 = fparg1;
    arg0->unk06 = arg0->unk06 | 0xC0;
}

INCLUDE_ASM(const s32, "game/code_003325F8", func_003335E0);

INCLUDE_ASM(const s32, "game/code_003325F8", func_003336E0);

INCLUDE_ASM(const s32, "game/code_003325F8", func_003338B0);

INCLUDE_ASM(const s32, "game/code_003325F8", func_00333918);

void *func_00333950(u32 *arg0, SdfNode *arg1, s32 arg2) {
    u32 *entry = arg0 + arg2;

    arg1->unk3 = 0x30;
    arg1->unk4 = entry[2] & 0x0FFFFFFF;
    arg1->unk0 = 0xA;
    arg1->unk8 = 0;
    arg1->unkC = 0;
    return (void *)((u8 *)arg1 + 0x10);
}

INCLUDE_ASM(const s32, "game/code_003325F8", func_00333990);

INCLUDE_ASM(const s32, "game/code_003325F8", func_00333A30);

void func_00333B18(s32 arg0, s32 arg1) {
    func_00333A30(arg1 + 0x68, *(u32 *)(arg0 + 0x38));
}

INCLUDE_ASM(const s32, "game/code_003325F8", func_00333B38);

void func_00333BC8(SdfTextParam *arg0, SdfTextParam *arg1) {
    arg1->unk28 = arg0->unk40;
    arg1->unk2C = arg0->unk44;
}

INCLUDE_ASM(const s32, "game/code_003325F8", func_00333BE0);

INCLUDE_ASM(const s32, "game/code_003325F8", func_00333CB0);

INCLUDE_ASM(const s32, "game/code_003325F8", func_00333D98);

INCLUDE_ASM(const s32, "game/code_003325F8", func_00333E38);

INCLUDE_ASM(const s32, "game/code_003325F8", func_00333E98);

INCLUDE_ASM(const s32, "game/code_003325F8", func_00333EF8);

INCLUDE_ASM(const s32, "game/code_003325F8", func_00334008);

INCLUDE_ASM(const s32, "game/code_003325F8", func_00334078);
INCLUDE_SDATA(const s32, "game/code_003325F8", D_00438A38);


INCLUDE_SDATA(const s32, "game/code_003325F8", D_00438A3C);

