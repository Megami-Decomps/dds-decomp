#include "common.h"

typedef struct { u32 word[6]; } BurstSprite;

typedef struct { BurstSprite sprite[8]; } BurstTable;

extern BurstTable D_00428420;

/* Result entries are updated and rendered at a 0x68-byte stride. */
typedef struct TitleResultRow {
    u8 pad00[0x2C];
    s8 unk2C;
    u8 pad2D[3];
    u32 opacity;
    u8 pad34[0x34];
} TitleResultRow;

typedef struct TitleMenuWork {
    u8 pad00[0xAEB0];
    s32 iconResource;
    u8 padAEB4[0x50C];
    TitleResultRow rows[1];
} TitleMenuWork;


extern void func_00306C28(s32, s32, s32, s32, s32, s32, s32, s32);
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

void func_002A0148(s32 x, s32 y, s32 z, TitleMenuWork *work,
                   s32 unused, s32 depth, s32 index) {
    u32 color[4];
    s32 packed;

    memcpy(color, D_004285E0, sizeof(color));

    if (work->iconResource != 0) {
        if (work->rows[index].unk2C >= 2) {
            packed = work->rows[index].opacity | 0x80808000;
            color[0] = packed;
            color[1] = packed;
            color[2] = packed;
            color[3] = packed;
            func_00306C28(x + 0x1180, y + 0x130, z, (s32)color, 0,
                         work->iconResource, 0x16, depth);
            func_00306C28(x + 0x17E0, y + 0x130, z, (s32)color, 0,
                         work->iconResource, 0x17, depth);
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

