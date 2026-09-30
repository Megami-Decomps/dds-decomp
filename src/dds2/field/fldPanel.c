#include "common.h"

extern u32 D_00436204;

extern u64 kwlnTaskGetUserValue(void);

extern s32 kwlnTaskDestroyWithHierarchy(s32 task, s32 flag);

void fldReleasePanelState(void) {
    u64 state;

    state = kwlnTaskGetUserValue();
    sdfReleaseChipBlock(state);
    D_00436204 = 0;
}

INCLUDE_ASM(const s32, "field/fldPanel", func_001441F0);

void fldDestroyPanelTaskIfPresent(void) {
    if (D_00436204 != 0) {
        kwlnTaskDestroyWithHierarchy(D_00436204, 1);
    }
}

INCLUDE_ASM(const s32, "field/fldPanel", func_00144270);

INCLUDE_ASM(const s32, "field/fldPanel", func_001442A0);

INCLUDE_SDATA(const s32, "field/fldPanel", D_00436204);

