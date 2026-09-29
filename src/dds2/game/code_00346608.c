#include "common.h"

typedef struct SdfPacFlags {
    u8 pad00;
    u8 flags;
} SdfPacFlags;

typedef struct SdfPacInput {
    u8 pad00[0x10];
    s32 cursor;
    s32 remaining;
    s32 processed;
} SdfPacInput;

typedef struct SdfPacWork {
    u8 pad00[0x33];
    u8 status33;
    u8 pad34[0xA];
    u8 status3E;
} SdfPacWork;

INCLUDE_ASM(const s32, "game/code_00346608", func_00346608);

INCLUDE_ASM(const s32, "game/code_00346608", func_00346778);

INCLUDE_ASM(const s32, "game/code_00346608", func_00346988);

s32 func_00346A60(SdfPacWork *work) {
    if (work->status33 == 0) {
        return 0;
    }
    return work->status3E == 0;
}

INCLUDE_ASM(const s32, "game/code_00346608", func_00346A80);

void func_00346AD8(SdfPacFlags *packet) {
    packet->flags = packet->flags | 1;
}

void func_00346AE8(SdfPacFlags *packet) {
    packet->flags = packet->flags | 2;
}

INCLUDE_ASM(const s32, "game/code_00346608", func_00346AF8);

void func_00346B30(u8 *status) {
    *status = 0xff;
}

void sdfPacAdvanceInput(SdfPacInput *input, s32 count) {
    input->cursor = input->cursor + count;
    input->remaining = input->remaining - count;
    input->processed = input->processed + count;
}

INCLUDE_ASM(const s32, "game/code_00346608", func_00346B68);

INCLUDE_ASM(const s32, "game/code_00346608", func_00346C40);

INCLUDE_SDATA(const s32, "game/code_00346608", D_00438D28);

