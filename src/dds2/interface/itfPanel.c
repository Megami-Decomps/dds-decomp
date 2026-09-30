#include "common.h"
#include "eff.h"

extern void func_003297C8(void *arg0);

void func_001A1930(EffPrim *arg0) {
    if (arg0 != NULL) {
        if (arg0->recordCount != 0) {
            func_003297C8(arg0->unk4);
        }
        func_003297C8(arg0->unk0);
    }
}

INCLUDE_ASM(const s32, "interface/itfPanel", func_001A1980);

INCLUDE_ASM(const s32, "interface/itfPanel", func_001A19C8);

INCLUDE_ASM(const s32, "interface/itfPanel", func_001A1A50);

INCLUDE_ASM(const s32, "interface/itfPanel", func_001A1A98);
