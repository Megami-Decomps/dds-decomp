#include "common.h"

extern s32 func_002C8128(u32);

INCLUDE_ASM(const s32, "file/fileManager", func_002C7D00);

INCLUDE_ASM(const s32, "file/fileManager", func_002C7D78);

INCLUDE_ASM(const s32, "file/fileManager", func_002C7E28);

void func_002C7EE8(s32 arg0, s32 arg1) {
    *(u32 *)(arg1 + 4) = *(u32 *)(arg0 + 0x18);
    *(s32 *)(arg0 + 0x18) = arg1;
}

INCLUDE_ASM(const s32, "file/fileManager", func_002C7EF8);

INCLUDE_ASM(const s32, "file/fileManager", func_002C7F38);

INCLUDE_ASM(const s32, "file/fileManager", func_002C7FF0);

INCLUDE_ASM(const s32, "file/fileManager", func_002C8018);

INCLUDE_ASM(const s32, "file/fileManager", func_002C8040);

INCLUDE_ASM(const s32, "file/fileManager", func_002C80C8);

INCLUDE_ASM(const s32, "file/fileManager", func_002C80E8);

u32 func_002C8108(s32 arg0) {
    return *(u32 *)(arg0 + 0x20);
}

u32 func_002C8110(s32 arg0) {
    return *(u32 *)(arg0 + 0x24);
}

u32 func_002C8118(s32 arg0) {
    return *(u32 *)(arg0 + 0x14);
}

u32 func_002C8120(s32 arg0) {
    return *(u32 *)(arg0 + 0x10);
}

INCLUDE_ASM(const s32, "file/fileManager", func_002C8128);

INCLUDE_ASM(const s32, "file/fileManager", func_002C8168);

void fileWaitReady(u32 arg0) {
    s64 temp_v0;

    while (temp_v0 = func_002C8128(arg0), temp_v0 == 0) {
        func_0033FF40();
        func_002C8D20();
    }
}

INCLUDE_ASM(const s32, "file/fileManager", func_002C81D0);

INCLUDE_ASM(const s32, "file/fileManager", func_002C81E8);

INCLUDE_ASM(const s32, "file/fileManager", func_002C8238);

INCLUDE_ASM(const s32, "file/fileManager", func_002C82C8);

INCLUDE_ASM(const s32, "file/fileManager", func_002C82E8);
