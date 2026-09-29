#include "common.h"

INCLUDE_ASM(const s32, "game/code_002EEA18", func_002EEA18);

INCLUDE_ASM(const s32, "game/code_002EEA18", func_002EEAA0);

INCLUDE_ASM(const s32, "game/code_002EEA18", func_002EEAE0);

typedef struct {
    u8 state;    /* 0x00 */
    u8 pad01[3];
    u32 value;   /* 0x04 */
} SdfWordState;

void func_002EEE98(SdfWordState *work, u32 value) {
    work->value = value;
    work->state = 1;
}

INCLUDE_ASM(const s32, "game/code_002EEA18", func_002EEEA8);

INCLUDE_ASM(const s32, "game/code_002EEA18", func_002EF2B0);

INCLUDE_ASM(const s32, "game/code_002EEA18", func_002EF2E0);

INCLUDE_ASM(const s32, "game/code_002EEA18", func_002EF340);

INCLUDE_ASM(const s32, "game/code_002EEA18", func_002EF3D0);

INCLUDE_ASM(const s32, "game/code_002EEA18", func_002EF408);

INCLUDE_ASM(const s32, "game/code_002EEA18", func_002EF560);

INCLUDE_ASM(const s32, "game/code_002EEA18", func_002EF698);

INCLUDE_ASM(const s32, "game/code_002EEA18", func_002EF788);

INCLUDE_ASM(const s32, "game/code_002EEA18", func_002EF7C8);

INCLUDE_ASM(const s32, "game/code_002EEA18", func_002EF858);

INCLUDE_ASM(const s32, "game/code_002EEA18", func_002EF8D8);

INCLUDE_ASM(const s32, "game/code_002EEA18", func_002EF958);

INCLUDE_ASM(const s32, "game/code_002EEA18", func_002EFA58);

INCLUDE_ASM(const s32, "game/code_002EEA18", func_002EFB30);

INCLUDE_ASM(const s32, "game/code_002EEA18", func_002EFB88);

INCLUDE_ASM(const s32, "game/code_002EEA18", func_002EFBF8);

INCLUDE_ASM(const s32, "game/code_002EEA18", func_002EFC68);

INCLUDE_ASM(const s32, "game/code_002EEA18", func_002EFCD0);

extern void func_002EFB30();
extern void func_002EFCD0();

void func_002EFD30(u32 *work) {
    work[4] = (u32)func_002EFB30;
    work[5] = (u32)func_002EFCD0;
}

INCLUDE_RODATA(const s32, "game/code_002EEA18", D_003B50C0);

INCLUDE_RODATA(const s32, "game/code_002EEA18", D_003B50D0);

INCLUDE_RODATA(const s32, "game/code_002EEA18", D_003B50E0);

INCLUDE_RODATA(const s32, "game/code_002EEA18", D_003B50F0);

