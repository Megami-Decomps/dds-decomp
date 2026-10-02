#include "common.h"
#include "pcp_vu0.h"

/* Packed effect parameter-set accessor (see game/code_001624D0). */
extern void *effParamTableGetBlock(void *data, s32 index);
extern void func_001760F8(void *work);

extern void parReleaseCellSystem(u32 handle);
extern void effReleaseAttachedResources(u32 handle);
extern void func_001770F8(u32 handle);
extern void sdfReleaseResourceAllocation(u32 handle);

/* Needle effect work: three resource handles released on free. */
typedef struct {
    u8 unk00[0x68]; /* 0x00 */
    u32 resource68; /* 0x68 released by parReleaseCellSystem */
    u32 resource6C; /* 0x6C released by effReleaseAttachedResources/func_001770F8 */
    u32 resource70; /* 0x70 released by sdfReleaseResourceAllocation */
} EffPCPNeedleWork;

void effPCPNeedleFree(EffPCPNeedleWork *work) {
    parReleaseCellSystem(work->resource68);
    effReleaseAttachedResources(work->resource6C);
    sdfReleaseResourceAllocation(work->resource70);
}

/* The first parameter block supplies the effect's runtime work. */
void effPCPNeedleCreate(void *data) {
    void *work;

    work = effParamTableGetBlock(data, 0);
    func_001760F8(work);
}

void func_00176410(void *work) {
    func_001760F8(work);
}

INCLUDE_ASM(const s32, "effect/effPCPNeedle", func_00176428);

void effPCPNeedleCopyVector(void *dst, void *src) {
    PCP_COPY_VECTOR(dst, src);
}

void func_00176A10(EffPCPNeedleWork *work) {
    func_001770F8(work->resource6C);
}
