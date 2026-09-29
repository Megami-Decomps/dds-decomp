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

void func_00176B50(u32 work) {
    func_002DAA68(((EffResourceWork *)work)->resource6C);
    func_00177048(work);
    func_002D0918(((EffResourceWork *)work)->resource70);
}

INCLUDE_ASM(const s32, "game/code_00176A28", func_00176B88);

INCLUDE_ASM(const s32, "game/code_00176A28", func_00176E20);

void func_00177048(s32 work) {
    if (((EffResourceWork *)work)->resource68 != 0) {
        func_002D0918(((EffResourceWork *)work)->resource68);
        return;
    }
}

void func_00177078(u8 *obj, s32 index, f32 *vec) {
    f32 *dst = (f32 *)(index * 0x14 + *(s32 *)(obj + 0x40));
    dst[0] = vec[0];
    dst[1] = vec[1];
    dst[2] = vec[2];
}

void func_001770A8(u8 *obj, s32 index, f32 *vec) {
    f32 *src = (f32 *)(index * 0x14 + *(s32 *)(obj + 0x40));
    vec[0] = src[0];
    vec[1] = src[1];
    vec[2] = src[2];
}

void func_001770D8(s32 work, s32 index, u32 value) {
    ((EffResourceWork *)work)->entries[index].value = value;
}

void func_001770F8(void *dst, void *src) {
    VU0_COPY_MATRIX(dst, src);
}

INCLUDE_ASM(const s32, "game/code_00176A28", func_00177120);
