#include "common.h"

typedef struct MdlViewState {
    s32 unk00;
    s32 unk04;
    s8 unk08;
    u8 pad09;
    s8 unk0A;
    u8 pad0B[11];
    s16 unk16;
    u8 pad18[0x74];
    s32 unk8C[1];
    s32 unk90[1];
} MdlViewState;

typedef struct MdlCtrlState {
    u8 pad00[4];
    u8 unk04;
    u8 pad05[3];
    s32 unk08;
} MdlCtrlState;

extern MdlViewState D_003D7A50;
extern MdlCtrlState D_003D7B50;
extern s32 D_003BAA00;
extern s32 D_003D7B10[];
extern s8 D_003D7A60[];
extern s8 D_00367A40[];

s32 func_0011D3E8(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6);
void func_0011E080(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6);
void func_0014FB70(s32 arg0, float arg1);
s32 func_0014FD20(s32 arg0);
void func_00152000(s32 arg0, float arg1, float arg2);
s32 func_00151D88(s32 arg0, s32 arg1);
s32 func_0021A608(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
void func_0021A660(void *arg0, s16 arg1);
void func_0021B9F8(void);
void func_0021BDD0(void);
void func_002D4038(s32 arg0, s32 arg1);
void func_002EC780(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
extern s32 kwlnTaskGetTaskByName(void *name);
extern void kwlnTaskDestroyWithHierarchy(s32 task, s32 flag);
extern s32 kwlnTaskCreate(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6);
void func_00102A18(void);
void func_00101A80(s32 arg0, s32 arg1);
void func_0021E3C0(MdlViewState *arg0);
INCLUDE_ASM(const s32, "game/code_00218B48", func_00218B48);

void func_00218BE8(s32 arg0) {
    func_002CFF98((void *)arg0);
}

INCLUDE_ASM(const s32, "game/code_00218B48", func_00218C00);

INCLUDE_ASM(const s32, "game/code_00218B48", func_00218CA8);

INCLUDE_ASM(const s32, "game/code_00218B48", func_00218E20);

u32 func_002192C0(s32 arg0) {
    return *(u32 *)(arg0 + 8);
}

u16 func_002192C8(s32 arg0) {
    return *(u16 *)(arg0 + 0xc);
}

INCLUDE_ASM(const s32, "game/code_00218B48", func_002192D0);

s32 * func_00219350(s32 arg0) {
    s32 *p = (s32 *)(arg0 + 8);

    if (*p == 0xffff) {
        p = NULL;
    }
    return p;
}

s32 * func_00219368(s32 arg0) {
    s32 *p = (s32 *)(arg0 + *(s32 *)(arg0 + 4));

    if (*p == 0xffff) {
        p = NULL;
    }
    return p;
}

INCLUDE_ASM(const s32, "game/code_00218B48", func_00219388);

u8 func_002193D8(s32 *arg0, s32 arg1) {
    return *arg0 == arg1;
}

u16 func_002193E8(s32 arg0) {
    return *(u16 *)(arg0 + 0xe);
}

u16 func_002193F0(s32 arg0) {
    return *(u16 *)(arg0 + 0x10);
}

INCLUDE_ASM(const s32, "game/code_00218B48", func_002193F8);

INCLUDE_ASM(const s32, "game/code_00218B48", func_00219590);

INCLUDE_ASM(const s32, "game/code_00218B48", func_002198D8);

void func_00219AF8(s32 arg0, s32 arg1) {
    s32 entry = *(s32 *)(arg0 + 0xc) + (*(s16 *)(arg0 + 4) << 4);

    *(s32 *)(entry + 4) = 0;
    *(s32 *)(entry + 0) = 0;
    *(s32 *)(entry + 8) = func_00151D88(1, arg1);
    *(s16 *)(arg0 + 4) += 1;
}

void func_00219B50(s32 arg0, s32 arg1) {
    s32 entry = *(s32 *)(arg0 + 0xc) + (*(s16 *)(arg0 + 4) << 4);

    *(s32 *)(entry + 0) = 1;
    *(s32 *)(entry + 4) = 0;
    *(s32 *)(entry + 8) = func_0014FD20(arg1);
    *(s16 *)(arg0 + 4) += 1;
}

INCLUDE_ASM(const s32, "game/code_00218B48", func_00219BB0);

void func_00219C38(s32 arg0) {
    if (*(u8 *)(arg0 + 9) != 0) {
        func_002EBB60((void *)(arg0 + 0x20));
    }
    func_002D0918(*(s32 *)(arg0 + 4));
    func_002CFF98((void *)arg0);
}

void func_00219C78(s32 arg0, s32 arg1, s32 arg2) {
    if (*(u8 *)(arg0 + 9) == 0) {
        *(u8 *)(arg0 + 9) = 1;
        func_002EC780(arg0 + 0x20, arg2, *(s32 *)(arg0 + 0), *(s32 *)(arg0 + 0x10), arg1);
    }
}

void func_00219CC8(u32 arg0) {
    func_002E75F0(arg0, 0x10, 4);
}

INCLUDE_ASM(const s32, "game/code_00218B48", func_00219CE8);

INCLUDE_ASM(const s32, "game/code_00218B48", func_00219DD8);

void func_00219E30(s32 arg0) {
    func_00151E60(*(u32 *)(arg0 + 8));
    *(s32 *)(arg0 + 4) = *(s32 *)(arg0 + 4) + 1;
}

void func_00219E68(s32 arg0) {
    func_0014FEB0(*(u32 *)(arg0 + 8));
    *(s32 *)(arg0 + 4) = *(s32 *)(arg0 + 4) + 1;
}

s32 func_00219EA0(s32 arg0, s32 arg1) {
    s32 tmp;

    tmp = *(s32 *)(*(s32 *)(arg0 + 0xc) + 0xa8);
    if (tmp == 0) {
        return 0;
    }
    if (arg1 >= *(s16 *)(tmp + 4)) {
        return 0;
    }
    return *(s32 *)(tmp + 0xc) + arg1 * 0x10;
}

INCLUDE_ASM(const s32, "game/code_00218B48", func_00219ED8);

INCLUDE_ASM(const s32, "game/code_00218B48", func_00219FD8);

void func_0021A088(u32 arg0, s32 arg1) {
    func_00217310(arg0, *(u16 *)(arg1 + 8), *(u16 *)(arg1 + 10),
                                *(u32 *)(arg1 + 0xc), *(u32 *)(arg1 + 0x10));
}

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021A0B0);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021A170);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021A1D0);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021A268);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021A2E0);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021A368);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021A3D8);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021A490);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021A560);

void func_0021A5B8(s32 arg0, s32 arg1, float arg2) {
    switch (*(u16 *)(arg1 + 4)) {
    case 0:
        func_00152000(*(s32 *)(arg1 + 8), arg2, arg2);
        return;
    case 1:
        func_0014FB70(*(s32 *)(arg1 + 8), arg2);
        break;
    }
}

s32 func_0021A608(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    return func_0011D3E8(arg0, arg1, arg2, arg3, arg4, 0x30000000, 0x60404040);
}

void func_0021A628(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    s32 tmp;

    tmp = D_003D7B10[0];
    func_002D4038(tmp, func_0021A608(arg0, arg1, arg2, arg3, arg4));
}

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021A660);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021A718);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021A880);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021A8F0);

void func_0021A948(void) {
    s32 i = D_003D7A50.unk16 - 1;
    s32 saved = D_003D7A50.unk90[i];

    if (i > 0) {
        do {
            D_003D7A50.unk90[i] = D_003D7A50.unk8C[i];
            i -= 1;
        } while (i > 0);
    }
    D_003D7A50.unk90[0] = saved;
}

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021A998);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021A9F8);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021AAB8);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABBB0);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABBC0);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABBD0);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABBE0);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABBF0);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABC00);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABC10);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABC20);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABC30);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABC40);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABC50);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABC60);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABC70);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABC80);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021ABB8);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021ACF0);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021AD78);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABCC8);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABCD8);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021AE00);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021B0B0);

u32 func_0021B4E8(void) {
    func_0021AE00();
    func_0021B0B0();
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021B510);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021B950);

u32 func_0021B9D0(void) {
    func_0021B510();
    func_0021B950();
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021B9F8);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021BDD0);

s32 func_0021BE50(void) {
    func_0021B9F8();
    if (D_003D7A60[0] == 0) {
        func_0021BDD0();
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021BE88);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021BFB0);

u32 func_0021C1E0(void) {
    func_0021BE88();
    func_0021BFB0();
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021C208);

void func_0021C2E0(void) {
}

u32 func_0021C2E8(void) {
    func_0021C208();
    func_0021C2E0();
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021C310);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021C518);

u32 func_0021C678(void) {
    func_0021C310();
    func_0021C518();
    return 0;
}

void func_0021C6A0(void) {
    s32 p = *(s32 *)(*(s32 *)(D_003D7A50.unk90[0] + 0x18) + 8);

    if (p != 0) {
        s16 v = *(s16 *)(p + 4);

        if (v > 0) {
            func_0021A660((void *)((s32)&D_003D7A50 + 0x3A), v);
        }
    }
}

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABD58);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABD68);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABD78);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABD88);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABD98);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021C6E8);

u32 func_0021C8D0(void) {
    func_0021C6A0();
    func_0021C6E8();
    return 0;
}

s32 func_0021C8F8(void) {
    return kwlnTaskGetTaskByName("DebugTimeGrph") != 0;
}

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021C920);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABE18);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABE30);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABE48);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABE60);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABE70);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABE80);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021CB80);

u32 func_0021CE70(void) {
    func_0021C920();
    func_0021CB80();
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021CE98);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021CF00);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021CFC0);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021D198);

u32 func_0021D568(void) {
    func_0021CFC0();
    func_0021D198();
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021D590);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021D5D0);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021D668);

void func_0021D740(void) {
    if (D_003D7A50.unk04 != 0) {
        kwlnTaskDestroyWithHierarchy(D_003D7A50.unk04, 0);
        D_003D7A50.unk04 = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021D780);

INCLUDE_ASM(const s32, "game/code_00218B48", modelViewer);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021DAE0);

INCLUDE_ASM(const s32, "game/code_00218B48", modelViewerEnd);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021DD88);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021E068);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021E1C8);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021E360);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021E3C0);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021E450);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021E618);

void func_0021EB10(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 temp = arg2 & 0xffffff;

    func_0011E080(D_003D7B50.unk08, arg0, arg1,
                  (D_003D7B50.unk04 == 0) ? -1 : arg3, temp | 0x80000000, 1, temp);
}

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021EB60);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021ECF0);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021F098);

void func_0021F4B8(void) {
    func_0021F4E8();
    func_002286F8(0);
    func_00228778();
    func_00228728();
}

void func_0021F4E8(void) {
    s32 i = 0x7f;
    u32 *p = (u32 *)(D_003BAA00 + 0x840);

    do {
        i -= 1;
        *p = 0;
        p += 1;
    } while (i >= 0);
}

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021F520);

void func_0021F580(s32 arg0) {
    s32 temp_v0 = (arg0 < 0) ? arg0 + 0x1f : arg0;
    s32 off = (temp_v0 >> 5) * 4 + 0x840;
    *(u32 *)(D_003BAA00 + off) |= 1 << arg0;
}

void func_0021F5C0(s32 arg0) {
    s32 temp_v0 = (arg0 < 0) ? arg0 + 0x1f : arg0;
    s32 off = (temp_v0 >> 5) * 4 + 0x840;
    *(u32 *)(D_003BAA00 + off) &= ~(1 << arg0);
}

s32 func_0021F600(s32 arg0) {
    s32 temp_v0 = (arg0 < 0) ? arg0 + 0x1f : arg0;
    s32 off = (temp_v0 >> 5) * 4 + 0x840;
    return (*(s32 *)(D_003BAA00 + off) >> arg0) & 1;
}

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABF78);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABF88);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABF98);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABFA8);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021F630);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021FA78);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021FB30);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021FC30);

INCLUDE_ASM(const s32, "game/code_00218B48", func_0021FD50);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABFE8);

INCLUDE_RODATA(const s32, "game/code_00218B48", D_003ABFF8);

