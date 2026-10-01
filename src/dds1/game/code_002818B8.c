#include "common.h"

void mnuClearPairedSpriteRecords(s32 scene, s32 groupIndex) {
    s32 remaining;
    s32 entry;

    remaining = 1;
    entry = groupIndex * 0x134 + scene + 0x16c;
    do {
        remaining = remaining - 1;
        mnuClearPanelWorkState(entry);
        entry = entry + 0x20;
    } while (-1 < remaining);
}

INCLUDE_ASM(const s32, "game/code_002818B8", func_00281908);

INCLUDE_ASM(const s32, "game/code_002818B8", func_002819F8);

INCLUDE_ASM(const s32, "game/code_002818B8", func_00281AE8);

INCLUDE_ASM(const s32, "game/code_002818B8", func_00281BE0);

INCLUDE_ASM(const s32, "game/code_002818B8", func_00281D40);

INCLUDE_RODATA(const s32, "game/code_002818B8", D_003B23D8);

INCLUDE_ASM(const s32, "game/code_002818B8", func_00282360);

INCLUDE_SDATA(const s32, "game/code_002818B8", D_003BC750);

INCLUDE_SDATA(const s32, "game/code_002818B8", D_003BC758);

INCLUDE_SDATA(const s32, "game/code_002818B8", D_003BC760);

INCLUDE_SDATA(const s32, "game/code_002818B8", D_003BC768);

INCLUDE_SDATA(const s32, "game/code_002818B8", D_003BC770);

