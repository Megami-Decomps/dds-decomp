#include "common.h"

extern u32 D_0043654C;

extern u32 D_0043655C;

extern u32 D_00436560;

extern u32 D_00436590;

extern s64 func_00101700(u32);

extern u64 func_0032C138(u32);

extern u64 func_00343ED0(u64, u32 *, u64);

INCLUDE_ASM(const s32, "game/code_0019E138", func_0019E138);

INCLUDE_ASM(const s32, "game/code_0019E138", func_0019E160);

INCLUDE_ASM(const s32, "game/code_0019E138", func_0019E1B8);

INCLUDE_ASM(const s32, "game/code_0019E138", func_0019E5D8);

INCLUDE_ASM(const s32, "game/code_0019E138", func_0019E7C8);

INCLUDE_ASM(const s32, "game/code_0019E138", func_0019E800);

INCLUDE_ASM(const s32, "game/code_0019E138", func_0019E848);

INCLUDE_ASM(const s32, "game/code_0019E138", func_0019E8A0);

u32 func_0019E8E0(u32 arg0) {
    return D_0043654C & arg0;
}

INCLUDE_ASM(const s32, "game/code_0019E138", func_0019E8F0);

u32 func_0019E908(void) {
    return D_0043655C;
}

u32 func_0019E910(void) {
    return D_00436560;
}

INCLUDE_ASM(const s32, "game/code_0019E138", func_0019E918);

INCLUDE_ASM(const s32, "game/code_0019E138", func_0019EC00);

INCLUDE_ASM(const s32, "game/code_0019E138", func_0019EDC0);

INCLUDE_ASM(const s32, "game/code_0019E138", func_0019EEE8);

INCLUDE_ASM(const s32, "game/code_0019E138", func_0019EF38);

INCLUDE_ASM(const s32, "game/code_0019E138", func_0019F048);

void func_0019F078(void) {
    func_0019C238(4);
    func_0019C238(5);
}

INCLUDE_ASM(const s32, "game/code_0019E138", func_0019F098);

INCLUDE_ASM(const s32, "game/code_0019E138", func_0019F0E8);

INCLUDE_ASM(const s32, "game/code_0019E138", func_0019F1B8);

INCLUDE_ASM(const s32, "game/code_0019E138", func_0019F280);

INCLUDE_ASM(const s32, "game/code_0019E138", func_0019F408);

INCLUDE_ASM(const s32, "game/code_0019E138", func_0019F448);

INCLUDE_ASM(const s32, "game/code_0019E138", func_0019F460);

INCLUDE_ASM(const s32, "game/code_0019E138", func_0019F5E8);

INCLUDE_ASM(const s32, "game/code_0019E138", func_0019F6C8);

INCLUDE_ASM(const s32, "game/code_0019E138", func_0019F798);

INCLUDE_ASM(const s32, "game/code_0019E138", func_0019F878);

INCLUDE_ASM(const s32, "game/code_0019E138", func_0019F940);

INCLUDE_ASM(const s32, "game/code_0019E138", func_0019F990);

INCLUDE_ASM(const s32, "game/code_0019E138", func_0019FA08);

INCLUDE_ASM(const s32, "game/code_0019E138", func_0019FC38);

INCLUDE_ASM(const s32, "game/code_0019E138", func_0019FE00);

INCLUDE_ASM(const s32, "game/code_0019E138", func_0019FEF8);

INCLUDE_ASM(const s32, "game/code_0019E138", func_001A0038);

void func_001A0040(void) {
}

void func_001A0048(void) {
}

INCLUDE_ASM(const s32, "game/code_0019E138", func_001A0050);

INCLUDE_ASM(const s32, "game/code_0019E138", func_001A0058);

INCLUDE_ASM(const s32, "game/code_0019E138", func_001A0060);

INCLUDE_ASM(const s32, "game/code_0019E138", func_001A0068);

INCLUDE_ASM(const s32, "game/code_0019E138", func_001A0098);

INCLUDE_ASM(const s32, "game/code_0019E138", func_001A00B8);

INCLUDE_ASM(const s32, "game/code_0019E138", func_001A01D8);

INCLUDE_ASM(const s32, "game/code_0019E138", func_001A0278);

INCLUDE_ASM(const s32, "game/code_0019E138", func_001A02A0);

u32 func_001A02D0(s32 arg0) {
    func_003297C8(*(u32 *)(arg0 - 4));
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0019E138", func_001A02F0);

INCLUDE_ASM(const s32, "game/code_0019E138", func_001A0338);

INCLUDE_ASM(const s32, "game/code_0019E138", func_001A0350);

INCLUDE_ASM(const s32, "game/code_0019E138", func_001A03D8);

INCLUDE_ASM(const s32, "game/code_0019E138", func_001A0438);

u32 func_001A0458(void) {
    s64 temp_v0;
    u32 temp_v1;

    func_001A0350();
    temp_v0 = func_00101700(D_00436590);
    temp_v1 = 0xffffffff;
    if (temp_v0 != 3) {
        temp_v1 = 0;
    }
    return temp_v1;
}

INCLUDE_ASM(const s32, "game/code_0019E138", func_001A0490);

INCLUDE_ASM(const s32, "game/code_0019E138", func_001A04F0);

INCLUDE_ASM(const s32, "game/code_0019E138", func_001A0558);

INCLUDE_ASM(const s32, "game/code_0019E138", func_001A05B0);

INCLUDE_ASM(const s32, "game/code_0019E138", func_001A0630);

INCLUDE_ASM(const s32, "game/code_0019E138", func_001A0688);

INCLUDE_ASM(const s32, "game/code_0019E138", func_001A06D8);

u64 func_001A0710(u64 arg0) {
    u64 temp_v0;
    u64 temp_v1;
    u32 temp_v2 [4];

    temp_v0 = func_00343ED0(arg0, temp_v2, 0);
    temp_v1 = func_0032C138(temp_v2[0]);
    func_003297C8(temp_v0);
    return temp_v1;
}

INCLUDE_ASM(const s32, "game/code_0019E138", func_001A0760);

INCLUDE_ASM(const s32, "game/code_0019E138", func_001A0888);

INCLUDE_ASM(const s32, "game/code_0019E138", func_001A09C0);

INCLUDE_ASM(const s32, "game/code_0019E138", func_001A0B60);

INCLUDE_ASM(const s32, "game/code_0019E138", func_001A0CA0);

INCLUDE_ASM(const s32, "game/code_0019E138", func_001A0E20);

INCLUDE_ASM(const s32, "game/code_0019E138", func_001A0F88);

INCLUDE_ASM(const s32, "game/code_0019E138", func_001A10B0);

INCLUDE_ASM(const s32, "game/code_0019E138", func_001A1200);

INCLUDE_ASM(const s32, "game/code_0019E138", func_001A1348);

INCLUDE_ASM(const s32, "game/code_0019E138", func_001A1490);

INCLUDE_ASM(const s32, "game/code_0019E138", func_001A1508);

INCLUDE_ASM(const s32, "game/code_0019E138", func_001A1590);

INCLUDE_ASM(const s32, "game/code_0019E138", func_001A1668);

u32 func_001A1810(void) {
    return 0;
}

u32 func_001A1818(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0019E138", func_001A1820);

INCLUDE_ASM(const s32, "game/code_0019E138", func_001A1858);
