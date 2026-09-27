#include "common.h"

extern u32 func_0019D920(void);

extern u32 D_00436568;

extern u32 D_00436554;

INCLUDE_ASM(const s32, "interface/frFont", func_0019C2A8);

INCLUDE_ASM(const s32, "interface/frFont", func_0019C2F8);

INCLUDE_ASM(const s32, "interface/frFont", func_0019C358);

INCLUDE_ASM(const s32, "interface/frFont", func_0019C418);

INCLUDE_ASM(const s32, "interface/frFont", func_0019C490);

INCLUDE_ASM(const s32, "interface/frFont", func_0019C4D0);

INCLUDE_ASM(const s32, "interface/frFont", func_0019C5B0);

INCLUDE_ASM(const s32, "interface/frFont", func_0019C608);

INCLUDE_ASM(const s32, "interface/frFont", func_0019C618);

INCLUDE_ASM(const s32, "interface/frFont", func_0019C628);

u32 func_0019C638(void) {
    return 0;
}

INCLUDE_ASM(const s32, "interface/frFont", func_0019C640);

INCLUDE_ASM(const s32, "interface/frFont", func_0019C850);

INCLUDE_ASM(const s32, "interface/frFont", func_0019C980);

INCLUDE_ASM(const s32, "interface/frFont", func_0019C9D0);

INCLUDE_ASM(const s32, "interface/frFont", func_0019CAB0);

INCLUDE_ASM(const s32, "interface/frFont", func_0019CB30);

INCLUDE_ASM(const s32, "interface/frFont", func_0019CC28);

INCLUDE_ASM(const s32, "interface/frFont", func_0019CC70);

INCLUDE_ASM(const s32, "interface/frFont", func_0019CCC0);

INCLUDE_ASM(const s32, "interface/frFont", func_0019CE10);

INCLUDE_ASM(const s32, "interface/frFont", func_0019CE78);

INCLUDE_ASM(const s32, "interface/frFont", func_0019D010);

void func_0019D038(s32 arg0) {
    *(u32 *)(arg0 + 0x40) = 1;
    func_0019D010(arg0, 0x80);
}

void func_0019D058(s32 arg0, u8 arg1) {
    u32 temp_v0;

    *(u8 *)(arg0 + 1) = arg1;
    temp_v0 = func_0019D920();
    *(u32 *)(arg0 + 0xc) = temp_v0;
}

INCLUDE_ASM(const s32, "interface/frFont", func_0019D088);

INCLUDE_ASM(const s32, "interface/frFont", func_0019D100);

void func_0019D110(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x14) = arg1 >> 4;
}

INCLUDE_ASM(const s32, "interface/frFont", func_0019D120);

INCLUDE_ASM(const s32, "interface/frFont", func_0019D178);

INCLUDE_ASM(const s32, "interface/frFont", func_0019D1D0);

INCLUDE_ASM(const s32, "interface/frFont", func_0019D1E0);

void func_0019D1F8(u32 arg0) {
    D_00436568 = arg0;
}

INCLUDE_ASM(const s32, "interface/frFont", func_0019D200);

INCLUDE_ASM(const s32, "interface/frFont", func_0019D288);

INCLUDE_ASM(const s32, "interface/frFont", func_0019D518);

INCLUDE_ASM(const s32, "interface/frFont", func_0019D530);

INCLUDE_ASM(const s32, "interface/frFont", func_0019D550);

INCLUDE_ASM(const s32, "interface/frFont", func_0019D7E0);

INCLUDE_ASM(const s32, "interface/frFont", func_0019D830);

INCLUDE_ASM(const s32, "interface/frFont", func_0019D848);

void func_0019D8A8(u32 arg0) {
    func_0019C130(8, arg0, 0);
}

void func_0019D8C8(void) {
    func_0019C238(8);
}

INCLUDE_ASM(const s32, "interface/frFont", func_0019D8E0);

INCLUDE_ASM(const s32, "interface/frFont", func_0019D920);

INCLUDE_ASM(const s32, "interface/frFont", func_0019D958);

INCLUDE_ASM(const s32, "interface/frFont", func_0019D9A8);

INCLUDE_ASM(const s32, "interface/frFont", func_0019DA98);

INCLUDE_ASM(const s32, "interface/frFont", func_0019DAD8);

void func_0019DB18(void) {
    D_00436554 = 0x19;
}

void func_0019DB28(u32 arg0) {
    D_00436554 = arg0;
}

INCLUDE_ASM(const s32, "interface/frFont", func_0019DB30);

INCLUDE_ASM(const s32, "interface/frFont", func_0019DBA8);

INCLUDE_ASM(const s32, "interface/frFont", func_0019DC68);

INCLUDE_ASM(const s32, "interface/frFont", func_0019DCF8);

INCLUDE_ASM(const s32, "interface/frFont", func_0019DD48);

INCLUDE_ASM(const s32, "interface/frFont", func_0019DE70);

INCLUDE_ASM(const s32, "interface/frFont", func_0019DEE0);

INCLUDE_ASM(const s32, "interface/frFont", func_0019E050);

void func_0019E0A0(u32 *arg0) {
    s8 temp_v0;

    if (arg0[5] == 0) {
        temp_v0 = *(s8 *)(arg0 + 7);
    }
    else {
        if (*(s32 *)(arg0[5] + 0x1c) == 0) {
            *(u8 *)(arg0 + 7) = 0;
        }
        temp_v0 = *(s8 *)(arg0 + 7);
    }
    if (temp_v0 == '\0') {
        temp_v0 = *(s8 *)((s32)arg0 + 0x1d);
    }
    else {
        func_0019E050();
        temp_v0 = *(s8 *)((s32)arg0 + 0x1d);
    }
    if (temp_v0 != '\0') {
        func_0019D100(arg0[5], *arg0, arg0[1]);
        *(u8 *)((s32)arg0 + 0x1d) = 0;
    }
}

INCLUDE_ASM(const s32, "interface/frFont", func_0019E110);
