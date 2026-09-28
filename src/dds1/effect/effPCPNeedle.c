#include "common.h"

/* Packed effect parameter-set accessor (see game/code_001624D0). */
extern void *effParamTableGetBlock(void *data, s32 index);
extern void func_001760F8(void *work);

extern void func_0015B8B8(u32 handle);
extern void func_00176B50(u32 handle);
extern void func_001770F8(u32 handle);
extern void func_002D0918(u32 handle);

/* Needle effect work: three resource handles released on free. */
typedef struct {
    u8 unk00[0x68]; /* 0x00 */
    u32 resource68; /* 0x68 released by func_0015B8B8 */
    u32 resource6C; /* 0x6C released by func_00176B50/func_001770F8 */
    u32 resource70; /* 0x70 released by func_002D0918 */
} EffPCPNeedleWork;

void effPCPNeedleFree(EffPCPNeedleWork *work) {
    func_0015B8B8(work->resource68);
    func_00176B50(work->resource6C);
    func_002D0918(work->resource70);
}

void effPCPNeedleCreate(void *data) {
    void *work;

    work = effParamTableGetBlock(data, 0);
    func_001760F8(work);
}

void func_00176410(void *work) {
    func_001760F8(work);
}

INCLUDE_ASM(const s32, "effect/effPCPNeedle", func_00176428);

INCLUDE_ASM(const s32, "effect/effPCPNeedle", func_00176A00);

void func_00176A10(EffPCPNeedleWork *work) {
    func_001770F8(work->resource6C);
}
