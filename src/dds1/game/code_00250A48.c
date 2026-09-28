#include "common.h"

extern s32 func_002CB3B8(u32, u32);

extern u32 D_003BC4CC;

INCLUDE_ASM(const s32, "game/code_00250A48", func_00250A48);

INCLUDE_ASM(const s32, "game/code_00250A48", func_00250B60);

INCLUDE_ASM(const s32, "game/code_00250A48", func_00250E88);

INCLUDE_ASM(const s32, "game/code_00250A48", func_00250F60);

INCLUDE_ASM(const s32, "game/code_00250A48", func_00251260);

INCLUDE_ASM(const s32, "game/code_00250A48", func_002512F0);

INCLUDE_ASM(const s32, "game/code_00250A48", func_002515C8);

INCLUDE_ASM(const s32, "game/code_00250A48", func_002515F0);

INCLUDE_ASM(const s32, "game/code_00250A48", func_002517C0);

INCLUDE_ASM(const s32, "game/code_00250A48", func_00251960);

INCLUDE_ASM(const s32, "game/code_00250A48", func_002519E8);

INCLUDE_RODATA(const s32, "game/code_00250A48", D_003AF810);

INCLUDE_RODATA(const s32, "game/code_00250A48", D_003AF830);

INCLUDE_RODATA(const s32, "game/code_00250A48", D_003AF840);

INCLUDE_RODATA(const s32, "game/code_00250A48", D_003AF850);

INCLUDE_ASM(const s32, "game/code_00250A48", func_00251A38);

INCLUDE_ASM(const s32, "game/code_00250A48", func_00251E38);

INCLUDE_ASM(const s32, "game/code_00250A48", func_00252CE8);

INCLUDE_ASM(const s32, "game/code_00250A48", func_00252E38);

INCLUDE_ASM(const s32, "game/code_00250A48", func_00252F88);

INCLUDE_ASM(const s32, "game/code_00250A48", func_00253018);

INCLUDE_ASM(const s32, "game/code_00250A48", func_002530D8);

INCLUDE_ASM(const s32, "game/code_00250A48", func_00253208);

INCLUDE_ASM(const s32, "game/code_00250A48", func_00253520);

INCLUDE_ASM(const s32, "game/code_00250A48", func_00253558);

s32 func_00253608(void) {
    s32 temp_v0 = func_002CB3B8(D_003BC4CC, 1);

    if (temp_v0 == 0) {
        return 0;
    }
    return *(s32 *)(*(s32 *)(*(s32 *)(temp_v0 + 0x484) + 8) + 4);
}

INCLUDE_ASM(const s32, "game/code_00250A48", func_00253640);

INCLUDE_ASM(const s32, "game/code_00250A48", func_00253778);

INCLUDE_ASM(const s32, "game/code_00250A48", func_00253830);

INCLUDE_ASM(const s32, "game/code_00250A48", func_00253AD0);

INCLUDE_ASM(const s32, "game/code_00250A48", func_00253C78);

INCLUDE_ASM(const s32, "game/code_00250A48", func_00253CC8);

INCLUDE_ASM(const s32, "game/code_00250A48", func_00253CF8);

INCLUDE_ASM(const s32, "game/code_00250A48", func_00253D40);

INCLUDE_ASM(const s32, "game/code_00250A48", func_00253E58);

INCLUDE_ASM(const s32, "game/code_00250A48", func_00254218);

INCLUDE_ASM(const s32, "game/code_00250A48", func_00254288);

INCLUDE_ASM(const s32, "game/code_00250A48", func_00254680);

INCLUDE_ASM(const s32, "game/code_00250A48", func_002546D8);

INCLUDE_ASM(const s32, "game/code_00250A48", func_00254758);

INCLUDE_ASM(const s32, "game/code_00250A48", func_00254778);

INCLUDE_ASM(const s32, "game/code_00250A48", func_00254810);

INCLUDE_ASM(const s32, "game/code_00250A48", func_002549F0);
