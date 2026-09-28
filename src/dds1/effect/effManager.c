#include "common.h"

void effManagerInitializeSubsystems(void) {
    func_001536A0();
    func_0015B250();
    func_001500F0();
}

INCLUDE_ASM(const s32, "effect/effManager", func_0014F860);

u32 effManagerUpdateAndDispatch(void) {
    func_0015B420();
    func_0015D0C0();
    func_00150750();
    func_00151010();
    func_001602F8();
    effDispatchActive();
    return 0;
}

void func_0014FA10(void) {
}

u32 func_0014FA18(void) {
    return 1;
}

void func_0014FA20(void) {
}

u32 func_0014FA28(void) {
    return 1;
}

INCLUDE_ASM(const s32, "effect/effManager", func_0014FA30);

INCLUDE_ASM(const s32, "effect/effManager", func_0014FAB8);

INCLUDE_ASM(const s32, "effect/effManager", func_0014FB00);

INCLUDE_ASM(const s32, "effect/effManager", func_0014FB38);

INCLUDE_ASM(const s32, "effect/effManager", func_0014FB70);

INCLUDE_ASM(const s32, "effect/effManager", func_0014FBA8);

INCLUDE_ASM(const s32, "effect/effManager", func_0014FBF0);

INCLUDE_ASM(const s32, "effect/effManager", func_0014FC28);

INCLUDE_ASM(const s32, "effect/effManager", func_0014FC60);

INCLUDE_ASM(const s32, "effect/effManager", func_0014FC98);

INCLUDE_ASM(const s32, "effect/effManager", func_0014FCD8);

INCLUDE_ASM(const s32, "effect/effManager", func_0014FD20);

void func_0014FE28(u32 arg0) {
    func_0014FA30(5, 0, arg0);
}

INCLUDE_ASM(const s32, "effect/effManager", func_0014FE48);
