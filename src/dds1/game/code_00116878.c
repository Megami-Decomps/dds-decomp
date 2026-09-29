#include "common.h"

INCLUDE_ASM(const s32, "game/code_00116878", func_00116878);

INCLUDE_ASM(const s32, "game/code_00116878", func_001168F0);

INCLUDE_ASM(const s32, "game/code_00116878", func_00116968);

INCLUDE_ASM(const s32, "game/code_00116878", func_001169B0);

extern void *func_002CFEB8(s32 size);

typedef struct ObjWithWork {
    u8 unk0[0x18];
    void *work;
} ObjWithWork;

s32 func_00116A10(ObjWithWork *obj) {
    obj->work = func_002CFEB8(0x18);
    memset(obj->work, 0, 0x18);
    return 1;
}
