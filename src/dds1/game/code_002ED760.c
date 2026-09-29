#include "common.h"

INCLUDE_ASM(const s32, "game/code_002ED760", func_002ED760);

INCLUDE_ASM(const s32, "game/code_002ED760", func_002ED8D0);

INCLUDE_ASM(const s32, "game/code_002ED760", func_002EDAE0);

INCLUDE_ASM(const s32, "game/code_002ED760", func_002EDBB8);

INCLUDE_ASM(const s32, "game/code_002ED760", func_002EDBD8);

/* Prefix of the PAC decode state used by the flag and stream cursors. */
typedef struct {
    u8 state;
    u8 flags;
    u8 pad02[0xE];
    u8 *input;     /* 0x10 */
    s32 available; /* 0x14 */
    s32 consumed;  /* 0x18 */
} PacStatePrefix;

void func_002EDC30(PacStatePrefix *state) {
    state->flags = state->flags | 1;
}

void func_002EDC40(PacStatePrefix *state) {
    state->flags = state->flags | 2;
}

INCLUDE_ASM(const s32, "game/code_002ED760", func_002EDC50);

void func_002EDC88(u8 *marker) {
    *marker = 0xff;
}

void sdfPacAdvanceInput(PacStatePrefix *state, s32 count) {
    state->input = state->input + count;
    state->available = state->available - count;
    state->consumed = state->consumed + count;
}

INCLUDE_ASM(const s32, "game/code_002ED760", func_002EDCC0);

INCLUDE_ASM(const s32, "game/code_002ED760", func_002EDD98);

INCLUDE_SDATA(const s32, "game/code_002ED760", D_003BD638);

