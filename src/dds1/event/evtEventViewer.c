#include "common.h"

extern s64 strcmp(u64, s32);

extern s32 func_0022BE40(u32);

void func_0022BE28(void) {
    func_0022B7A0();
}

INCLUDE_ASM(const s32, "event/evtEventViewer", func_0022BE40);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_0022BEA8);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_0022BF00);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_0022BFD8);

void func_0022C0A8(u32 arg0) {
    s64 temp_v0;

    while (temp_v0 = func_0022BE40(arg0), temp_v0 != 0) {
        func_0022BF00(arg0);
    }
}

INCLUDE_ASM(const s32, "event/evtEventViewer", func_0022C0E8);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_0022C188);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_0022C1D8);

s32 func_0022C250(s32 arg0, s32 arg1) {
    s32 *piVar1;
    s32 temp_v0;
    s32 temp_v1;

    piVar1 = *(s32 **)(arg1 + 0x2034);
    temp_v1 = 0;
    while (piVar1 != (s32 *)0x0) {
        temp_v0 = *piVar1;
        piVar1 = (s32 *)piVar1[0x1f];
        if (temp_v0 == arg0) {
            temp_v1 = temp_v1 + 1;
        }
    }
    return temp_v1;
}

s32 func_0022C288(s32 arg0) {
    s32 temp_v0;
    s32 temp_v1;

    temp_v1 = 0;
    for (temp_v0 = *(s32 *)(arg0 + 0x2034); temp_v0 != 0; temp_v0 = *(s32 *)(temp_v0 + 0x7c)) {
        temp_v1 = temp_v1 + 1;
    }
    return temp_v1;
}

INCLUDE_ASM(const s32, "event/evtEventViewer", func_0022C2C0);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_0022C340);

void func_0022C3C8(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0x18);
    *(s32 *)(arg0 + 0x10) = temp_v0;
    if (*(s32 *)(arg0 + 0x14) < temp_v0) {
        *(s32 *)(arg0 + 0x14) = temp_v0;
    }
}

void func_0022C3E8(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 0x18);
    *(s32 *)(arg0 + 0x14) = temp_v0;
    if (temp_v0 < *(s32 *)(arg0 + 0x10)) {
        *(s32 *)(arg0 + 0x10) = temp_v0;
    }
}

INCLUDE_ASM(const s32, "event/evtEventViewer", func_0022C408);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_0022C478);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_0022C4F0);

s32 func_0022C560(u64 arg0, s32 arg1) {
    s64 temp_v0;
    s32 temp_v1;
    s32 temp_v2;

    temp_v2 = 0;
    if (0 < *(s32 *)(arg1 + 0x20)) {
        temp_v1 = arg1 + 0x24;
        do {
            temp_v0 = strcmp(arg0, temp_v1);
            if (temp_v0 == 0) {
                return temp_v2;
            }
            temp_v2 = temp_v2 + 1;
            temp_v1 = temp_v1 + 0x20;
        } while (temp_v2 < *(s32 *)(arg1 + 0x20));
    }
    return -1;
}

INCLUDE_ASM(const s32, "event/evtEventViewer", func_0022C5E8);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_0022C648);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_0022C6B0);

INCLUDE_ASM(const s32, "event/evtEventViewer", func_0022C7F0);

void func_0022CA48(s32 arg0, s32 arg1) {
    s32 temp_v0;
    s32 *piVar2;

    piVar2 = (s32 *)(arg0 * 4 + arg1 + 0x203c);
    temp_v0 = *piVar2;
    if (temp_v0 != 0) {
        func_00110928(temp_v0);
        *piVar2 = 0;
    }
}

INCLUDE_ASM(const s32, "event/evtEventViewer", func_0022CA88);

void func_0022CB68(s32 arg0) {
    if (*(s32 *)(arg0 + 0x2c) != 0) {
        func_00110928(*(s32 *)(arg0 + 0x2c));
    }
    *(u32 *)(arg0 + 0x2c) = 0;
}
