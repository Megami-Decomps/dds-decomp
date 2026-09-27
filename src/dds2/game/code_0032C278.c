#include "common.h"

extern s32 D_004389FC;

extern s32 func_003282F0(u32, u32, u32);

extern u64 func_0032CD38(void);

extern s64 func_0036DE70(void);

extern s32 D_00438A04;

extern u32 D_00438A08;

extern s32 D_00438A10;

extern s32 D_00438A14;

extern u32 func_0032D168(u32);

extern u32 func_00328D68(u32);

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032C278);

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032C408);

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032C448);

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032C468);

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032C658);

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032C730);

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032C768);

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032C860);

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032C9D8);

void func_0032CA90(u32 *arg0, u32 arg1) {
    if (D_004389FC < 0) {
        D_004389FC = func_003282F0(1, 0x7f, 0);
    }
    *arg0 = arg1;
    arg0[1] = 0;
}

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032CAE0);

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032CBB0);

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032CBF0);

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032CCC0);

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032CD38);

u64 func_0032CD98(void) {
    s64 temp_v0;
    u64 temp_v1;

    temp_v0 = func_0036DE70();
    temp_v1 = func_0032CD38();
    if (temp_v0 != 0) {
        EIntr();
    }
    return temp_v1;
}

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032CDE0);

void func_0032CE50(s32 arg0) {
    D_00438A10 = (&D_00438A08)[arg0];
    D_00438A14 = (&D_00438A08)[arg0] + D_00438A04;
}

s32 func_0032CE70(void) {
    return D_00438A14 - D_00438A10;
}

s32 func_0032CE80(s32 arg0) {
    s32 temp_v0;

    temp_v0 = D_00438A10;
    D_00438A10 = D_00438A10 + ((arg0 + 0xfU) & 0xfffffff0);
    return temp_v0;
}

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032CEA0);

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032CEA8);

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032CEC0);

void func_0032CEE8(s32 arg0, u32 arg1) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 8);
    if (temp_v0 == 0) {
        *(u32 *)(arg0 + 4) = arg1;
    }
    else {
        *(u8 *)(temp_v0 + 3) = 0x20;
        *(u32 *)(temp_v0 + 4) = arg1 & 0xfffffff;
    }
    *(u32 *)(arg0 + 8) = arg1;
}

void func_0032CF20(s32 arg0, u32 arg1, u32 arg2) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 8);
    if (temp_v0 == 0) {
        *(u32 *)(arg0 + 4) = arg1;
    }
    else {
        *(u8 *)(temp_v0 + 3) = 0x20;
        *(u32 *)(temp_v0 + 4) = arg1 & 0xfffffff;
    }
    *(u32 *)(arg0 + 8) = arg2;
}

void func_0032CF58(s32 arg0, u32 arg1) {
    s32 temp_v0;

    *(u8 *)(arg1 + 3) = 0x30;
    temp_v0 = *(s32 *)(arg0 + 8);
    if (temp_v0 == 0) {
        *(u32 *)(arg0 + 4) = arg1;
    }
    else {
        *(u8 *)(temp_v0 + 3) = 0x20;
        *(u32 *)(temp_v0 + 4) = arg1 & 0xfffffff;
    }
    *(u32 *)(arg0 + 8) = arg1 + 0x10;
}

void func_0032CF98(s32 arg0, u32 arg1) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(arg0 + 8);
    if (temp_v0 == 0) {
        *(u32 *)(arg0 + 4) = arg1;
    }
    else {
        *(u8 *)(temp_v0 + 3) = 0x20;
        *(u32 *)(temp_v0 + 4) = arg1 & 0xfffffff;
    }
    *(u32 *)(arg0 + 8) = arg1 + 0x30;
}

void func_0032CFD0(s32 arg0, u32 arg1) {
    s32 temp_v0;

    *(u8 *)(arg1 + 3) = 0x50;
    temp_v0 = *(s32 *)(arg0 + 8);
    if (temp_v0 == 0) {
        *(u32 *)(arg0 + 4) = arg1;
    }
    else {
        *(u8 *)(temp_v0 + 3) = 0x20;
        *(u32 *)(temp_v0 + 4) = arg1 & 0xfffffff;
    }
    *(u32 *)(arg0 + 8) = arg1 + 0x10;
}

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032D010);

void func_0032D070(s32 arg0, s32 arg1) {
    s32 *piVar1;

    if (*(s32 *)(arg1 + 4) != 0) {
        piVar1 = *(s32 **)(arg0 + 8);
        if (piVar1 == (s32 *)0x0) {
            *(s32 *)(arg0 + 4) = arg1;
        }
        else {
            *piVar1 = arg1;
            func_0032D218(piVar1);
        }
        *(s32 *)(arg0 + 8) = arg1;
    }
}

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032D0C8);

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032D0F0);

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032D168);

s32 func_0032D1D0(s32 arg0, u32 arg1) {
    u32 temp_v0;

    temp_v0 = func_0032D168(arg1);
    *(u8 *)(arg0 + 3) = 0x20;
    *(u32 *)(arg0 + 4) = temp_v0 & 0xfffffff;
    return temp_v0 + 0x10;
}

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032D218);

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032D2A8);

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032D340);

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032D3F0);

void func_0032D408(s32 arg0, u32 *arg1) {
    if (*(u32 **)(arg0 + 8) == (u32 *)0x0) {
        *(u32 **)(arg0 + 4) = arg1;
    }
    else {
        **(u32 **)(arg0 + 8) = arg1;
    }
    *(u32 **)(arg0 + 8) = arg1;
    *arg1 = 0;
}

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032D428);

void func_0032D438(s32 *arg0, s32 arg1) {
    if (arg0[1] == 0) {
        *arg0 = arg1;
    }
    else {
        **(u32 **)(arg0[1] + 8) = *(u32 *)(arg1 + 8);
    }
    arg0[1] = arg1;
}

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032D460);

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032D4A0);

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032D528);

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032D5E0);

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032D668);

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032D6B0);

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032D758);

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032D898);

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032DA50);

void func_0032DAF0(u32 arg0, u32 arg1, u32 arg2) {
    func_0032D408(arg1, arg2);
    func_0032CEE8(arg0, (s32)arg2 + 0x10);
}

void func_0032DB30(s32 arg0, u32 arg1, s32 arg2) {
    if (arg2 == 0) {
        func_0032D4A0(arg1, arg0 + 0x180, 1);
        return;
    }
    func_0032D4A0(arg1, arg0 + 400, 1);
}

void func_0032DB78(s32 arg0, u32 arg1, s32 arg2) {
    if (arg2 == 0) {
        func_0032D4A0(arg1, arg0 + 0x70, 1);
        return;
    }
    func_0032D4A0(arg1, arg0 + 0xb0, 1);
}

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032DBC0);

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032DC20);

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032DC80);

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032DD20);

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032DD98);

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032DE98);

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032DEB0);

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032DEC8);

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032E348);

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032E3C0);

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032E408);

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032E468);

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032E490);

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032E4B8);

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032E518);

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032E578);

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032E5A0);

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032E5C8);

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032E628);

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032E688);

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032E6B0);

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032E6D8);

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032E738);

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032E798);

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032E7C0);

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032E7E8);

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032E848);

void func_0032E8A8(u64 *arg0) {
    *arg0 = 0;
    arg0[1] = 0x3f;
}

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032E8B8);

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032E918);

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032E9C8);

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032EA20);

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032EB40);

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032EB80);

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032ECA8);

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032ED60);

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032EE88);

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032EF30);

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032F038);

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032F108);

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032F230);

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032F300);

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032F428);

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032F540);

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032F698);

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032F788);

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032F8F0);

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032FA48);

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032FBF0);

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032FD30);

INCLUDE_ASM(const s32, "game/code_0032C278", func_0032FEB8);

INCLUDE_ASM(const s32, "game/code_0032C278", func_00330068);

INCLUDE_ASM(const s32, "game/code_0032C278", func_00330240);

INCLUDE_ASM(const s32, "game/code_0032C278", func_003302C0);

INCLUDE_ASM(const s32, "game/code_0032C278", func_003303B0);

INCLUDE_ASM(const s32, "game/code_0032C278", func_00330430);

INCLUDE_ASM(const s32, "game/code_0032C278", func_00330520);

INCLUDE_ASM(const s32, "game/code_0032C278", func_003305D0);

INCLUDE_ASM(const s32, "game/code_0032C278", func_003306C0);

INCLUDE_ASM(const s32, "game/code_0032C278", func_003306E0);

void func_00330768(s32 arg0) {
    u32 temp_v0;

    if (*(s32 *)(arg0 + 0x30) == 0) {
        temp_v0 = func_00328D68(0x100);
        *(u32 *)(arg0 + 0x30) = temp_v0;
    }
}

INCLUDE_ASM(const s32, "game/code_0032C278", func_003307A0);

void func_00330838(u32 arg0) {
    func_003307A0();
    func_00328E48(*(u32 *)((s32)arg0 + 0x30));
    *(u32 *)((s32)arg0 + 0x30) = 0;
    func_00328E48(arg0);
}

INCLUDE_ASM(const s32, "game/code_0032C278", func_00330870);

INCLUDE_ASM(const s32, "game/code_0032C278", func_00330900);

INCLUDE_ASM(const s32, "game/code_0032C278", func_00330978);

void func_00330A00(u32 *arg0) {
    func_003405D8(*arg0);
}

INCLUDE_ASM(const s32, "game/code_0032C278", func_00330A18);

void func_00330A88(s32 *arg0, u32 arg1, u32 arg2) {
    s16 temp_v0;
    s32 temp_v1;
    s32 temp_v2;
    s32 temp_v3;

    temp_v2 = *arg0;
    temp_v0 = *(s16 *)(temp_v2 + 4);
    temp_v3 = temp_v0 + 1;
    if ((s64)*(s16 *)(temp_v2 + 6) < (s64)temp_v3) {
        func_00340558(temp_v2);
        temp_v2 = *arg0;
    }
    temp_v1 = *(s32 *)(temp_v2 + 0xc);
    *(s32 **)((s32)arg2 + 0x10) = arg0;
    *(s16 *)(temp_v2 + 4) = (s16)temp_v3;
    *(s32 *)(temp_v0 * 4 + temp_v1) = (s32)arg2;
    func_00330B80(arg2, arg1);
}

INCLUDE_ASM(const s32, "game/code_0032C278", func_00330B18);

INCLUDE_ASM(const s32, "game/code_0032C278", func_00330B80);
