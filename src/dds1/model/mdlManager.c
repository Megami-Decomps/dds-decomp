#include "common.h"

extern s32 func_00217E10(void);

extern u64 func_00288B88(u64);
extern u64 func_00288B90(void);
extern u32 func_002EB090(u64);

extern u32 D_003BD878;

extern u64 func_00216708(void);

INCLUDE_ASM(const s32, "model/mdlManager", func_00216B78);

INCLUDE_ASM(const s32, "model/mdlManager", func_00216BB0);

INCLUDE_ASM(const s32, "model/mdlManager", func_00216C00);

void func_00216CC8(u64 arg0, u64 arg1, u64 arg2) {
    u64 temp_v0;

    temp_v0 = func_00216708();
    func_00216C00(temp_v0, arg2);
}

INCLUDE_ASM(const s32, "model/mdlManager", func_00216CF8);

INCLUDE_ASM(const s32, "model/mdlManager", func_00216DB8);

INCLUDE_ASM(const s32, "model/mdlManager", func_00216E68);

void func_00216EC0(u32 arg0) {
    u16 *puVar1;

    puVar1 = (u16 *)arg0;
    func_00216DB8(*puVar1, puVar1[1], *(u32 *)(puVar1 + 4), puVar1 + 6);
    WaitSema(D_003BD878);
    func_002167E0(*puVar1, puVar1[1]);
    SignalSema(D_003BD878);
    func_002CFF98(arg0);
}

void func_00216F18(u64 arg0, s32 arg1) {
    u64 temp_v0;
    u32 temp_v1;

    temp_v0 = func_00288B90();
    temp_v1 = func_002EB090(temp_v0);
    *(u32 *)(arg1 + 0xc) = temp_v1;
    temp_v0 = func_00288B88(arg0);
    func_002D0918(temp_v0);
    func_002887A0(arg0);
}

INCLUDE_ASM(const s32, "model/mdlManager", func_00216F68);

INCLUDE_ASM(const s32, "model/mdlManager", func_00216FE0);

INCLUDE_ASM(const s32, "model/mdlManager", func_00217038);

INCLUDE_ASM(const s32, "model/mdlManager", func_00217068);

void func_00217298(u32 arg0, u32 arg1) {
    func_00217068(arg0, arg1, 1);
}

INCLUDE_ASM(const s32, "model/mdlManager", func_002172B0);

INCLUDE_ASM(const s32, "model/mdlManager", func_00217310);

INCLUDE_ASM(const s32, "model/mdlManager", func_00217438);

INCLUDE_ASM(const s32, "model/mdlManager", func_002174C0);

INCLUDE_ASM(const s32, "model/mdlManager", func_00217680);

INCLUDE_ASM(const s32, "model/mdlManager", func_002177D0);

INCLUDE_ASM(const s32, "model/mdlManager", func_00217878);

INCLUDE_ASM(const s32, "model/mdlManager", func_002179A8);

INCLUDE_ASM(const s32, "model/mdlManager", func_00217C60);

INCLUDE_ASM(const s32, "model/mdlManager", func_00217CA8);

INCLUDE_ASM(const s32, "model/mdlManager", func_00217DA0);

INCLUDE_ASM(const s32, "model/mdlManager", func_00217DC0);

INCLUDE_ASM(const s32, "model/mdlManager", func_00217DE0);

INCLUDE_ASM(const s32, "model/mdlManager", func_00217DF8);

INCLUDE_ASM(const s32, "model/mdlManager", func_00217E10);

INCLUDE_ASM(const s32, "model/mdlManager", func_00217E58);

INCLUDE_ASM(const s32, "model/mdlManager", func_00217E88);

INCLUDE_ASM(const s32, "model/mdlManager", func_00217EB0);

INCLUDE_ASM(const s32, "model/mdlManager", func_00217EE0);

INCLUDE_ASM(const s32, "model/mdlManager", func_00217F18);

INCLUDE_ASM(const s32, "model/mdlManager", func_00217F40);

INCLUDE_ASM(const s32, "model/mdlManager", func_00217F70);

INCLUDE_ASM(const s32, "model/mdlManager", func_00217F88);

INCLUDE_ASM(const s32, "model/mdlManager", func_00217FA0);

INCLUDE_ASM(const s32, "model/mdlManager", func_00217FB8);

INCLUDE_ASM(const s32, "model/mdlManager", func_00218010);

INCLUDE_ASM(const s32, "model/mdlManager", func_00218028);

u32 func_00218040(s32 arg0) {
    return *(u32 *)(*(s32 *)(arg0 + 0x18) + 0x1c);
}

void func_00218050(u32 arg0, u32 arg1) {
    u32 *puVar1;

    for (puVar1 = *(u32 **)((s32)arg0 + 0x14); puVar1 != (u32 *)0x0;
            puVar1 = (u32 *)*puVar1) {
        func_0021A560(arg0, puVar1, arg1);
    }
}

INCLUDE_ASM(const s32, "model/mdlManager", func_002180A8);

INCLUDE_ASM(const s32, "model/mdlManager", func_002180E0);

INCLUDE_ASM(const s32, "model/mdlManager", func_00218100);

INCLUDE_ASM(const s32, "model/mdlManager", func_00218158);

INCLUDE_ASM(const s32, "model/mdlManager", func_00218228);

void func_00218320(s32 arg0) {
    u32 *puVar1;

    for (puVar1 = *(u32 **)(*(s32 *)(arg0 + 0x18) + 0x14); puVar1 != (u32 *)0x0;
            puVar1 = (u32 *)*puVar1) {
        func_002DB768(puVar1);
    }
}

void func_00218368(s32 arg0) {
    u32 *puVar1;

    for (puVar1 = *(u32 **)(*(s32 *)(arg0 + 0x18) + 0x14); puVar1 != (u32 *)0x0;
            puVar1 = (u32 *)*puVar1) {
        func_002DB788(puVar1);
    }
}

u8 func_002183B0(void) {
    s64 temp_v0;

    temp_v0 = func_00217E10();
    return temp_v0 != 0;
}

u16 func_002183D0(s32 arg0) {
    return *(u16 *)(*(s32 *)(arg0 + 0xc) + 8);
}

u16 func_002183E0(s32 arg0) {
    return *(u16 *)(*(s32 *)(arg0 + 0xc) + 10);
}

u32 func_002183F0(void) {
    return 8;
}

INCLUDE_ASM(const s32, "model/mdlManager", func_002183F8);

INCLUDE_ASM(const s32, "model/mdlManager", func_00218410);

void func_00218440(s32 arg0) {
    func_002DA1B0(*(u32 *)(*(s32 *)(arg0 + 0x18) + 8));
}

INCLUDE_ASM(const s32, "model/mdlManager", func_00218460);

INCLUDE_ASM(const s32, "model/mdlManager", func_00218580);

INCLUDE_ASM(const s32, "model/mdlManager", func_002185B0);

INCLUDE_ASM(const s32, "model/mdlManager", func_00218768);

void func_002189D8(u32 arg0) {
    func_00288788(*(u32 *)((s32)arg0 + 8));
    func_002CFF98(arg0);
}

INCLUDE_ASM(const s32, "model/mdlManager", func_00218A08);

INCLUDE_ASM(const s32, "model/mdlManager", func_00218A88);
