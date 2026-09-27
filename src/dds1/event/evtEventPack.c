#include "common.h"

extern u64 func_00101A70(void);

INCLUDE_ASM(const s32, "event/evtEventPack", func_00241E18);

INCLUDE_ASM(const s32, "event/evtEventPack", func_00241F78);

INCLUDE_ASM(const s32, "event/evtEventPack", func_002420B8);

void func_00242278(void) {
    u64 temp_v0;

    temp_v0 = func_00101A70();
    func_002CFF98(temp_v0);
}

INCLUDE_ASM(const s32, "event/evtEventPack", func_00242298);

INCLUDE_ASM(const s32, "event/evtEventPack", func_00242340);

INCLUDE_ASM(const s32, "event/evtEventPack", func_002423C8);

INCLUDE_ASM(const s32, "event/evtEventPack", func_002424B0);

INCLUDE_ASM(const s32, "event/evtEventPack", func_00242510);
