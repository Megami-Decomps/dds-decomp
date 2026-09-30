#include "common.h"

INCLUDE_ASM(const s32, "game/code_00116878", func_00116878);

INCLUDE_ASM(const s32, "game/code_00116878", func_001168F0);

typedef struct ObjWithWork {
    u8 unk0[0x18];
    void *work;
} ObjWithWork;

u32 func_00116968(ObjWithWork *obj) {
    u32 *work = (u32 *)obj->work;
    u32 result;

    if (work[3] != 0) {
        result = sdfModelCreateWithItems(work[3], work[1]);
    } else {
        result = sdfModelCreateWithItems(((u32 *)work[0])[6], work[1]);
    }
    return result;
}

INCLUDE_ASM(const s32, "game/code_00116878", func_001169B0);

extern void *func_002CFEB8(s32 size);

s32 dds3AllocateObjectWork(ObjWithWork *obj) {
    obj->work = func_002CFEB8(0x18);
    memset(obj->work, 0, 0x18);
    return 1;
}
