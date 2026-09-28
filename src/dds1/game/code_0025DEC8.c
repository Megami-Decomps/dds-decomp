#include "common.h"

extern s32 func_002877A8(void);

extern s32 func_00105C48(void);

extern char D_003AFD80[]; /* "titleProc" */

extern u32 D_003DA180[];

extern u32 D_003D9140[];

extern u32 D_003BD8D0;

extern u32 D_003BD8D4;

extern u32 D_003BC5BC;
extern u32 D_003BC5C8;
extern u8 D_003D9178[];

extern s32 func_0026A720(void);

extern u64 func_002E97E0(void);

extern u32 func_00265E68(u32, s32);

extern s8 D_003BC529;

extern s8 D_003BC52A;

extern s8 D_003BC52B;

extern u32 D_003BAA9C;

extern u64 func_00197C40(u64, u64, u64, u16, u32, u64);

typedef struct { u64 v; } __attribute__((packed)) u64p;

extern u8 D_003BC590[];

extern u8 D_003BC598[];

extern char *strcat(char *, char *);

extern s32 func_0021F600(u32);

extern s32 func_00101A70();

extern s32 D_003BC588;

void func_0025DEC8(s32 arg0, u64 arg1, u64 arg2, u64 arg3,
                                    u64 arg4) {
    s32 temp_v0;
    u64 temp_v1;

    temp_v0 = *(s32 *)(*(s32 *)(arg0 + 0x70) + 0x14);
    if (*(s32 *)(temp_v0 + 0x20) != 0) {
        temp_v1 = func_00197C40(0x970, 0xb58, 1, *(u16 *)(*(s32 *)(temp_v0 + 0x1c) + 100), D_003BAA9C,
                                                    arg2);
        func_001954C8(temp_v1, arg3);
        func_001958A0(temp_v1, 1, arg4);
        func_00194920(temp_v1);
        return;
    }
}

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_0025DF68);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_0025E108);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_0025E308);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_0025E420);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_0025E508);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_0025E5D8);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_0025E6B0);

INCLUDE_RODATA(const s32, "game/code_0025DEC8", D_003AF9F0);

INCLUDE_RODATA(const s32, "game/code_0025DEC8", D_003AFA00);

INCLUDE_RODATA(const s32, "game/code_0025DEC8", D_003AFA18);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_0025E820);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_0025ECD0);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_0025F138);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_0025F378);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_0025F408);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_0025F4E0);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_0025F680);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_0025F7F0);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_0025FB30);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_0025FC38);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_0025FD50);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_0025FE68);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_0025FEB8);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_0025FFC8);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_00260100);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_00260208);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_00260370);

void func_00260530(s32 arg0, u32 arg1) {
    u32 *puVar1;

    puVar1 = *(u32 **)(arg0 + 0x30);
    if (puVar1 != (u32 *)0x0) {
        *puVar1 = arg1;
        puVar1[1] = 1;
    }
}

void func_00260550(s32 arg0, u32 arg1) {
    u32 *puVar1;

    puVar1 = *(u32 **)(arg0 + 0x30);
    if (puVar1 != (u32 *)0x0) {
        *puVar1 = arg1;
        puVar1[1] = 2;
    }
}

void func_00260570(s32 arg0, u32 arg1) {
    u32 *puVar1;

    puVar1 = *(u32 **)(arg0 + 0x30);
    if (puVar1 != (u32 *)0x0) {
        *puVar1 = arg1;
        puVar1[1] = 1;
    }
}

void func_00260590(s32 arg0, u32 arg1) {
    u32 *puVar1;

    puVar1 = *(u32 **)(arg0 + 0x30);
    if (puVar1 != (u32 *)0x0) {
        *puVar1 = arg1;
        puVar1[1] = 2;
    }
}

void func_002605B0(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0xac) = arg1;
    *(u32 *)(arg0 + 0xa8) = 0;
}

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_002605C0);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_00260670);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_002609D8);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_00260AB0);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_00261688);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_00261760);

void func_00261F58(void) {
    func_00262818();
    D_003BC52A = 0;
    D_003BC52B = 1;
}

s8 func_00261F80(void) {
    return D_003BC52B;
}

u32 func_00261F88(void) {
    D_003BC52A = 1;
    return 1;
}

void func_00261F98(void) {
    func_00262938();
}

s8 func_00261FB0(void) {
    return D_003BC529;
}

s8 func_00261FB8(s32 arg0) {
    if (*(s32 *)(arg0 + 0xd44) != 0) {
        D_003BC52B = 0;
    }
    return D_003BC52B ? 0 : D_003BC52A;
}

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_00261FD8);

void func_00262038(s32 arg0) {
    func_001198B8(*(u32 *)(arg0 + 0xc));
}

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_00262050);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_00262148);

void func_002622B0(u32 arg0, u32 arg1, u32 arg2) {
    func_00261FD8(arg1);
    func_00262038(arg1);
    func_00262148(arg0, arg2);
}

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_00262300);

void func_00262398(s32 arg0) {
    s32 temp_v0;

    temp_v0 = arg0 + 0x680;
    func_002BDD60(*(u32 *)(arg0 + 0x90));
    func_0027F6B8(temp_v0);
    func_0027FA20(temp_v0);
    func_00280488(temp_v0);
    func_00283038(*(u32 *)(arg0 + 0xd10));
    func_002832F8(*(u32 *)(arg0 + 0xd14));
    func_0027B010(arg0 + 0xd1c);
    func_00276320(arg0 + 0x4f8);
    func_00271648(arg0 + 0x4f8);
    func_00287548();
}

INCLUDE_RODATA(const s32, "game/code_0025DEC8", D_003AFA88);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_00262418);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_002624C0);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_00262570);

void func_00262600(u32 arg0, u32 arg1, u32 arg2) {
    func_00262570(arg0, arg1, 2);
    func_00262570(arg0, arg2, 1);
}

void func_00262640(s32 arg0) {
    if (*(s32 *)(arg0 + 0x344) == 0) {
        D_003BC529 = 0;
    } else {
        D_003BC529 = 1;
    }
}

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_00262660);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_00262790);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_00262818);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_002628C8);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_00262938);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_00262970);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_002629A8);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_00262A30);

s32 func_00262A88(void) {
    if (func_00105C48() != 0) {
        return 0;
    }
    return func_002877A8() != 1;
}

void func_00262AC0(u32 arg0, s32 arg1) {
    func_00285A68(arg1 + 0x574);
    func_0027FF60(arg1 + 0x680);
    func_00280048(arg1 + 0x680);
}

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_00262AF8);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_00262BA8);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_00262C08);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_00262CE8);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_00262EB8);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_00262F90);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_00262FF0);

u32 func_00263050(void) {
    return 1;
}

u32 func_00263058(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_00263060);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_002630B0);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_00263100);

u32 func_00263138(void) {
    return 1;
}

u32 func_00263140(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_00263148);

u32 func_00263220(u32 arg0) {
    u32 temp_v0;
    s32 temp_v1;

    temp_v1 = (s32)arg0;
    temp_v0 = func_00265E68(**(u32 **)(temp_v1 + 0x98), temp_v1 + 0x4c4);
    *(u32 *)(temp_v1 + 0x244) = temp_v0;
    func_00263148(arg0);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_00263260);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_00263378);

u32 func_002633D8(void) {
    s64 temp_v0;

    temp_v0 = func_0021F600(0x911);
    if (temp_v0 == 0) {
        func_0021F580(0x911);
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_00263408);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_00263570);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_002635C0);

s32 func_00263608(void) {
    s32 *temp_v0 = (s32 *)func_00101A70();
    temp_v0[144] = 0;
    temp_v0[241] = 0;
    return 1;
}

u32 func_00263638(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_00263640);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_00263728);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_00263838);

void func_002639E0(s32 arg0) {
    func_0027B268(arg0 + 0xd1c, 0x20);
}

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_00263A00);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_00263B78);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_00263C98);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_00263D10);

u32 func_00263D70(void) {
    s8 temp_v0;
    s32 temp_v1;
    u32 *puVar3;
    s8 *pcVar4;
    s32 temp_v2;
    s32 temp_v3;
    s32 temp_v4;

    temp_v1 = func_00101A70();
    temp_v4 = 0;
    temp_v2 = 4;
    temp_v3 = (*(s32 **)(temp_v1 + 0x98))[1] * 3;
    pcVar4 = (s8 *)(**(s32 **)(temp_v1 + 0x98) + 0x16);
    do {
        temp_v0 = *pcVar4;
        pcVar4 = pcVar4 + 1;
        temp_v2 = temp_v2 - 1;
        temp_v4 = temp_v4 + temp_v0;
    } while (-1 < temp_v2);
    *(u32 *)(temp_v1 + 0x3cc) = 0;
    temp_v2 = 4;
    puVar3 = (u32 *)(temp_v1 + 0x3e0);
    if (0x1ef - temp_v4 < temp_v3) {
        temp_v3 = 0x1ef - temp_v4;
    }
    *(s32 *)(temp_v1 + 0x3c8) = temp_v3;
    do {
        temp_v2 = temp_v2 - 1;
        *puVar3 = 0;
        puVar3 = puVar3 + -1;
    } while (-1 < temp_v2);
    if (*(s32 *)(temp_v1 + 0x1578) != 0) {
        func_002830F0(*(u32 *)(temp_v1 + 0xd10), 0);
    }
    return 1;
}

u32 func_00263E30(void) {
    return 1;
}

void func_00263E38(s32 arg0) {
    s32 temp_v0;
    u32 *puVar2;

    *(u32 *)(arg0 + 0x3cc) = 0;
    puVar2 = (u32 *)(arg0 + 0x3e0);
    temp_v0 = 4;
    do {
        temp_v0 = temp_v0 - 1;
        *puVar2 = 0;
        puVar2 = puVar2 + -1;
    } while (-1 < temp_v0);
}

void func_00263E70(u32 arg0, u32 arg1) {
    func_002CCE60(arg0, (s32)arg1 + 0x3d0);
    func_00262AC0(arg0, arg1);
}

s32 func_00263EB0(s32 arg0, s32 arg1) {
    s32 *temp_p = (s32 *)(arg1 + 0x3d0);
    s8 *temp_q = (s8 *)(arg0 + 0x16);
    s32 temp_i = 0;

    do {
        s32 temp_sum = *temp_q + *temp_p;

        temp_q++;
        temp_p++;
        if (temp_sum < 0x63) {
            return 0;
        }
        temp_i++;
    } while (temp_i < 5);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_00263EF8);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_002641E0);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_00264238);

u32 func_00264280(void) {
    s32 temp_v0;

    temp_v0 = func_00101A70();
    func_002830F0(*(u32 *)(temp_v0 + 0xd10), 0xffffffffffffffff);
    func_0024DA58(0x16);
    func_0024DAE8(0);
    func_0024DAB8(0x1d);
    return 1;
}

u32 func_002642C8(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_002642D0);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_00264498);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_002644F0);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_00264538);

u32 func_00264608(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_00264610);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_002646A0);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_002646F8);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_00264740);

u32 func_002647B0(void) {
    func_0024DBB0();
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_002647D0);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_002648A8);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_002649B0);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_00264A60);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_00264AB8);

INCLUDE_RODATA(const s32, "game/code_0025DEC8", D_003AFAB8);

INCLUDE_RODATA(const s32, "game/code_0025DEC8", D_003AFAC8);

INCLUDE_RODATA(const s32, "game/code_0025DEC8", D_003AFAD8);

INCLUDE_RODATA(const s32, "game/code_0025DEC8", D_003AFAE8);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_00264B08);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_00264D90);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_00264E90);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_00264EF0);

void func_00265078(void) {
}

void func_00265080(void) {
}

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_00265088);

u32 func_002650B0(s32 arg0) {
    return *(u32 *)(arg0 + 0x1574);
}

void func_002650B8(s32 arg0) {
    *(u32 *)(arg0 + 0x1574) = 0;
}

void func_002650C0(void) {
}

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_002650C8);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_00265220);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_002652E0);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_002653A0);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_00265478);

void func_002654E8(s32 arg0) {
    func_00265088(arg0);
}

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_00265500);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_00265540);

u32 func_00265590(void) {
    return 1;
}

u32 func_00265598(void) {
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_002655A0);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_00265610);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_00265648);

s32 func_002656C0(u8 *entry) {
    if (func_0021F600(0x902) == 0 && *(u16 *)(entry + 4) == 4) {
        return 1;
    }
    return 0;
}

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_00265700);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_002658B8);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_00265968);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_002659C8);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_00265AB8);

s32 func_00265BE0(u8 *entry) {
    s32 step = func_002658B8(entry);
    *(u16 *)(entry + 0x14) += step;
    func_002CD0C0(entry);
    return step;
}

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_00265C28);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_00265C90);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_00265E68);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_00265FD8);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_00266048);

void func_00266130(u32 arg0) {
    func_001953D8(arg0, 0xc, 0x10);
    func_001953A8(arg0, 0xfffffffffffffffc);
}

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_00266168);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_002661A8);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_00266250);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_002665E0);

INCLUDE_RODATA(const s32, "game/code_0025DEC8", D_003AFB20);

INCLUDE_RODATA(const s32, "game/code_0025DEC8", D_003AFB30);

INCLUDE_RODATA(const s32, "game/code_0025DEC8", D_003AFB40);

INCLUDE_RODATA(const s32, "game/code_0025DEC8", D_003AFBA0);

INCLUDE_RODATA(const s32, "game/code_0025DEC8", D_003AFBB0);

INCLUDE_RODATA(const s32, "game/code_0025DEC8", D_003AFBC0);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_00266668);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_00266908);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_00266B10);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_00266BC0);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_00266E28);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_002673C8);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_00267850);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_00267E20);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_00267FF0);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_00268590);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_002687C0);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_00268AB8);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_00268D40);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_002692E0);

void func_002693E0(u32 arg0) {
    func_002E8F78(arg0, 0x7f, 0x3f);
}

void func_00269400(void) {
}

void func_00269408(void) {
}

void func_00269410(void) {
}

void func_00269418(void) {
}

char *func_00269420(char *arg0, char *arg1) {
    *(u64p *)arg0 = *(u64p *)D_003BC590;
    return strcat(arg0, arg1);
}

char *func_00269450(char *arg0, char *arg1) {
    *(u64p *)arg0 = *(u64p *)D_003BC598;
    return strcat(arg0, arg1);
}

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_00269480);

u32 func_002694F8(void) {
    return 0x599;
}

u32 func_00269500(void) {
    s32 temp_v0;

    temp_v0 = func_00101A70();
    *(s32 *)(temp_v0 + 4) = *(s32 *)(temp_v0 + 4) + 1;
    return 0;
}

void func_00269530(void) {
    func_002CFF98(func_00101A70());
    D_003BC588 = 0;
}

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_00269558);

void func_002695C8(s32 effect) {
    s32 context = func_00101A70(D_003BC588);
    if (func_002E97E0() != 0) {
        func_002E97E8();
    }
    func_002E9788(effect, 0x7f);
    *(s32 *)(context + 4) = 0;
}

void func_00269628(s32 arg0) {
    *(s32 *)func_00101A70(D_003BC588) = arg0;
}

INCLUDE_RODATA(const s32, "game/code_0025DEC8", D_003AFBF0);

INCLUDE_RODATA(const s32, "game/code_0025DEC8", D_003AFC30);

INCLUDE_RODATA(const s32, "game/code_0025DEC8", D_003AFC40);

INCLUDE_RODATA(const s32, "game/code_0025DEC8", D_003AFC50);

INCLUDE_RODATA(const s32, "game/code_0025DEC8", D_003AFC60);

INCLUDE_RODATA(const s32, "game/code_0025DEC8", D_003AFC70);

INCLUDE_RODATA(const s32, "game/code_0025DEC8", D_003AFC80);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_00269658);

void func_002696F8(void) {
    func_002E97E8();
}

void func_00269710(void) {
    func_002E97E0();
}

void func_00269728(void) {
}

s32 func_00269730(void) {
    return *(s32 *)(func_00101A70(D_003BC588) + 4);
}

u32 func_00269758(void) {
    u64 temp_v0;

    temp_v0 = func_0010D428(0);
    func_002E8D10(temp_v0);
    return 1;
}

u32 func_00269780(void) {
    u64 temp_v0;

    temp_v0 = func_0010D428(0);
    func_002E8F78(temp_v0, 0x7f, 0x3f);
    return 1;
}

u32 func_002697B0(void) {
    func_002E8E50();
    return 1;
}

s32 func_002697D0(void) {
    s32 value;
    value = func_0010D5A8(0);
    if (func_002E97E0() != 0) {
        func_002E97E8();
    }
    func_002E9788(value, 0x7f);
    return 1;
}

u32 func_00269820(void) {
    func_002E97E8();
    return 1;
}

u32 func_00269840(void) {
    u64 temp_v0;

    temp_v0 = func_002E97E0();
    func_0010D5F0(temp_v0);
    return 1;
}

u32 func_00269868(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_00269870);

u32 func_002698C0(void) {
    func_0026A778();
    return 1;
}

u32 func_002698E0(void) {
    func_0026A808();
    func_0026A950();
    return 1;
}

u32 func_00269908(void) {
    func_0026A840();
    return 1;
}

u8 func_00269928(void) {
    s64 temp_v0;

    temp_v0 = func_0026A720();
    return temp_v0 == 0;
}

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_00269948);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_002699A0);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_00269A68);

void func_00269B50(u32 arg0) {
    sceSdRemoteInit();
    func_002699A0(arg0);
    D_003BC5BC = 0;
}

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_00269B80);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_00269C10);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_00269CA0);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_00269D18);

void func_0026A1C8(void) {
    for (;;) {
        func_002CFAD8(1);
        WaitSema(D_003BD8D0);
        func_00269D18();
        SignalSema(D_003BD8D0);
    }
}

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_0026A1F8);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_0026A248);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_0026A340);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_0026A390);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_0026A3E0);

void func_0026A460(void) {
    D_003BD8D4 = func_00288B48();
    D_003D9140[9] = 1;
}

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_0026A490);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_0026A588);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_0026A5F0);

u32 func_0026A6E0(void) {
    if (D_003D9140[9] == 1) {
        func_0026A490(D_003D9140);
    }
    return D_003D9140[9];
}

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_0026A720);

INCLUDE_RODATA(const s32, "game/code_0025DEC8", D_003AFCF0);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_0026A778);

void func_0026A808(void) {
    WaitSema(D_003BD8D0);
    D_003D9140[1] = 0;
    D_003D9140[4] = 2;
    SignalSema(D_003BD8D0);
}

void func_0026A840(void) {
    WaitSema(D_003BD8D0);
    if (D_003D9140[4] == 1 && D_003D9140[9] == 3) {
        D_003D9140[9] = 4;
        *(u32 *)D_003D9178 = 0;
        D_003BC5C8 = 6;
    }
    SignalSema(D_003BD8D0);
}

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_0026A8A0);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_0026A900);

void func_0026A950(void) {
    WaitSema(D_003BD8D0);
    func_0026A900();
    SignalSema(D_003BD8D0);
}

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_0026A980);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_0026AA28);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_0026ABA8);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_0026AD28);

void func_0026AD98(void) {
    WaitSema(D_003BD8D0);
    if (D_003DA180[4] != 1) {
        D_003DA180[4] = 0;
    }
    func_003003F0("---------- AT3 --------\n");
    SignalSema(D_003BD8D0);
}

void func_0026ADE8(void) {
    D_003DA180[1] = 0;
    D_003DA180[4] = 2;
    func_0026AE10();
}

void func_0026AE10(void) {
    u32 *temp_v0 = D_003DA180;
    u32 temp_v1 = temp_v0[8];

    if (temp_v1 == 0) {
        return;
    }
    func_002D0918(temp_v1);
    temp_v0[8] = 0;
}

void func_0026AE50(void) {
    WaitSema(D_003BD8D0);
    func_0026ADE8();
    SignalSema(D_003BD8D0);
}

void func_0026AE80(void) {
    WaitSema(D_003BD8D0);
    func_0026AE10();
    SignalSema(D_003BD8D0);
}

void func_0026AEB0(void) {
    func_0026BFC8();
}

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_0026AEC8);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_0026AF30);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_0026AF78);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_0026B020);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_0026B050);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_0026B160);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_0026B1C0);

INCLUDE_RODATA(const s32, "game/code_0025DEC8", D_003AFD80);

INCLUDE_ASM(const s32, "game/code_0025DEC8", func_0026B1F0);

u32 func_0026BCE8(void) {
    func_0026B050();
    return 0xffffffff;
}

s32 func_0026BD08(void) {
    func_0026B160();
    kwlnTaskDestroyWithHierarchyByName(D_003AFD80, 1);
    return 0;
}

u32 func_0026BD38(void) {
    func_0026B050(0);
    return 0;
}

void func_0026BD58(void) {
    func_0021FE38();
    func_00117730();
    func_001176A0();
}
