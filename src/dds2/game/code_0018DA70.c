#include "common.h"

typedef struct PairedEffectResources {
    u8 pad00[0x70];
    u32 first;
    u32 second;
    u8 pad78[4];
    u32 value7C;
} PairedEffectResources;

void effFreePairedResources(PairedEffectResources *resources) {
    func_0016EFA8(resources->second);
    func_0016EFA8(resources->first);
    func_00328E48(resources);
}

INCLUDE_ASM(const s32, "game/code_0018DA70", func_0018DAA8);

INCLUDE_ASM(const s32, "game/code_0018DA70", func_0018DC68);

void func_0018DC78(PairedEffectResources *resources, u32 value) {
    resources->value7C = value;
}

INCLUDE_ASM(const s32, "game/code_0018DA70", func_0018DC80);

INCLUDE_ASM(const s32, "game/code_0018DA70", func_0018DFD0);

INCLUDE_ASM(const s32, "game/code_0018DA70", func_0018E0D0);

INCLUDE_ASM(const s32, "game/code_0018DA70", func_0018E350);

INCLUDE_ASM(const s32, "game/code_0018DA70", func_0018E628);

INCLUDE_ASM(const s32, "game/code_0018DA70", func_0018E6D0);

INCLUDE_ASM(const s32, "game/code_0018DA70", func_0018E758);

INCLUDE_ASM(const s32, "game/code_0018DA70", func_0018E800);

INCLUDE_ASM(const s32, "game/code_0018DA70", func_0018E850);
