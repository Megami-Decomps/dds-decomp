#include "common.h"

extern s32 kwlnTaskDestroyWithHierarchy(s32 task, s32 flag);

void *func_002CFEB8(s32 size);

void func_0022A850(s32 arg0);

void func_00101A68(s32 arg0, void *arg1);

void func_00101A80(s32 arg0, s32 arg1);

s32 func_0010D6A0(void);

char *func_0010D5A8(s32 idx);

void func_0010D5F0(s32 value);

extern char D_003ACAE0[];

void func_002287C0(void);

void func_0022AB00(s32 arg0);

extern u32 D_003BBDC0;

extern char D_003BBDC8[];

extern s32 kwlnTaskCreate(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6);

extern u64 func_00101A70(void);

extern u32 D_003BBDB8;

extern s32 D_003BAA00;

extern s64 func_001019C8(u64);

extern u64 func_0010D428(u64);

INCLUDE_ASM(const s32, "game/code_00228058", func_00228058);

INCLUDE_RODATA(const s32, "game/code_00228058", D_003ACA78);

INCLUDE_ASM(const s32, "game/code_00228058", func_002280B0);

u32 func_00228180(void) {
    s32 v0;
    s32 v1;

    v0 = func_0010D6A0();
    if (v0 == 0) {
        return 1;
    }
    if (*(s32 *)(v0 + 0xe4) == 0) {
        func_003003F0(D_003ACAE0);
        return 1;
    }
    v1 = func_00235270(0x2afe, func_0010D5A8(0));
    func_00101A80(*(s32 *)(v0 + 0xe4), v1);
    func_0010D5F0(v1);
    return 1;
}

u32 func_00228210(void) {
    u64 temp_v0;
    s64 temp_v1;

    temp_v0 = func_0010D428(0);
    temp_v1 = func_001019C8(temp_v0);
    if (temp_v1 != 0) {
        func_00235088(temp_v0);
    }
    return 1;
}

u32 func_00228258(void) {
    u64 temp_v0;
    s64 temp_v1;

    temp_v0 = func_0010D428(0);
    temp_v1 = func_001019C8(temp_v0);
    if (temp_v1 != 0) {
        func_002350B0(temp_v0);
    }
    return 1;
}

u32 func_002282A0(void) {
    u64 temp_v0;
    s64 temp_v1;

    temp_v0 = func_0010D428(0);
    temp_v1 = func_001019C8(temp_v0);
    if (temp_v1 != 0) {
        func_002350E0(temp_v0);
    }
    return 1;
}

u32 func_002282E8(void) {
    u64 temp_v0;
    u64 temp_v1;
    u64 temp_v2;

    func_00132B60(0);
    func_00132B70(0x80);
    temp_v0 = func_0010D428(0);
    temp_v1 = func_0010D428(1);
    temp_v2 = func_0010D428(2);
    func_00132AE8(temp_v0, temp_v1, temp_v2);
    return 1;
}

INCLUDE_RODATA(const s32, "game/code_00228058", D_003ACAE0);

u32 func_00228350(void) {
    s64 v;

    v = func_0010D428(1);
    if (v < -255) {
        func_0010AC10("warning : SET_SKY_A alpha < -255\n");
        v = -255;
    }
    if (v > 255) {
        func_0010AC10("warning : SET_SKY_A alpha > 255\n");
        v = 255;
    }
    func_00235348(func_0010D428(0), v);
    return 1;
}

u32 func_002283C8(void) {
    func_00235340(1);
    return 1;
}

u32 func_002283E8(void) {
    func_00235340(0);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00228058", func_00228408);

u32 func_00228460(void) {
    u64 temp_v0;
    u64 temp_v1;

    temp_v0 = func_0010D428(0);
    temp_v1 = func_0010D428(1);
    func_00241938(temp_v0, temp_v1);
    return 1;
}

u32 func_002284A0(void) {
    u64 temp_v0;
    u64 temp_v1;

    temp_v0 = func_0010D428(0);
    temp_v1 = func_0010D428(1);
    func_00241990(temp_v0, temp_v1);
    return 1;
}

u32 func_002284E0(void) {
    u64 temp_v0;
    u64 temp_v1;

    temp_v0 = func_0010D428(0);
    temp_v1 = func_0010D428(1);
    func_00241A50(temp_v0, temp_v1);
    return 1;
}

u32 func_00228520(void) {
    func_002E9708();
    func_001F3448();
    return 1;
}

u32 func_00228548(void) {
    func_002E9730();
    func_001F3448();
    return 1;
}

u32 func_00228570(void) {
    u64 temp_v0;
    u64 temp_v1;

    temp_v0 = func_0010D428(0);
    temp_v1 = func_0010D428(1);
    func_00241A98(temp_v0, temp_v1);
    return 1;
}

u32 func_002285B0(void) {
    u64 temp_v0;
    u64 temp_v1;

    temp_v0 = func_0010D428(0);
    temp_v1 = func_0010D428(1);
    func_00241AE8(temp_v0, temp_v1);
    return 1;
}

u32 func_002285F0(void) {
    u64 temp_v0;
    u64 temp_v1;

    temp_v0 = func_0010D428(0);
    temp_v1 = func_0010D428(1);
    func_00241CA0(temp_v0, temp_v1);
    return 1;
}

u32 func_00228630(void) {
    func_0018F570();
    return 1;
}

INCLUDE_ASM(const s32, "game/code_00228058", func_00228650);

s32 func_002286C0(void) {
    s32 v;

    v = *(u8 *)(D_003BAA00 + 0xa41);
    if (v >= 9) {
        v = 8 - (v & 7);
    }
    return v;
}

u8 func_002286E8(void) {
    return *(u8 *)(D_003BAA00 + 0xa41);
}

void func_002286F8(u8 arg0) {
    *(u8 *)(D_003BAA00 + 0xa41) = arg0 & 0xf;
    *(u32 *)(D_003BAA00 + 0xa44) = 0;
}

void func_00228710(void) {
    *(u8 *)(D_003BAA00 + 0xa40) = *(u8 *)(D_003BAA00 + 0xa40) | 1;
}

void func_00228728(void) {
    *(u8 *)(D_003BAA00 + 0xa40) = *(u8 *)(D_003BAA00 + 0xa40) & 0xfe;
}

void func_00228740(void) {
    *(u8 *)(D_003BAA00 + 0xa40) = *(u8 *)(D_003BAA00 + 0xa40) | 2;
    *(f32 *)&D_003BBDB8 = 1.0f;
    func_0022AB00(0);
}

void func_00228778(void) {
    *(u8 *)(D_003BAA00 + 0xa40) = *(u8 *)(D_003BAA00 + 0xa40) & 0xfd;
    D_003BBDB8 = 0;
}

void func_00228790(void) {
    *(u8 *)(D_003BAA00 + 0xa40) = *(u8 *)(D_003BAA00 + 0xa40) | 2;
}

void func_002287A8(void) {
    *(u8 *)(D_003BAA00 + 0xa40) = *(u8 *)(D_003BAA00 + 0xa40) & 0xfd;
}

INCLUDE_ASM(const s32, "game/code_00228058", func_002287C0);

INCLUDE_ASM(const s32, "game/code_00228058", func_00228930);

void *func_00228A00(s32 arg0) {
    s32 p;

    p = (s32)func_002CFEB8(0x104);
    func_0022A850(p);
    func_00228B38((u32 *)p);
    func_00101A68(arg0, (void *)p);
    return (void *)func_002287C0;
}

void func_00228A58(void) {
    u64 temp_v0;

    temp_v0 = func_00101A70();
    func_00228B78(temp_v0);
    func_002CFF98(temp_v0);
    D_003BBDC0 = 0;
}

void func_00228A98(void) {
    if (D_003BBDC0 != 0) {
        return;
    }
    D_003BBDC0 = kwlnTaskCreate((s32)D_003BBDC8, 0x2b0b, 1, 1, (s32)func_00228A00, (s32)func_00228A58, 0);
    func_00228710();
    func_00228778();
    func_002286F8(0);
    *(u8 *)(D_003BAA00 + 0xa42) = 0;
}

void func_00228B00(void) {
    if (D_003BBDC0 != 0) {
        kwlnTaskDestroyWithHierarchy(D_003BBDC0, 1);
    }
}




INCLUDE_SDATA(const s32, "game/code_00228058", D_003BBDB8);

INCLUDE_SDATA(const s32, "game/code_00228058", D_003BBDBC);

INCLUDE_SDATA(const s32, "game/code_00228058", D_003BBDC0);


INCLUDE_SDATA(const s32, "game/code_00228058", D_003BBDC8);

