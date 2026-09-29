#include "common.h"

void effManagerInitializeSubsystems(void) {
    func_0015B290();
    func_00162E40();
    func_00157CE0();
}

INCLUDE_ASM(const s32, "effect/effManager", func_00157400);

u32 effManagerUpdateAndDispatch(void) {
    func_00163010();
    func_00164CB0();
    func_00158340();
    func_00158C00();
    func_00167EE8();
    effDispatchActive();
    return 0;
}

void func_001575B0(void) {
}

u32 func_001575B8(void) {
    return 1;
}

void func_001575C0(void) {
}

u32 func_001575C8(void) {
    return 1;
}

INCLUDE_ASM(const s32, "effect/effManager", effCreateNode);

INCLUDE_ASM(const s32, "effect/effManager", effDestroyNode);

INCLUDE_ASM(const s32, "effect/effManager", effUpdateNode);

INCLUDE_ASM(const s32, "effect/effManager", func_001576D8);

INCLUDE_ASM(const s32, "effect/effManager", func_00157710);

INCLUDE_ASM(const s32, "effect/effManager", func_00157748);

INCLUDE_ASM(const s32, "effect/effManager", func_00157790);

INCLUDE_ASM(const s32, "effect/effManager", func_001577C8);

INCLUDE_ASM(const s32, "effect/effManager", func_00157800);

INCLUDE_ASM(const s32, "effect/effManager", func_00157838);

INCLUDE_ASM(const s32, "effect/effManager", func_00157878);

INCLUDE_ASM(const s32, "effect/effManager", func_001578C0);

void func_001579C8(u32 parameter) {
    effCreateNode(5, 0, parameter);
}

INCLUDE_ASM(const s32, "effect/effManager", func_001579E8);
