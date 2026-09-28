#include "common.h"

extern s32 func_002CB3B8(u32, u32);

extern u32 D_003BC4CC;

typedef struct {
    u32 unk0;
    u16 unk4;
    u16 unk6;
    u32 unk8;
} SceneEntry;

extern SceneEntry D_0036BE38[];

INCLUDE_ASM(const s32, "game/code_00250E88", func_00250E88);

INCLUDE_ASM(const s32, "game/code_00250E88", func_00250F60);

INCLUDE_ASM(const s32, "game/code_00250E88", func_00251260);

INCLUDE_ASM(const s32, "game/code_00250E88", func_002512F0);

INCLUDE_ASM(const s32, "game/code_00250E88", func_002515C8);

INCLUDE_ASM(const s32, "game/code_00250E88", func_002515F0);

INCLUDE_ASM(const s32, "game/code_00250E88", func_002517C0);

INCLUDE_ASM(const s32, "game/code_00250E88", func_00251960);

INCLUDE_ASM(const s32, "game/code_00250E88", func_002519E8);

INCLUDE_RODATA(const s32, "game/code_00250E88", D_003AF810);

INCLUDE_RODATA(const s32, "game/code_00250E88", D_003AF830);

INCLUDE_RODATA(const s32, "game/code_00250E88", D_003AF840);

INCLUDE_RODATA(const s32, "game/code_00250E88", D_003AF850);

INCLUDE_ASM(const s32, "game/code_00250E88", func_00251A38);

INCLUDE_ASM(const s32, "game/code_00250E88", func_00251E38);

INCLUDE_ASM(const s32, "game/code_00250E88", func_00252CE8);

INCLUDE_ASM(const s32, "game/code_00250E88", func_00252E38);

INCLUDE_ASM(const s32, "game/code_00250E88", func_00252F88);

INCLUDE_ASM(const s32, "game/code_00250E88", func_00253018);

INCLUDE_ASM(const s32, "game/code_00250E88", func_002530D8);

INCLUDE_ASM(const s32, "game/code_00250E88", func_00253208);

INCLUDE_ASM(const s32, "game/code_00250E88", func_00253520);

INCLUDE_ASM(const s32, "game/code_00250E88", func_00253558);

s32 func_00253608(void) {
    s32 temp_v0 = func_002CB3B8(D_003BC4CC, 1);

    if (temp_v0 == 0) {
        return 0;
    }
    return *(s32 *)(*(s32 *)(*(s32 *)(temp_v0 + 0x484) + 8) + 4);
}

INCLUDE_ASM(const s32, "game/code_00250E88", func_00253640);

INCLUDE_ASM(const s32, "game/code_00250E88", func_00253778);

INCLUDE_ASM(const s32, "game/code_00250E88", func_00253830);

INCLUDE_ASM(const s32, "game/code_00250E88", func_00253AD0);

void func_00253C78(s32 context) {
    s32 node = *(s32 *)(*(s32 *)(*(s32 *)(context + 0x484) + 8) + 4);
    u16 index = *(u16 *)(node + 0xC);
    *(u16 *)(context + 0x59C) = D_0036BE38[index].unk4;
    index = *(u16 *)(node + 0xC);
    *(u16 *)(context + 0x59E) = D_0036BE38[index].unk6;
}

s32 func_00253CC8(void) {
    func_00253C78(func_002CB3B8(D_003BC4CC, 1));
    return 0;
}

INCLUDE_ASM(const s32, "game/code_00250E88", func_00253CF8);


INCLUDE_SDATA(const s32, "game/code_00250E88", D_003BC420);

INCLUDE_SDATA(const s32, "game/code_00250E88", D_003BC424);

INCLUDE_SDATA(const s32, "game/code_00250E88", D_003BC428);

INCLUDE_SDATA(const s32, "game/code_00250E88", D_003BC430);


INCLUDE_SDATA(const s32, "game/code_00250E88", D_003BC438);

