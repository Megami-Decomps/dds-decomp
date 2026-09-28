#include "common.h"

void effMathReleaseWorkResource(s32 arg0) {
    func_003297C8(*(u32 *)(arg0 + 8));
}

INCLUDE_ASM(const s32, "effect/effMath", func_00195BE0);

INCLUDE_ASM(const s32, "effect/effMath", func_00195D00);

INCLUDE_ASM(const s32, "effect/effMath", func_00195E10);

s32 func_00195E38(s32 *arg0, s32 arg1) {
    return *arg0 + arg1 * 0x38;
}
