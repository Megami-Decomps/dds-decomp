#include "common.h"

extern s32 func_0030AC10(void);

extern u32 D_004388AC;

extern u32 D_004388B0;

extern u32 D_0043908C;

extern u32 D_00439090;

extern s32 mdlFlagTest(u32);

extern s32 func_00101740(u32);

extern char D_0042D240[]; /* "LmapMain" */

extern void func_0030AB88(void);

extern void kwlnTaskDestroyWithHierarchyByName(char *, s32);

typedef struct LmapTaskState {
    u32 value0;       /* Purpose not established */
    u32 value4;       /* Cleared when the Lmap task initializes */
    u32 variant;      /* 1..3, selected by the two model flags */
} LmapTaskState;

INCLUDE_ASM(const s32, "game/code_0030A128", func_0030A128);

INCLUDE_ASM(const s32, "game/code_0030A128", func_0030A160);

INCLUDE_ASM(const s32, "game/code_0030A128", func_0030A1D0);

INCLUDE_ASM(const s32, "game/code_0030A128", func_0030A270);

INCLUDE_ASM(const s32, "game/code_0030A128", func_0030A300);

INCLUDE_ASM(const s32, "game/code_0030A128", func_0030A378);

INCLUDE_ASM(const s32, "game/code_0030A128", func_0030A3E8);

INCLUDE_ASM(const s32, "game/code_0030A128", func_0030A528);

INCLUDE_ASM(const s32, "game/code_0030A128", func_0030A700);

INCLUDE_ASM(const s32, "game/code_0030A128", func_0030A7C8);

INCLUDE_ASM(const s32, "game/code_0030A128", func_0030A8A8);

INCLUDE_ASM(const s32, "game/code_0030A128", func_0030A970);

void fldStopLmapTask(void) {
    func_0030AB88();
    kwlnTaskDestroyWithHierarchyByName(D_0042D240, 1);
}

s32 fldLmapTaskExists(void) {
    return func_00101740((u32)D_0042D240) != 0;
}

INCLUDE_ASM(const s32, "game/code_0030A128", func_0030AA68);

INCLUDE_RODATA(const s32, "game/code_0030A128", D_0042D240);

INCLUDE_ASM(const s32, "game/code_0030A128", func_0030AAB0);

/* The 0x1C flag takes precedence over 0x13 when selecting the map variant. */
void fldInitializeLmapTaskVariant(LmapTaskState *task) {
    s64 flagSet;
    u32 variant;

    D_0043908C = 0;
    D_004388AC = 1;
    D_00439090 = 0;
    D_004388B0 = 0;
    flagSet = mdlFlagTest(0x1c);
    variant = 3;
    if (flagSet == 0) {
        flagSet = mdlFlagTest(0x13);
        variant = 2;
        if (flagSet == 0) {
            variant = 1;
        }
    }
    task->variant = variant;
    task->value4 = 0;
}

INCLUDE_ASM(const s32, "game/code_0030A128", func_0030AB88);

u8 func_0030ABF0(void) {
    s64 status;

    status = func_0030AC10();
    return status != 0;
}

INCLUDE_ASM(const s32, "game/code_0030A128", func_0030AC10);

INCLUDE_ASM(const s32, "game/code_0030A128", func_0030B078);

INCLUDE_ASM(const s32, "game/code_0030A128", func_0030B1E8);

INCLUDE_ASM(const s32, "game/code_0030A128", func_0030B470);

INCLUDE_ASM(const s32, "game/code_0030A128", func_0030B568);

INCLUDE_ASM(const s32, "game/code_0030A128", func_0030B600);

u32 func_0030B678(void) {
    return 1;
}

INCLUDE_ASM(const s32, "game/code_0030A128", func_0030B680);

INCLUDE_ASM(const s32, "game/code_0030A128", func_0030B6D8);

INCLUDE_ASM(const s32, "game/code_0030A128", func_0030B728);

INCLUDE_SDATA(const s32, "game/code_0030A128", D_00438888);

INCLUDE_SDATA(const s32, "game/code_0030A128", D_0043888C);

INCLUDE_SDATA(const s32, "game/code_0030A128", D_00438890);

INCLUDE_SDATA(const s32, "game/code_0030A128", D_00438894);

INCLUDE_SDATA(const s32, "game/code_0030A128", D_00438898);

INCLUDE_SDATA(const s32, "game/code_0030A128", D_004388A0);

INCLUDE_SDATA(const s32, "game/code_0030A128", D_004388A4);

INCLUDE_SDATA(const s32, "game/code_0030A128", D_004388A8);

INCLUDE_SDATA(const s32, "game/code_0030A128", D_004388AC);

INCLUDE_SDATA(const s32, "game/code_0030A128", D_004388B0);

