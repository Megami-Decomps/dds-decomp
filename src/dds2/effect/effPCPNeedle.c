#include "common.h"

extern u64 effParamTableGetBlock(u64, u64);

typedef struct {
    u8 pad00[0x68];
    u32 resource68;
    u32 resource6C;
    u32 resource70;
} EffPCPNeedleWork;

void effPCPNeedleFree(EffPCPNeedleWork *work) {
    func_001634A8(work->resource68);
    effReleaseAttachedResources(work->resource6C);
    func_003297C8(work->resource70);
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

INCLUDE_ASM(const s32, "effect/effPCPNeedle", effPCPNeedleCopyVector);

void func_0017E668(EffPCPNeedleWork *work) {
    func_0017ED50(work->resource6C);
}
