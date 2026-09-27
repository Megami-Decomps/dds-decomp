#include "common.h"

INCLUDE_ASM(const s32, "game/code_00151F58", func_00151F58);

u32 func_00151FC0(void) {
    return 0xf;
}

INCLUDE_ASM(const s32, "game/code_00151F58", func_00151FC8);

INCLUDE_ASM(const s32, "game/code_00151F58", func_00151FE8);

INCLUDE_ASM(const s32, "game/code_00151F58", func_00151FF8);

INCLUDE_ASM(const s32, "game/code_00151F58", func_00152000);

void func_00152010(s32 arg0, u32 arg1) {
    *(u32 *)(arg0 + 0x24) = arg1;
}

INCLUDE_ASM(const s32, "game/code_00151F58", func_00152018);

INCLUDE_ASM(const s32, "game/code_00151F58", func_00152050);

INCLUDE_ASM(const s32, "game/code_00151F58", func_00152100);

INCLUDE_ASM(const s32, "game/code_00151F58", func_00152170);

u16 func_00152190(s32 arg0) {
    return *(u16 *)(arg0 + 0x2c);
}

INCLUDE_ASM(const s32, "game/code_00151F58", func_00152198);

INCLUDE_ASM(const s32, "game/code_00151F58", func_001521D0);

INCLUDE_ASM(const s32, "game/code_00151F58", func_00152200);

INCLUDE_ASM(const s32, "game/code_00151F58", func_00152240);

INCLUDE_ASM(const s32, "game/code_00151F58", func_00152260);

INCLUDE_ASM(const s32, "game/code_00151F58", func_00152288);

INCLUDE_ASM(const s32, "game/code_00151F58", func_001522E8);

INCLUDE_ASM(const s32, "game/code_00151F58", func_00152348);

INCLUDE_ASM(const s32, "game/code_00151F58", func_00152370);

INCLUDE_ASM(const s32, "game/code_00151F58", func_00152390);

INCLUDE_ASM(const s32, "game/code_00151F58", func_001523B0);

INCLUDE_ASM(const s32, "game/code_00151F58", func_001523D8);

INCLUDE_ASM(const s32, "game/code_00151F58", func_00152408);

INCLUDE_ASM(const s32, "game/code_00151F58", func_00152560);

INCLUDE_ASM(const s32, "game/code_00151F58", func_001525C8);

INCLUDE_ASM(const s32, "game/code_00151F58", func_00152650);

void func_00152720(u32 arg0) {
    func_002DAA68(*(u32 *)((s32)arg0 + 0x84));
    func_00151F00(*(u32 *)((s32)arg0 + 0x80));
    func_002CFF98(arg0);
}

void func_00152758(s32 arg0) {
    func_00151FE8(*(u32 *)(arg0 + 0x80));
}

INCLUDE_ASM(const s32, "game/code_00151F58", func_00152770);

INCLUDE_ASM(const s32, "game/code_00151F58", func_00152790);

INCLUDE_ASM(const s32, "game/code_00151F58", func_001527C0);

INCLUDE_ASM(const s32, "game/code_00151F58", func_001527D8);

INCLUDE_ASM(const s32, "game/code_00151F58", func_00152800);

INCLUDE_ASM(const s32, "game/code_00151F58", func_001528C0);

INCLUDE_ASM(const s32, "game/code_00151F58", func_00152E40);

INCLUDE_ASM(const s32, "game/code_00151F58", func_001530C0);

INCLUDE_ASM(const s32, "game/code_00151F58", func_00153128);

INCLUDE_ASM(const s32, "game/code_00151F58", func_00153618);

INCLUDE_ASM(const s32, "game/code_00151F58", func_00153680);

void func_001536A0(void) {
}

INCLUDE_ASM(const s32, "game/code_00151F58", func_001536A8);

void func_00153728(u32 *arg0) {
    func_002D0918(*arg0);
}

INCLUDE_ASM(const s32, "game/code_00151F58", func_00153740);

INCLUDE_ASM(const s32, "game/code_00151F58", func_00153920);

INCLUDE_ASM(const s32, "game/code_00151F58", func_001539D0);

void func_00153A40(float arg0, s32 arg1) {
    *(float *)(arg1 + 0x10) = *(float *)(arg1 + 0x10) * arg0;
    *(float *)(arg1 + 0x14) = *(float *)(arg1 + 0x14) * arg0;
    *(float *)(arg1 + 0x18) = *(float *)(arg1 + 0x18) * arg0;
    *(float *)(arg1 + 0x158) = *(float *)(arg1 + 0x158) * arg0;
    *(float *)(arg1 + 0x160) = *(float *)(arg1 + 0x160) * arg0;
    *(float *)(arg1 + 0x168) = *(float *)(arg1 + 0x168) * arg0;
}

INCLUDE_ASM(const s32, "game/code_00151F58", func_00153A90);

void func_00153B10(u32 arg0) {
    func_00153920();
    func_002CFF98(arg0);
}

INCLUDE_ASM(const s32, "game/code_00151F58", func_00153B38);

INCLUDE_ASM(const s32, "game/code_00151F58", func_00153D98);

void func_00154048(s32 arg0) {
    s32 temp_v0;
    u32 temp_v1;

    temp_v1 = 0;
    temp_v0 = *(s32 *)(*(s32 *)(arg0 + 0xf8) + 4);
    if (*(s32 *)(arg0 + 0x20) != 0) {
        do {
            *(u32 *)(temp_v0 + 0x20) = 0xf0000001;
            temp_v1 = temp_v1 + 1;
            temp_v0 = temp_v0 + 0x40;
        } while (temp_v1 < *(u32 *)(arg0 + 0x20));
    }
}

void func_00154090(float arg0, s32 arg1) {
    *(float *)(arg1 + 0x10) = *(float *)(arg1 + 0x10) * arg0;
    *(float *)(arg1 + 0x14) = *(float *)(arg1 + 0x14) * arg0;
    *(float *)(arg1 + 0x18) = *(float *)(arg1 + 0x18) * arg0;
    *(float *)(arg1 + 0x158) = *(float *)(arg1 + 0x158) * arg0;
    *(float *)(arg1 + 0x15c) = *(float *)(arg1 + 0x15c) * arg0;
    *(float *)(arg1 + 0x164) = *(float *)(arg1 + 0x164) * arg0;
}

INCLUDE_ASM(const s32, "game/code_00151F58", func_001540E0);

void func_00154160(u32 arg0) {
    func_00153920();
    func_002CFF98(arg0);
}

INCLUDE_ASM(const s32, "game/code_00151F58", func_00154188);

INCLUDE_ASM(const s32, "game/code_00151F58", func_00154430);

void func_00154698(s32 arg0) {
    s32 temp_v0;
    u32 temp_v1;

    temp_v1 = 0;
    temp_v0 = *(s32 *)(*(s32 *)(arg0 + 0xf8) + 4);
    if (*(s32 *)(arg0 + 0x20) != 0) {
        do {
            *(u32 *)(temp_v0 + 0x20) = 0xf0000001;
            temp_v1 = temp_v1 + 1;
            temp_v0 = temp_v0 + 0x40;
        } while (temp_v1 < *(u32 *)(arg0 + 0x20));
    }
}

void func_001546E0(float arg0, s32 arg1) {
    *(float *)(arg1 + 0x10) = *(float *)(arg1 + 0x10) * arg0;
    *(float *)(arg1 + 0x14) = *(float *)(arg1 + 0x14) * arg0;
    *(float *)(arg1 + 0x18) = *(float *)(arg1 + 0x18) * arg0;
    *(float *)(arg1 + 0x15c) = *(float *)(arg1 + 0x15c) * arg0;
    *(float *)(arg1 + 0x164) = *(float *)(arg1 + 0x164) * arg0;
    *(float *)(arg1 + 0x168) = *(float *)(arg1 + 0x168) * arg0;
}

INCLUDE_ASM(const s32, "game/code_00151F58", func_00154730);

void func_001547B0(u32 arg0) {
    func_00153920();
    func_002CFF98(arg0);
}

INCLUDE_ASM(const s32, "game/code_00151F58", func_001547D8);

INCLUDE_ASM(const s32, "game/code_00151F58", func_00154FD8);

INCLUDE_ASM(const s32, "game/code_00151F58", func_00155200);

void func_001552A0(float arg0, s32 arg1) {
    *(float *)(arg1 + 0x10) = *(float *)(arg1 + 0x10) * arg0;
    *(float *)(arg1 + 0x14) = *(float *)(arg1 + 0x14) * arg0;
    *(float *)(arg1 + 0x18) = *(float *)(arg1 + 0x18) * arg0;
    *(float *)(arg1 + 0x164) = *(float *)(arg1 + 0x164) * arg0;
}

INCLUDE_ASM(const s32, "game/code_00151F58", func_001552D8);

void func_00155358(u32 arg0) {
    func_00153920();
    func_002CFF98(arg0);
}

INCLUDE_ASM(const s32, "game/code_00151F58", func_00155380);

INCLUDE_ASM(const s32, "game/code_00151F58", func_00155618);

void func_00155830(s32 arg0) {
    s32 temp_v0;
    u32 temp_v1;

    temp_v1 = 0;
    temp_v0 = *(s32 *)(*(s32 *)(arg0 + 0xf8) + 4);
    if (*(s32 *)(arg0 + 0x20) != 0) {
        do {
            *(u32 *)(temp_v0 + 0x20) = 0xf0000001;
            temp_v1 = temp_v1 + 1;
            temp_v0 = temp_v0 + 0x40;
        } while (temp_v1 < *(u32 *)(arg0 + 0x20));
    }
}

void func_00155878(float arg0, s32 arg1) {
    *(float *)(arg1 + 0x10) = *(float *)(arg1 + 0x10) * arg0;
    *(float *)(arg1 + 0x14) = *(float *)(arg1 + 0x14) * arg0;
    *(float *)(arg1 + 0x18) = *(float *)(arg1 + 0x18) * arg0;
    *(float *)(arg1 + 0x158) = *(float *)(arg1 + 0x158) * arg0;
    *(float *)(arg1 + 0x160) = *(float *)(arg1 + 0x160) * arg0;
}

INCLUDE_ASM(const s32, "game/code_00151F58", func_001558B8);

void func_00155938(u32 arg0) {
    func_00153920();
    func_002CFF98(arg0);
}

INCLUDE_ASM(const s32, "game/code_00151F58", func_00155960);

INCLUDE_ASM(const s32, "game/code_00151F58", func_00155CC0);

void func_00155F30(s32 arg0) {
    s32 temp_v0;
    u32 temp_v1;

    temp_v1 = 0;
    temp_v0 = *(s32 *)(*(s32 *)(arg0 + 0xf8) + 4);
    if (*(s32 *)(arg0 + 0x20) != 0) {
        do {
            *(u32 *)(temp_v0 + 0x20) = 0xf0000001;
            temp_v1 = temp_v1 + 1;
            temp_v0 = temp_v0 + 0x40;
        } while (temp_v1 < *(u32 *)(arg0 + 0x20));
    }
}

void func_00155F78(float arg0, s32 arg1) {
    *(float *)(arg1 + 0x10) = *(float *)(arg1 + 0x10) * arg0;
    *(float *)(arg1 + 0x14) = *(float *)(arg1 + 0x14) * arg0;
    *(float *)(arg1 + 0x18) = *(float *)(arg1 + 0x18) * arg0;
    *(float *)(arg1 + 0x158) = *(float *)(arg1 + 0x158) * arg0;
    *(float *)(arg1 + 0x15c) = *(float *)(arg1 + 0x15c) * arg0;
    *(float *)(arg1 + 0x164) = *(float *)(arg1 + 0x164) * arg0;
}

INCLUDE_ASM(const s32, "game/code_00151F58", func_00155FC8);

void func_00156048(u32 arg0) {
    func_00153920();
    func_002CFF98(arg0);
}

INCLUDE_ASM(const s32, "game/code_00151F58", func_00156070);

INCLUDE_ASM(const s32, "game/code_00151F58", func_00156378);

INCLUDE_ASM(const s32, "game/code_00151F58", func_001565E0);

void func_00156650(float arg0, s32 arg1) {
    *(float *)(arg1 + 0x10) = *(float *)(arg1 + 0x10) * arg0;
    *(float *)(arg1 + 0x14) = *(float *)(arg1 + 0x14) * arg0;
    *(float *)(arg1 + 0x18) = *(float *)(arg1 + 0x18) * arg0;
    *(float *)(arg1 + 0x158) = *(float *)(arg1 + 0x158) * arg0;
}

INCLUDE_ASM(const s32, "game/code_00151F58", func_00156688);

void func_00156708(u32 arg0) {
    func_00153920();
    func_002CFF98(arg0);
}

INCLUDE_ASM(const s32, "game/code_00151F58", func_00156730);

INCLUDE_ASM(const s32, "game/code_00151F58", func_00156920);

void func_00156B98(s32 arg0) {
    s32 temp_v0;
    u32 temp_v1;

    temp_v1 = 0;
    temp_v0 = *(s32 *)(*(s32 *)(arg0 + 0xf8) + 4);
    if (*(s32 *)(arg0 + 0x20) != 0) {
        do {
            *(u32 *)(temp_v0 + 0x20) = 0xf0000001;
            temp_v1 = temp_v1 + 1;
            temp_v0 = temp_v0 + 0x40;
        } while (temp_v1 < *(u32 *)(arg0 + 0x20));
    }
}

void func_00156BE0(float arg0, s32 arg1) {
    *(float *)(arg1 + 0x10) = *(float *)(arg1 + 0x10) * arg0;
    *(float *)(arg1 + 0x14) = *(float *)(arg1 + 0x14) * arg0;
    *(float *)(arg1 + 0x18) = *(float *)(arg1 + 0x18) * arg0;
    *(float *)(arg1 + 0x15c) = *(float *)(arg1 + 0x15c) * arg0;
    *(float *)(arg1 + 0x164) = *(float *)(arg1 + 0x164) * arg0;
    *(float *)(arg1 + 0x168) = *(float *)(arg1 + 0x168) * arg0;
}

INCLUDE_ASM(const s32, "game/code_00151F58", func_00156C30);

void func_00156CB0(u32 arg0) {
    func_00153920();
    func_002CFF98(arg0);
}

INCLUDE_ASM(const s32, "game/code_00151F58", func_00156CD8);

INCLUDE_ASM(const s32, "game/code_00151F58", func_00156F30);

INCLUDE_ASM(const s32, "game/code_00151F58", func_00157140);

void func_00157188(float arg0, s32 arg1) {
    *(float *)(arg1 + 0x10) = *(float *)(arg1 + 0x10) * arg0;
    *(float *)(arg1 + 0x14) = *(float *)(arg1 + 0x14) * arg0;
    *(float *)(arg1 + 0x18) = *(float *)(arg1 + 0x18) * arg0;
    *(float *)(arg1 + 0x158) = *(float *)(arg1 + 0x158) * arg0;
    *(float *)(arg1 + 0x15c) = *(float *)(arg1 + 0x15c) * arg0;
    *(float *)(arg1 + 0x160) = *(float *)(arg1 + 0x160) * arg0;
}

INCLUDE_ASM(const s32, "game/code_00151F58", func_001571D8);

void func_00157258(u32 arg0) {
    func_00153920();
    func_002CFF98(arg0);
}

INCLUDE_ASM(const s32, "game/code_00151F58", func_00157280);

INCLUDE_ASM(const s32, "game/code_00151F58", func_001575D0);

void func_00157970(void) {
    func_00157A58();
}

void func_00157988(float arg0, s32 arg1) {
    *(float *)(arg1 + 0x10) = *(float *)(arg1 + 0x10) * arg0;
    *(float *)(arg1 + 0x14) = *(float *)(arg1 + 0x14) * arg0;
    *(float *)(arg1 + 0x18) = *(float *)(arg1 + 0x18) * arg0;
}

INCLUDE_ASM(const s32, "game/code_00151F58", func_001579B0);

void func_00157A30(u32 arg0) {
    func_00153920();
    func_002CFF98(arg0);
}

INCLUDE_ASM(const s32, "game/code_00151F58", func_00157A58);

INCLUDE_ASM(const s32, "game/code_00151F58", func_00157B58);

void func_00157BE8(s32 arg0) {
    s32 temp_v0;
    u32 temp_v1;

    temp_v1 = 0;
    temp_v0 = *(s32 *)(*(s32 *)(arg0 + 0xf8) + 4);
    if (*(s32 *)(arg0 + 0x20) != 0) {
        do {
            *(u32 *)(temp_v0 + 0x20) = 0xf0000001;
            temp_v1 = temp_v1 + 1;
            temp_v0 = temp_v0 + 0x40;
        } while (temp_v1 < *(u32 *)(arg0 + 0x20));
    }
}

void func_00157C30(float arg0, s32 arg1) {
    *(float *)(arg1 + 0x10) = *(float *)(arg1 + 0x10) * arg0;
    *(float *)(arg1 + 0x14) = *(float *)(arg1 + 0x14) * arg0;
    *(float *)(arg1 + 0x18) = *(float *)(arg1 + 0x18) * arg0;
    *(float *)(arg1 + 0x15c) = *(float *)(arg1 + 0x15c) * arg0;
    *(float *)(arg1 + 0x164) = *(float *)(arg1 + 0x164) * arg0;
    *(float *)(arg1 + 0x168) = *(float *)(arg1 + 0x168) * arg0;
}

INCLUDE_ASM(const s32, "game/code_00151F58", func_00157C80);

void func_00157D00(u32 arg0) {
    func_00153920();
    func_002CFF98(arg0);
}

INCLUDE_ASM(const s32, "game/code_00151F58", func_00157D28);

INCLUDE_ASM(const s32, "game/code_00151F58", func_001584B8);

void func_00158718(s32 arg0) {
    s32 temp_v0;
    u32 temp_v1;

    temp_v1 = 0;
    temp_v0 = *(s32 *)(*(s32 *)(arg0 + 0xf8) + 4);
    if (*(s32 *)(arg0 + 0x20) != 0) {
        do {
            *(u32 *)(temp_v0 + 0x20) = 0xf0000001;
            temp_v1 = temp_v1 + 1;
            temp_v0 = temp_v0 + 0x40;
        } while (temp_v1 < *(u32 *)(arg0 + 0x20));
    }
}

void func_00158760(float arg0, s32 arg1) {
    *(float *)(arg1 + 0x10) = *(float *)(arg1 + 0x10) * arg0;
    *(float *)(arg1 + 0x14) = *(float *)(arg1 + 0x14) * arg0;
    *(float *)(arg1 + 0x18) = *(float *)(arg1 + 0x18) * arg0;
    *(float *)(arg1 + 0x158) = *(float *)(arg1 + 0x158) * arg0;
    *(float *)(arg1 + 0x15c) = *(float *)(arg1 + 0x15c) * arg0;
    *(float *)(arg1 + 0x164) = *(float *)(arg1 + 0x164) * arg0;
}

INCLUDE_ASM(const s32, "game/code_00151F58", func_001587B0);

void func_00158848(u32 arg0) {
    func_002D0918(*(u32 *)((s32)arg0 + 0x17c));
    func_00153920(arg0);
    func_002CFF98(arg0);
}

INCLUDE_ASM(const s32, "game/code_00151F58", func_00158880);

INCLUDE_ASM(const s32, "game/code_00151F58", func_00158B10);

void func_00158D88(s32 arg0) {
    s32 temp_v0;
    u32 temp_v1;

    temp_v1 = 0;
    temp_v0 = *(s32 *)(*(s32 *)(arg0 + 0xf8) + 4);
    if (*(s32 *)(arg0 + 0x20) != 0) {
        do {
            *(u32 *)(temp_v0 + 0x20) = 0xf0000001;
            temp_v1 = temp_v1 + 1;
            temp_v0 = temp_v0 + 0x40;
        } while (temp_v1 < *(u32 *)(arg0 + 0x20));
    }
}

void func_00158DD0(float arg0, s32 arg1) {
    *(float *)(arg1 + 0x10) = *(float *)(arg1 + 0x10) * arg0;
    *(float *)(arg1 + 0x14) = *(float *)(arg1 + 0x14) * arg0;
    *(float *)(arg1 + 0x18) = *(float *)(arg1 + 0x18) * arg0;
    *(float *)(arg1 + 0x158) = *(float *)(arg1 + 0x158) * arg0;
    *(float *)(arg1 + 0x160) = *(float *)(arg1 + 0x160) * arg0;
}

INCLUDE_ASM(const s32, "game/code_00151F58", func_00158E10);
