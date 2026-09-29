#include "common.h"

extern u32 D_00435D8C;

extern s32 func_00102790(void);

/* Prefix of the administration record returned by func_00102790. */
typedef struct AdminWork {
    u32 flags;
    u32 value;
} AdminWork;

INCLUDE_ASM(const s32, "game/code_001019F0", func_001019F0);

void dds3SetScopedObjectFlags(u32 object, u32 mask, u32 scope) {
    func_00100F48(1, object, mask, scope);
}

void dds3ClearScopedObjectFlags(u32 object, u32 mask, u32 scope) {
    func_00100F48(0, object, mask, scope);
}

INCLUDE_ASM(const s32, "game/code_001019F0", func_00101AC0);

INCLUDE_ASM(const s32, "game/code_001019F0", func_00101C50);

INCLUDE_ASM(const s32, "game/code_001019F0", func_00101D30);

INCLUDE_ASM(const s32, "game/code_001019F0", func_001023C8);

u32 func_00102740(void) {
    func_0010FC28(D_00435D8C);
    return 0;
}

u32 func_00102768(void) {
    func_0010FC68(D_00435D8C);
    return 0;
}

INCLUDE_ASM(const s32, "game/code_001019F0", func_00102790);

u32 func_001027B8(void) {
    AdminWork *work;

    work = (AdminWork *)func_00102790();
    return work->value;
}
INCLUDE_ASM(const s32, "game/code_001019F0", func_001027D8);

INCLUDE_RODATA(const s32, "game/code_001019F0", D_00411198);

INCLUDE_SDATA(const s32, "game/code_001019F0", D_00435C0C);

INCLUDE_SDATA(const s32, "game/code_001019F0", D_00435C14);

INCLUDE_SDATA(const s32, "game/code_001019F0", D_00435C18);

