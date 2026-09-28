#include "common.h"

extern s64 func_0024DC08(void);

extern s64 func_00285670(s32, s32 *, u64, u64);

extern s32 func_00101A70();

INCLUDE_ASM(const s32, "game/code_0024B3A8", func_0024B3A8);

u32 func_0024B470(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0024B3A8", func_0024B478);

INCLUDE_ASM(const s32, "game/code_0024B3A8", func_0024B6C0);

INCLUDE_ASM(const s32, "game/code_0024B3A8", func_0024B750);

INCLUDE_ASM(const s32, "game/code_0024B3A8", func_0024B798);

INCLUDE_ASM(const s32, "game/code_0024B3A8", func_0024B7E8);

INCLUDE_ASM(const s32, "game/code_0024B3A8", func_0024B868);

INCLUDE_ASM(const s32, "game/code_0024B3A8", func_0024B958);

INCLUDE_ASM(const s32, "game/code_0024B3A8", func_0024B990);

INCLUDE_ASM(const s32, "game/code_0024B3A8", func_0024B9D8);

INCLUDE_ASM(const s32, "game/code_0024B3A8", func_0024BA78);

INCLUDE_ASM(const s32, "game/code_0024B3A8", func_0024BB00);

INCLUDE_ASM(const s32, "game/code_0024B3A8", func_0024BC18);

INCLUDE_ASM(const s32, "game/code_0024B3A8", func_0024BC88);

INCLUDE_ASM(const s32, "game/code_0024B3A8", func_0024BCD0);

INCLUDE_ASM(const s32, "game/code_0024B3A8", func_0024BD48);

INCLUDE_ASM(const s32, "game/code_0024B3A8", func_0024BDB8);

INCLUDE_ASM(const s32, "game/code_0024B3A8", func_0024BF48);

INCLUDE_ASM(const s32, "game/code_0024B3A8", func_0024C028);

INCLUDE_ASM(const s32, "game/code_0024B3A8", func_0024C0B0);

u32 func_0024C0F8(void) {
    s32 temp_v0;

    temp_v0 = func_00101A70();
    func_0027BB28(*(u32 *)(temp_v0 + 0x70));
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0024B3A8", func_0024C120);

INCLUDE_ASM(const s32, "game/code_0024B3A8", func_0024C1B8);

INCLUDE_ASM(const s32, "game/code_0024B3A8", func_0024C298);

INCLUDE_ASM(const s32, "game/code_0024B3A8", func_0024C2E0);

INCLUDE_ASM(const s32, "game/code_0024B3A8", func_0024C368);

INCLUDE_ASM(const s32, "game/code_0024B3A8", func_0024C3F8);

INCLUDE_ASM(const s32, "game/code_0024B3A8", func_0024C578);

INCLUDE_ASM(const s32, "game/code_0024B3A8", func_0024C638);

INCLUDE_ASM(const s32, "game/code_0024B3A8", func_0024C670);

u32 func_0024C6F8(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0024B3A8", func_0024C700);

INCLUDE_ASM(const s32, "game/code_0024B3A8", func_0024C7E8);

INCLUDE_ASM(const s32, "game/code_0024B3A8", func_0024C858);

INCLUDE_ASM(const s32, "game/code_0024B3A8", func_0024C8A0);

s64 func_0024C918(u64 arg0) {
    s32 temp_v0;
    s64 temp_v1;
    s32 *piVar3;

    temp_v0 = func_00101A70();
    piVar3 = (s32 *)(temp_v0 + 0x54);
    temp_v1 = func_00285670(temp_v0 + 8, piVar3, 0, arg0);
    if (temp_v1 == 0) {
        if ((*piVar3 == 0) && (temp_v1 = func_0024DC08(), temp_v1 == 0)) {
            func_002858E8(piVar3, *(u32 *)(temp_v0 + 0x58));
        }
        temp_v1 = 0;
    }
    return temp_v1;
}

INCLUDE_ASM(const s32, "game/code_0024B3A8", func_0024C9A0);

INCLUDE_ASM(const s32, "game/code_0024B3A8", func_0024CA10);

INCLUDE_ASM(const s32, "game/code_0024B3A8", func_0024CA58);

INCLUDE_ASM(const s32, "game/code_0024B3A8", func_0024CB00);

INCLUDE_ASM(const s32, "game/code_0024B3A8", func_0024CB80);

INCLUDE_ASM(const s32, "game/code_0024B3A8", func_0024CCD8);

INCLUDE_ASM(const s32, "game/code_0024B3A8", func_0024CD78);

INCLUDE_ASM(const s32, "game/code_0024B3A8", func_0024CDB0);

u32 func_0024CE20(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0024B3A8", func_0024CE28);

INCLUDE_ASM(const s32, "game/code_0024B3A8", func_0024CF28);

INCLUDE_ASM(const s32, "game/code_0024B3A8", func_0024CF78);
