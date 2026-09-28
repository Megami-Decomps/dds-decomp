#include "common.h"

extern u64 func_00216708(void);

extern u8 D_003BBB0D;

extern s32 func_00211568(void);

extern s32 func_001D45A8(u32);

extern u32 func_00207BF0(void);

extern u32 func_001A3360(u64, u64, u64);

extern u64 func_001DADC8(u64);

extern u32 func_001DAE48();

extern s32 func_001A8818(void);

extern s32 func_001A2B00(void);

extern s32 func_001A8EA8(void);

extern s32 func_00200628();

extern s32 func_002007A8(u32, u32, u32);

extern s32 func_001A87A0(void);

extern s8 D_003BB870;

extern s32 *D_003BB87C;

extern s8 D_003D7588[];

extern s8 D_003D7580[];

extern char D_003BB8A0[];

extern char D_003BB898[];

extern char D_003BB8A8[];

extern s32 D_003BAA60;

extern s32 func_002CF440(s32, s32, s32);

extern u32 D_003BD878;

extern s32 D_00367940[];

extern s32 D_00367960[];

extern s8 D_003A5A80[];

extern s8 D_003A5AA8[];

extern void func_003014F0();

extern u32 func_0020A3F0(void);

extern u32 func_00207BB0(void);

extern u32 func_00207B28(void);

extern u32 func_002099A0(void);

extern u32 func_00208C68(void);

extern s32 func_001A17F0(void);

INCLUDE_ASM(const s32, "game/code_001FF030", func_001FF030);

INCLUDE_ASM(const s32, "game/code_001FF030", func_001FF0C8);

u32 func_001FF558(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_001FF560);

INCLUDE_ASM(const s32, "game/code_001FF030", func_001FF8D8);

INCLUDE_ASM(const s32, "game/code_001FF030", func_001FFC18);

INCLUDE_ASM(const s32, "game/code_001FF030", func_001FFCD8);

INCLUDE_ASM(const s32, "game/code_001FF030", func_001FFDA0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A5988);

INCLUDE_ASM(const s32, "game/code_001FF030", func_001FFE30);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00200120);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00200188);

u32 func_00200208(void) {
    u32 temp_v0;

    temp_v0 = func_00200FB0();
    if (temp_v0 == 0) {
        return temp_v0;
    }
    *(u16 *)(*(s32 *)D_003BB87C + 0x88) = 0;
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_00200238);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00200298);

u8 func_00200308(void) {
    s32 temp_v0;

    temp_v0 = func_001A87A0();
    return temp_v0 != 0;
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_00200328);

s32 func_00200388(s32 arg0, s32 arg1) {
    return (func_001A1938(arg0 + 0x120, arg1) & arg1) != 0;
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_002003B8);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00200438);

INCLUDE_ASM(const s32, "game/code_001FF030", func_002004B8);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00200530);

INCLUDE_ASM(const s32, "game/code_001FF030", func_002005A0);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00200628);

INCLUDE_ASM(const s32, "game/code_001FF030", func_002007A8);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00200948);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00200A08);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00200A88);

u8 func_00200B08(u32 arg0, u32 arg1) {
    s32 temp_v0;

    temp_v0 = func_002007A8(arg0, arg1, 0);
    return temp_v0 != 0;
}

u8 func_00200B28(u32 arg0, u32 arg1) {
    s32 temp_v0;

    temp_v0 = func_002007A8(arg0, arg1, 1);
    return temp_v0 != 0;
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_00200B48);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00200BD0);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00200C58);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00200CE0);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00200D68);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00200DF0);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00200E78);

u8 func_00200EE0(u32 arg0) {
    s32 temp_v0;

    temp_v0 = func_00200628(arg0, 10);
    return temp_v0 != 0;
}

s32 func_00200F00(void) {
    return func_00200628() != 0;
}

s32 func_00200F20(s32 battler) {
    u32 flags;

    if (*(u32 *)(battler + 0x110) & 0x200) {
        return 0;
    }
    flags = *(u16 *)(battler + 0x120) & 0x2000;
    return flags != 0;
}

s32 func_00200F48(void) {
    return ((*(s32 *)(*(s32 *)D_003BB87C + 0xc) & 2) > 0);
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_00200F60);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00200F80);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00200FB0);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00201000);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00201030);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00201070);

u8 func_002010D8(void) {
    s32 temp_v0;

    temp_v0 = func_001A8EA8();
    return temp_v0 != 0;
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_002010F8);

INCLUDE_ASM(const s32, "game/code_001FF030", func_002011C8);

s32 func_002012B0(s32 arg0) {
    return ((*(s32 *)(arg0 + 0x110) & 0x1000) > 0);
}

u8 func_002012C0(void) {
    s32 temp_v0;

    temp_v0 = func_001A2B00();
    return temp_v0 == 0;
}

u8 func_002012E0(void) {
    s32 temp_v0;

    temp_v0 = func_001A8818();
    return temp_v0 != 0;
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_00201300);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00201408);

INCLUDE_ASM(const s32, "game/code_001FF030", func_002014E8);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00201568);

INCLUDE_ASM(const s32, "game/code_001FF030", func_002015E8);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00201658);

INCLUDE_ASM(const s32, "game/code_001FF030", func_002016E0);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00201748);

INCLUDE_ASM(const s32, "game/code_001FF030", func_002017C0);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00201828);

INCLUDE_ASM(const s32, "game/code_001FF030", func_002018A0);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00201900);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00201A40);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00201B10);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00201B30);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00201B50);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00201BD8);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00201C60);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00201CD0);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00201D40);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00201DA8);

s32 func_00201E10(void) {
    return D_003BB870 < 1;
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_00201E20);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00201E88);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00201EF0);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00201F10);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00201F30);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00201FE0);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00202158);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00202178);

s32 func_002022D0(s32 arg0, s32 arg1) {
    s32 temp_v0 = 1;
    s32 temp_v1;

    temp_v1 = *(s32 *)(func_001A17F0() + 0x228);
    if (temp_v1 != 0) {
        do {
            temp_v0 = func_00202178(temp_v1, arg1, 0x200);
            if (temp_v0 == 0) {
                goto done;
            }
            temp_v1 = *(s32 *)(temp_v1 + 0x344);
        } while (temp_v1 != 0);
        temp_v0 = 1;
    done:
        ;
    }
    return temp_v0;
}

s32 func_00202330(s32 arg0, s32 arg1) {
    s32 temp_v0 = 1;
    s32 temp_v1;

    temp_v1 = *(s32 *)(func_001A17F0() + 0x228);
    if (temp_v1 != 0) {
        do {
            temp_v0 = func_00202178(temp_v1, arg1, 0x400);
            if (temp_v0 == 0) {
                goto done;
            }
            temp_v1 = *(s32 *)(temp_v1 + 0x344);
        } while (temp_v1 != 0);
        temp_v0 = 1;
    done:
        ;
    }
    return temp_v0;
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_00202390);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00202410);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00202448);

INCLUDE_ASM(const s32, "game/code_001FF030", func_002024A8);

u64 func_002025F8(u64 arg0, u32 *arg1, u32 *arg2) {
    u32 temp_v0;
    u64 temp_v1;

    temp_v1 = func_001DADC8(0xd);
    temp_v0 = func_001A3360(arg0, temp_v1, 0);
    *arg1 = temp_v0;
    temp_v0 = func_001DAE48(temp_v1);
    *arg2 = temp_v0;
    return temp_v1;
}

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A5A08);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A5A30);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A5A80);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A5AA8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A5AD0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A5B50);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A5BD0);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00202668);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00202F90);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00203098);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00203248);

INCLUDE_ASM(const s32, "game/code_001FF030", func_002033C0);

INCLUDE_ASM(const s32, "game/code_001FF030", func_002034D0);

INCLUDE_ASM(const s32, "game/code_001FF030", func_002035E0);

INCLUDE_ASM(const s32, "game/code_001FF030", func_002036E8);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00203A80);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00203BA8);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00203CA8);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00203E38);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00203F98);

u32 func_00204008(u32 arg0, u32 arg1) {
    func_002042E8(arg0, arg1, 1);
    return 1;
}

u32 func_00204028(u32 arg0, u32 arg1) {
    func_002042E8(arg0, arg1, 0);
    return 1;
}

u32 func_00204048(s32 arg0) {
    u32 temp_v0;

    temp_v0 = func_00207BF0();
    func_001DAE20(*(u32 *)(arg0 + 0x60), temp_v0);
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_00204080);

u32 func_00204198(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_002041A0);

INCLUDE_ASM(const s32, "game/code_001FF030", func_002042E8);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00204430);

INCLUDE_ASM(const s32, "game/code_001FF030", func_002044F0);

INCLUDE_ASM(const s32, "game/code_001FF030", func_002045E8);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00204838);

u32 func_00204AC0(void) {
    return 0xffffffff;
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_00204AC8);

u32 func_00204B40(void) {
    return 0xffffffff;
}

s32 func_00204B48(s32 battler, s32 command) {
    if (command == 1 || command == 0x12) {
        if ((*(u16 *)(battler + 0x120) & 0x2000) != 0) {
            return -1;
        }
    }
    return command;
}

u8 func_00204B78(u32 arg0, s32 arg1) {
    return arg1 == 0xf;
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_00204B88);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00204BA8);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00204C88);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00204D08);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00204D98);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00204F08);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00204FE0);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00205070);

INCLUDE_ASM(const s32, "game/code_001FF030", func_002051B0);

INCLUDE_ASM(const s32, "game/code_001FF030", func_002052F0);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00205420);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00205520);

void func_002055B8(s32 arg0) {
    u16 temp_v0;

    if (*(s32 *)(arg0 + 0x110) & 0x200) {
        temp_v0 = *(u16 *)(arg0 + 0x124);
        if (temp_v0 == 1) {
            if (*(s32 *)(arg0 + 0xe0) == temp_v0) {
                *(s32 *)(arg0 + 0xe0) = 0x11;
            }
        }
    }
}

u32 func_002055F0(void) {
    return 2;
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_002055F8);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00205690);

INCLUDE_ASM(const s32, "game/code_001FF030", func_002056C0);

INCLUDE_ASM(const s32, "game/code_001FF030", func_002056E0);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00205730);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00205838);

INCLUDE_ASM(const s32, "game/code_001FF030", func_002058C8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A5D50);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00205918);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00205B18);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00205BD8);

void func_00205EE0(void) {
    func_00205BD8();
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_00205EF8);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00206128);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00206180);

INCLUDE_ASM(const s32, "game/code_001FF030", func_002062B8);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00206388);

s32 func_00206418(s32 arg0) {
    s32 temp_v0 = *(u16 *)(arg0 + 0x124);

    if ((temp_v0 >= 0x107) && ((temp_v0 < 0x109) || (temp_v0 == 0x124))) {
        return 0x124;
    }
    return *(s32 *)(arg0 + 0xe0);
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_00206450);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00206608);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00206888);

INCLUDE_ASM(const s32, "game/code_001FF030", func_002068E0);

INCLUDE_ASM(const s32, "game/code_001FF030", func_002069C0);

s32 func_00206F38(s32 arg0) {
    s32 temp_v0;

    temp_v0 = *(s32 *)(*(s32 *)(func_001A17F0() + 0x694));
    if (temp_v0 == 0) {
        return 0;
    }
    return (temp_v0 ^ arg0) == 0;
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_00206F78);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00207030);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00207070);

INCLUDE_ASM(const s32, "game/code_001FF030", func_002071B0);

INCLUDE_ASM(const s32, "game/code_001FF030", func_002072F0);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00207640);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00207718);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00207948);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00207B00);

u32 func_00207B28(void) {
    s32 temp_v0;
    s32 temp_v1;

    temp_v0 = func_001A17F0();
    if (*(s32 *)(temp_v0 + 0x27c) != 0x108) {
        return 0;
    }
    temp_v1 = *(s32 *)(temp_v0 + 0x694);
    if (temp_v1 == 0) {
        return 0;
    }
    return *(u8 *)(temp_v1 + 0xe);
}

s32 func_00207B68(void) {
    s32 temp_v0 = 0;
    s32 temp_v1;
    s32 temp_v2;

    temp_v1 = func_001A17F0();
    if (*(s32 *)(temp_v1 + 0x27c) != 0x108) {
        return temp_v0;
    }
    temp_v2 = *(s32 *)(temp_v1 + 0x694);
    if (temp_v2 == 0) {
        return temp_v0;
    }
    return (*(s32 *)(temp_v2 + 0) != 0);
}

u32 func_00207BB0(void) {
    s32 temp_v0;
    s32 temp_v1;

    temp_v0 = func_001A17F0();
    if (*(s32 *)(temp_v0 + 0x27c) != 0x108) {
        return 0;
    }
    temp_v1 = *(s32 *)(temp_v0 + 0x694);
    if (temp_v1 == 0) {
        return 0;
    }
    return *(u32 *)(temp_v1 + 0x8);
}

u32 func_00207BF0(void) {
    s32 temp_v0;

    temp_v0 = func_001A17F0();
    return *(u32 *)(*(s32 *)(temp_v0 + 0x694));
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_00207C18);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00207CA0);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00207DD0);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00207E68);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00207FF0);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00208248);

INCLUDE_ASM(const s32, "game/code_001FF030", func_002082A0);

INCLUDE_ASM(const s32, "game/code_001FF030", func_002082E8);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00208358);

void func_00208400(void) {
    s32 *data;
    s32 temp_v0;

    temp_v0 = func_001A17F0();
    data = *(s32 **)(temp_v0 + 0x694);
    temp_v0 = *data;
    if (temp_v0 != 0) {
        func_001DAB20(temp_v0);
        *data = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_00208440);

INCLUDE_ASM(const s32, "game/code_001FF030", func_002084B0);

INCLUDE_ASM(const s32, "game/code_001FF030", func_002085E8);

u32 func_00208660(void) {
    return 0xffffffff;
}

s32 func_00208668(s32 arg0) {
    return ((*(s32 *)(arg0 + 0x110) & 0x200) < 1);
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_00208678);

INCLUDE_ASM(const s32, "game/code_001FF030", func_002087E0);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00208860);

void func_002089F0(s32 arg0) {
    if (*(u16 *)(arg0 + 0x124) != 0x10d) {
        return;
    }
    func_001A17F0();
    func_001D4CA8(arg0, 1, 0x11d);
    func_00208860(arg0, 0);
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_00208A50);

void func_00208C20(void) {
    func_001F53F0();
}

u32 func_00208C38(u32 arg0, u32 arg1, s32 arg2) {
    if ((0x171 < arg2) && ((arg2 < 0x175 || (arg2 == 0x1a1)))) {
        return 200;
    }
    return 100;
}

u32 func_00208C68(void) {
    s32 temp_v0;
    s32 temp_v1;

    temp_v0 = func_001A17F0();
    if (*(s32 *)(temp_v0 + 0x27c) != 0x116) {
        return 0;
    }
    temp_v1 = *(s32 *)(temp_v0 + 0x694);
    if (temp_v1 == 0) {
        return 0;
    }
    return *(u16 *)(temp_v1 + 0x4);
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_00208CA8);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00208D30);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00208D98);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00208E10);

void func_00208EA8(void) {
    func_00208E10();
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_00208EC0);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00208F10);

void func_00208FB0(void) {
    s32 *data;
    s32 temp_v0;

    temp_v0 = func_001A17F0();
    data = *(s32 **)(temp_v0 + 0x694);
    temp_v0 = *data;
    if (temp_v0 != 0) {
        func_001DAB20(temp_v0);
        *data = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_00208FF0);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00209140);

u32 func_00209220(u32 arg0, s32 arg1) {
    u32 temp_v0;

    temp_v0 = 4;
    if ((*(u32 *)(arg1 + 0x110) & 0x400) == 0) {
        temp_v0 = 0;
    }
    return temp_v0;
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_00209238);

s32 func_00209400(void) {
    s32 temp_v0;

    temp_v0 = func_001A17F0();
    if (*(s32 *)(temp_v0 + 0x250) == 0) {
        return -1;
    }
    if ((*(s32 *)(temp_v0 + 0x1f4) & 0x800) != 0) {
        return -1;
    }
    if (*(u16 *)(temp_v0 + 0x24c) != 1) {
        return -1;
    }
    return func_0020F8E0(D_003BB898);
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_00209460);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00209528);

INCLUDE_ASM(const s32, "game/code_001FF030", func_002096E8);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00209780);

INCLUDE_ASM(const s32, "game/code_001FF030", func_002097D0);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00209818);

INCLUDE_ASM(const s32, "game/code_001FF030", func_002098B8);

u32 func_002099A0(void) {
    s32 temp_v0;
    s32 temp_v1;

    temp_v0 = func_001A17F0();
    if (*(s32 *)(temp_v0 + 0x27c) != 0x10b) {
        return 0;
    }
    temp_v1 = *(s32 *)(temp_v0 + 0x694);
    if (temp_v1 == 0) {
        return 0;
    }
    return *(u32 *)(temp_v1 + 0xc);
}

s32 func_002099E0(void) {
    s32 temp_v0 = 0;
    s32 temp_v1;
    s32 temp_v2;

    temp_v1 = func_001A17F0();
    if (*(s32 *)(temp_v1 + 0x27c) != 0x10b) {
        return temp_v0;
    }
    temp_v2 = *(s32 *)(temp_v1 + 0x694);
    if (temp_v2 == 0) {
        return temp_v0;
    }
    return ((*(s32 *)(temp_v2 + 0x8) & 2) > 0);
}

void func_00209A28(void) {
    u8 *data;
    s32 temp_v0;

    temp_v0 = func_001A17F0();
    data = *(u8 **)(temp_v0 + 0x694);
    data[1] = 1;
    *data = 0;
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_00209A58);

s32 func_00209B70(void) {
    s32 temp_v0 = -1;
    s32 temp_v1;

    temp_v1 = *(s32 *)(func_001A17F0() + 0x694);
    if (*(s8 *)(temp_v1 + 0) != 0) {
        if (*(s8 *)(temp_v1 + 1) != 0) {
            *(u8 *)(temp_v1 + 1) = 0;
            return func_0020F8E0(D_003BB8A0);
        }
        *(u8 *)(temp_v1 + 0) = 0;
        return -1;
    }
    return temp_v0;
}

void func_00209BC8(void) {
    s32 temp_v0;

    temp_v0 = func_001A17F0();
    **(u16 **)(temp_v0 + 0x694) = 0;
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_00209BF0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A5DB8);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00209C90);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00209EB8);

u32 func_0020A3F0(void) {
    s32 temp_v0;

    temp_v0 = func_001A17F0();
    return *(u16 *)(*(s32 *)(temp_v0 + 0x694));
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_0020A418);

INCLUDE_ASM(const s32, "game/code_001FF030", func_0020A4C0);

INCLUDE_ASM(const s32, "game/code_001FF030", func_0020A560);

INCLUDE_ASM(const s32, "game/code_001FF030", func_0020A720);

INCLUDE_ASM(const s32, "game/code_001FF030", func_0020A780);

INCLUDE_ASM(const s32, "game/code_001FF030", func_0020A860);

INCLUDE_ASM(const s32, "game/code_001FF030", func_0020AB08);

INCLUDE_ASM(const s32, "game/code_001FF030", func_0020ABE0);

INCLUDE_ASM(const s32, "game/code_001FF030", func_0020AD20);

INCLUDE_ASM(const s32, "game/code_001FF030", func_0020AD80);

INCLUDE_ASM(const s32, "game/code_001FF030", func_0020ADA8);

INCLUDE_ASM(const s32, "game/code_001FF030", func_0020AF00);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A5FF0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6018);

INCLUDE_ASM(const s32, "game/code_001FF030", func_0020AFB8);

INCLUDE_ASM(const s32, "game/code_001FF030", func_0020B190);

INCLUDE_ASM(const s32, "game/code_001FF030", func_0020B348);

INCLUDE_ASM(const s32, "game/code_001FF030", func_0020B560);

INCLUDE_ASM(const s32, "game/code_001FF030", func_0020B580);

INCLUDE_ASM(const s32, "game/code_001FF030", func_0020B640);

INCLUDE_ASM(const s32, "game/code_001FF030", func_0020B770);

INCLUDE_ASM(const s32, "game/code_001FF030", func_0020B818);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6338);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6358);

INCLUDE_ASM(const s32, "game/code_001FF030", func_0020BE30);

INCLUDE_ASM(const s32, "game/code_001FF030", func_0020C9E8);

u32 func_0020CB28(s32 arg0) {
    u32 temp_v0;

    temp_v0 = 2;
    if (arg0 != 0x1d7) {
        temp_v0 = 0;
    }
    return temp_v0;
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_0020CB38);

void func_0020CCA8(void) {
    func_0020ADA8();
}

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6398);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A63C0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A63E8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6410);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6438);

INCLUDE_RODATA(const s32, "game/code_001FF030", jtbl_003A6460);

INCLUDE_RODATA(const s32, "game/code_001FF030", jtbl_003A6480);

INCLUDE_ASM(const s32, "game/code_001FF030", func_0020CCC0);

INCLUDE_ASM(const s32, "game/code_001FF030", func_0020D168);

INCLUDE_ASM(const s32, "game/code_001FF030", func_0020D2E0);

INCLUDE_ASM(const s32, "game/code_001FF030", func_0020D4B0);

void func_0020D4F0(s32 arg0, s32 arg1, s32 arg2) {
    if (arg1 != 0xd5) {
        return;
    }
    func_003014F0(arg2, "%s%03X_%02X.BED", D_003BB8A8, *(u16 *)(D_003BAA60 + 0x1aa4), *(s32 *)(arg0 + 0x38) - 0x13d);
}

u32 func_0020D548(void) {
    return 7;
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_0020D550);

INCLUDE_ASM(const s32, "game/code_001FF030", func_0020D598);

INCLUDE_ASM(const s32, "game/code_001FF030", func_0020D5E0);

INCLUDE_ASM(const s32, "game/code_001FF030", func_0020D668);

INCLUDE_ASM(const s32, "game/code_001FF030", func_0020D690);

INCLUDE_ASM(const s32, "game/code_001FF030", func_0020D6D8);

INCLUDE_ASM(const s32, "game/code_001FF030", func_0020D818);

INCLUDE_ASM(const s32, "game/code_001FF030", func_0020D858);

u32 func_0020D998(s32 arg0) {
    u32 temp_v0;

    temp_v0 = 0x7d;
    if (arg0 != 0x143) {
        temp_v0 = 0;
    }
    return temp_v0;
}

u8 func_0020D9A8(s32 arg0) {
    return arg0 != 0xd3;
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_0020D9B8);

INCLUDE_ASM(const s32, "game/code_001FF030", func_0020D9F8);

INCLUDE_ASM(const s32, "game/code_001FF030", func_0020DA40);

INCLUDE_ASM(const s32, "game/code_001FF030", func_0020DB90);

INCLUDE_ASM(const s32, "game/code_001FF030", func_0020DC38);

INCLUDE_ASM(const s32, "game/code_001FF030", func_0020DE50);

INCLUDE_ASM(const s32, "game/code_001FF030", func_0020DE70);

INCLUDE_ASM(const s32, "game/code_001FF030", func_0020E058);

INCLUDE_ASM(const s32, "game/code_001FF030", func_0020E170);

INCLUDE_ASM(const s32, "game/code_001FF030", func_0020E868);

INCLUDE_ASM(const s32, "game/code_001FF030", func_0020E910);

INCLUDE_ASM(const s32, "game/code_001FF030", func_0020EA40);

INCLUDE_ASM(const s32, "game/code_001FF030", func_0020EBA0);

INCLUDE_ASM(const s32, "game/code_001FF030", func_0020EBE0);

INCLUDE_ASM(const s32, "game/code_001FF030", func_0020EC20);

INCLUDE_ASM(const s32, "game/code_001FF030", func_0020ECF8);

INCLUDE_ASM(const s32, "game/code_001FF030", func_0020ED90);

INCLUDE_ASM(const s32, "game/code_001FF030", func_0020F808);

INCLUDE_ASM(const s32, "game/code_001FF030", func_0020F840);

INCLUDE_ASM(const s32, "game/code_001FF030", func_0020F8E0);

INCLUDE_ASM(const s32, "game/code_001FF030", func_0020F940);

INCLUDE_ASM(const s32, "game/code_001FF030", func_0020F9B8);

INCLUDE_ASM(const s32, "game/code_001FF030", func_0020FA08);

INCLUDE_ASM(const s32, "game/code_001FF030", func_0020FA70);

INCLUDE_ASM(const s32, "game/code_001FF030", func_0020FB00);

INCLUDE_ASM(const s32, "game/code_001FF030", func_0020FB20);

INCLUDE_ASM(const s32, "game/code_001FF030", func_0020FB98);

INCLUDE_ASM(const s32, "game/code_001FF030", func_0020FC28);

INCLUDE_ASM(const s32, "game/code_001FF030", func_0020FC48);

INCLUDE_ASM(const s32, "game/code_001FF030", func_0020FF50);

INCLUDE_ASM(const s32, "game/code_001FF030", func_002100A8);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00210128);

INCLUDE_ASM(const s32, "game/code_001FF030", func_002101C8);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00210288);

INCLUDE_ASM(const s32, "game/code_001FF030", func_002102D8);

INCLUDE_ASM(const s32, "game/code_001FF030", func_002103A0);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00210450);

u8 func_00210520(void) {
    s32 temp_v0;

    temp_v0 = func_001D45A8(0x1a);
    return temp_v0 == 0;
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_00210540);

INCLUDE_ASM(const s32, "game/code_001FF030", func_002105F8);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00210670);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00210710);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00210790);

INCLUDE_ASM(const s32, "game/code_001FF030", func_002107F8);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00210840);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00210890);

INCLUDE_ASM(const s32, "game/code_001FF030", func_002108E0);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00210928);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00210A18);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6860);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00210BA8);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00210D00);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00210EB0);

INCLUDE_ASM(const s32, "game/code_001FF030", func_002110B8);

INCLUDE_ASM(const s32, "game/code_001FF030", func_002111A0);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00211328);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00211390);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00211450);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00211490);

INCLUDE_ASM(const s32, "game/code_001FF030", func_002114E8);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00211568);

void func_002115E0(void) {
}

void func_002115E8(void) {
    func_00211450();
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_00211600);

void func_00211708(void) {
    s64 temp_v0;

    temp_v0 = func_00211568();
    if (temp_v0 != 0) {
        func_00211390(temp_v0);
        return;
    }
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_00211740);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00211880);

INCLUDE_ASM(const s32, "game/code_001FF030", func_002118A8);

INCLUDE_ASM(const s32, "game/code_001FF030", func_002118D8);

INCLUDE_ASM(const s32, "game/code_001FF030", func_002119A8);

void func_00211A28(s32 *arg0) {
    s32 *temp_a0 = arg0;
    s32 temp_v0;
    s32 i = 0xff;

    do {
        temp_v0 = *temp_a0;
        i--;
        *temp_a0 = (temp_v0 & 0xffffff) | (temp_v0 << 0x18);
        temp_a0++;
    } while (i >= 0);
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_00211A60);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00211B88);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00211CE8);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00211D40);

INCLUDE_ASM(const s32, "game/code_001FF030", func_002121E8);

INCLUDE_ASM(const s32, "game/code_001FF030", func_002124E8);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00212640);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00212680);

INCLUDE_ASM(const s32, "game/code_001FF030", func_002127A8);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00212998);

INCLUDE_ASM(const s32, "game/code_001FF030", func_002131F8);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00213280);

void func_00213368(void) {
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_00213370);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00213538);

INCLUDE_ASM(const s32, "game/code_001FF030", func_002135B8);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00213600);

INCLUDE_ASM(const s32, "game/code_001FF030", func_002136C0);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00213750);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00213808);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00213A38);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00213A90);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00213AD0);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00213B28);

s32 func_00213B50(void) {
    return D_003D7588[0];
}

s32 func_00213B60(void) {
    u16 state;

    if (D_003D7580[8] == 0) {
        return 1;
    }
    state = *(u16 *)(D_003D7580 + 4);
    if (state == 0) {
        return 1;
    }
    return state == 2;
}

s32 func_00213B90(void) {
    u16 state;

    if (D_003D7580[8] == 0) {
        return 1;
    }
    state = *(u16 *)(D_003D7580 + 4);
    if (state == 0) {
        return 1;
    }
    return state == 4;
}

void func_00213BC0(void) {
    if (D_003D7580[8] != 0) {
        D_003D7580[7] = 1;
    }
}

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6A00);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6A10);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6A20);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6A30);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6A40);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6A50);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6A60);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6A70);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6A80);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6A90);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6AA0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6AB0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6AC0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6AD0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6AE0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6AF0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6B00);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6B10);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6B20);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6B30);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6B40);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6B50);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6B68);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6B80);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6B98);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6BB0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6BC8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6BE0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6BF8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6C10);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6C28);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6C40);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6C50);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6C60);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6C70);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6C80);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6C90);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6CA0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6CB0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6CC0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6CD0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6CE0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6CF0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6D00);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6D10);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6D20);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6D30);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6D40);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6D50);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6D60);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6D70);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6D80);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6D90);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6DA0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6DF0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6E00);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6E10);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6E20);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6E30);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6E40);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6E50);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6E60);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6E70);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6E80);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6E90);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6EA0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6EB0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6EC0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A6EF8);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00213BE0);

INCLUDE_ASM(const s32, "game/code_001FF030", func_002142C0);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00214438);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00214490);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00214588);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00214618);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00214768);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00214868);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00215A50);

void func_00215FE0(void) {
    D_003BBB0D = 0;
    func_0011CE18();
}

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7128);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7138);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7148);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7158);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7168);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7178);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7188);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7198);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A71A8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A71B8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A71C8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A71D8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A71E8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A71F8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7208);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7218);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7228);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7238);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7248);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7258);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7268);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7278);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7288);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7298);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A72A8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A72B8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A72C8);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00215FF8);

INCLUDE_ASM(const s32, "game/code_001FF030", func_002162E0);

void func_002166A8(void) {
    s32 i;

    D_003BD878 = func_002CF440(1, 0x7f, 0);
    for (i = 0; i != 8; i++) {
        D_00367940[i] = 0;
        D_00367960[i] = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_00216708);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00216738);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00216788);

void func_002167E0(s32 arg0, s32 arg1) {
    s32 *temp_v0;
    s32 *temp_v1;

    temp_v0 = &D_00367960[arg0];
    temp_v1 = (s32 *)*temp_v0;
    if (temp_v1 == 0) {
        return;
    }
    do {
        if (*(temp_v1 + 1) == arg1) {
            *temp_v0 = *temp_v1;
            func_002CFF98(temp_v1);
            break;
        } else {
            temp_v0 = temp_v1;
            temp_v1 = (s32 *)*temp_v1;
        }
    } while (temp_v1 != 0);
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_00216840);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00216958);

void func_00216A70(void) {
    u64 temp_v0;

    temp_v0 = func_00216708();
    func_00216958(temp_v0);
}

INCLUDE_ASM(const s32, "game/code_001FF030", func_00216A90);

INCLUDE_ASM(const s32, "game/code_001FF030", func_00216B00);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A72F0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7300);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7310);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7320);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7330);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7340);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7350);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7360);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7370);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7380);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7390);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A73A0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A73B0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A73C0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A73D0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A73E0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A73F0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7400);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7410);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7420);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7430);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7440);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7450);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7460);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7470);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7480);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7490);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A74A0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A74B0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A74C0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A74D0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A74E0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A74F0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7500);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7510);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7520);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7530);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7540);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7550);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7560);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7570);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7580);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7590);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A75A0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A75B0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A75C0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A75D0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A75E0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A75F0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7600);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7610);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7620);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7630);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7640);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7650);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7660);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7670);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7680);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7690);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A76A0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A76B0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A76C0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A76D0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A76E0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A76F0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7700);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7710);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7720);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7730);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7740);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7750);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7760);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7770);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7780);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7790);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A77A0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A77B0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A77C0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A77D0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A77E0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A77F0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7800);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7810);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7820);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7830);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7840);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7850);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7860);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7870);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7880);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7890);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A78A0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A78B0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A78C0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A78D0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A78E0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A78F0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7900);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7910);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7920);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7930);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7940);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7950);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7960);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7970);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7980);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7990);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A79A0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A79B0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A79C0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A79D0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A79E0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A79F0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7A00);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7A10);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7A20);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7A30);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7A40);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7A50);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7A60);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7A70);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7A80);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7A90);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7AA0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7AB0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7AC0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7AD0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7AE0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7AF0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7B00);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7B10);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7B20);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7B30);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7B40);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7B50);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7B60);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7B70);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7B80);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7B90);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7BA0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7BB0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7BC0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7BD0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7BE0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7BF0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7C00);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7C10);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7C20);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7C30);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7C40);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7C50);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7C60);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7C70);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7C80);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7C90);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7CA0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7CB0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7CC0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7CD0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7CE0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7CF0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7D00);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7D10);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7D20);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7D30);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7D40);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7D50);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7D60);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7D70);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7D80);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7D90);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7DA0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7DB0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7DC0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7DD0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7DE0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7DF0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7E00);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7E10);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7E20);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7E30);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7E40);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7E50);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7E60);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7E70);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7E80);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7E90);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7EA0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7EB0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7EC0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7ED0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7EE0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7EF0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7F00);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7F10);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7F20);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7F30);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7F40);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7F50);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7F60);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7F70);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7F80);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7F90);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7FA0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7FB0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7FC0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7FD0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7FE0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A7FF0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8000);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8010);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8020);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8030);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8040);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8050);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8060);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8070);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8080);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8090);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A80A0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A80B0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A80C0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A80D0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A80E0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A80F0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8100);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8110);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8120);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8130);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8140);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8150);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8160);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8170);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8180);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8190);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A81A0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A81B0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A81C0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A81D0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A81E0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A81F0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8200);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8210);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8220);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8230);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8240);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8250);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8260);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8270);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8280);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8290);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A82A0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A82B0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A82C0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A82D0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A82E0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A82F0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8300);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8310);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8320);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8330);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8340);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8350);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8360);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8370);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8380);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8390);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A83A0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A83B0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A83C0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A83D0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A83E0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A83F0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8400);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8410);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8420);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8430);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8440);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8450);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8460);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8470);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8480);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8490);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A84A0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A84B0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A84C0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A84D0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A84E0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A84F0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8500);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8510);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8520);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8530);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8540);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8558);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8570);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8588);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A85A0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A85B8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A85D0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A85E8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8600);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8618);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8630);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8648);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8660);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8678);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8690);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A86A8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A86C0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A86D8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A86F0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8708);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8720);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8738);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8750);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8768);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8780);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8798);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A87B0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A87C8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A87E0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A87F8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8810);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8828);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8840);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8850);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8860);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8870);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8880);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8890);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A88A0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A88B0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A88C0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A88D0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A88E0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A88F0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8900);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8910);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8920);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8930);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8940);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8950);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8960);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8970);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8980);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8990);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A89A0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A89B0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A89C0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A89D0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A89E0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A89F0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8A00);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8A10);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8A20);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8A30);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8A40);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8A50);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8A60);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8A70);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8A80);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8A90);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8AA0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8AB0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8AC0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8AD0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8AE0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8AF0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8B00);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8B10);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8B20);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8B30);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8B40);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8B50);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8B60);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8B70);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8B80);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8B90);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8BA0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8BB0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8BC0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8BD0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8BE0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8BF0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8C00);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8C10);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8C20);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8C30);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8C40);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8C50);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8C60);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8C70);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8C80);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8C90);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8CA0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8CB0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8CC0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8CD0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8CE0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8CF0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8D00);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8D10);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8D20);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8D30);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8D40);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8D50);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8D60);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8D70);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8D80);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8D90);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8DA0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8DB0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8DC0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8DD0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8DE0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8DF0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8E00);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8E10);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8E20);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8E30);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8E40);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8E50);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8E60);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8E70);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8E80);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8E90);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8EA0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8EB0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8EC0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8ED0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8EE0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8EF0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8F00);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8F10);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8F20);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8F30);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8F40);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8F50);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8F60);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8F70);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8F80);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8F90);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8FA0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8FB0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8FC0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8FD0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8FE0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A8FF0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9000);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9010);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9020);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9030);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9040);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9050);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9060);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9070);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9080);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9090);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A90A0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A90B0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A90C0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A90D0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A90E0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A90F0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9100);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9110);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9120);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9130);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9140);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9150);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9160);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9170);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9180);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9190);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A91A0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A91B0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A91C0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A91D0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A91E0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A91F0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9200);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9210);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9220);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9230);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9240);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9250);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9260);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9270);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9280);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9290);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A92A0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A92B0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A92C0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A92D0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A92E0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A92F0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9300);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9310);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9320);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9330);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9340);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9350);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9360);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9370);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9380);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9390);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A93A0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A93B0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A93C0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A93D0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A93E0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A93F0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9400);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9410);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9420);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9430);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9440);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9450);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9460);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9470);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9480);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9490);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A94A0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A94B0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A94C0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A94D0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A94E0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A94F0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9500);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9510);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9520);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9530);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9540);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9550);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9560);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9570);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9580);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9590);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A95A0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A95B0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A95C0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A95D0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A95E0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A95F0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9600);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9610);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9620);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9630);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9640);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9650);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9660);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9670);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9680);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9690);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A96A0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A96B0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A96C0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A96D0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A96E0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A96F0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9700);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9710);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9720);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9730);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9740);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9750);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9760);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9770);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9780);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9790);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A97A0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A97B0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A97C0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A97D0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A97E0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A97F0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9800);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9810);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9820);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9830);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9840);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9850);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9860);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9870);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9880);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9890);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A98A0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A98B0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A98C0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A98D0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A98E0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A98F0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9900);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9910);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9920);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9930);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9940);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9950);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9960);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9970);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9980);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9990);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A99A0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A99B0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A99C0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A99D0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A99E0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A99F0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9A00);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9A10);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9A20);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9A30);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9A40);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9A50);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9A60);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9A70);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9A80);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9A90);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9AA0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9AB0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9AC0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9AD0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9AE0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9AF0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9B00);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9B10);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9B20);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9B30);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9B40);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9B50);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9B60);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9B70);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9B80);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9B90);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9BA0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9BB0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9BC0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9BD0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9BE0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9BF0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9C00);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9C10);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9C20);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9C30);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9C40);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9C50);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9C60);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9C70);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9C80);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9C90);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9CA0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9CB0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9CC0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9CD0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9CE0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9CF0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9D00);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9D10);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9D20);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9D30);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9D40);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9D50);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9D60);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9D70);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9D80);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9D90);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9DA0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9DB0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9DC0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9DD0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9DE0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9DF0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9E00);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9E10);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9E20);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9E30);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9E40);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9E58);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9E70);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9E88);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9E98);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9EB0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9EC8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9EE0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9EF8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9F10);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9F28);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9F40);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9F58);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9F70);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9F88);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9FA0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9FB8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9FD0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003A9FE8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA000);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA018);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA030);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA048);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA060);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA078);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA090);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA0A8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA0B8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA0D0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA0E8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA100);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA110);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA128);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA140);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA158);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA168);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA178);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA188);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA198);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA1A8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA1C0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA1D8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA1F0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA208);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA218);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA228);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA238);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA248);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA258);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA270);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA288);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA2A0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA2D0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA2F8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA328);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA350);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA378);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA3A0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA3C8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA3F0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA418);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA440);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA468);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA490);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA4B8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA4E0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA508);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA530);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA558);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA580);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA5A8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA5D0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA5F8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA620);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA648);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA670);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA698);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA6C0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA6E8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA710);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA738);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA760);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA788);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA7B0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA7D8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA800);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA828);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA850);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA878);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA8A0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA8C8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA8F0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA918);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA940);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA968);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA990);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA9B8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AA9E0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AAA08);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AAA30);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AAA58);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AAA80);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AAAA8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AAAD0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AAAF8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AAB20);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AAB48);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AAB60);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AAB80);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AABA0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AABC0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AABE0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AAC00);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AAC20);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AAC40);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AAC60);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AAC80);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AACA0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AACC0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AACE8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AAD08);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AAD28);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AAD48);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AAD68);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AAD88);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AADA8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AADC8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AADE8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AAE08);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AAE28);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AAE48);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AAE68);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AAE88);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AAEA8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AAEC8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AAEE8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AAF08);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AAF28);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AAF48);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AAF68);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AAF88);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AAFA8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AAFC8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AAFE8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB008);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB028);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB048);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB068);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB088);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB0A8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB0C8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB0E8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB108);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB128);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB148);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB168);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB188);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB1A8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB1C8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB1E8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB208);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB228);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB248);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB268);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB288);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB2A8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB2C8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB2E8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB308);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB328);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB348);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB368);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB388);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB3A8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB3C8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB3E8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB408);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB428);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB448);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB468);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB488);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB4A8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB4C8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB4E8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB508);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB528);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB548);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB568);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB588);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB5A8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB5C8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB5E8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB608);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB628);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB648);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB668);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB688);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB6A8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB6C8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB6E8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB708);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB728);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB748);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB768);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB788);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB7A8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB7C8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB7E8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB808);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB828);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB848);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB868);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB888);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB8A8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB8C8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB8E8);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB908);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB928);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB940);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB958);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB970);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB988);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB9A0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB9B0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB9C0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB9D0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB9E0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003AB9F0);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003ABA00);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003ABA10);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003ABA20);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003ABA38);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003ABA50);

INCLUDE_RODATA(const s32, "game/code_001FF030", D_003ABA68);
