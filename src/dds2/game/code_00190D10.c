#include "common.h"

typedef struct MagatuhiEffectOwner {
    u8 pad00[0x1C];
    u32 resource;
} MagatuhiEffectOwner;

INCLUDE_ASM(const s32, "game/code_00190D10", func_00190D10);

void func_00190DB0(MagatuhiEffectOwner *effect) {
    effMagatuhiReleaseResource(effect->resource);
    func_00328E48(effect);
}

void func_00190DE0(MagatuhiEffectOwner *effect) {
    func_00191010(effect->resource);
}

INCLUDE_ASM(const s32, "game/code_00190D10", func_00190DF8);

INCLUDE_ASM(const s32, "game/code_00190D10", func_00190E58);
