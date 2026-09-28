#include "common.h"

extern u64 func_00101958(void);

INCLUDE_ASM(const s32, "event/evtEventPack", func_0025D230);

INCLUDE_ASM(const s32, "event/evtEventPack", func_0025D390);

INCLUDE_ASM(const s32, "event/evtEventPack", func_0025D4D0);

void evtFreeEventPackState(void) {
    u64 temp_v0;

    temp_v0 = func_00101958();
    func_00328E48(temp_v0);
}

INCLUDE_ASM(const s32, "event/evtEventPack", func_0025D6B0);

INCLUDE_ASM(const s32, "event/evtEventPack", func_0025D758);

INCLUDE_ASM(const s32, "event/evtEventPack", func_0025D7E0);

INCLUDE_ASM(const s32, "event/evtEventPack", func_0025D8C8);

INCLUDE_ASM(const s32, "event/evtEventPack", func_0025D928);

INCLUDE_SDATA(const s32, "event/evtEventPack", D_004377D8);

INCLUDE_SDATA(const s32, "event/evtEventPack", D_004377E0);

INCLUDE_SDATA(const s32, "event/evtEventPack", D_004377E8);

