#include "common.h"

extern void *func_00328D68(s32 size);

typedef struct ObjWithWork {
    u8 unk0[0x18];
    void *work;
} ObjWithWork;

INCLUDE_ASM(const s32, "game/code_00116AE0", func_00116AE0);

INCLUDE_ASM(const s32, "game/code_00116AE0", func_00116B58);

INCLUDE_ASM(const s32, "game/code_00116AE0", func_00116BD0);

INCLUDE_ASM(const s32, "game/code_00116AE0", func_00116C18);

s32 dds3AllocateObjectWork(ObjWithWork *obj) {
    obj->work = func_00328D68(0x18);
    memset(obj->work, 0, 0x18);
    return 1;
}
