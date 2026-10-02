#include "common.h"
#include "pcp_vu0.h"

/* Parameter block zero supplies the work passed to the needle initializer. */
extern u64 effParamTableGetBlock(u64, u64);

/* Resource handles released when the needle effect is torn down. */
typedef struct {
    u8 pad00[0x68];
    u32 resource68;
    u32 resource6C;
    u32 resource70;
} EffPCPNeedleWork;

void effPCPNeedleFree(EffPCPNeedleWork *work) {
    parReleaseCellSystem(work->resource68);
    effReleaseAttachedResources(work->resource6C);
    sdfReleaseResourceAllocation(work->resource70);
}

void effPCPNeedleCreate(u64 parameters) {
    u64 work;

    work = effParamTableGetBlock(parameters, 0);
    func_0017DD50(work);
}

void func_0017E068(void) {
    func_0017DD50();
}

INCLUDE_ASM(const s32, "effect/effPCPNeedle", func_0017E080);

void effPCPNeedleCopyVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_0017E668(EffPCPNeedleWork *work) {
    func_0017ED50(work->resource6C);
}
