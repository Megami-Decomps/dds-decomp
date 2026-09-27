#include "common.h"

extern u8 D_004389E0;

extern u32 D_004389E4;

extern u32 D_004389E8;

extern u32 D_00439140;

extern u32 D_00439144;

void func_00329F60(u32 arg0, u32 arg1) {
    D_004389E4 = arg0;
    D_004389E8 = arg1;
    D_004389E0 = 1;
}

INCLUDE_ASM(const s32, "game/code_00329F60", func_00329F78);

INCLUDE_ASM(const s32, "game/code_00329F60", func_0032A1C8);

INCLUDE_ASM(const s32, "game/code_00329F60", func_0032A200);

INCLUDE_ASM(const s32, "game/code_00329F60", func_0032A230);

INCLUDE_ASM(const s32, "game/code_00329F60", func_0032A378);

INCLUDE_ASM(const s32, "game/code_00329F60", func_0032A440);

INCLUDE_ASM(const s32, "game/code_00329F60", func_0032A548);

INCLUDE_ASM(const s32, "game/code_00329F60", func_0032A5A0);

INCLUDE_ASM(const s32, "game/code_00329F60", func_0032A5F0);

INCLUDE_ASM(const s32, "game/code_00329F60", func_0032A648);

INCLUDE_ASM(const s32, "game/code_00329F60", func_0032A688);

INCLUDE_ASM(const s32, "game/code_00329F60", func_0032A7A8);

INCLUDE_ASM(const s32, "game/code_00329F60", func_0032A8C8);

INCLUDE_ASM(const s32, "game/code_00329F60", func_0032A968);

INCLUDE_ASM(const s32, "game/code_00329F60", func_0032A9D8);

INCLUDE_ASM(const s32, "game/code_00329F60", func_0032AA40);

INCLUDE_ASM(const s32, "game/code_00329F60", func_0032AAB8);

INCLUDE_ASM(const s32, "game/code_00329F60", func_0032AAD8);

INCLUDE_ASM(const s32, "game/code_00329F60", func_0032AB20);

u32 func_0032ABC0(void) {
    return D_00439140;
}

u32 func_0032ABC8(void) {
    return D_00439144;
}

INCLUDE_ASM(const s32, "game/code_00329F60", func_0032ABD0);

INCLUDE_ASM(const s32, "game/code_00329F60", func_0032AC30);

INCLUDE_ASM(const s32, "game/code_00329F60", func_0032AEA0);

INCLUDE_ASM(const s32, "game/code_00329F60", func_0032AF20);

INCLUDE_ASM(const s32, "game/code_00329F60", func_0032AFD8);

INCLUDE_ASM(const s32, "game/code_00329F60", func_0032AFF0);

INCLUDE_ASM(const s32, "game/code_00329F60", func_0032B018);

INCLUDE_ASM(const s32, "game/code_00329F60", func_0032B170);

u32 func_0032B1B0(s32 arg0) {
    return *(u32 *)(arg0 + 0x28);
}

INCLUDE_ASM(const s32, "game/code_00329F60", func_0032B1B8);

s32 func_0032B1E0(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0x2c);
    if (temp_v0 == 0) {
        func_0032BE30();
        temp_v0 = *(s32 *)(arg0 + 0x2c);
    }
    return temp_v0;
}

INCLUDE_ASM(const s32, "game/code_00329F60", func_0032B218);

u8 func_0032B240(s32 arg0) {
    return *(u8 *)(arg0 + 0x18);
}

u32 func_0032B248(s32 arg0) {
    u32 temp_v0;

    temp_v0 = 0;
    if (*(s32 *)(arg0 + 0x14) != 0) {
        temp_v0 = *(u32 *)(*(s32 *)(arg0 + 0x14) + 0xc);
    }
    return temp_v0;
}

u32 func_0032B260(s32 arg0) {
    return *(u32 *)(*(s32 *)(arg0 + 0x10) + 0xc);
}

INCLUDE_ASM(const s32, "game/code_00329F60", func_0032B270);

INCLUDE_ASM(const s32, "game/code_00329F60", func_0032B2C0);

u64 func_0032B318(s32 arg0) {
    return *(u64 *)(*(s32 *)(arg0 + 0x28) + 0x20);
}

u64 func_0032B328(s32 arg0) {
    return *(u64 *)(*(s32 *)(arg0 + 0x28) + 0x10);
}

u64 func_0032B338(s32 arg0) {
    return *(u64 *)(*(s32 *)(arg0 + 0x28) + 0x30);
}

INCLUDE_ASM(const s32, "game/code_00329F60", func_0032B348);

INCLUDE_ASM(const s32, "game/code_00329F60", func_0032B370);

void func_0032B3E0(s32 arg0, u8 arg1) {
    *(u8 *)(arg0 + 0x1f) = arg1;
    func_0032BE60();
}

INCLUDE_ASM(const s32, "game/code_00329F60", func_0032B3F8);

INCLUDE_ASM(const s32, "game/code_00329F60", func_0032B500);

INCLUDE_ASM(const s32, "game/code_00329F60", func_0032B558);

INCLUDE_ASM(const s32, "game/code_00329F60", func_0032B5B0);

INCLUDE_ASM(const s32, "game/code_00329F60", func_0032B5D8);

INCLUDE_ASM(const s32, "game/code_00329F60", func_0032B6B0);

INCLUDE_ASM(const s32, "game/code_00329F60", func_0032B800);
