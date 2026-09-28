#include "common.h"

extern s32 D_00436558;

/* Value with u16 pair read by func_001971E0/func_00197200. */
typedef struct Unk6C84Val {
    u8 unk0[0x10]; /* 0x0 */
    u16 unk10;     /* 0x10 */
    u16 unk12;     /* 0x12 */
} Unk6C84Val;

/* 0x24-byte record pointing at the value. */
typedef struct Unk6C84Rec {
    Unk6C84Val *unk0; /* 0x0 */
    u8 unk4[0x20];    /* 0x4 */
} Unk6C84Rec;

extern Unk6C84Rec D_00452724[];

INCLUDE_ASM(const s32, "game/code_0019B840", func_0019B840);

u16 func_0019B870(s32 arg0) {
    return D_00452724[arg0].unk0->unk10;
}

u16 func_0019B890(s32 arg0) {
    return D_00452724[arg0].unk0->unk12;
}

void func_0019B8B0(s32 arg0) {
    if (arg0 < 1) {
        arg0 = 0x14;
    }
    D_00436558 = arg0;
}

INCLUDE_ASM(const s32, "game/code_0019B840", func_0019B8C8);

INCLUDE_ASM(const s32, "game/code_0019B840", func_0019B900);

INCLUDE_ASM(const s32, "game/code_0019B840", func_0019B928);

INCLUDE_ASM(const s32, "game/code_0019B840", func_0019B968);

INCLUDE_ASM(const s32, "game/code_0019B840", func_0019BA00);

INCLUDE_ASM(const s32, "game/code_0019B840", func_0019BC60);

INCLUDE_ASM(const s32, "game/code_0019B840", func_0019BCC8);

INCLUDE_ASM(const s32, "game/code_0019B840", func_0019BD48);

INCLUDE_ASM(const s32, "game/code_0019B840", func_0019BE20);

INCLUDE_ASM(const s32, "game/code_0019B840", func_0019BEB8);

INCLUDE_ASM(const s32, "game/code_0019B840", func_0019C0D0);

INCLUDE_ASM(const s32, "game/code_0019B840", func_0019C130);

INCLUDE_ASM(const s32, "game/code_0019B840", func_0019C238);
