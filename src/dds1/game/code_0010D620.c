#include "common.h"

extern u64 func_0010D428(u64);
extern u64 func_0021F600(u64);

extern s32 D_003BD78C;

u32 func_0010D620(s32 arg0) {
    return *(u32 *)(arg0 * 0x20 + *(s32 *)(D_003BD78C + 0xb4) + 0x18);
}

u32 func_0010D638(s32 arg0) {
    return *(u32 *)(arg0 * 0x20 + *(s32 *)(D_003BD78C + 0xb8) + 0x18);
}

u32 func_0010D650(void) {
    return *(u32 *)(D_003BD78C + 0x18);
}

void func_0010D660(u32 arg0) {
    *(u32 *)(D_003BD78C + 0x18) = arg0;
}

u32 func_0010D670(void) {
    return *(u32 *)(D_003BD78C + 0xd0);
}

u32 func_0010D680(void) {
    return *(u32 *)(D_003BD78C + 0xd4);
}

u32 func_0010D690(void) {
    return *(u32 *)(D_003BD78C + 0xcc);
}

INCLUDE_ASM(const s32, "game/code_0010D620", func_0010D6A0);

u32 func_0010D6A8(void) {
    return *(u32 *)(D_003BD78C + 0xf0);
}

INCLUDE_ASM(const s32, "game/code_0010D620", func_0010D6B8);

u32 func_0010D6E8(void) {
    u64 temp_v0;

    temp_v0 = func_0010D428(0);
    temp_v0 = func_0021F600(temp_v0);
    func_0010D5F0(temp_v0);
    return 1;
}

u32 func_0010D718(void) {
    u64 temp_v0;

    temp_v0 = func_0010D428(0);
    func_0021F580(temp_v0);
    return 1;
}

u32 func_0010D740(void) {
    u64 temp_v0;

    temp_v0 = func_0010D428(0);
    func_0021F5C0(temp_v0);
    return 1;
}
