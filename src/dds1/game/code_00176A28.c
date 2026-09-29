#include "common.h"
#include "pcp_vu0.h"

typedef struct {
    u8 pad00[0x10];
    u32 value;
} EffResourceEntry;

/* Nested resource group released when its effect work is destroyed. */
typedef struct {
    u8 pad00[0x40];
    EffResourceEntry *entries;
    u8 pad44[0x1C];
    u32 value60;
    u8 pad64[4];
    u32 resource68;
    u32 resource6C;
    u32 resource70;
} EffResourceWork;

void func_00176A28(s32 work, u32 value) {
    ((EffResourceWork *)work)->value60 = value;
}

INCLUDE_ASM(const s32, "game/code_00176A28", func_00176A30);

void effReleaseResourceHandles(u32 work) {
    sdfQueueAssetRelease(((EffResourceWork *)work)->resource6C);
    effReleaseOptionalResource(work);
    func_002D0918(((EffResourceWork *)work)->resource70);
}

INCLUDE_ASM(const s32, "game/code_00176A28", func_00176B88);

INCLUDE_ASM(const s32, "game/code_00176A28", func_00176E20);

void effReleaseOptionalResource(s32 work) {
    if (((EffResourceWork *)work)->resource68 != 0) {
        func_002D0918(((EffResourceWork *)work)->resource68);
        return;
    }
}

void effSetResourceEntryPosition(u8 *obj, s32 index, f32 *vec) {
    f32 *dst = (f32 *)(index * 0x14 + *(s32 *)(obj + 0x40));
    dst[0] = vec[0];
    dst[1] = vec[1];
    dst[2] = vec[2];
}

void effGetResourceEntryPosition(u8 *obj, s32 index, f32 *vec) {
    f32 *src = (f32 *)(index * 0x14 + *(s32 *)(obj + 0x40));
    vec[0] = src[0];
    vec[1] = src[1];
    vec[2] = src[2];
}

void effSetResourceEntryValue(s32 work, s32 index, u32 value) {
    ((EffResourceWork *)work)->entries[index].value = value;
}

/* vu0 routine: copy a 4x4 matrix (four quadwords) through vf28-vf31 */
void func_001770F8(void *dst, void *src) {
    VU0_COPY_MATRIX(dst, src);
}

INCLUDE_ASM(const s32, "game/code_00176A28", func_00177120);
