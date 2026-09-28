#include "common.h"

extern u32 D_0043895C;

extern void (*D_004389C4)(void);

INCLUDE_ASM(const s32, "game/code_0031E430", func_0031E430);

void func_0031E4D0(u32 arg0) {
    D_0043895C = arg0;
}

void func_0031E4D8(void) {
    D_0043895C = 0;
}

void func_0031E4E0(u32 *entry, s32 mode, s32 value) {
    switch (mode) {
    case 0:
        if (value >= 0x80) {
            entry[5] = 0;
        }
        break;
    case 1:
        if (value >= 0x80) {
            entry[5] = 0x80;
        }
        break;
    }
    entry[0] = mode;
    entry[1] = value;
}

void func_0031E530(u32 *entry, u32 index, u32 value, u32 next) {
    entry[2] = index + 1;
    entry[3] = value;
    entry[4] = next;
}

u32 func_0031E548(s32 arg0) {
    return *(u32 *)(arg0 + 0x14);
}

s32 func_0031E550(context)
    u32 *context;
{
    if (context[5] == 0) {
        if (context[0] == 0) {
            return 0;
        }
    }
    return 1;
}

void func_0031E578(s32 *entry) {
    if (entry[2] > 0) {
        if (entry[4] == 0) {
            func_0031E4E0((u32 *)entry, entry[2] - 1, entry[3]);
            entry[2] = 0;
        } else {
            entry[4]--;
        }
    }
    switch (entry[0]) {
    case 1:
        if (entry[5] < 0x80) {
            entry[5] += entry[1];
        }
        if (entry[5] > 0x80) {
            entry[5] = 0x80;
        }
        break;
    case 0:
        if (entry[5] > 0) {
            entry[5] -= entry[1];
        }
        if (entry[5] < 0) {
            entry[5] = 0;
        }
        break;
    }
}

void func_0031E640(u32 arg0) {
    s64 temp_v0;
    s32 temp_v1;

    temp_v0 = func_0031E550();
    if (temp_v0 != 0) {
        temp_v1 = (s32)arg0;
        func_0031E430(0, 0, *(u32 *)(temp_v1 + 0x14), 7, 0x54);
        func_0031E430(0, 0, *(u32 *)(temp_v1 + 0x14), 8, 0x54);
        func_0031E430(0, 0, *(u32 *)(temp_v1 + 0x14), 9, 0x54);
        func_0031E430(0, 0, *(u32 *)(temp_v1 + 0x14), 10, 0x54);
        func_0031E578(arg0);
        return;
    }
}

INCLUDE_ASM(const s32, "game/code_0031E430", func_0031E6E0);

void func_0031E7C8(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x18) = arg1;
}

void func_0031E7D0(u32 arg0) {
    s64 temp_v0;
    s32 temp_v1;

    temp_v0 = func_0031E550();
    if (temp_v0 != 0) {
        temp_v1 = (s32)arg0;
        func_0031E430(0, 0, *(u32 *)(temp_v1 + 0x14), 3, 0x54);
        func_0031E430(0, 0, *(u32 *)(temp_v1 + 0x14), 2, 0x54);
        func_0031E6E0(0xe0, 0x2a8, *(u32 *)(temp_v1 + 0x14), *(u32 *)(temp_v1 + 0x18));
        func_0031E578(arg0);
        return;
    }
}

void func_0031E850(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x18) = arg1;
}

void func_0031E858(u32 arg0) {
    s64 temp_v0;
    s32 temp_v1;

    temp_v0 = func_0031E550();
    if (temp_v0 != 0) {
        temp_v1 = (s32)arg0;
        func_0031E430(0, 0, *(u32 *)(temp_v1 + 0x14), 1, 0x54);
        func_0031E430(0, 0, *(u32 *)(temp_v1 + 0x14), 0, 0x54);
        func_0031E6E0(0xe0, 0x118, *(u32 *)(temp_v1 + 0x14), *(u32 *)(temp_v1 + 0x18));
        func_0031E578(arg0);
        return;
    }
}

void func_0031E8D8(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x1c) = arg1;
}

void func_0031E8E0(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x20) = arg1;
}

void func_0031E8E8(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x18) = arg1;
}

INCLUDE_ASM(const s32, "game/code_0031E430", func_0031E8F0);

void func_0031ED68(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x18) = arg1;
}

INCLUDE_ASM(const s32, "game/code_0031E430", func_0031ED70);

void func_0031EE28(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x18) = arg1;
}

INCLUDE_ASM(const s32, "game/code_0031E430", func_0031EE30);

void func_0031EEE8(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x18) = arg1;
}

INCLUDE_ASM(const s32, "game/code_0031E430", func_0031EEF0);

void func_0031EFB8(u32 arg0) {
    s64 temp_v0;
    s32 temp_v1;

    temp_v0 = func_0031E550();
    if (temp_v0 != 0) {
        temp_v1 = (s32)arg0;
        func_0031E430(0, 0, *(u32 *)(temp_v1 + 0x14), 0x1a, 0x54);
        func_0031E430(0, 0, *(u32 *)(temp_v1 + 0x14), 0x23, 0x54);
        func_0031E430(0, 0, *(u32 *)(temp_v1 + 0x14), 0x24, 0x54);
        func_0031E578(arg0);
        return;
    }
}

void func_0031F040(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x18) = arg1;
}

void func_0031F048(u32 arg0) {
    s64 temp_v0;
    s32 temp_v1;

    temp_v0 = func_0031E550();
    if (temp_v0 == 0) {
        return;
    }
    temp_v1 = (s32)arg0;
    func_0031E430(0, 0, *(u32 *)(temp_v1 + 0x14), 6, 0x54);
    if (*(s32 *)(temp_v1 + 0x18) != 1) {
        if (*(s32 *)(temp_v1 + 0x18) != 2) goto LAB_0031f0c4;
        func_0031E430(0, 0, *(u32 *)(temp_v1 + 0x14), 5, 0x54);
    }
    func_0031E430(0, 0, *(u32 *)(temp_v1 + 0x14), 4, 0x54);
LAB_0031f0c4:
    func_0031E578(arg0);
}

INCLUDE_SDATA(const s32, "game/code_0031E430", D_0043895C);

