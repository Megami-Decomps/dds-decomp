#include "common.h"

extern u32 D_003BAE74;
extern void *func_00101A70(void);
extern void func_002CFF98(void *);

void func_001410E8(void) {
    void *panelState;

    panelState = func_00101A70();
    func_002CFF98(panelState);
    D_003BAE74 = 0;
}

INCLUDE_ASM(const s32, "field/fldPanel", func_00141110);

INCLUDE_ASM(const s32, "field/fldPanel", func_00141158);

INCLUDE_ASM(const s32, "field/fldPanel", func_00141190);

INCLUDE_ASM(const s32, "field/fldPanel", func_001411C0);

INCLUDE_SDATA(const s32, "field/fldPanel", D_003BAE74);

