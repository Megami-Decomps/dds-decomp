#include "common.h"
#include "mnu_result.h"

typedef struct { u32 word[6]; } BurstSprite;

typedef struct { BurstSprite sprite[8]; } BurstTable;

extern BurstTable D_00428420;



struct EffectSlotSet;
extern void func_00306C28(s32, s32, s32, u32 *, s32, struct EffectSlotSet *, s32, s32);
extern void *memcpy(void *, const void *, u32);
extern u32 D_004285E0[4];


INCLUDE_ASM(const s32, "game/code_0029DF18", func_0029DF18);

INCLUDE_RODATA(const s32, "game/code_0029DF18", D_00428560);

INCLUDE_RODATA(const s32, "game/code_0029DF18", D_00428570);

INCLUDE_ASM(const s32, "game/code_0029DF18", func_0029DFB0);

INCLUDE_ASM(const s32, "game/code_0029DF18", func_0029E220);

INCLUDE_ASM(const s32, "game/code_0029DF18", func_0029E478);

INCLUDE_ASM(const s32, "game/code_0029DF18", func_0029E548);

INCLUDE_ASM(const s32, "game/code_0029DF18", func_0029E820);

INCLUDE_ASM(const s32, "game/code_0029DF18", func_0029EE80);

INCLUDE_ASM(const s32, "game/code_0029DF18", func_0029F440);

INCLUDE_ASM(const s32, "game/code_0029DF18", func_0029FA98);

INCLUDE_ASM(const s32, "game/code_0029DF18", func_0029FBE0);

void func_002A0148(s32 x, s32 y, s32 z, BrsSkillPackageWork *work,
                   BrsProgressRow *unused, s32 depth, s32 index) {
    u32 color[4];
    s32 packed;

    memcpy(color, D_004285E0, sizeof(color));

    if (work->teardownHandle != 0) {
        if (work->profileAnimation[index].iconState >= 2) {
            packed = work->profileAnimation[index].iconOpacity | 0x80808000;
            color[0] = packed;
            color[1] = packed;
            color[2] = packed;
            color[3] = packed;
            func_00306C28(x + 0x1180, y + 0x130, z, color, 0,
                         (struct EffectSlotSet *)work->teardownHandle, 0x16, depth);
            func_00306C28(x + 0x17E0, y + 0x130, z, color, 0,
                         (struct EffectSlotSet *)work->teardownHandle, 0x17, depth);
        }
    }
}

INCLUDE_ASM(const s32, "game/code_0029DF18", func_002A0278);

INCLUDE_RODATA(const s32, "game/code_0029DF18", D_00428590);

INCLUDE_RODATA(const s32, "game/code_0029DF18", D_004285A0);

INCLUDE_RODATA(const s32, "game/code_0029DF18", D_004285B0);

INCLUDE_RODATA(const s32, "game/code_0029DF18", D_004285C0);

INCLUDE_RODATA(const s32, "game/code_0029DF18", D_004285D0);

INCLUDE_RODATA(const s32, "game/code_0029DF18", D_004285E0);

