#include "common.h"
#include "eff.h"

extern void func_002D0918(void *arg0);

void func_00199900(EffPrim *arg0) {
    if (arg0 != NULL) {
        if (arg0->recordCount != 0) {
            func_002D0918(arg0->unk4);
        }
        func_002D0918(arg0->unk0);
    }
}

INCLUDE_ASM(const s32, "interface/itfPanel", func_00199950);

INCLUDE_ASM(const s32, "interface/itfPanel", func_00199998);

INCLUDE_ASM(const s32, "interface/itfPanel", func_00199A20);

INCLUDE_ASM(const s32, "interface/itfPanel", func_00199A68);
