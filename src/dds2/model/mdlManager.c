#include "common.h"

extern s32 func_00232928(void);

INCLUDE_ASM(const s32, "model/mdlManager", func_00231690);

INCLUDE_ASM(const s32, "model/mdlManager", func_002316C8);

INCLUDE_ASM(const s32, "model/mdlManager", func_00231718);

INCLUDE_ASM(const s32, "model/mdlManager", func_002317E0);

INCLUDE_ASM(const s32, "model/mdlManager", func_00231810);

INCLUDE_ASM(const s32, "model/mdlManager", func_002318D0);

INCLUDE_ASM(const s32, "model/mdlManager", func_00231980);

INCLUDE_ASM(const s32, "model/mdlManager", func_002319D8);

INCLUDE_ASM(const s32, "model/mdlManager", func_00231A30);

INCLUDE_ASM(const s32, "model/mdlManager", func_00231A80);

INCLUDE_ASM(const s32, "model/mdlManager", func_00231AF8);

INCLUDE_ASM(const s32, "model/mdlManager", func_00231B50);

INCLUDE_ASM(const s32, "model/mdlManager", func_00231B80);

INCLUDE_ASM(const s32, "model/mdlManager", func_00231DB0);

INCLUDE_ASM(const s32, "model/mdlManager", func_00231DC8);

INCLUDE_ASM(const s32, "model/mdlManager", func_00231E28);

INCLUDE_ASM(const s32, "model/mdlManager", func_00231F50);

INCLUDE_ASM(const s32, "model/mdlManager", func_00231FD8);

INCLUDE_ASM(const s32, "model/mdlManager", func_00232198);

INCLUDE_ASM(const s32, "model/mdlManager", func_002322E8);

INCLUDE_ASM(const s32, "model/mdlManager", func_00232390);

INCLUDE_ASM(const s32, "model/mdlManager", func_002324C0);

INCLUDE_ASM(const s32, "model/mdlManager", func_00232778);

INCLUDE_ASM(const s32, "model/mdlManager", func_002327C0);

INCLUDE_ASM(const s32, "model/mdlManager", func_002328B8);

INCLUDE_ASM(const s32, "model/mdlManager", func_002328D8);

INCLUDE_ASM(const s32, "model/mdlManager", func_002328F8);

INCLUDE_ASM(const s32, "model/mdlManager", func_00232910);

INCLUDE_ASM(const s32, "model/mdlManager", func_00232928);

INCLUDE_ASM(const s32, "model/mdlManager", func_00232970);

INCLUDE_ASM(const s32, "model/mdlManager", func_002329A0);

INCLUDE_ASM(const s32, "model/mdlManager", func_002329C8);

INCLUDE_ASM(const s32, "model/mdlManager", func_002329F8);

INCLUDE_ASM(const s32, "model/mdlManager", func_00232A30);

INCLUDE_ASM(const s32, "model/mdlManager", func_00232A58);

INCLUDE_ASM(const s32, "model/mdlManager", func_00232A88);

INCLUDE_ASM(const s32, "model/mdlManager", func_00232AA0);

INCLUDE_ASM(const s32, "model/mdlManager", func_00232AB8);

INCLUDE_ASM(const s32, "model/mdlManager", func_00232AD0);

INCLUDE_ASM(const s32, "model/mdlManager", func_00232B28);

INCLUDE_ASM(const s32, "model/mdlManager", func_00232B40);

u32 func_00232B58(s32 arg0) {
    return *(u32 *)(*(s32 *)(arg0 + 0x18) + 0x1c);
}

void func_00232B68(u32 arg0, u32 arg1) {
    u32 *puVar1;

    for (puVar1 = *(u32 **)((s32)arg0 + 0x14); puVar1 != (u32 *)0x0;
            puVar1 = (u32 *)*puVar1) {
        func_002350D0(arg0, puVar1, arg1);
    }
}

INCLUDE_ASM(const s32, "model/mdlManager", func_00232BC0);

INCLUDE_ASM(const s32, "model/mdlManager", func_00232BF8);

INCLUDE_ASM(const s32, "model/mdlManager", func_00232C18);

INCLUDE_ASM(const s32, "model/mdlManager", func_00232C70);

INCLUDE_ASM(const s32, "model/mdlManager", func_00232D40);

void func_00232E38(s32 arg0) {
    u32 *puVar1;

    for (puVar1 = *(u32 **)(*(s32 *)(arg0 + 0x18) + 0x14); puVar1 != (u32 *)0x0;
            puVar1 = (u32 *)*puVar1) {
        func_00334618(puVar1);
    }
}

void func_00232E80(s32 arg0) {
    u32 *puVar1;

    for (puVar1 = *(u32 **)(*(s32 *)(arg0 + 0x18) + 0x14); puVar1 != (u32 *)0x0;
            puVar1 = (u32 *)*puVar1) {
        func_00334638(puVar1);
    }
}

u8 func_00232EC8(void) {
    s64 temp_v0;

    temp_v0 = func_00232928();
    return temp_v0 != 0;
}

u16 func_00232EE8(s32 arg0) {
    return *(u16 *)(*(s32 *)(arg0 + 0xc) + 8);
}

u16 func_00232EF8(s32 arg0) {
    return *(u16 *)(*(s32 *)(arg0 + 0xc) + 10);
}

u32 func_00232F08(void) {
    return 8;
}

INCLUDE_ASM(const s32, "model/mdlManager", func_00232F10);

INCLUDE_ASM(const s32, "model/mdlManager", func_00232F28);

INCLUDE_ASM(const s32, "model/mdlManager", func_00232F58);

INCLUDE_ASM(const s32, "model/mdlManager", func_00232F78);

INCLUDE_ASM(const s32, "model/mdlManager", func_00233098);

INCLUDE_ASM(const s32, "model/mdlManager", func_002330C8);

INCLUDE_ASM(const s32, "model/mdlManager", func_00233280);

INCLUDE_ASM(const s32, "model/mdlManager", func_002334F0);

INCLUDE_ASM(const s32, "model/mdlManager", func_00233520);

INCLUDE_ASM(const s32, "model/mdlManager", func_002335A0);
