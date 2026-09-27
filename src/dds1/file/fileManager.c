#include "common.h"

extern s32 func_00288BA8(u32);

INCLUDE_ASM(const s32, "file/fileManager", func_002887A0);

INCLUDE_ASM(const s32, "file/fileManager", func_00288818);

INCLUDE_ASM(const s32, "file/fileManager", func_002888C8);

void func_00288988(s32 arg0, s32 arg1) {
    *(u32 *)(arg1 + 4) = *(u32 *)(arg0 + 0x18);
    *(s32 *)(arg0 + 0x18) = arg1;
}

INCLUDE_ASM(const s32, "file/fileManager", func_00288998);

INCLUDE_ASM(const s32, "file/fileManager", func_002889D8);

void func_00288A80(u32 arg0) {
    func_002889D8(arg0, 0, 0, 0, 0);
}

void func_00288AA8(u32 arg0) {
    func_002889D8(arg0, 1, 0, 0, 0);
}

INCLUDE_ASM(const s32, "file/fileManager", func_00288AD0);

void func_00288B48(u32 arg0) {
    func_00288AD0(arg0, 0, 0, 0);
}

void func_00288B68(u32 arg0) {
    func_00288AD0(arg0, 1, 0, 0);
}

u32 func_00288B88(s32 arg0) {
    return *(u32 *)(arg0 + 0x20);
}

u32 func_00288B90(s32 arg0) {
    return *(u32 *)(arg0 + 0x24);
}

u32 func_00288B98(s32 arg0) {
    return *(u32 *)(arg0 + 0x14);
}

u32 func_00288BA0(s32 arg0) {
    return *(u32 *)(arg0 + 0x10);
}

INCLUDE_ASM(const s32, "file/fileManager", func_00288BA8);

INCLUDE_ASM(const s32, "file/fileManager", func_00288BE8);

void func_00288C08(u32 arg0) {
    s64 temp_v0;

    while (temp_v0 = func_00288BA8(arg0), temp_v0 == 0) {
        func_002E7098();
        func_002897A0();
    }
}

INCLUDE_ASM(const s32, "file/fileManager", func_00288C50);

INCLUDE_ASM(const s32, "file/fileManager", func_00288C68);

INCLUDE_ASM(const s32, "file/fileManager", func_00288CB8);

INCLUDE_ASM(const s32, "file/fileManager", func_00288D48);

INCLUDE_ASM(const s32, "file/fileManager", func_00288D68);
