#include "common.h"

extern u64 effParamTableGetBlock(u64, u64);

void effPCPNeedleFree(s32 arg0) {
    func_001634A8(*(u32 *)(arg0 + 0x68));
    func_0017E7A8(*(u32 *)(arg0 + 0x6c));
    func_003297C8(*(u32 *)(arg0 + 0x70));
}

void effPCPNeedleCreate(u64 arg0) {
    u64 temp_v0;

    temp_v0 = effParamTableGetBlock(arg0, 0);
    func_0017DD50(temp_v0);
}

void func_0017E068(void) {
    func_0017DD50();
}

INCLUDE_ASM(const s32, "effect/effPCPNeedle", func_0017E080);

INCLUDE_ASM(const s32, "effect/effPCPNeedle", func_0017E658);

void func_0017E668(s32 arg0) {
    func_0017ED50(*(u32 *)(arg0 + 0x6c));
}
