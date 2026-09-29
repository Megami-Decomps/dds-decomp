#include "common.h"

extern u32 D_003BAE74;
extern void *func_00101A70(void);
extern void func_002CFF98(void *);

extern s32 kwlnTaskDestroyWithHierarchy(s32 task, s32 flag);

void fldReleasePanelState(void) {
    void *panelState;

    panelState = func_00101A70();
    func_002CFF98(panelState);
    D_003BAE74 = 0;
}

INCLUDE_ASM(const s32, "field/fldPanel", func_00141110);

void func_00141158(void) {
    if (D_003BAE74 != 0) {
        kwlnTaskDestroyWithHierarchy(D_003BAE74, 1);
    }
}

INCLUDE_ASM(const s32, "field/fldPanel", func_00141190);

INCLUDE_ASM(const s32, "field/fldPanel", func_001411C0);

INCLUDE_SDATA(const s32, "field/fldPanel", D_003BAE74);

