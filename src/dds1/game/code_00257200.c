#include "common.h"

INCLUDE_ASM(const s32, "game/code_00257200", func_00257200);

INCLUDE_ASM(const s32, "game/code_00257200", func_00257270);

INCLUDE_ASM(const s32, "game/code_00257200", func_002573E8);

INCLUDE_ASM(const s32, "game/code_00257200", func_00257670);

INCLUDE_ASM(const s32, "game/code_00257200", func_00257718);

INCLUDE_ASM(const s32, "game/code_00257200", func_002579B0);

typedef struct {
    u8 pad00[0x484];
    s32 field_0x484;
    u8 pad488[8];
    s32 field_0x490;
} DisplayGridWork;

extern void func_00257670(s32 argument, s32 work);
void func_00257BD8(DisplayGridWork *work, s32 argument) {
    func_00257670(argument, work->field_0x484);
    mnuAdvanceWrappingFrame(&work->field_0x490);
}

INCLUDE_ASM(const s32, "game/code_00257200", mnuDrawMantraPulseFrame);

INCLUDE_ASM(const s32, "game/code_00257200", func_00257DF0);

typedef struct {
    s32 x;
    s32 y;
} SceneCoordPair;

typedef struct {
    u8 pad00[0x49C];
    SceneCoordPair points[10]; /* 0x49C */
    u8 pad4EC[0xB0];
    s16 entryX;                /* 0x59C */
    s16 entryY;                /* 0x59E */
} SceneCoordWork;

void mnuCopySceneCoordinates(SceneCoordWork *work) {
    s32 x = work->entryX;
    s32 y = work->entryY;
    SceneCoordPair *point = work->points;
    s32 remaining = 9;

    do {
        point->x = x;
        point->y = y;
        point++;
        remaining--;
    } while (remaining >= 0);
}

void mnuAdvanceWrappingFrame(s32 *frame) {
    s32 previous;

    previous = *frame;
    *frame = previous + 1;
    if (0x3c < previous + 1) {
        *frame = 0;
    }
}

INCLUDE_ASM(const s32, "game/code_00257200", func_00257ED0);

INCLUDE_RODATA(const s32, "game/code_00257200", D_003AF950);

